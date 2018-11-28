#ifndef MYTEST_UTILS_HPP
#define MYTEST_UTILS_HPP

#include "my_struct.h"

struct options global_options;

//////////////////////////////////////////////////

void *xmalloc(size_t n);

void xalloc_die(void);

void logprintf(enum log_options o, const char *fmt, ...);

//////////////////////////////////////////////////

static int safe_copy(char *buf, size_t buflen, const char *msg) {
    size_t len = strlen(msg);
    int ret;

    if (len < buflen) {
        /* Although POSIX allows memcpy() to corrupt errno, we don't
           know of any implementation where this is a real problem.  */
        memcpy(buf, msg, len + 1);
        ret = 0;
    } else {
        memcpy(buf, msg, buflen - 1);
        buf[buflen - 1] = '\0';
        ret = ERANGE;
    }
    return ret;
}

/*#define __strerror_r strerror_r
int strerror_r(int errnum, char *buf, size_t buflen)
//#undef strerror_r
{
    *//* Filter this out now, so that rest of this replacement knows that
       there is room for a non-empty message and trailing NUL.  *//*
    if (buflen <= 1) {
        if (buflen)
            *buf = '\0';
        return ERANGE;
    }
    *buf = '\0';

    *//* Check for gnulib overrides.  *//*
    {
        char const *msg = strerror_override (errnum);

        if (msg)
            return safe_copy(buf, buflen, msg);
    }

    {
        int ret;
        int saved_errno = errno;

        gl_lock_lock(strerror_lock);

        {
            char *errmsg = strerror(errnum);

            *//* For invalid error numbers, strerror() on
                 - IRIX 6.5 returns NULL,
                 - HP-UX 11 returns an empty string.  *//*
            if (errmsg == NULL || *errmsg == '\0')
                ret = EINVAL;
            else
                ret = safe_copy(buf, buflen, errmsg);
        }

        gl_lock_unlock(strerror_lock);

        if (ret == EINVAL && !*buf) {
            snprintf(buf, buflen, "Unknown error %d", errnum);
        }

        errno = saved_errno;
        return ret;
    }
}*/

static void print_errors(void) {
    unsigned long err;
    while ((err = ERR_get_error()) != 0)
        logprintf(LOG_NOTQUIET, "OpenSSL: %s\n", ERR_error_string(err, NULL));
}

static int key_type_to_ssl_type(enum keyfile_type type) {
    switch (type) {
        case keyfile_pem:
            return SSL_FILETYPE_PEM;
        case keyfile_asn1:
            return SSL_FILETYPE_ASN1;
        default:
            abort();
    }
}

void *xnmalloc(size_t n, size_t s)
_GL_ATTRIBUTE_MALLOC _GL_ATTRIBUTE_ALLOC_SIZE((1, 2));

void *xnmalloc(size_t n, size_t s) {
    if (xalloc_oversized (n, s))
        xalloc_die();
    return xmalloc(n * s);
}

#define XNMALLOC(n, t) \
   ((t *) (sizeof (t) == 1 ? xmalloc (n) : xnmalloc (n, sizeof (t))))

char *xcharalloc(size_t n) {
    return XNMALLOC (n, char);
}

#if !_LIBC

/* Return non-zero if FD is open.  */
static int is_open(int fd) {
# if (defined _WIN32 || defined __WIN32__) && !defined __CYGWIN__
    /* On native Windows: The initial state of unassigned standard file
     descriptors is that they are open but point to an INVALID_HANDLE_VALUE.
     There is no fcntl, and the gnulib replacement fcntl does not support
     F_GETFL.  */
  return (HANDLE) _get_osfhandle (fd) != INVALID_HANDLE_VALUE;
# else
#  ifndef F_GETFL
#   error Please port fcntl to your platform
#  endif
    return 0 <= fcntl(fd, F_GETFL);
# endif
}

#endif

static void flush_stdout(void) {
#if !_LIBC
    int stdout_fd;

#if GNULIB_FREOPEN_SAFER
    /* Use of gnulib's freopen-safer module normally ensures that
       fileno (stdout) == 1
     whenever stdout is open.  */
  stdout_fd = STDOUT_FILENO;
#else
    /* POSIX states that fileno (stdout) after fclose is unspecified.  But in
       practice it is not a problem, because stdout is statically allocated and
       the fd of a FILE stream is stored as a field in its allocated memory.  */
    stdout_fd = fileno(stdout);
#endif
    /* POSIX states that fflush (stdout) after fclose is unspecified; it
       is safe in glibc, but not on all other platforms.  fflush (NULL)
       is always defined, but too draconian.  */
    if (0 <= stdout_fd && is_open(stdout_fd))
#endif
        fflush(stdout);
}

char const *getprogname(void) {
# if HAVE_DECL_PROGRAM_INVOCATION_SHORT_NAME                /* glibc, BeOS */
    /* https://www.gnu.org/software/libc/manual/html_node/Error-Messages.html */
    return program_invocation_short_name;
# elif HAVE_DECL_PROGRAM_INVOCATION_NAME                    /* glibc, BeOS */
    /* https://www.gnu.org/software/libc/manual/html_node/Error-Messages.html */
  return last_component (program_invocation_name);
# elif HAVE_GETEXECNAME                                     /* Solaris */
  /* https://docs.oracle.com/cd/E19253-01/816-5168/6mbb3hrb1/index.html */
  const char *p = getexecname ();
  if (!p)
    p = "?";
  return last_component (p);
# elif HAVE_DECL___ARGV                                     /* mingw, MSVC */
  /* https://msdn.microsoft.com/en-us/library/dn727674.aspx */
  const char *p = __argv && __argv[0] ? __argv[0] : "?";
  return last_component (p);
# elif HAVE_VAR___PROGNAME                                  /* OpenBSD, QNX */
  /* https://man.openbsd.org/style.9 */
  /* http://www.qnx.de/developers/docs/6.5.0/index.jsp?topic=%2Fcom.qnx.doc.neutrino_lib_ref%2Fp%2F__progname.html */
  /* Be careful to declare this only when we absolutely need it
     (OpenBSD 5.1), rather than when it's available.  Otherwise,
     its mere declaration makes program_invocation_short_name
     malfunction (have zero length) with Fedora 25's glibc.  */
  extern char *__progname;
  const char *p = __progname;
  return p && p[0] ? p : "?";
# elif _AIX                                                 /* AIX */
  /* Idea by Bastien ROUCARIÈS,
     https://lists.gnu.org/r/bug-gnulib/2010-12/msg00095.html
     Reference: https://www.ibm.com/support/knowledgecenter/en/ssw_aix_61/com.ibm.aix.basetrf1/getprocs.htm
  */
  static char *p;
  static int first = 1;
  if (first)
    {
      first = 0;
      pid_t pid = getpid ();
      struct procentry64 procs;
      p = (0 < getprocs64 (&procs, sizeof procs, NULL, 0, &pid, 1)
           ? strdup (procs.pi_comm)
           : NULL);
      if (!p)
        p = "?";
    }
  return p;
# elif defined __hpux
  static char *p;
  static int first = 1;
  if (first)
    {
      first = 0;
      pid_t pid = getpid ();
      struct pst_status status;
      p = (0 < pstat_getproc (&status, sizeof status, 0, pid)
           ? strdup (status.pst_ucomm)
           : NULL);
      if (!p)
        p = "?";
    }
  return p;
# elif __MVS__                                              /* z/OS */
  /* https://www.ibm.com/support/knowledgecenter/SSLTBW_2.1.0/com.ibm.zos.v2r1.bpxbd00/rtwgetp.htm */
  static char *p = "?";
  static int first = 1;
  if (first)
    {
      pid_t pid = getpid ();
      int token;
      W_PSPROC buf;
      first = 0;
      memset (&buf, 0, sizeof(buf));
      buf.ps_cmdptr    = (char *) malloc (buf.ps_cmdlen    = PS_CMDBLEN_LONG);
      buf.ps_conttyptr = (char *) malloc (buf.ps_conttylen = PS_CONTTYBLEN);
      buf.ps_pathptr   = (char *) malloc (buf.ps_pathlen   = PS_PATHBLEN);
      if (buf.ps_cmdptr && buf.ps_conttyptr && buf.ps_pathptr)
        {
          for (token = 0; token >= 0;
               token = w_getpsent (token, &buf, sizeof(buf)))
            {
              if (token > 0 && buf.ps_pid == pid)
                {
                  char *s = strdup (last_component (buf.ps_pathptr));
                  if (s)
                    p = s;
                  break;
                }
            }
        }
      free (buf.ps_cmdptr);
      free (buf.ps_conttyptr);
      free (buf.ps_pathptr);
    }
  return p;
# elif defined __sgi                                        /* IRIX */
  char filename[50];
  int fd;

  sprintf (filename, "/proc/pinfo/%d", (int) getpid ());
  fd = open (filename, O_RDONLY);
  if (0 <= fd)
    {
      prpsinfo_t buf;
      int ioctl_ok = 0 <= ioctl (fd, PIOCPSINFO, &buf);
      close (fd);
      if (ioctl_ok)
        {
          char *name = buf.pr_fname;
          size_t namesize = sizeof buf.pr_fname;
          char *namenul = memchr (name, '\0', namesize);
          size_t namelen = namenul ? namenul - name : namesize;
          char *namecopy = malloc (namelen + 1);
          if (namecopy)
            {
              namecopy[namelen] = 0;
              return memcpy (namecopy, name, namelen);
            }
        }
    }
  return NULL;
# else
#  error "getprogname module not ported to this OS"
# endif
}

char *
strerror(int n)
#undef strerror
{
    static char buf[STACKBUF_LEN];
    size_t len;

    /* Cast away const, due to the historical signature of strerror;
       callers should not be modifying the string.  */
    const char *msg = strerror_override(n);
    if (msg)
        return (char *) msg;

    msg = strerror(n);

    /* Our strerror_r implementation might use the system's strerror
       buffer, so all other clients of strerror have to see the error
       copied into a buffer that we manage.  This is not thread-safe,
       even if the system strerror is, but portable programs shouldn't
       be using strerror if they care about thread-safety.  */
    if (!msg || !*msg) {
        static char const fmt[] = "Unknown error %d";
        //verify(sizeof buf >= sizeof(fmt) + INT_STRLEN_BOUND(n));
        sprintf(buf, fmt, n);
        errno = EINVAL;
        return buf;
    }

    /* Fix STACKBUF_LEN if this ever aborts.  */
    len = strlen(msg);
    if (sizeof buf <= len)
        abort();

    memcpy(buf, msg, len + 1);
    return buf;
}

static void print_errno_message(int errnum) {
    char const *s;

#if _LIBC || GNULIB_STRERROR_R_POSIX || defined HAVE_STRERROR_R
    char errbuf[1024];
# if _LIBC || (!GNULIB_STRERROR_R_POSIX && STRERROR_R_CHAR_P)
    s = __strerror_r (errnum, errbuf, sizeof errbuf);
# else
    if (__strerror_r(errnum, errbuf, sizeof errbuf) == 0)
        s = errbuf;
    else
        s = 0;
# endif
#else
    s = strerror (errnum);
#endif

#if !_LIBC
    if (!s)
        s = _("Unknown system error");
#endif

#if _LIBC
    __fxprintf (NULL, ": %s", s);
#else
    fprintf(stderr, ": %s", s);
#endif
}

static void error_tail(int status, int errnum, const char *message, va_list args) {
    vfprintf(stderr, message, args);
    ++error_message_count;
    if (errnum)
        print_errno_message(errnum);
    putc('\n', stderr);
    fflush(stderr);
    if (status)
        exit(status);
}

void error(int status, int errnum, const char *message, ...) {
    va_list args;

#if defined _LIBC && defined __libc_ptf_call
    /* We do not want this call to be cut short by a thread
     cancellation.  Therefore disable cancellation for now.  */
  int state = PTHREAD_CANCEL_ENABLE;
  __libc_ptf_call (pthread_setcancelstate, (PTHREAD_CANCEL_DISABLE, &state),
                   0);
#endif

    flush_stdout();
#ifdef _LIBC
    _IO_flockfile (stderr);
#endif
    if (error_print_progname)
        (*error_print_progname)();
    else {
#if _LIBC
        __fxprintf (NULL, "%s: ", program_name);
#else
        fprintf(stderr, "%s: ", program_name);
#endif
    }

    va_start (args, message);
    error_tail(status, errnum, message, args);
    va_end (args);

#ifdef _LIBC
    _IO_funlockfile (stderr);
# ifdef __libc_ptf_call
  __libc_ptf_call (pthread_setcancelstate, (state, NULL), 0);
# endif
#endif
}

void xalloc_die(void) {
    error(exit_failure, 0, "%s", _("memory exhausted"));

    /* _Noreturn cannot be given to error, since it may return if
       its first argument is 0.  To help compilers understand the
       xalloc_die does not return, call abort.  Also, the abort is a
       safety feature if exit_failure is 0 (which shouldn't happen).  */
    abort();
}

void *xmalloc(size_t n) {
    void *p = malloc(n);
    if (!p && n != 0)
        xalloc_die();
    return p;
}

void *xcalloc(size_t n, size_t s) {
    void *p;
    /* Test for overflow, since objects with size greater than
       PTRDIFF_MAX cause pointer subtraction to go awry.  Omit size-zero
       tests if HAVE_GNU_CALLOC, since GNU calloc never returns NULL if
       successful.  */
    if (xalloc_oversized(n, s)
        || (!(p = calloc(n, s)) && (HAVE_GNU_CALLOC || n != 0)))
        xalloc_die();
    return p;
}

/* Clone an object P of size S, with error checking.  There's no need
   for xnmemdup (P, N, S), since xmemdup (P, N * S) works without any
   need for an arithmetic overflow check.  */
void *xmemdup(void const *p, size_t s) {
    return memcpy(xmalloc(s), p, s);
}

/* Clone STRING.  */
char *xstrdup(char const *string) {
    return (char *) xmemdup(string, strlen(string) + 1);
}

/* Does FILENAME exist? */
bool file_exists_p(const char *filename, file_stats_t *fstats) {
    struct stat buf;

#if defined(WINDOWS) || defined(__VMS)
    int ret = stat (filename, &buf);
    if (ret >= 0)
    {
      if (fstats != NULL)
        fstats->access_err = errno;
    }
    return ret >= 0;
#else
    errno = 0;
    if (stat (filename, &buf) == 0 && S_ISREG(buf.st_mode) &&
                                 (((S_IRUSR & buf.st_mode) && (getuid() == buf.st_uid)) ||
                                  ((S_IRGRP & buf.st_mode) && group_member(buf.st_gid)) ||
                                  (S_IROTH & buf.st_mode))) {
        if (fstats != NULL) {
            fstats->access_err = 0;
            fstats->st_ino = buf.st_ino;
            fstats->st_dev = buf.st_dev;
        }
        return true;
    }
    else
    {
        if (fstats != NULL)
            fstats->access_err = (errno == 0 ? EACCES : errno);
        errno = 0;
        return false;
    }
    /* NOTREACHED */
#endif
}

char *number_to_string(char *buffer, wgint number) {
    char *p = buffer;
    wgint n = number;

    int last_digit_char = 0;

#if (SIZEOF_WGINT != 4) && (SIZEOF_WGINT != 8)
    /* We are running in a very strange environment.  Leave the correct
     printing to sprintf.  */
  p += sprintf (buf, "%j", (intmax_t) (n));
#else  /* (SIZEOF_WGINT == 4) || (SIZEOF_WGINT == 8) */

    if (n < 0) {
        if (n < -WGINT_MAX) {
            /* n = -n would overflow because -n would evaluate to a
               wgint value larger than WGINT_MAX.  Need to make n
               smaller and handle the last digit separately.  */
            int last_digit = n % 10;
            /* The sign of n%10 is implementation-defined. */
            if (last_digit < 0)
                last_digit_char = '0' - last_digit;
            else
                last_digit_char = '0' + last_digit;
            /* After n is made smaller, -n will not overflow. */
            n /= 10;
        }

        *p++ = '-';
        n = -n;
    }

    /* Use the DIGITS_ macro appropriate for N's number of digits.  That
       way printing any N is fully open-coded without a loop or jump.
       (Also see description of DIGITS_*.)  */

    if (n < 10) DIGITS_1(1);
    else if (n < 100) DIGITS_2(10);
    else if (n < 1000) DIGITS_3(100);
    else if (n < 10000) DIGITS_4(1000);
    else if (n < 100000) DIGITS_5(10000);
    else if (n < 1000000) DIGITS_6(100000);
    else if (n < 10000000) DIGITS_7(1000000);
    else if (n < 100000000) DIGITS_8(10000000);
    else if (n < 1000000000) DIGITS_9(100000000);
#if SIZEOF_WGINT == 4
        /* wgint is 32 bits wide: no number has more than 10 digits. */
  else                                   DIGITS_10 (1000000000);
#else
        /* wgint is 64 bits wide: handle numbers with 9-19 decimal digits.
           Constants are constructed by compile-time multiplication to avoid
           dealing with different notations for 64-bit constants
           (nL/nLL/nI64, depending on the compiler and architecture).  */
    else if (n < 10 * (W) 1000000000) DIGITS_10(1000000000);
    else if (n < 100 * (W) 1000000000) DIGITS_11(10 * (W) 1000000000);
    else if (n < 1000 * (W) 1000000000) DIGITS_12(100 * (W) 1000000000);
    else if (n < 10000 * (W) 1000000000) DIGITS_13(1000 * (W) 1000000000);
    else if (n < 100000 * (W) 1000000000) DIGITS_14(10000 * (W) 1000000000);
    else if (n < 1000000 * (W) 1000000000) DIGITS_15(100000 * (W) 1000000000);
    else if (n < 10000000 * (W) 1000000000) DIGITS_16(1000000 * (W) 1000000000);
    else if (n < 100000000 * (W) 1000000000) DIGITS_17(10000000 * (W) 1000000000);
    else if (n < 1000000000 * (W) 1000000000) DIGITS_18(100000000 * (W) 1000000000);
    else
        DIGITS_19(1000000000 * (W) 1000000000);
#endif

    if (last_digit_char)
        *p++ = last_digit_char;

    *p = '\0';
#endif /* (SIZEOF_WGINT == 4) || (SIZEOF_WGINT == 8) */

    return p;
}

static char *unique_name_1(const char *prefix) {
    int count = 1;
    int plen = strlen(prefix);
    char *template_ = (char *) alloca (plen + 1 + 24);
    char *template_tail = template_ + plen;

    memcpy(template_, prefix, plen);
    *template_tail++ = UNIQ_SEP;

    do
        number_to_string(template_tail, count++);
    while (file_exists_p(template_, NULL));

    return xstrdup(template_);
}

char *unique_name(const char *file, bool allow_passthrough) {
    /* If the FILE itself doesn't exist, return it without
       modification. */
    if (!file_exists_p(file, NULL))
        return allow_passthrough ? (char *) file : xstrdup(file);

    /* Otherwise, find a numeric suffix that results in unused file name
       and return it.  */
    return unique_name_1(file);
}

FILE *fopen_excl(const char *fname, int binary) {
    int fd;
#ifdef O_EXCL

/* 2005-04-14 SMS.
   VMS lacks O_BINARY, but makes up for it in weird and wonderful ways.
   It also has file versions which obviate all the O_EXCL effort.
   O_TRUNC (something of a misnomer) requests a new version.
*/
# ifdef __VMS
    /* Common open() optional arguments:
   sequential access only, access callback function.
*/
#  define OPEN_OPT_ARGS "fop=sqo", "acc", acc_cb, &open_id

  int open_id;
  int flags = O_WRONLY | O_CREAT | O_TRUNC;

  if (binary > 1)
    {
      open_id = 11;
      fd = open( fname,                 /* File name. */
       flags,                           /* Flags. */
       0777,                            /* Mode for default protection. */
       "ctx=bin,stm",                   /* Binary, stream access. */
       "rfm=stmlf",                     /* Stream_LF. */
       OPEN_OPT_ARGS);                  /* Access callback. */
    }
  else if (binary)
    {
      open_id = 12;
      fd = open( fname,                 /* File name. */
       flags,                           /* Flags. */
       0777,                            /* Mode for default protection. */
       "ctx=bin,stm",                   /* Binary, stream access. */
       "rfm=fix",                       /* Fixed-length, */
       "mrs=512",                       /* 512-byte records. */
       OPEN_OPT_ARGS);                  /* Access callback. */
    }
  else
    {
      open_id = 13;
      fd = open( fname,                 /* File name. */
       flags,                           /* Flags. */
       0777,                            /* Mode for default protection. */
       "rfm=stmlf",                     /* Stream_LF. */
       OPEN_OPT_ARGS);                  /* Access callback. */
    }
# else /* def __VMS */
    int flags = O_WRONLY | O_CREAT | O_EXCL;
# ifdef O_BINARY
    if (binary)
    flags |= O_BINARY;
# endif
    fd = open(fname, flags, 0666);
# endif /* def __VMS [else] */

    if (fd < 0)
        return NULL;
    return fdopen(fd, binary ? "wb" : "w");
#else  /* not O_EXCL */
    /* Manually check whether the file exists.  This is prone to race
     conditions, but systems without O_EXCL haven't deserved
     better.  */
  if (file_exists_p (fname, NULL))
    {
      errno = EEXIST;
      return NULL;
    }
  return fopen (fname, binary ? "wb" : "w");
#endif /* not O_EXCL */
}

FILE *unique_create(const char *name, bool binary, char **opened_name) {
    /* unique file name, based on NAME */
    char *uname = unique_name(name, false);
    FILE *fp;
    while ((fp = fopen_excl(uname, binary)) == NULL && errno == EEXIST) {
        xfree(uname);
        uname = unique_name(name, false);
    }
    if (opened_name) {
        if (fp)
            *opened_name = uname;
        else {
            *opened_name = NULL;
            xfree(uname);
        }
    } else
        xfree(uname);
    return fp;
}

static FILE *get_log_fp(void) {
    if (inhibit_logging)
        return NULL;
    if (logfp)
        return logfp;
    return stderr;
}

static FILE *get_warc_log_fp(void) {
    if (inhibit_logging)
        return NULL;
    if (warclogfp)
        return warclogfp;
    if (logfp)
        return NULL;
    return stderr;
}

/* Dump saved lines to logfp. */
static void log_dump_context(void) {
    int num = log_line_current;
    FILE *fp = get_log_fp();
    FILE *warcfp = get_warc_log_fp();
    if (!fp)
        return;

    if (num == -1)
        return;
    if (trailing_line)
        ROT_ADVANCE(num);
    do {
        struct log_ln *ln = log_lines + num;
        if (ln->content) {
            FPUTS (ln->content, fp);
            if (warcfp != NULL)
                FPUTS (ln->content, warcfp);
        }
        ROT_ADVANCE(num);
    } while (num != log_line_current);
    if (trailing_line)
        if (log_lines[log_line_current].content) {
            FPUTS (log_lines[log_line_current].content, fp);
            if (warcfp != NULL)
                FPUTS (log_lines[log_line_current].content, warcfp);
        }
    fflush(fp);
    fflush(warcfp);
}

void *xrealloc(void *p, size_t n) {
    if (!n && p) {
        /* The GNU and C99 realloc behaviors disagree here.  Act like
           GNU, even if the underlying realloc is C99.  */
        free(p);
        return NULL;
    }

    p = realloc(p, n);
    if (!p && n)
        xalloc_die();
    return p;
}

static const char *get_charset_aliases(void) {
    const char *cp;
    cp = charset_aliases;
    if (cp == NULL) {
#if !(defined DARWIN7 || defined VMS || defined WINDOWS_NATIVE || defined __CYGWIN__ || defined OS2)
        char *malloc_dir = NULL;
        const char *dir;
        const char *base = "charset.alias";
        char *file_name;

        /* Make it possible to override the charset.alias location.  This is
           necessary for running the testsuite before "make install".  */
        dir = getenv("CHARSETALIASDIR");
        if (dir == NULL || dir[0] == '\0')
            dir = relocate2(LIBDIR, &malloc_dir);

        /* Concatenate dir and base into freshly allocated file_name.  */
        {
            size_t dir_len = strlen(dir);
            size_t base_len = strlen(base);
            int add_slash = (dir_len > 0 && !ISSLASH(dir[dir_len - 1]));
            file_name = (char *) malloc(dir_len + add_slash + base_len + 1);
            if (file_name != NULL) {
                memcpy(file_name, dir, dir_len);
                if (add_slash)
                    file_name[dir_len] = DIRECTORY_SEPARATOR;
                memcpy(file_name + dir_len + add_slash, base, base_len + 1);
            }
        }

        free(malloc_dir);

        if (file_name == NULL)
            /* Out of memory.  Treat the file as empty.  */
            cp = "";
        else {
            int fd;

            /* Open the file.  Reject symbolic links on platforms that support
               O_NOFOLLOW.  This is a security feature.  Without it, an attacker
               could retrieve parts of the contents (namely, the tail of the
               first line that starts with "* ") of an arbitrary file by placing
               a symbolic link to that file under the name "charset.alias" in
               some writable directory and defining the environment variable
               CHARSETALIASDIR to point to that directory.  */
            fd = open(file_name, O_RDONLY | (HAVE_WORKING_O_NOFOLLOW ? O_NOFOLLOW : 0));
            if (fd < 0)
                /* File not found.  Treat it as empty.  */
                cp = "";
            else {
                FILE *fp;

                fp = fdopen(fd, "r");
                if (fp == NULL) {
                    /* Out of memory.  Treat the file as empty.  */
                    close(fd);
                    cp = "";
                } else {
                    /* Parse the file's contents.  */
                    char *res_ptr = NULL;
                    size_t res_size = 0;

                    for (;;) {
                        int c;
                        char buf1[50 + 1];
                        char buf2[50 + 1];
                        size_t l1, l2;
                        char *old_res_ptr;

                        c = getc(fp);
                        if (c == EOF)
                            break;
                        if (c == '\n' || c == ' ' || c == '\t')
                            continue;
                        if (c == '#') {
                            /* Skip comment, to end of line.  */
                            do
                                c = getc(fp);
                            while (!(c == EOF || c == '\n'));
                            if (c == EOF)
                                break;
                            continue;
                        }
                        ungetc(c, fp);
                        if (fscanf(fp, "%50s %50s", buf1, buf2) < 2)
                            break;
                        l1 = strlen(buf1);
                        l2 = strlen(buf2);
                        old_res_ptr = res_ptr;
                        if (res_size == 0) {
                            res_size = l1 + 1 + l2 + 1;
                            res_ptr = (char *) malloc(res_size + 1);
                        } else {
                            res_size += l1 + 1 + l2 + 1;
                            res_ptr = (char *) realloc(res_ptr, res_size + 1);
                        }
                        if (res_ptr == NULL) {
                            /* Out of memory. */
                            res_size = 0;
                            free(old_res_ptr);
                            break;
                        }
                        strcpy(res_ptr + res_size - (l2 + 1) - (l1 + 1), buf1);
                        strcpy(res_ptr + res_size - (l2 + 1), buf2);
                    }
                    fclose(fp);
                    if (res_size == 0)
                        cp = "";
                    else {
                        *(res_ptr + res_size) = '\0';
                        cp = res_ptr;
                    }
                }
            }

            free(file_name);
        }

#else

# if defined DARWIN7
        /* To avoid the trouble of installing a file that is shared by many
           GNU packages -- many packaging systems have problems with this --,
           simply inline the aliases here.  */
        cp = "ISO8859-1" "\0" "ISO-8859-1" "\0"
             "ISO8859-2" "\0" "ISO-8859-2" "\0"
             "ISO8859-4" "\0" "ISO-8859-4" "\0"
             "ISO8859-5" "\0" "ISO-8859-5" "\0"
             "ISO8859-7" "\0" "ISO-8859-7" "\0"
             "ISO8859-9" "\0" "ISO-8859-9" "\0"
             "ISO8859-13" "\0" "ISO-8859-13" "\0"
             "ISO8859-15" "\0" "ISO-8859-15" "\0"
             "KOI8-R" "\0" "KOI8-R" "\0"
             "KOI8-U" "\0" "KOI8-U" "\0"
             "CP866" "\0" "CP866" "\0"
             "CP949" "\0" "CP949" "\0"
             "CP1131" "\0" "CP1131" "\0"
             "CP1251" "\0" "CP1251" "\0"
             "eucCN" "\0" "GB2312" "\0"
             "GB2312" "\0" "GB2312" "\0"
             "eucJP" "\0" "EUC-JP" "\0"
             "eucKR" "\0" "EUC-KR" "\0"
             "Big5" "\0" "BIG5" "\0"
             "Big5HKSCS" "\0" "BIG5-HKSCS" "\0"
             "GBK" "\0" "GBK" "\0"
             "GB18030" "\0" "GB18030" "\0"
             "SJIS" "\0" "SHIFT_JIS" "\0"
             "ARMSCII-8" "\0" "ARMSCII-8" "\0"
             "PT154" "\0" "PT154" "\0"
           /*"ISCII-DEV" "\0" "?" "\0"*/
             "*" "\0" "UTF-8" "\0";
# endif

# if defined VMS
        /* To avoid the troubles of an extra file charset.alias_vms in the
           sources of many GNU packages, simply inline the aliases here.  */
        /* The list of encodings is taken from the OpenVMS 7.3-1 documentation
           "Compaq C Run-Time Library Reference Manual for OpenVMS systems"
           section 10.7 "Handling Different Character Sets".  */
        cp = "ISO8859-1" "\0" "ISO-8859-1" "\0"
             "ISO8859-2" "\0" "ISO-8859-2" "\0"
             "ISO8859-5" "\0" "ISO-8859-5" "\0"
             "ISO8859-7" "\0" "ISO-8859-7" "\0"
             "ISO8859-8" "\0" "ISO-8859-8" "\0"
             "ISO8859-9" "\0" "ISO-8859-9" "\0"
             /* Japanese */
             "eucJP" "\0" "EUC-JP" "\0"
             "SJIS" "\0" "SHIFT_JIS" "\0"
             "DECKANJI" "\0" "DEC-KANJI" "\0"
             "SDECKANJI" "\0" "EUC-JP" "\0"
             /* Chinese */
             "eucTW" "\0" "EUC-TW" "\0"
             "DECHANYU" "\0" "DEC-HANYU" "\0"
             "DECHANZI" "\0" "GB2312" "\0"
             /* Korean */
             "DECKOREAN" "\0" "EUC-KR" "\0";
# endif

# if defined WINDOWS_NATIVE || defined __CYGWIN__
        /* To avoid the troubles of installing a separate file in the same
           directory as the DLL and of retrieving the DLL's directory at
           runtime, simply inline the aliases here.  */

        cp = "CP936" "\0" "GBK" "\0"
             "CP1361" "\0" "JOHAB" "\0"
             "CP20127" "\0" "ASCII" "\0"
             "CP20866" "\0" "KOI8-R" "\0"
             "CP20936" "\0" "GB2312" "\0"
             "CP21866" "\0" "KOI8-RU" "\0"
             "CP28591" "\0" "ISO-8859-1" "\0"
             "CP28592" "\0" "ISO-8859-2" "\0"
             "CP28593" "\0" "ISO-8859-3" "\0"
             "CP28594" "\0" "ISO-8859-4" "\0"
             "CP28595" "\0" "ISO-8859-5" "\0"
             "CP28596" "\0" "ISO-8859-6" "\0"
             "CP28597" "\0" "ISO-8859-7" "\0"
             "CP28598" "\0" "ISO-8859-8" "\0"
             "CP28599" "\0" "ISO-8859-9" "\0"
             "CP28605" "\0" "ISO-8859-15" "\0"
             "CP38598" "\0" "ISO-8859-8" "\0"
             "CP51932" "\0" "EUC-JP" "\0"
             "CP51936" "\0" "GB2312" "\0"
             "CP51949" "\0" "EUC-KR" "\0"
             "CP51950" "\0" "EUC-TW" "\0"
             "CP54936" "\0" "GB18030" "\0"
             "CP65001" "\0" "UTF-8" "\0";
# endif
# if defined OS2
        /* To avoid the troubles of installing a separate file in the same
           directory as the DLL and of retrieving the DLL's directory at
           runtime, simply inline the aliases here.  */

        /* The list of encodings is taken from "List of OS/2 Codepages"
           by Alex Taylor:
           <http://altsan.org/os2/toolkits/uls/index.html#codepages>.
           See also "IBM Globalization - Code page identifiers":
           <https://www-01.ibm.com/software/globalization/cp/cp_cpgid.html>.  */
        cp = "CP813" "\0" "ISO-8859-7" "\0"
             "CP878" "\0" "KOI8-R" "\0"
             "CP819" "\0" "ISO-8859-1" "\0"
             "CP912" "\0" "ISO-8859-2" "\0"
             "CP913" "\0" "ISO-8859-3" "\0"
             "CP914" "\0" "ISO-8859-4" "\0"
             "CP915" "\0" "ISO-8859-5" "\0"
             "CP916" "\0" "ISO-8859-8" "\0"
             "CP920" "\0" "ISO-8859-9" "\0"
             "CP921" "\0" "ISO-8859-13" "\0"
             "CP923" "\0" "ISO-8859-15" "\0"
             "CP954" "\0" "EUC-JP" "\0"
             "CP964" "\0" "EUC-TW" "\0"
             "CP970" "\0" "EUC-KR" "\0"
             "CP1089" "\0" "ISO-8859-6" "\0"
             "CP1208" "\0" "UTF-8" "\0"
             "CP1381" "\0" "GB2312" "\0"
             "CP1386" "\0" "GBK" "\0"
             "CP3372" "\0" "EUC-JP" "\0";
# endif
#endif

        charset_aliases = cp;
    }

    return cp;
}

const char *locale_charset(void) {
    const char *codeset;
    const char *aliases;

#if !(defined WINDOWS_NATIVE || defined OS2)

# if HAVE_LANGINFO_CODESET

# else

    /* On old systems which lack it, use setlocale or getenv.  */
    const char *locale = NULL;

    /* But most old systems don't have a complete set of locales.  Some
       (like SunOS 4 or DJGPP) have only the C locale.  Therefore we don't
       use setlocale here; it would return "C" when it doesn't support the
       locale name the user has set.  */
#  if 0

#  endif
    if (locale == NULL || locale[0] == '\0') {
        locale = getenv("LC_ALL");
        if (locale == NULL || locale[0] == '\0') {
            locale = getenv("LC_CTYPE");
            if (locale == NULL || locale[0] == '\0')
                locale = getenv("LANG");
        }
    }

    /* On some old systems, one used to set locale = "iso8859_1". On others,
       you set it to "language_COUNTRY.charset". In any case, we resolve it
       through the charset.alias file.  */
    codeset = locale;

# endif

#elif defined WINDOWS_NATIVE

#endif

    if (codeset == NULL)
        /* The canonical name cannot be determined.  */
        codeset = "";

    /* Resolve alias. */
    for (aliases = get_charset_aliases();
         *aliases != '\0';
         aliases += strlen(aliases) + 1, aliases += strlen(aliases) + 1)
        if (strcmp(codeset, aliases) == 0
            || (aliases[0] == '*' && aliases[1] == '\0')) {
            codeset = aliases + strlen(aliases) + 1;
            break;
        }
    if (codeset[0] == '\0')
        codeset = "ASCII";

    return codeset;
}

int c_tolower(int c) {
    switch (c) {
        _C_CTYPE_UPPER:
            return c - 'A' + 'a';
        default:
            return c;
    }
}

#define STRCASEEQ(s1, s2, s20, s21, s22, s23, s24, s25, s26, s27, s28) \
  (c_strcasecmp (s1, s2) == 0)

int c_strcasecmp(const char *s1, const char *s2) {
    register const unsigned char *p1 = (const unsigned char *) s1;
    register const unsigned char *p2 = (const unsigned char *) s2;
    unsigned char c1, c2;

    if (p1 == p2)
        return 0;

    do {
        c1 = c_tolower(*p1);
        c2 = c_tolower(*p2);

        if (c1 == '\0')
            break;

        ++p1;
        ++p2;
    } while (c1 == c2);

    if (UCHAR_MAX <= INT_MAX)
        return c1 - c2;
    else
        /* On machines where 'char' and 'int' are types of the same size, the
           difference of two 'unsigned char' values - including the sign bit -
           doesn't fit in an 'int'.  */
        return (c1 > c2 ? 1 : c1 < c2 ? -1 : 0);
}

static char const *gettext_quote(char const *msgid, enum quoting_style s) {
    char const *translation = _(msgid);
    char const *locale_code;

    if (translation != msgid)
        return translation;

    /* For UTF-8 and GB-18030, use single quotes U+2018 and U+2019.
       Here is a list of other locales that include U+2018 and U+2019:

          ISO-8859-7   0xA1                 KOI8-T       0x91
          CP869        0x8B                 CP874        0x91
          CP932        0x81 0x65            CP936        0xA1 0xAE
          CP949        0xA1 0xAE            CP950        0xA1 0xA5
          CP1250       0x91                 CP1251       0x91
          CP1252       0x91                 CP1253       0x91
          CP1254       0x91                 CP1255       0x91
          CP1256       0x91                 CP1257       0x91
          EUC-JP       0xA1 0xC6            EUC-KR       0xA1 0xAE
          EUC-TW       0xA1 0xE4            BIG5         0xA1 0xA5
          BIG5-HKSCS   0xA1 0xA5            EUC-CN       0xA1 0xAE
          GBK          0xA1 0xAE            Georgian-PS  0x91
          PT154        0x91

       None of these is still in wide use; using iconv is overkill.  */
    locale_code = locale_charset();
    if (STRCASEEQ(locale_code, "UTF-8", 'U', 'T', 'F', '-', '8', 0, 0, 0, 0))
        return msgid[0] == '`' ? "\xe2\x80\x98" : "\xe2\x80\x99";
    if (STRCASEEQ(locale_code, "GB18030", 'G', 'B', '1', '8', '0', '3', '0', 0, 0))
        return msgid[0] == '`' ? "\xa1\ae" : "\xa1\xaf";

    return (s == clocale_quoting_style ? "\"" : "'");
}

int mbsinit(const mbstate_t *ps) {
# if (defined _WIN32 || defined __WIN32__) && !defined __CYGWIN__
    /* Native Windows.  */
#  ifdef __MINGW32__
  /* On mingw, 'mbstate_t' is defined as 'int'.  */
  return ps == NULL || *ps == 0;
#  else
  /* MSVC defines 'mbstate_t' as an 8-byte struct; the first 4-bytes matter.  */
  return ps == NULL || *(const unsigned int *)ps == 0;
#  endif
# else
    /* Minix, HP-UX 11.00, Solaris 2.6, Interix, ...  */
    /* Maybe this definition works, maybe not...  */
    return ps == NULL || *(const char *) ps == 0;
# endif
}

static size_t quotearg_buffer_restyled(char *buffer, size_t buffersize,
                                       char const *arg, size_t argsize,
                                       enum quoting_style quoting_style, int flags,
                                       unsigned int const *quote_these_too,
                                       char const *left_quote,
                                       char const *right_quote) {
    size_t i;
    size_t len = 0;
    size_t orig_buffersize = 0;
    char const *quote_string = 0;
    size_t quote_string_len = 0;
    bool backslash_escapes = false;
    bool unibyte_locale = MB_CUR_MAX == 1;
    bool elide_outer_quotes = (flags & QA_ELIDE_OUTER_QUOTES) != 0;
    bool pending_shell_escape_end = false;
    bool encountered_single_quote = false;
    bool all_c_and_shell_quote_compat = true;

#define STORE(c) \
    do \
      { \
        if (len < buffersize) \
          buffer[len] = (c); \
        len++; \
      } \
    while (0)

#define START_ESC() \
    do \
      { \
        if (elide_outer_quotes) \
          goto force_outer_quoting_style; \
        escaping = true; \
        if (quoting_style == shell_always_quoting_style \
            && ! pending_shell_escape_end) \
          { \
            STORE ('\''); \
            STORE ('$'); \
            STORE ('\''); \
            pending_shell_escape_end = true; \
          } \
        STORE ('\\'); \
      } \
    while (0)

#define END_ESC() \
    do \
      { \
        if (pending_shell_escape_end && ! escaping) \
          { \
            STORE ('\''); \
            STORE ('\''); \
            pending_shell_escape_end = false; \
          } \
      } \
    while (0)

    process_input:

    switch (quoting_style) {
        case c_maybe_quoting_style:
            quoting_style = c_quoting_style;
            elide_outer_quotes = true;
            FALLTHROUGH;
        case c_quoting_style:
            if (!elide_outer_quotes)
                STORE ('"');
            backslash_escapes = true;
            quote_string = "\"";
            quote_string_len = 1;
            break;

        case escape_quoting_style:
            backslash_escapes = true;
            elide_outer_quotes = false;
            break;

        case locale_quoting_style:
        case clocale_quoting_style:
        case custom_quoting_style: {
            if (quoting_style != custom_quoting_style) {
                /* TRANSLATORS:
                   Get translations for open and closing quotation marks.
                   The message catalog should translate "`" to a left
                   quotation mark suitable for the locale, and similarly for
                   "'".  For example, a French Unicode local should translate
                   these to U+00AB (LEFT-POINTING DOUBLE ANGLE
                   QUOTATION MARK), and U+00BB (RIGHT-POINTING DOUBLE ANGLE
                   QUOTATION MARK), respectively.

                   If the catalog has no translation, we will try to
                   use Unicode U+2018 (LEFT SINGLE QUOTATION MARK) and
                   Unicode U+2019 (RIGHT SINGLE QUOTATION MARK).  If the
                   current locale is not Unicode, locale_quoting_style
                   will quote 'like this', and clocale_quoting_style will
                   quote "like this".  You should always include translations
                   for "`" and "'" even if U+2018 and U+2019 are appropriate
                   for your locale.

                   If you don't know what to put here, please see
                   <https://en.wikipedia.org/wiki/Quotation_marks_in_other_languages>
                   and use glyphs suitable for your language.  */
                left_quote = gettext_quote(N_("`"), quoting_style);
                right_quote = gettext_quote(N_("'"), quoting_style);
            }
            if (!elide_outer_quotes)
                for (quote_string = left_quote; *quote_string; quote_string++)
                    STORE (*quote_string);
            backslash_escapes = true;
            quote_string = right_quote;
            quote_string_len = strlen(quote_string);
        }
            break;

        case shell_escape_quoting_style:
            backslash_escapes = true;
            FALLTHROUGH;
        case shell_quoting_style:
            elide_outer_quotes = true;
            FALLTHROUGH;
        case shell_escape_always_quoting_style:
            if (!elide_outer_quotes)
                backslash_escapes = true;
            FALLTHROUGH;
        case shell_always_quoting_style:
            quoting_style = shell_always_quoting_style;
            if (!elide_outer_quotes)
                STORE ('\'');
            quote_string = "'";
            quote_string_len = 1;
            break;

        case literal_quoting_style:
            elide_outer_quotes = false;
            break;

        default:
            abort();
    }

    for (i = 0; !(argsize == SIZE_MAX ? arg[i] == '\0' : i == argsize); i++) {
        unsigned char c;
        unsigned char esc;
        bool is_right_quote = false;
        bool escaping = false;
        bool c_and_shell_quote_compat = false;

        if (backslash_escapes
            && quoting_style != shell_always_quoting_style
            && quote_string_len
            && (i + quote_string_len
                <= (argsize == SIZE_MAX && 1 < quote_string_len
                    /* Use strlen only if we must: when argsize is SIZE_MAX,
                       and when the quote string is more than 1 byte long.
                       If we do call strlen, save the result.  */
                    ? (argsize = strlen(arg)) : argsize))
            && memcmp(arg + i, quote_string, quote_string_len) == 0) {
            if (elide_outer_quotes)
                goto force_outer_quoting_style;
            is_right_quote = true;
        }

        c = arg[i];
        switch (c) {
            case '\0':
                if (backslash_escapes) {
                    START_ESC ();
                    /* If quote_string were to begin with digits, we'd need to
                       test for the end of the arg as well.  However, it's
                       hard to imagine any locale that would use digits in
                       quotes, and set_custom_quoting is documented not to
                       accept them.  Use only a single \0 with shell-escape
                       as currently digits are not printed within $'...'  */
                    if (quoting_style != shell_always_quoting_style
                        && i + 1 < argsize && '0' <= arg[i + 1] && arg[i + 1] <= '9') {
                        STORE ('0');
                        STORE ('0');
                    }
                    c = '0';
                    /* We don't have to worry that this last '0' will be
                       backslash-escaped because, again, quote_string should
                       not start with it and because quote_these_too is
                       documented as not accepting it.  */
                } else if (flags & QA_ELIDE_NULL_BYTES)
                    continue;
                break;

            case '?':
                switch (quoting_style) {
                    case shell_always_quoting_style:
                        if (elide_outer_quotes)
                            goto force_outer_quoting_style;
                        break;

                    case c_quoting_style:
                        if ((flags & QA_SPLIT_TRIGRAPHS)
                            && i + 2 < argsize && arg[i + 1] == '?')
                            switch (arg[i + 2]) {
                                case '!':
                                case '\'':
                                case '(':
                                case ')':
                                case '-':
                                case '/':
                                case '<':
                                case '=':
                                case '>':
                                    /* Escape the second '?' in what would otherwise be
                                       a trigraph.  */
                                    if (elide_outer_quotes)
                                        goto force_outer_quoting_style;
                                    c = arg[i + 2];
                                    i += 2;
                                    STORE ('?');
                                    STORE ('"');
                                    STORE ('"');
                                    STORE ('?');
                                    break;

                                default:
                                    break;
                            }
                        break;

                    default:
                        break;
                }
                break;

            case '\a':
                esc = 'a';
                goto c_escape;
            case '\b':
                esc = 'b';
                goto c_escape;
            case '\f':
                esc = 'f';
                goto c_escape;
            case '\n':
                esc = 'n';
                goto c_and_shell_escape;
            case '\r':
                esc = 'r';
                goto c_and_shell_escape;
            case '\t':
                esc = 't';
                goto c_and_shell_escape;
            case '\v':
                esc = 'v';
                goto c_escape;
            case '\\':
                esc = c;
                /* Never need to escape '\' in shell case.  */
                if (quoting_style == shell_always_quoting_style) {
                    if (elide_outer_quotes)
                        goto force_outer_quoting_style;
                    goto store_c;
                }

                /* No need to escape the escape if we are trying to elide
                   outer quotes and nothing else is problematic.  */
                if (backslash_escapes && elide_outer_quotes && quote_string_len)
                    goto store_c;

            c_and_shell_escape:
                if (quoting_style == shell_always_quoting_style
                    && elide_outer_quotes)
                    goto force_outer_quoting_style;
                /* fall through */
            c_escape:
                if (backslash_escapes) {
                    c = esc;
                    goto store_escape;
                }
                break;

            case '{':
            case '}': /* sometimes special if isolated */
                if (!(argsize == SIZE_MAX ? arg[1] == '\0' : argsize == 1))
                    break;
                FALLTHROUGH;
            case '#':
            case '~':
                if (i != 0)
                    break;
                FALLTHROUGH;
            case ' ':
                c_and_shell_quote_compat = true;
                FALLTHROUGH;
            case '!': /* special in bash */
            case '"':
            case '$':
            case '&':
            case '(':
            case ')':
            case '*':
            case ';':
            case '<':
            case '=': /* sometimes special in 0th or (with "set -k") later args */
            case '>':
            case '[':
            case '^': /* special in old /bin/sh, e.g. SunOS 4.1.4 */
            case '`':
            case '|':
                /* A shell special character.  In theory, '$' and '`' could
                   be the first bytes of multibyte characters, which means
                   we should check them with mbrtowc, but in practice this
                   doesn't happen so it's not worth worrying about.  */
                if (quoting_style == shell_always_quoting_style
                    && elide_outer_quotes)
                    goto force_outer_quoting_style;
                break;

            case '\'':
                encountered_single_quote = true;
                c_and_shell_quote_compat = true;
                if (quoting_style == shell_always_quoting_style) {
                    if (elide_outer_quotes)
                        goto force_outer_quoting_style;

                    if (buffersize && !orig_buffersize) {
                        /* Just scan string to see if supports a more concise
                           representation, rather than writing a longer string
                           but returning the length of the more concise form.  */
                        orig_buffersize = buffersize;
                        buffersize = 0;
                    }

                    STORE ('\'');
                    STORE ('\\');
                    STORE ('\'');
                    pending_shell_escape_end = false;
                }
                break;

            case '%':
            case '+':
            case ',':
            case '-':
            case '.':
            case '/':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case ':':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case ']':
            case '_':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
                /* These characters don't cause problems, no matter what the
                   quoting style is.  They cannot start multibyte sequences.
                   A digit or a special letter would cause trouble if it
                   appeared at the beginning of quote_string because we'd then
                   escape by prepending a backslash.  However, it's hard to
                   imagine any locale that would use digits or letters as
                   quotes, and set_custom_quoting is documented not to accept
                   them.  Also, a digit or a special letter would cause
                   trouble if it appeared in quote_these_too, but that's also
                   documented as not accepting them.  */
                c_and_shell_quote_compat = true;
                break;

            default:
                /* If we have a multibyte sequence, copy it until we reach
                   its end, find an error, or come back to the initial shift
                   state.  For C-like styles, if the sequence has
                   unprintable characters, escape the whole sequence, since
                   we can't easily escape single characters within it.  */
            {
                /* Length of multibyte sequence found so far.  */
                size_t m;

                bool printable;

                if (unibyte_locale) {
                    m = 1;
                    printable = isprint(c) != 0;
                } else {
                    mbstate_t mbstate;
                    memset(&mbstate, 0, sizeof mbstate);

                    m = 0;
                    printable = true;
                    if (argsize == SIZE_MAX)
                        argsize = strlen(arg);

                    do {
                        wchar_t w;
                        size_t bytes = mbrtowc(&w, &arg[i + m], argsize - (i + m), &mbstate);
                        if (bytes == 0)
                            break;
                        else if (bytes == (size_t) -1) {
                            printable = false;
                            break;
                        } else if (bytes == (size_t) -2) {
                            printable = false;
                            while (i + m < argsize && arg[i + m])
                                m++;
                            break;
                        } else {
                            /* Work around a bug with older shells that "see" a '\'
                               that is really the 2nd byte of a multibyte character.
                               In practice the problem is limited to ASCII
                               chars >= '@' that are shell special chars.  */
                            if ('[' == 0x5b && elide_outer_quotes
                                && quoting_style == shell_always_quoting_style) {
                                size_t j;
                                for (j = 1; j < bytes; j++)
                                    switch (arg[i + m + j]) {
                                        case '[':
                                        case '\\':
                                        case '^':
                                        case '`':
                                        case '|':
                                            goto force_outer_quoting_style;

                                        default:
                                            break;
                                    }
                            }

                            if (!iswprint(w))
                                printable = false;
                            m += bytes;
                        }
                    } while (!mbsinit(&mbstate));
                }

                c_and_shell_quote_compat = printable;

                if (1 < m || (backslash_escapes && !printable)) {
                    /* Output a multibyte sequence, or an escaped
                       unprintable unibyte character.  */
                    size_t ilim = i + m;

                    for (;;) {
                        if (backslash_escapes && !printable) {
                            START_ESC ();
                            STORE ('0' + (c >> 6));
                            STORE ('0' + ((c >> 3) & 7));
                            c = '0' + (c & 7);
                        } else if (is_right_quote) {
                            STORE ('\\');
                            is_right_quote = false;
                        }
                        if (ilim <= i + 1)
                            break;
                        END_ESC ();
                        STORE (c);
                        c = arg[++i];
                    }

                    goto store_c;
                }
            }
        }

        if (!(((backslash_escapes && quoting_style != shell_always_quoting_style)
               || elide_outer_quotes)
              && quote_these_too
              && quote_these_too[c / INT_BITS] >> (c % INT_BITS) & 1)
            && !is_right_quote)
            goto store_c;

        store_escape:
        START_ESC ();

        store_c:
        END_ESC ();
        STORE (c);

        if (!c_and_shell_quote_compat)
            all_c_and_shell_quote_compat = false;
    }

    if (len == 0 && quoting_style == shell_always_quoting_style
        && elide_outer_quotes)
        goto force_outer_quoting_style;

    /* Single shell quotes (') are commonly enough used as an apostrophe,
       that we attempt to minimize the quoting in this case.  Note itʼs
       better to use the apostrophe modifier "\u02BC" if possible, as that
       renders better and works with the word match regex \W+ etc.  */
    if (quoting_style == shell_always_quoting_style && !elide_outer_quotes
        && encountered_single_quote) {
        if (all_c_and_shell_quote_compat)
            return quotearg_buffer_restyled(buffer, orig_buffersize, arg, argsize,
                                            c_quoting_style,
                                            flags, quote_these_too,
                                            left_quote, right_quote);
        else if (!buffersize && orig_buffersize) {
            /* Disable read-only scan, and reprocess to write quoted string.  */
            buffersize = orig_buffersize;
            len = 0;
            goto process_input;
        }
    }

    if (quote_string && !elide_outer_quotes)
        for (; *quote_string; quote_string++)
            STORE (*quote_string);

    if (len < buffersize)
        buffer[len] = '\0';
    return len;

    force_outer_quoting_style:
    /* Don't reuse quote_these_too, since the addition of outer quotes
       sufficiently quotes the specified characters.  */
    if (quoting_style == shell_always_quoting_style && backslash_escapes)
        quoting_style = shell_escape_always_quoting_style;
    return quotearg_buffer_restyled(buffer, buffersize, arg, argsize,
                                    quoting_style,
                                    flags & ~QA_ELIDE_OUTER_QUOTES, NULL,
                                    left_quote, right_quote);
}

/* The quoting option used by the functions of quote.h.  */
struct quoting_options quote_quoting_options =
        {
                locale_quoting_style,
                0,
                {0},
                NULL, NULL
        };

static char *quotearg_n_options(int n, char const *arg, size_t argsize,
                                struct quoting_options const *options) {
    int e = errno;

    struct slotvec *sv = slotvec;

    if (n < 0)
        abort();

    if (nslots <= n) {
        bool preallocated = (sv == &slotvec0);
        int nmax = MIN(INT_MAX, MIN(PTRDIFF_MAX, SIZE_MAX) / sizeof *sv) - 1;

        if (nmax < n)
            xalloc_die();

        slotvec = sv = (struct slotvec *) xrealloc(preallocated ? NULL : sv, (n + 1) * sizeof *sv);
        if (preallocated)
            *sv = slotvec0;
        memset(sv + nslots, 0, (n + 1 - nslots) * sizeof *sv);
        nslots = n + 1;
    }

    {
        size_t size = sv[n].size;
        char *val = sv[n].val;
        /* Elide embedded null bytes since we don't return a size.  */
        int flags = options->flags | QA_ELIDE_NULL_BYTES;
        size_t qsize = quotearg_buffer_restyled(val, size, arg, argsize,
                                                options->style, flags,
                                                options->quote_these_too,
                                                options->left_quote,
                                                options->right_quote);

        if (size <= qsize) {
            sv[n].size = size = qsize + 1;
            if (val != slot0)
                free(val);
            sv[n].val = val = xcharalloc(size);
            quotearg_buffer_restyled(val, size, arg, argsize, options->style,
                                     flags, options->quote_these_too,
                                     options->left_quote,
                                     options->right_quote);
        }

        errno = e;
        return val;
    }
}

char const *quote_n_mem(int n, char const *arg, size_t argsize) {
    return quotearg_n_options(n, arg, argsize, &quote_quoting_options);
}

char const *quote_mem(char const *arg, size_t argsize) {
    return quote_n_mem(0, arg, argsize);
}

char const *quote_n(int n, char const *arg) {
    return quote_n_mem(n, arg, SIZE_MAX);
}

char const *quote(char const *arg) {
    return quote_n(0, arg);
}

void redirect_output(bool to_file, const char *signal_name) {
    if (to_file && logfp != filelogfp) {
        if (signal_name) {
            fprintf(stderr, "\n%s received.", signal_name);
        }
        if (!filelogfp) {
            filelogfp = unique_create(DEFAULT_LOGFILE, false, &logfile);
            if (filelogfp) {
                fprintf(stderr, _("\nRedirecting output to %s.\n"), quote(logfile));
                /* Store signal name to tell wget it's permanent redirect to log file */
                redirect_request_signal_name = signal_name;
                logfp = filelogfp;
                /* Dump the context output to the newly opened log.  */
                log_dump_context();
            } else {
                /* Eek!  Opening the alternate log file has failed.  Nothing we
                  can do but disable printing completely. */
                fprintf(stderr, _("%s: %s; disabling logging.\n"),
                        (logfile) ? logfile : DEFAULT_LOGFILE, strerror(errno));
                inhibit_logging = true;
            }
        } else {
            fprintf(stderr, _("\nRedirecting output to %s.\n"), quote(logfile));
            logfp = filelogfp;
            log_dump_context();
        }
    } else if (!to_file && logfp != stdlogfp) {
        logfp = stdlogfp;
        log_dump_context();
    }
}

static void check_redirect_output(void) {
#ifndef WINDOWS
    /* If it was redirected already to log file by SIGHUP, SIGUSR1 or -o parameter,
     * it was permanent.
     * If there was no SIGHUP or SIGUSR1 and shell is interactive
     * we check if process is fg or bg before every line is printed.*/
    if (!redirect_request_signal_name && shell_is_interactive && !global_options.lfilename) {
        pid_t foreground_pgrp = tcgetpgrp(STDIN_FILENO);

        if (foreground_pgrp != -1 && foreground_pgrp != getpgrp()) {
            /* Process backgrounded */
            redirect_output(true, NULL);
        } else {
            /* Process foregrounded */
            redirect_output(false, NULL);
        }
    }
#endif /* WINDOWS */
}

static void free_log_line(int num) {
    struct log_ln *ln = log_lines + num;
    xfree (ln->malloced_line);
    ln->content = NULL;
}

char *strdupdelim(const char *beg, const char *end) {
    if (beg && beg <= end) {
        char *res = (char *) xmalloc(end - beg + 1);
        memcpy(res, beg, end - beg);
        res[end - beg] = '\0';
        return res;
    }

    return xstrdup("");
}

static void saved_append_1(const char *start, const char *end) {
    int len = end - start;
    if (!len)
        return;

    /* First, check whether we need to append to an existing line or to
       create a new one.  */
    if (!trailing_line) {
        /* Create a new line. */
        struct log_ln *ln;

        if (log_line_current == -1)
            log_line_current = 0;
        else
            free_log_line(log_line_current);
        ln = log_lines + log_line_current;
        if (len > STATIC_LENGTH) {
            ln->malloced_line = strdupdelim(start, end);
            ln->content = ln->malloced_line;
        } else {
            memcpy(ln->static_line, start, len);
            ln->static_line[len] = '\0';
            ln->content = ln->static_line;
        }
    } else {
        /* Append to the last line.  If the line is malloc'ed, we just
           call realloc and append the new string.  If the line is
           static, we have to check whether appending the new string
           would make it exceed STATIC_LENGTH characters, and if so,
           convert it to malloc(). */
        struct log_ln *ln = log_lines + log_line_current;
        if (ln->malloced_line) {
            /* Resize malloc'ed line and append. */
            int old_len = strlen(ln->malloced_line);
            ln->malloced_line = (char *) xrealloc(ln->malloced_line, old_len + len + 1);
            memcpy(ln->malloced_line + old_len, start, len);
            ln->malloced_line[old_len + len] = '\0';
            /* might have changed due to realloc */
            ln->content = ln->malloced_line;
        } else {
            int old_len = strlen(ln->static_line);
            if (old_len + len > STATIC_LENGTH) {
                /* Allocate memory and concatenate the old and the new
                   contents. */
                ln->malloced_line = (char *) xmalloc(old_len + len + 1);
                memcpy(ln->malloced_line, ln->static_line, old_len);
                memcpy(ln->malloced_line + old_len, start, len);
                ln->malloced_line[old_len + len] = '\0';
                ln->content = ln->malloced_line;
            } else {
                /* Just append to the old, statically allocated
                   contents.  */
                memcpy(ln->static_line + old_len, start, len);
                ln->static_line[old_len + len] = '\0';
                ln->content = ln->static_line;
            }
        }
    }
    trailing_line = !(end[-1] == '\n');
    if (!trailing_line)
        ROT_ADVANCE (log_line_current);
}

static void saved_append(const char *s) {
    while (*s) {
        const char *end = strchr(s, '\n');
        if (!end)
            end = s + strlen(s);
        else
            ++end;
        saved_append_1(s, end);
        s = end;
    }
}

void logflush(void) {
    FILE *fp = get_log_fp();
    FILE *warcfp = get_warc_log_fp();
    if (fp) {
/* 2005-10-25 SMS.
   On VMS, flush only for a terminal.  See note at FPUTS macro, above.
*/
#ifdef __VMS
        if (isatty( fileno( fp)))
          {
            fflush (fp);
          }
#else /* def __VMS */
        fflush(fp);
#endif /* def __VMS [else] */
    }

    if (warcfp != NULL)
        fflush(warcfp);

    needs_flushing = false;
}

static bool log_vprintf_internal(struct logvprintf_state *state, const char *fmt, va_list args) {
    char smallmsg[128];
    char *write_ptr = smallmsg;
    int available_size = sizeof(smallmsg);
    int numwritten;
    FILE *fp = get_log_fp();
    FILE *warcfp = get_warc_log_fp();

    if (!save_context_p && warcfp == NULL) {
        /* In the simple case just call vfprintf(), to avoid needless
           allocation and games with vsnprintf(). */
        vfprintf(fp, fmt, args);
        goto flush;
    }

    if (state->allocated != 0) {
        write_ptr = state->bigmsg;
        available_size = state->allocated;
    }

    /* The GNU coding standards advise not to rely on the return value
       of sprintf().  However, vsnprintf() is a relatively new function
       missing from legacy systems.  Therefore I consider it safe to
       assume that its return value is meaningful.  On the systems where
       vsnprintf() is not available, we use the implementation from
       snprintf.c which does return the correct value.  */
    numwritten = vsnprintf(write_ptr, available_size, fmt, args);

    /* vsnprintf() will not step over the limit given by available_size.
       If it fails, it returns either -1 (older implementations) or the
       number of characters (not counting the terminating \0) that
       *would have* been written if there had been enough room (C99).
       In the former case, we double available_size and malloc to get a
       larger buffer, and try again.  In the latter case, we use the
       returned information to build a buffer of the correct size.  */

    if (numwritten == -1) {
        /* Writing failed, and we don't know the needed size.  Try
           again with doubled size. */
        int newsize = available_size << 1;
        state->bigmsg = (char *) xrealloc(state->bigmsg, newsize);
        state->allocated = newsize;
        return false;
    } else if (numwritten >= available_size) {
        /* Writing failed, but we know exactly how much space we
           need. */
        int newsize = numwritten + 1;
        state->bigmsg = (char *) xrealloc(state->bigmsg, newsize);
        state->allocated = newsize;
        return false;
    }

    /* Writing succeeded. */
    if (save_context_p)
        saved_append(write_ptr);
    FPUTS (write_ptr, fp);
    if (warcfp != NULL)
        FPUTS (write_ptr, warcfp);
    xfree (state->bigmsg);

    flush:
    if (flush_log_p)
        logflush();
    else
        needs_flushing = true;

    return true;
}

void logprintf(enum log_options o, const char *fmt, ...) {
    va_list args;
    struct logvprintf_state lpstate;
    bool done;
    int errno_saved = errno;

    check_redirect_output();
    errno = errno_saved;
    if (inhibit_logging)
        return;
    CHECK_VERBOSE(o);

    xzero(lpstate);
    errno = 0;
    do {
        va_start (args, fmt);
        done = log_vprintf_internal(&lpstate, fmt, args);
        va_end (args);

        if (done && errno == EPIPE)
            exit(WGET_EXIT_GENERIC_ERROR);
    } while (!done);

    errno = errno_saved;
}

#endif //MYTEST_UTILS_HPP
