#ifndef MYTEST_HTTP_HPP
#define MYTEST_HTTP_HPP

#include "utils.hpp"

static void init_prng(void) {
    char namebuf[256];
    const char *random_file;

    /* Seed from a file specified by the user.  This will be the file
       specified with --random-file, $RANDFILE, if set, or ~/.rnd, if it
       exists.  */
    if (global_options.random_file)
        random_file = global_options.random_file;
    else {
        /* Get the random file name using RAND_file_name. */
        namebuf[0] = '\0';
        random_file = RAND_file_name(namebuf, sizeof(namebuf));
    }

    if (random_file && *random_file)
        /* Seed at most 16k (apparently arbitrary value borrowed from
           curl) from random file. */
        RAND_load_file(random_file, 16384);
}

bool ssl_init(void) {
    SSL_METHOD const *meth;
    long ssl_options = 0;
    char *ciphers_string = NULL;
#if !defined(LIBRESSL_VERSION_NUMBER) && (OPENSSL_VERSION_NUMBER >= 0x10100000L)
    int ssl_proto_version = 0;
#endif

#if OPENSSL_VERSION_NUMBER >= 0x00907000
    if (ssl_true_initialized == 0) {
#if OPENSSL_API_COMPAT < 0x10100000L
        OPENSSL_config(NULL);
#endif
        ssl_true_initialized = 1;
    }
#endif
    //logprintf(LOG_VERBOSE, "ssl_init() ssl_ctx: %p\n", ssl_ctx);
    if (ssl_ctx)
        /* The SSL has already been initialized. */
        return true;

    /* Init the PRNG.  If that fails, bail out.  */
    init_prng();
    if (RAND_status() != 1) {
        //logprintf(LOG_NOTQUIET, _("Could not seed PRNG; consider using --random-file.\n"));
        goto error;
    }
    logprintf(LOG_VERBOSE, "ssl_init() init_prng()\n");

#if OPENSSL_VERSION_NUMBER >= 0x00907000
    OPENSSL_load_builtin_modules();
    //logprintf(LOG_VERBOSE, "ssl_init() OPENSSL_load_builtin_modules()\n");
    ENGINE_load_builtin_engines();
    //logprintf(LOG_VERBOSE, "ssl_init() ENGINE_load_builtin_engines()\n");
    CONF_modules_load_file(NULL, NULL,
                           CONF_MFLAGS_DEFAULT_SECTION | CONF_MFLAGS_IGNORE_MISSING_FILE);
    //logprintf(LOG_VERBOSE, "ssl_init() CONF_modules_load_file()\n");
#endif
#if OPENSSL_API_COMPAT >= 0x10100000L
    OPENSSL_init_ssl(0, NULL);
#else
    SSL_library_init();
    //logprintf(LOG_VERBOSE, "ssl_init() SSL_library_init()\n");
    SSL_load_error_strings();
    //logprintf(LOG_VERBOSE, "ssl_init() SSL_load_error_strings()\n");
#endif
#if OPENSSL_VERSION_NUMBER < 0x10100000L
    SSLeay_add_all_algorithms ();
    //logprintf(LOG_VERBOSE, "ssl_init() SSLeay_add_all_algorithms()\n");
    SSLeay_add_ssl_algorithms ();
    //logprintf(LOG_VERBOSE, "ssl_init() SSLeay_add_ssl_algorithms()\n");
#endif

    switch (global_options.secure_protocol) {
#if !defined OPENSSL_NO_SSL2 && OPENSSL_VERSION_NUMBER < 0x10100000L
        case secure_protocol_sslv2:
      meth = SSLv2_client_method ();
      break;
#endif

#ifndef OPENSSL_NO_SSL3_METHOD
        case secure_protocol_sslv3:
            meth = SSLv3_client_method();
            break;
#endif

        case secure_protocol_auto:
        case secure_protocol_pfs:
            meth = SSLv23_client_method();
            ssl_options |= SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3;
            break;
        case secure_protocol_tlsv1:
#if !defined(LIBRESSL_VERSION_NUMBER) && (OPENSSL_VERSION_NUMBER >= 0x10100000L)
            meth = TLS_client_method();
      ssl_proto_version = TLS1_VERSION;
#else
            meth = TLSv1_client_method();
#endif
            break;

#if OPENSSL_VERSION_NUMBER >= 0x10001000
        case secure_protocol_tlsv1_1:
#if !defined(LIBRESSL_VERSION_NUMBER) && (OPENSSL_VERSION_NUMBER >= 0x10100000L)
            meth = TLS_client_method();
      ssl_proto_version = TLS1_1_VERSION;
#else
            meth = TLSv1_1_client_method();
#endif
            break;

        case secure_protocol_tlsv1_2:
#if !defined(LIBRESSL_VERSION_NUMBER) && (OPENSSL_VERSION_NUMBER >= 0x10100000L)
            meth = TLS_client_method();
      ssl_proto_version = TLS1_2_VERSION;
#else
            meth = TLSv1_2_client_method();
#endif
            break;

        case secure_protocol_tlsv1_3:
#if !defined(LIBRESSL_VERSION_NUMBER) && (OPENSSL_VERSION_NUMBER >= 0x10100000L) && defined TLS1_3_VERSION
            meth = TLS_client_method();
      ssl_proto_version = TLS1_3_VERSION;
#else
            //logprintf(LOG_NOTQUIET, _("Your OpenSSL version is too old to support TLS 1.3\n"));
            goto error;
#endif
            break;
#else
        case secure_protocol_tlsv1_1:
      logprintf (LOG_NOTQUIET, _("Your OpenSSL version is too old to support TLSv1.1\n"));
      goto error;

    case secure_protocol_tlsv1_2:
      logprintf (LOG_NOTQUIET, _("Your OpenSSL version is too old to support TLSv1.2\n"));
      goto error;

#endif

        default:
            //logprintf(LOG_NOTQUIET, _("OpenSSL: unimplemented 'secure-protocol' option value %d\n"),
                    //global_options.secure_protocol);
            //logprintf(LOG_NOTQUIET, _("Please report this issue to bug-wget@gnu.org\n"));
            abort();
    }

    /* The type cast below accommodates older OpenSSL versions (0.9.8)
       where SSL_CTX_new() is declared without a "const" argument. */
    ssl_ctx = SSL_CTX_new((SSL_METHOD *) meth);
    if (!ssl_ctx)
        goto error;

    if (ssl_options)
        SSL_CTX_set_options (ssl_ctx, ssl_options);

#if !defined(LIBRESSL_VERSION_NUMBER) && (OPENSSL_VERSION_NUMBER >= 0x10100000L)
    if (ssl_proto_version)
    SSL_CTX_set_min_proto_version(ssl_ctx, ssl_proto_version);
#endif

    /* OpenSSL ciphers: https://www.openssl.org/docs/apps/ciphers.html
     *
     * Rules:
     *  1. --ciphers overrides everything
     *  2. We allow RSA key exchange by default (secure_protocol_auto)
     *  3. We disallow RSA key exchange if PFS was requested (secure_protocol_pfs)
     */
    if (!global_options.tls_ciphers_string) {
        if (global_options.secure_protocol == secure_protocol_auto)
            ciphers_string = "HIGH:!aNULL:!RC4:!MD5:!SRP:!PSK";
        else if (global_options.secure_protocol == secure_protocol_pfs)
            ciphers_string = "HIGH:!aNULL:!RC4:!MD5:!SRP:!PSK:!kRSA";
    } else {
        ciphers_string = global_options.tls_ciphers_string;
    }

    if (ciphers_string && !SSL_CTX_set_cipher_list(ssl_ctx, ciphers_string)) {
        //logprintf(LOG_NOTQUIET, _("OpenSSL: Invalid cipher list: %s\n"), ciphers_string);
        goto error;
    }

    SSL_CTX_set_default_verify_paths(ssl_ctx);
    SSL_CTX_load_verify_locations(ssl_ctx, global_options.ca_cert, global_options.ca_directory);

    if (global_options.crl_file) {
        X509_STORE *store = SSL_CTX_get_cert_store(ssl_ctx);
        X509_LOOKUP *lookup;

        if (!(lookup = X509_STORE_add_lookup(store, X509_LOOKUP_file()))
            || (!X509_load_crl_file(lookup, global_options.crl_file, X509_FILETYPE_PEM)))
            goto error;

        X509_STORE_set_flags(store, X509_V_FLAG_CRL_CHECK | X509_V_FLAG_CRL_CHECK_ALL);
    }

    /* SSL_VERIFY_NONE instructs OpenSSL not to abort SSL_connect if the
       certificate is invalid.  We verify the certificate separately in
       ssl_check_certificate, which provides much better diagnostics
       than examining the error stack after a failed SSL_connect.  */
    SSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_NONE, NULL);

    /* Use the private key from the cert file unless otherwise specified. */
    if (global_options.cert_file && !global_options.private_key) {
        global_options.private_key = xstrdup(global_options.cert_file);
        global_options.private_key_type = global_options.cert_type;
    }

    /* Use cert from private key file unless otherwise specified. */
    if (global_options.private_key && !global_options.cert_file) {
        global_options.cert_file = xstrdup(global_options.private_key);
        global_options.cert_type = global_options.private_key_type;
    }

    if (global_options.cert_file)
        if (SSL_CTX_use_certificate_file(ssl_ctx, global_options.cert_file,
                                         key_type_to_ssl_type(global_options.cert_type))
            != 1)
            goto error;
    if (global_options.private_key)
        if (SSL_CTX_use_PrivateKey_file(ssl_ctx, global_options.private_key,
                                        key_type_to_ssl_type(global_options.private_key_type))
            != 1)
            goto error;

    /* Since fd_write unconditionally assumes partial writes (and
       handles them correctly), allow them in OpenSSL.  */
    SSL_CTX_set_mode (ssl_ctx, SSL_MODE_ENABLE_PARTIAL_WRITE);

    /* The OpenSSL library can handle renegotiations automatically, so
       tell it to do so.  */
    SSL_CTX_set_mode (ssl_ctx, SSL_MODE_AUTO_RETRY);

    return true;

    error:
    if (ssl_ctx)
        SSL_CTX_free(ssl_ctx);
    print_errors();
    return false;
}

static uerr_t gethttp(const struct url *u, struct url *original_url, struct http_stat *hstat,
                      int *dt, struct url *proxy, struct iri *iri, int count) {
    fprintf(stdout, _("\n"));
    fprintf(stdout, _("gethttp() start\n"));
    if (u) {
        fprintf(stdout, _("gethttp() u->url: %s\n"), u->url);
        fprintf(stdout, _("gethttp() u->scheme: %d\n"), u->scheme);
        fprintf(stdout, _("gethttp() u->host: %s\n"), u->host);
        fprintf(stdout, _("gethttp() u->port: %d\n"), u->port);
        fprintf(stdout, _("gethttp() u->path: %s\n"), u->path);
        fprintf(stdout, _("gethttp() u->params: %s\n"), u->params);
        fprintf(stdout, _("gethttp() u->query: %s\n"), u->query);
        fprintf(stdout, _("gethttp() u->fragment: %s\n"), u->fragment);
        fprintf(stdout, _("gethttp() u->dir: %s\n"), u->dir);
        fprintf(stdout, _("gethttp() u->file: %s\n"), u->file);
        fprintf(stdout, _("gethttp() u->user: %s\n"), u->user);
        fprintf(stdout, _("gethttp() u->passwd: %s\n"), u->passwd);
    }
    if (original_url) {
        fprintf(stdout, _("gethttp() original_url->url: %s\n"), original_url->url);
        fprintf(stdout, _("gethttp() original_url->scheme: %d\n"), original_url->scheme);
        fprintf(stdout, _("gethttp() original_url->host: %s\n"), original_url->host);
        fprintf(stdout, _("gethttp() original_url->port: %d\n"), original_url->port);
        fprintf(stdout, _("gethttp() original_url->path: %s\n"), original_url->path);
        fprintf(stdout, _("gethttp() original_url->params: %s\n"), original_url->params);
        fprintf(stdout, _("gethttp() original_url->query: %s\n"), original_url->query);
        fprintf(stdout, _("gethttp() original_url->fragment: %s\n"), original_url->fragment);
        fprintf(stdout, _("gethttp() original_url->dir: %s\n"), original_url->dir);
        fprintf(stdout, _("gethttp() original_url->file: %s\n"), original_url->file);
        fprintf(stdout, _("gethttp() original_url->user: %s\n"), original_url->user);
        fprintf(stdout, _("gethttp() original_url->passwd: %s\n"), original_url->passwd);
    }
    if (iri) {
        fprintf(stdout, _("gethttp() iri->uri_encoding: %s\n"), iri->uri_encoding);
        fprintf(stdout, _("gethttp() iri->content_encoding: %s\n"), iri->content_encoding);
        fprintf(stdout, _("gethttp() iri->orig_url: %s\n"), iri->orig_url);
        fprintf(stdout, _("gethttp() iri->utf8_encode: %d\n"), iri->utf8_encode);
    }
    fprintf(stdout, _("gethttp() *dt: %d\n"), *dt);
    fprintf(stdout, _("gethttp() count: %d\n"), count);
    if (!proxy) {
        fprintf(stdout, _("gethttp() proxy is NULL\n"));
    }

    global_options.debug = 1;

    struct request *local_request = NULL;

    char *type = NULL;
    char *user, *passwd;
    char *proxyauth;
    int statcode;
    int write_error;
    wgint contlen, contrange;
    const struct url *conn;
    FILE *fp;
    int err;
    uerr_t retval;
#ifdef HAVE_HSTS
#ifdef TESTING
    /* we don't link against main.o when we're testing */
  hsts_store_t hsts_store = NULL;
#else
    extern hsts_store_t hsts_store;
#endif
    const char *hsts_params;
    time_t max_age;
    bool include_subdomains;
#endif

    int sock = -1;

    /* Set to 1 when the authorization has already been sent and should
       not be tried again. */
    bool auth_finished = false;

    /* Set to 1 when just globally-set Basic authorization has been sent;
     * should prevent further Basic negotiations, but not other
     * mechanisms. */
    bool basic_auth_finished = false;

    /* Whether NTLM authentication is used for this request. */
    bool ntlm_seen = false;

    /* Whether our connection to the remote host is through SSL.  */
    bool using_ssl = false;

    /* Whether a HEAD request will be issued (as opposed to GET or
       POST). */
    bool head_only = !!(*dt & HEAD_ONLY);
    fprintf(stdout, _("gethttp() head_only: %d\n"), head_only);

    /* Whether conditional get request will be issued.  */
    bool cond_get = !!(*dt & IF_MODIFIED_SINCE);
    fprintf(stdout, _("gethttp() cond_get: %d\n"), cond_get);

    char *head = NULL;
    struct response *resp = NULL;
    char hdrval[512];
    char *message = NULL;

    /* Declare WARC variables. */
    bool warc_enabled = (global_options.warc_filename != NULL);
    fprintf(stdout, _("gethttp() warc_enabled: %d\n"), warc_enabled);// 0
    FILE *warc_tmp = NULL;
    char warc_timestamp_str[21];
    char warc_request_uuid[48];
    ip_address *warc_ip = NULL;
    off_t warc_payload_offset = -1;

    /* Whether this connection will be kept alive after the HTTP request
       is done. */
    bool keep_alive;

    /* Is the server using the chunked transfer encoding?  */
    bool chunked_transfer_encoding = false;

    /* Whether keep-alive should be inhibited.  */
    bool inhibit_keep_alive =
            !global_options.http_keep_alive || global_options.ignore_length;
    fprintf(stdout, _("gethttp() inhibit_keep_alive: %d\n"), inhibit_keep_alive);

    /* Headers sent when using POST. */
    wgint body_data_size = 0;

#ifdef HAVE_SSL
    if (u->scheme == SCHEME_HTTPS) {
        fprintf(stdout, _("gethttp() ssl_init() start\n"));
        /* Initialize the SSL context.  After this has once been done,
           it becomes a no-op.  */
        if (!ssl_init()) {
            //scheme_disable(SCHEME_HTTPS);
            //logprintf(LOG_NOTQUIET, _("Disabling SSL due to encountered errors.\n"));
            retval = SSLINITFAILED;
            //goto cleanup;
        }
    }
#endif /* HAVE_SSL */

}


#endif //MYTEST_HTTP_HPP
