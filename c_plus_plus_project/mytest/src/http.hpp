#ifndef MYTEST_HTTP_HPP
#define MYTEST_HTTP_HPP

#include "utils.hpp"

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
        /* Initialize the SSL context.  After this has once been done,
           it becomes a no-op.  */
        if (!ssl_init()) {
            scheme_disable(SCHEME_HTTPS);
            logprintf(LOG_NOTQUIET, _("Disabling SSL due to encountered errors.\n"));
            retval = SSLINITFAILED;
            goto cleanup;
        }
    }
#endif /* HAVE_SSL */

}


#endif //MYTEST_HTTP_HPP
