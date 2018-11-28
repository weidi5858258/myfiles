#ifndef MYTEST_MY_STRUCT_H
#define MYTEST_MY_STRUCT_H

#include "my_define.h"

/* Universal error type -- used almost everywhere.  Error reporting of
   this detail is not generally used or needed and should be
   simplified.  */
typedef enum {
    NOCONERROR,
    HOSTERR,
    CONSOCKERR,
    CONERROR,
    CONSSLERR,
    CONIMPOSSIBLE,
    NEWLOCATION,
    FTPOK,
    FTPLOGINC,
    FTPLOGREFUSED,
    FTPPORTERR,
    FTPSYSERR,
    FTPNSFOD,
    FTPUNKNOWNTYPE,
    FTPRERR,
    FTPSRVERR,
    FTPRETRINT,
    FTPRESTFAIL,
    URLERROR,
    FOPENERR,
    FOPEN_EXCL_ERR,
    FWRITEERR,
    HEOF,
    GATEWAYTIMEOUT,
    HERR,
    RETROK,
    RECLEVELEXC,
    WRONGCODE,
    FTPINVPASV,
    FTPNOPASV,
    FTPNOPBSZ,
    FTPNOPROT,
    FTPNOAUTH,
    CONTNOTSUPPORTED,
    RETRUNNEEDED,
    RETRFINISHED,
    READERR, TRYLIMEXC, FILEBADFILE, RANGEERR,
    RETRBADPATTERN, PROXERR,
    AUTHFAILED, QUOTEXC, WRITEFAILED, SSLINITFAILED, VERIFCERTERR,
    UNLINKERR, NEWLOCATION_KEEP_POST, CLOSEFAILED, ATTRMISSING, UNKNOWNATTR,
    WARC_ERR, WARC_TMP_FOPENERR, WARC_TMP_FWRITEERR,
    TIMECONV_ERR,
    METALINK_PARSE_ERROR, METALINK_RETR_ERROR,
    METALINK_CHKSUM_ERROR, METALINK_SIG_ERROR, METALINK_MISSING_RESOURCE,
    RETR_WITH_METALINK,
    METALINK_SIZE_ERROR
} uerr_t;

enum url_scheme {
    SCHEME_HTTP,
#ifdef HAVE_SSL
    SCHEME_HTTPS,
#endif
    SCHEME_FTP,
#ifdef HAVE_SSL
    SCHEME_FTPS,
#endif
    SCHEME_INVALID
};

/* Structure containing info on a URL.  */
struct url {
    char *url;                /* Original URL */
    enum url_scheme scheme;   /* URL scheme */

    char *host;               /* Extracted hostname */
    int port;                 /* Port number */

    /* URL components (URL-quoted). */
    char *path;
    char *params;
    char *query;
    char *fragment;

    /* Extracted path info (unquoted). */
    char *dir;
    char *file;

    /* Username and password (unquoted). */
    char *user;
    char *passwd;
};

typedef enum {
    ENC_INVALID = -1,             /* invalid encoding */
    ENC_NONE = 0,                 /* no special encoding */
    ENC_GZIP,                     /* gzip compression */
    ENC_DEFLATE,                  /* deflate compression */
    ENC_COMPRESS,                 /* compress compression */
    ENC_BROTLI                    /* brotli compression */
} encoding_t;

struct http_stat {
    wgint len;                    /* received length */
    wgint contlen;                /* expected length */
    wgint restval;                /* the restart value */
    int res;                      /* the result of last read */
    char *rderrmsg;               /* error message from read error */
    char *newloc;                 /* new location (redirection) */
    char *remote_time;            /* remote time-stamp string */
    char *error;                  /* textual HTTP error */
    int statcode;                 /* status code */
    char *message;                /* status message */
    wgint rd_size;                /* amount of data read from socket */
    double dltime;                /* time it took to download the data */
    const char *referer;          /* value of the referer header. */
    char *local_file;             /* local file name. */
    bool existence_checked;       /* true if we already checked for a file's
                                   existence after having begun to download
                                   (needed in gethttp for when connection is
                                   interrupted/restarted. */
    bool timestamp_checked;       /* true if pre-download time-stamping checks
                                 * have already been performed */
    char *orig_file_name;         /* name of file to compare for time-stamping
                                 * (might be != local_file if -K is set) */
    wgint orig_file_size;         /* size of file to compare for time-stamping */
    time_t orig_file_tstamp;      /* time-stamp of file to compare for
                                 * time-stamping */
    encoding_t local_encoding;    /* the encoding of the local file */
    encoding_t remote_encoding;   /* the encoding of the remote file */
    bool temporary;               /* downloading a temporary file */
};

struct iri {
    char *uri_encoding;      /* Encoding of the uri to fetch */
    char *content_encoding;  /* Encoding of links inside the fetched file */
    char *orig_url;          /* */
    bool utf8_encode;        /* Will/Is the current url encoded in utf8 */
};

enum rp {
    rel_none, rel_name, rel_value, rel_both
};

struct request {
    const char *method;
    char *arg;

    struct request_header {
        char *name, *value;
        enum rp release_policy;
    } *headers;
    int hcount, hcapacity;
};

enum SECURE_PROTOCOL_{
    secure_protocol_auto,
    secure_protocol_sslv2,
    secure_protocol_sslv3,
    secure_protocol_tlsv1,
    secure_protocol_tlsv1_1,
    secure_protocol_tlsv1_2,
    secure_protocol_tlsv1_3,
    secure_protocol_pfs
};            /* type of secure protocol to use. */

enum keyfile_type {
    keyfile_pem,
    keyfile_asn1
};                  /* type of client certificate file */

struct options {
    int verbose;                  /* Are we verbose?  (First set to -1,
                                   hence not boolean.) */
    bool quiet;                   /* Are we quiet? */
    int ntry;                     /* Number of tries per URL */
    bool retry_connrefused;       /* Treat CONNREFUSED as non-fatal. */
    char *retry_on_http_error;    /* Treat given HTTP errors as non-fatal. */
    bool background;              /* Whether we should work in background. */
    bool ignore_length;           /* Do we heed content-length at all?  */
    bool recursive;               /* Are we recursive? */
    bool spanhost;                /* Do we span across hosts in
                                   recursion? */
    int max_redirect;            /* Maximum number of times we'll allow
                                   a page to redirect. */
    bool relative_only;           /* Follow only relative links. */
    bool no_parent;               /* Restrict access to the parent
                                   directory.  */
    int reclevel;                 /* Maximum level of recursion */
    bool dirstruct;               /* Do we build the directory structure
                                   as we go along? */
    bool no_dirstruct;            /* Do we hate dirstruct? */
    int cut_dirs;                 /* Number of directory components to cut. */
    bool add_hostdir;             /* Do we add hostname directory? */
    bool protocol_directories;    /* Whether to prepend "http"/"ftp" to dirs. */
    bool noclobber;               /* Disables clobbering of existing data. */
    bool unlink_requested;        /* remove file before clobbering */
    char *dir_prefix;             /* The top of directory tree */
    char *lfilename;              /* Log filename */
    char *input_filename;         /* Input filename */
    char *choose_config;          /* Specified config file */
    bool noconfig;                /* Ignore all config files? */
    bool force_html;              /* Is the input file an HTML file? */

    char *default_page;           /* Alternative default page (index file) */

    bool spider;                  /* Is Wget in spider mode? */

    char **accepts;               /* List of patterns to accept. */
    char **rejects;               /* List of patterns to reject. */
    const char **excludes;        /* List of excluded FTP directories. */
    const char **includes;        /* List of FTP directories to
                                   follow. */
    bool ignore_case;             /* Whether to ignore case when
                                   matching dirs and files */

    char *acceptregex_s;          /* Patterns to accept (a regex string). */
    char *rejectregex_s;          /* Patterns to reject (a regex string). */
    void *acceptregex;            /* Patterns to accept (a regex struct). */
    void *rejectregex;            /* Patterns to reject (a regex struct). */
    enum {
#ifdef HAVE_LIBPCRE
        regex_type_pcre,
#endif
        regex_type_posix
    } regex_type;                 /* The regex library. */
    void *(*regex_compile_fun)(const char *);             /* Function to compile a regex. */
    bool (*regex_match_fun)(const void *, const char *);  /* Function to match a string to a regex. */

    char **domains;               /* See host.c */
    char **exclude_domains;
    bool dns_cache;               /* whether we cache DNS lookups. */

    char **follow_tags;           /* List of HTML tags to recursively follow. */
    char **ignore_tags;           /* List of HTML tags to ignore if recursing. */

    bool follow_ftp;              /* Are FTP URL-s followed in recursive
                                   retrieving? */
    bool retr_symlinks;           /* Whether we retrieve symlinks in
                                   FTP. */
    char *output_document;        /* The output file to which the
                                   documents will be printed.  */
    char *warc_filename;          /* WARC output filename */
    char *warc_tempdir;           /* WARC temp dir */
    char *warc_cdx_dedup_filename;/* CDX file to be used for deduplication. */
    wgint warc_maxsize;           /* WARC max archive size */
    bool warc_compression_enabled;/* For GZIP compression. */
    bool warc_digests_enabled;    /* For SHA1 digests. */
    bool warc_cdx_enabled;        /* Create CDX files? */
    bool warc_keep_log;           /* Store the log file in a WARC record. */
    char **warc_user_headers;     /* User-defined WARC header(s). */

    bool enable_xattr;            /* Store metadata in POSIX extended attributes. */

    char *user;                   /* Generic username */
    char *passwd;                 /* Generic password */
    bool ask_passwd;              /* Ask for password? */
    char *use_askpass;           /* value to use for use-askpass if WGET_ASKPASS is not set */

    bool always_rest;             /* Always use REST. */
    wgint start_pos;              /* Start position of a download. */
    char *ftp_user;               /* FTP username */
    char *ftp_passwd;             /* FTP password */
    bool netrc;                   /* Whether to read .netrc. */
    bool ftp_glob;                /* FTP globbing */
    bool ftp_pasv;                /* Passive FTP. */

    char *http_user;              /* HTTP username. */
    char *http_passwd;            /* HTTP password. */
    char **user_headers;          /* User-defined header(s). */
    bool http_keep_alive;         /* whether we use keep-alive */

    bool use_proxy;               /* Do we use proxy? */
    bool allow_cache;             /* Do we allow server-side caching? */
    char *http_proxy, *ftp_proxy, *https_proxy;
    char **no_proxy;
    char *base_href;
    char *progress_type;          /* progress indicator type. */
    int show_progress;           /* Show only the progress bar */
    bool noscroll;                /* Don't scroll the filename in the progressbar */
    char *proxy_user; /*oli*/
    char *proxy_passwd;

    double read_timeout;          /* The read/write timeout. */
    double dns_timeout;           /* The DNS timeout. */
    double connect_timeout;       /* The connect timeout. */

    bool random_wait;             /* vary from 0 .. wait secs by random()? */
    double wait;                  /* The wait period between retrievals. */
    double waitretry;             /* The wait period between retries. - HEH */
    bool use_robots;              /* Do we heed robots.txt? */

    wgint limit_rate;             /* Limit the download rate to this
                                   many bps. */
    SUM_SIZE_INT quota;           /* Maximum file size to download and
                                   store. */

    bool server_response;         /* Do we print server response? */
    bool save_headers;            /* Do we save headers together with
                                   file? */
    bool content_on_error;        /* Do we output the content when the HTTP
                                   status code indicates a server error */

    bool debug;                   /* Debugging on/off */

    bool timestamping;            /* Whether to use time-stamping. */
    bool if_modified_since;       /* Whether to use conditional get requests.  */

    bool backup_converted;        /* Do we save pre-converted files as *.orig? */
    int backups;                  /* Are numeric backups made? */

    char *useragent;              /* User-Agent string, which can be set
                                   to something other than Wget. */
    char *referer;                /* Naughty Referer, which can be
                                   set to something other than
                                   NULL. */
    bool convert_links;           /* Will the links be converted
                                   locally? */
    bool convert_file_only;       /* Convert only the file portion of the URI (i.e. basename).
                                   Leave everything else untouched. */

    bool remove_listing;          /* Do we remove .listing files
                                   generated by FTP? */
    bool htmlify;                 /* Do we HTML-ify the OS-dependent
                                   listings? */

    char *dot_style;
    wgint dot_bytes;              /* How many bytes in a printing
                                   dot. */
    int dots_in_line;             /* How many dots in one line. */
    int dot_spacing;              /* How many dots between spacings. */

    bool delete_after;            /* Whether the files will be deleted
                                   after download. */

    bool adjust_extension;        /* Use ".html" extension on all text/html? */

    bool page_requisites;         /* Whether we need to download all files
                                   necessary to display a page properly. */
    char *bind_address;           /* What local IP address to bind to. */

#ifdef HAVE_SSL
    int check_cert;               /* whether to validate the server's cert */
    char *cert_file;              /* external client certificate to use. */
    char *private_key;            /* private key file (if not internal). */

    char *ca_directory;           /* CA directory (hash files) */
    char *ca_cert;                /* CA certificate file to use */
    char *crl_file;               /* file with CRLs */

    char *pinnedpubkey;           /* Public key (PEM/DER) file, or any number
                                   of base64 encoded sha256 hashes preceded by
                                   \'sha256//\' and separated by \';\', to verify
                                   peer against */

    char *random_file;            /* file with random data to seed the PRNG */
    char *egd_file;               /* file name of the egd daemon socket */
    bool https_only;              /* whether to follow HTTPS only */
    bool ftps_resume_ssl;
    bool ftps_fallback_to_ftp;
    bool ftps_implicit;
    bool ftps_clear_data_connection;

    char *tls_ciphers_string;
#endif /* HAVE_SSL */

    bool cookies;                 /* whether cookies are used. */
    char *cookies_input;          /* file we're loading the cookies from. */
    char *cookies_output;         /* file we're saving the cookies to. */
    bool keep_badhash;            /* Keep files with checksum mismatch. */
    bool keep_session_cookies;    /* whether session cookies should be
                                   saved and loaded. */

    char *post_data;              /* POST query string */
    char *post_file_name;         /* File to post */
    char *method;                 /* HTTP Method to use in Header */
    char *body_data;              /* HTTP Method Data String */
    char *body_file;              /* HTTP Method File */

    enum {
        restrict_unix,
        restrict_vms,
        restrict_windows
    } restrict_files_os;          /* file name restriction ruleset. */
    bool restrict_files_ctrl;     /* non-zero if control chars in URLs
                                   are restricted from appearing in
                                   generated file names. */
    bool restrict_files_nonascii; /* non-zero if bytes with values greater
                                   than 127 are restricted. */
    enum {
        restrict_no_case_restriction,
        restrict_lowercase,
        restrict_uppercase
    } restrict_files_case;        /* file name case restriction. */

    bool strict_comments;         /* whether strict SGML comments are
                                   enforced.  */

    bool preserve_perm;           /* whether remote permissions are used
                                   or that what is set by umask. */

#ifdef ENABLE_IPV6
    bool ipv4_only;               /* IPv4 connections have been requested. */
    bool ipv6_only;               /* IPv4 connections have been requested. */
#endif
    enum {
        prefer_ipv4,
        prefer_ipv6,
        prefer_none
    } prefer_family;              /* preferred address family when more
                                   than one type is available */

    bool content_disposition;     /* Honor HTTP Content-Disposition header. */
    bool auth_without_challenge;  /* Issue Basic authentication creds without
                                   waiting for a challenge. */

    bool enable_iri;
    char *encoding_remote;
    const char *locale;

    bool trustservernames;

    bool useservertimestamps;     /* Update downloaded files' timestamps to
                                   match those on server? */

    bool show_all_dns_entries;    /* Show all the DNS entries when resolving a
                                   name. */
    bool report_bps;              /*Output bandwidth in bits format*/

#ifdef HAVE_LIBZ
    enum compression_options {
        compression_auto,
        compression_gzip,
        compression_none
    } compression;                /* type of HTTP compression to use */
#endif

    char *rejected_log;           /* The file to log rejected URLS to. */

#ifdef HAVE_HSTS
    bool hsts;
    char *hsts_file;
#endif

    const char *homedir;          /* the homedir of the running process */
    const char *wgetrcfile;       /* the wgetrc file to be loaded */

    enum SECURE_PROTOCOL_ secure_protocol;
    enum keyfile_type cert_type;
    enum keyfile_type private_key_type;           /* type of private key file */
};

extern struct options global_options;

enum log_options {
    LOG_VERBOSE,
    LOG_NOTQUIET,
    LOG_NONVERBOSE,
    LOG_ALWAYS,
    LOG_PROGRESS
};

#define CHECK_VERBOSE(x)                        \
  switch (x)                                    \
    {                                           \
    case LOG_PROGRESS:                          \
      if (!global_options.show_progress)                   \
        return;                                 \
      break;                                    \
    case LOG_ALWAYS:                            \
      break;                                    \
    case LOG_NOTQUIET:                          \
      if (global_options.quiet)                            \
        return;                                 \
      break;                                    \
    case LOG_NONVERBOSE:                        \
      if (global_options.verbose || global_options.quiet)             \
        return;                                 \
      break;                                    \
    case LOG_VERBOSE:                           \
      if (!global_options.verbose)                         \
        return;                                 \
    }

/* Document type ("dt") flags */
enum {
    TEXTHTML = 0x0001,        /* document is of type text/html
                                           or application/xhtml+xml */
    RETROKF = 0x0002,        /* retrieval was OK */
    HEAD_ONLY = 0x0004,        /* only send the HEAD request */
    SEND_NOCACHE = 0x0008,        /* send Pragma: no-cache directive */
    ACCEPTRANGES = 0x0010,        /* Accept-ranges header was found */
    ADDED_HTML_EXTENSION = 0x0020,        /* added ".html" extension due to -E */
    TEXTCSS = 0x0040,        /* document is of type text/css */
    IF_MODIFIED_SINCE = 0x0080,        /* use if-modified-since header */
    METALINK_METADATA = 0x0100         /* use HTTP response for Metalink metadata */
};

struct response {
    /* The response data. */
    const char *data;

    /* The array of pointers that indicate where each header starts.
       For example, given this HTTP response:

         HTTP/1.0 200 Ok
         Description: some
          text
         Etag: x

       The headers are located like this:

       "HTTP/1.0 200 Ok\r\nDescription: some\r\n text\r\nEtag: x\r\n\r\n"
       ^                   ^                             ^          ^
       headers[0]          headers[1]                    headers[2] headers[3]

       I.e. headers[0] points to the beginning of the request,
       headers[1] points to the end of the first header and the
       beginning of the second one, etc.  */

    const char **headers;
};

typedef struct {
    /* Address family, one of AF_INET or AF_INET6. */
    int family;

    /* The actual data, in the form of struct in_addr or in6_addr: */
    union {
        struct in_addr d4;      /* IPv4 address */
#ifdef ENABLE_IPV6
        struct in6_addr d6;     /* IPv6 address */
#endif
    } data;

    /* Under IPv6 getaddrinfo also returns scope_id.  Since it's
       IPv6-specific it strictly belongs in the above union, but we put
       it here for simplicity.  */
#if defined ENABLE_IPV6 && defined HAVE_SOCKADDR_IN6_SCOPE_ID
    int ipv6_scope;
#endif
} ip_address;

struct logvprintf_state {
    char *bigmsg;
    int expected_size;
    int allocated;
};

static const char *redirect_request_signal_name;

typedef __loff_t loff_t;
typedef __ino_t ino_t;
typedef __dev_t dev_t;
typedef struct file_stat_s {
    int access_err;               /* Error in accecssing file : Not present vs permission */
    ino_t st_ino;                 /* st_ino from stats() on the file before open() */
    dev_t st_dev;                 /* st_dev from stats() on the file before open() */
} file_stats_t;

/*struct stat {
    __dev_t st_dev;        *//* Device.  *//*
#ifndef __x86_64__
    unsigned short int __pad1;
#endif
#if defined __x86_64__ || !defined __USE_FILE_OFFSET64
    __ino_t st_ino;        *//* File serial number.	*//*
#else
    __ino_t __st_ino;			*//* 32bit file serial number.	*//*
#endif
#ifndef __x86_64__
    __mode_t st_mode;			*//* File mode.  *//*
    __nlink_t st_nlink;			*//* Link count.  *//*
#else
    __nlink_t st_nlink;        *//* Link count.  *//*
    __mode_t st_mode;        *//* File mode.  *//*
#endif
    __uid_t st_uid;        *//* User ID of the file's owner.	*//*
    __gid_t st_gid;        *//* Group ID of the file's group.*//*
#ifdef __x86_64__
    int __pad0;
#endif
    __dev_t st_rdev;        *//* Device number, if device.  *//*
#ifndef __x86_64__
    unsigned short int __pad2;
#endif
#if defined __x86_64__ || !defined __USE_FILE_OFFSET64
    __off_t st_size;            *//* Size of file, in bytes.  *//*
#else
    __off64_t st_size;			*//* Size of file, in bytes.  *//*
#endif
    __blksize_t st_blksize;    *//* Optimal block size for I/O.  *//*
#if defined __x86_64__  || !defined __USE_FILE_OFFSET64
    __blkcnt_t st_blocks;        *//* Number 512-byte blocks allocated. *//*
#else
    __blkcnt64_t st_blocks;		*//* Number 512-byte blocks allocated. *//*
#endif
#ifdef __USE_XOPEN2K8
    *//* Nanosecond resolution timestamps are stored in a format
       equivalent to 'struct timespec'.  This is the type used
       whenever possible but the Unix namespace rules do not allow the
       identifier 'timespec' to appear in the <sys/stat.h> header.
       Therefore we have to handle the use of this header in strictly
       standard-compliant sources special.  *//*
    struct timespec st_atim;        *//* Time of last access.  *//*
    struct timespec st_mtim;        *//* Time of last modification.  *//*
    struct timespec st_ctim;        *//* Time of last status change.  *//*
# define st_atime st_atim.tv_sec    *//* Backward compatibility.  *//*
# define st_mtime st_mtim.tv_sec
# define st_ctime st_ctim.tv_sec
#else
    __time_t st_atime;			*//* Time of last access.  *//*
    __syscall_ulong_t st_atimensec;	*//* Nscecs of last access.  *//*
    __time_t st_mtime;			*//* Time of last modification.  *//*
    __syscall_ulong_t st_mtimensec;	*//* Nsecs of last modification.  *//*
    __time_t st_ctime;			*//* Time of last status change.  *//*
    __syscall_ulong_t st_ctimensec;	*//* Nsecs of last status change.  *//*
#endif
#ifdef __x86_64__
    __syscall_slong_t __glibc_reserved[3];
#else
# ifndef __USE_FILE_OFFSET64
    unsigned long int __glibc_reserved4;
    unsigned long int __glibc_reserved5;
# else
    __ino64_t st_ino;			*//* File serial number.	*//*
# endif
#endif
};*/

enum {
    HAVE_GNU_CALLOC = 1
};

enum quoting_style {
    /* Output names as-is (ls --quoting-style=literal).  Can result in
       embedded null bytes if QA_ELIDE_NULL_BYTES is not in
       effect.

       quotearg_buffer:
       "simple", "\0 \t\n'\"\033??/\\", "a:b"
       quotearg:
       "simple", " \t\n'\"\033??/\\", "a:b"
       quotearg_colon:
       "simple", " \t\n'\"\033??/\\", "a:b"
    */
            literal_quoting_style,

    /* Quote names for the shell if they contain shell metacharacters
       or would cause ambiguous output (ls --quoting-style=shell).
       Can result in embedded null bytes if QA_ELIDE_NULL_BYTES is not
       in effect.

       quotearg_buffer:
       "simple", "'\0 \t\n'\\''\"\033??/\\'", "a:b"
       quotearg:
       "simple", "' \t\n'\\''\"\033??/\\'", "a:b"
       quotearg_colon:
       "simple", "' \t\n'\\''\"\033??/\\'", "'a:b'"
    */
            shell_quoting_style,

    /* Quote names for the shell, even if they would normally not
       require quoting (ls --quoting-style=shell-always).  Can result
       in embedded null bytes if QA_ELIDE_NULL_BYTES is not in effect.
       Behaves like shell_quoting_style if QA_ELIDE_OUTER_QUOTES is in
       effect.

       quotearg_buffer:
       "'simple'", "'\0 \t\n'\\''\"\033??/\\'", "'a:b'"
       quotearg:
       "'simple'", "' \t\n'\\''\"\033??/\\'", "'a:b'"
       quotearg_colon:
       "'simple'", "' \t\n'\\''\"\033??/\\'", "'a:b'"
    */
            shell_always_quoting_style,

    /* Quote names for the shell if they contain shell metacharacters
       or other problematic characters (ls --quoting-style=shell-escape).
       Non printable characters are quoted using the $'...' syntax,
       which originated in ksh93 and is widely supported by most shells,
       and proposed for inclusion in POSIX.

       quotearg_buffer:
       "simple", "''$'\\0'' '$'\\t\\n'\\''\"'$'\\033''??/\\'", "a:b"
       quotearg:
       "simple", "''$'\\0'' '$'\\t\\n'\\''\"'$'\\033''??/\\'", "a:b"
       quotearg_colon:
       "simple", "''$'\\0'' '$'\\t\\n'\\''\"'$'\\033''??/\\'", "'a:b'"
    */
            shell_escape_quoting_style,

    /* Quote names for the shell even if they would normally not
       require quoting (ls --quoting-style=shell-escape).
       Non printable characters are quoted using the $'...' syntax,
       which originated in ksh93 and is widely supported by most shells,
       and proposed for inclusion in POSIX.  Behaves like
       shell_escape_quoting_style if QA_ELIDE_OUTER_QUOTES is in effect.

       quotearg_buffer:
       "simple", "''$'\\0'' '$'\\t\\n'\\''\"'$'\\033''??/\'", "a:b"
       quotearg:
       "simple", "''$'\\0'' '$'\\t\\n'\\''\"'$'\\033''??/\'", "a:b"
       quotearg_colon:
       "simple", "''$'\\0'' '$'\\t\\n'\\''\"'$'\\033''??/\'", "'a:b'"
    */
            shell_escape_always_quoting_style,

    /* Quote names as for a C language string (ls --quoting-style=c).
       Behaves like c_maybe_quoting_style if QA_ELIDE_OUTER_QUOTES is
       in effect.  Split into consecutive strings if
       QA_SPLIT_TRIGRAPHS.

       quotearg_buffer:
       "\"simple\"", "\"\\0 \\t\\n'\\\"\\033??/\\\\\"", "\"a:b\""
       quotearg:
       "\"simple\"", "\"\\0 \\t\\n'\\\"\\033??/\\\\\"", "\"a:b\""
       quotearg_colon:
       "\"simple\"", "\"\\0 \\t\\n'\\\"\\033??/\\\\\"", "\"a\\:b\""
    */
            c_quoting_style,

    /* Like c_quoting_style except omit the surrounding double-quote
       characters if no quoted characters are encountered.

       quotearg_buffer:
       "simple", "\"\\0 \\t\\n'\\\"\\033??/\\\\\"", "a:b"
       quotearg:
       "simple", "\"\\0 \\t\\n'\\\"\\033??/\\\\\"", "a:b"
       quotearg_colon:
       "simple", "\"\\0 \\t\\n'\\\"\\033??/\\\\\"", "\"a:b\""
    */
            c_maybe_quoting_style,

    /* Like c_quoting_style except always omit the surrounding
       double-quote characters and ignore QA_SPLIT_TRIGRAPHS
       (ls --quoting-style=escape).

       quotearg_buffer:
       "simple", "\\0 \\t\\n'\"\\033??/\\\\", "a:b"
       quotearg:
       "simple", "\\0 \\t\\n'\"\\033??/\\\\", "a:b"
       quotearg_colon:
       "simple", "\\0 \\t\\n'\"\\033??/\\\\", "a\\:b"
    */
            escape_quoting_style,

    /* Like clocale_quoting_style, but use single quotes in the
       default C locale or if the program does not use gettext
       (ls --quoting-style=locale).  For UTF-8 locales, quote
       characters will use Unicode.

       LC_MESSAGES=C
       quotearg_buffer:
       "`simple'", "`\\0 \\t\\n\\'\"\\033??/\\\\'", "`a:b'"
       quotearg:
       "`simple'", "`\\0 \\t\\n\\'\"\\033??/\\\\'", "`a:b'"
       quotearg_colon:
       "`simple'", "`\\0 \\t\\n\\'\"\\033??/\\\\'", "`a\\:b'"

       LC_MESSAGES=pt_PT.utf8
       quotearg_buffer:
       "\302\253simple\302\273",
       "\302\253\\0 \\t\\n'\"\\033??/\\\\\302\253", "\302\253a:b\302\273"
       quotearg:
       "\302\253simple\302\273",
       "\302\253\\0 \\t\\n'\"\\033??/\\\\\302\253", "\302\253a:b\302\273"
       quotearg_colon:
       "\302\253simple\302\273",
       "\302\253\\0 \\t\\n'\"\\033??/\\\\\302\253", "\302\253a\\:b\302\273"
    */
            locale_quoting_style,

    /* Like c_quoting_style except use quotation marks appropriate for
       the locale and ignore QA_SPLIT_TRIGRAPHS
       (ls --quoting-style=clocale).

       LC_MESSAGES=C
       quotearg_buffer:
       "\"simple\"", "\"\\0 \\t\\n'\\\"\\033??/\\\\\"", "\"a:b\""
       quotearg:
       "\"simple\"", "\"\\0 \\t\\n'\\\"\\033??/\\\\\"", "\"a:b\""
       quotearg_colon:
       "\"simple\"", "\"\\0 \\t\\n'\\\"\\033??/\\\\\"", "\"a\\:b\""

       LC_MESSAGES=pt_PT.utf8
       quotearg_buffer:
       "\302\253simple\302\273",
       "\302\253\\0 \\t\\n'\"\\033??/\\\\\302\253", "\302\253a:b\302\273"
       quotearg:
       "\302\253simple\302\273",
       "\302\253\\0 \\t\\n'\"\\033??/\\\\\302\253", "\302\253a:b\302\273"
       quotearg_colon:
       "\302\253simple\302\273",
       "\302\253\\0 \\t\\n'\"\\033??/\\\\\302\253", "\302\253a\\:b\302\273"
    */
            clocale_quoting_style,

    /* Like clocale_quoting_style except use the custom quotation marks
       set by set_custom_quoting.  If custom quotation marks are not
       set, the behavior is undefined.

       left_quote = right_quote = "'"
       quotearg_buffer:
       "'simple'", "'\\0 \\t\\n\\'\"\\033??/\\\\'", "'a:b'"
       quotearg:
       "'simple'", "'\\0 \\t\\n\\'\"\\033??/\\\\'", "'a:b'"
       quotearg_colon:
       "'simple'", "'\\0 \\t\\n\\'\"\\033??/\\\\'", "'a\\:b'"

       left_quote = "(" and right_quote = ")"
       quotearg_buffer:
       "(simple)", "(\\0 \\t\\n'\"\\033??/\\\\)", "(a:b)"
       quotearg:
       "(simple)", "(\\0 \\t\\n'\"\\033??/\\\\)", "(a:b)"
       quotearg_colon:
       "(simple)", "(\\0 \\t\\n'\"\\033??/\\\\)", "(a\\:b)"

       left_quote = ":" and right_quote = " "
       quotearg_buffer:
       ":simple ", ":\\0\\ \\t\\n'\"\\033??/\\\\ ", ":a:b "
       quotearg:
       ":simple ", ":\\0\\ \\t\\n'\"\\033??/\\\\ ", ":a:b "
       quotearg_colon:
       ":simple ", ":\\0\\ \\t\\n'\"\\033??/\\\\ ", ":a\\:b "

       left_quote = "\"'" and right_quote = "'\""
       Notice that this is treated as a single level of quotes or two
       levels where the outer quote need not be escaped within the inner
       quotes.  For two levels where the outer quote must be escaped
       within the inner quotes, you must use separate quotearg
       invocations.
       quotearg_buffer:
       "\"'simple'\"", "\"'\\0 \\t\\n\\'\"\\033??/\\\\'\"", "\"'a:b'\""
       quotearg:
       "\"'simple'\"", "\"'\\0 \\t\\n\\'\"\\033??/\\\\'\"", "\"'a:b'\""
       quotearg_colon:
       "\"'simple'\"", "\"'\\0 \\t\\n\\'\"\\033??/\\\\'\"", "\"'a\\:b'\""
    */
            custom_quoting_style
};

struct quoting_options {
    /* Basic quoting style.  */
    enum quoting_style style;

    /* Additional flags.  Bitwise combination of enum quoting_flags.  */
    int flags;

    /* Quote the characters indicated by this bit vector even if the
       quoting style would not normally require them to be quoted.  */
    unsigned int quote_these_too[(UCHAR_MAX / INT_BITS) + 1];

    /* The left quote for custom_quoting_style.  */
    char const *left_quote;

    /* The right quote for custom_quoting_style.  */
    char const *right_quote;
};

enum {
    WGET_EXIT_SUCCESS = 0,
    WGET_EXIT_GENERIC_ERROR = 1,
    WGET_EXIT_PARSE_ERROR = 2,
    WGET_EXIT_IO_FAIL = 3,
    WGET_EXIT_NETWORK_FAIL = 4,
    WGET_EXIT_SSL_AUTH_FAIL = 5,
    WGET_EXIT_SERVER_AUTH_FAIL = 6,
    WGET_EXIT_PROTOCOL_ERROR = 7,
    WGET_EXIT_SERVER_ERROR = 8,

    WGET_EXIT_UNKNOWN
};

enum quoting_flags {
    /* Always elide null bytes from styles that do not quote them,
       even when the length of the result is available to the
       caller.  */
            QA_ELIDE_NULL_BYTES = 0x01,

    /* Omit the surrounding quote characters if no escaped characters
       are encountered.  Note that if no other character needs
       escaping, then neither does the escape character.  */
            QA_ELIDE_OUTER_QUOTES = 0x02,

    /* In the c_quoting_style and c_maybe_quoting_style, split ANSI
       trigraph sequences into concatenated strings (for example,
       "?""?/" rather than "??/", which could be confused with
       "\\").  */
            QA_SPLIT_TRIGRAPHS = 0x04
};


#endif //MYTEST_MY_STRUCT_H
