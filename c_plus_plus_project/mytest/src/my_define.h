#ifndef MYTEST_MY_DEFINE_H
#define MYTEST_MY_DEFINE_H

#include "my_head.h"

#define SIZEOF_LONG 8
#define SIZEOF_LONG_LONG 8

typedef __time_t time_t;

#ifdef WINDOWS
/* nothing to do, see mswindows.h */
#elif SIZEOF_LONG >= 8
/* long is large enough, so use it. */
typedef long wgint;
#define SIZEOF_WGINT SIZEOF_LONG
#elif SIZEOF_LONG_LONG >= 8
/* long long is large enough and available, use that */
  typedef long long wgint;
#define SIZEOF_WGINT SIZEOF_LONG_LONG
#elif HAVE_INT64_T
typedef int64_t wgint;
#define SIZEOF_WGINT 8
#elif SIZEOF_OFF_T >= 8
/* In case off_t is typedeffed to a large non-standard type that our
     tests don't find. */
  typedef off_t wgint;
#define SIZEOF_WGINT SIZEOF_OFF_T
#else
/* Fall back to using long, which is always available and in most
   cases large enough. */
typedef long wgint;
#define SIZEOF_WGINT SIZEOF_LONG
#endif

#define HAVE_SOCKADDR_IN6_SCOPE_ID 1
#define HAVE_LIBZ 1
#define ENABLE_IPV6 1
#define HAVE_LIBPCRE 1
#define HAVE_LIBGNUTLS 1
/* Is OpenSSL or GNUTLS available? */
#if defined HAVE_LIBSSL || defined HAVE_LIBSSL32 || defined HAVE_LIBGNUTLS
#define HAVE_SSL
#define HAVE_HSTS /* There's no sense in enabling HSTS without SSL */
#endif

#define _(STRING) gettext(STRING)

#if SIZEOF_WGINT >= 8
/* just use wgint */
typedef wgint SUM_SIZE_INT;
#else
/* On systems without LFS, use double, which buys us integers up to 2^53. */
typedef double SUM_SIZE_INT;
#endif

struct cell {
    void *key;
    void *value;
};

typedef unsigned long (*hashfun_t)(const void *);

typedef int (*testfun_t)(const void *, const void *);

struct hash_table {
    hashfun_t hash_function;
    testfun_t test_function;

    struct cell *cells;           /* contiguous array of cells. */
    int size;                     /* size of the array. */

    int count;                    /* number of occupied entries. */
    int resize_threshold;         /* after size exceeds this number of
                                   entries, resize the table.  */
    int prime_offset;             /* the offset of the current prime in
                                   the prime table. */
};

struct hsts_store {
    struct hash_table *table;
    time_t last_mtime;
    bool changed;
};
typedef struct hsts_store *hsts_store_t;

/* Standard file descriptors.  */
#define    STDIN_FILENO    0    /* Standard input.  */
#define    STDOUT_FILENO    1    /* Standard output.  */
#define    STDERR_FILENO    2    /* Standard error output.  */

#ifdef __VMS
#define FPUTS( s, f) fprintf( (f), "%s", (s))
#else /* def __VMS */
#define FPUTS(s, f) fputs( (s), (f))
#endif /* def __VMS [else] */

/* The file descriptor used for logging.  This is NULL before log_init
   is called; logging functions log to stderr then.  log_init sets it
   either to stderr or to a file pointer obtained from fopen().  If
   logging is inhibited, logfp is set back to NULL. */
static FILE *logfp;

/* Descriptor of the stdout|stderr */
static FILE *stdlogfp;

/* Descriptor of the wget.log* file (if created) */
static FILE *filelogfp;

/* Name of log file */
static char *logfile;

/* Is interactive shell ? */
static int shell_is_interactive;

/* A second file descriptor pointing to the temporary log file for the
   WARC writer.  If WARC writing is disabled, this is NULL.  */
static FILE *warclogfp;

/* If true, it means logging is inhibited, i.e. nothing is printed or
   stored.  */
static bool inhibit_logging;

/* Whether the last output lines are stored for use as context.  */
static bool save_context_p;

/* Whether the log is flushed after each command. */
static bool flush_log_p = true;

/* Whether any output has been received while flush_log_p was 0. */
static bool needs_flushing;

#define SAVED_LOG_LINES 24
#define STATIC_LENGTH 128
#define DEFAULT_LOGFILE "wget-log"

#define HAVE_DECL_PROGRAM_INVOCATION_NAME 1
#define HAVE_DECL_PROGRAM_INVOCATION_SHORT_NAME 1
#define HAVE_DECL_PROGRAM_INVOCATION_NAME 1
#define HAVE_DECL___ARGV 0
#define GNULIB_STRERROR_R_POSIX 1
#define HAVE_STRERROR_R 1

extern int volatile exit_failure;
int volatile exit_failure = EXIT_FAILURE;
#define program_name getprogname()

/* If NULL, error will flush stdout, then print on stderr the program
   name, a colon and a space.  Otherwise, error will call this
   function without parameters instead.  */
void (*error_print_progname)(void);

extern unsigned int error_message_count;
extern int error_one_per_line;

#define STACKBUF_LEN 256
#define strerror_override(ignored) NULL

#define TYPE_SIGNED(t) (! ((t) 0 < (t) -1))
#define _GL_SIGNED_TYPE_OR_EXPR(t) TYPE_SIGNED (__typeof__ (t))
#define CHAR_BIT __CHAR_BIT__
#define TYPE_WIDTH(t) (sizeof (t) * CHAR_BIT)
#define INT_BITS_STRLEN_BOUND(b) (((b) * 146 + 484) / 485)
#define INT_STRLEN_BOUND(t)                                     \
  (INT_BITS_STRLEN_BOUND (TYPE_WIDTH (t) - _GL_SIGNED_TYPE_OR_EXPR (t)) \
   + _GL_SIGNED_TYPE_OR_EXPR (t))

//#undef _Static_assert
//#define _GL_VERIFY _Static_assert
//#define verify(R) _GL_VERIFY(R, "verify (" #R ")")

#define __xalloc_oversized(n, s) \
  ((size_t) (PTRDIFF_MAX < SIZE_MAX ? PTRDIFF_MAX : SIZE_MAX - 1) / (s) < (n))
#define xalloc_oversized(n, s) __xalloc_oversized (n, s)

#define UNIQ_SEP '.'
#define TYPE_WIDTH(t) (sizeof (t) * CHAR_BIT)
/* The maximum and minimum values for the integer type T.  */
#define TYPE_MINIMUM(t) ((t) ~ TYPE_MAXIMUM (t))
#define TYPE_MAXIMUM(t)                                                 \
  ((t) (! TYPE_SIGNED (t)                                               \
        ? (t) -1                                                        \
        : ((((t) 1 << (TYPE_WIDTH (t) - 2)) - 1) * 2 + 1)))
#define WGINT_MAX TYPE_MAXIMUM (wgint)

#define PR(mask) *p++ = n / (mask) + '0'
#define DIGITS_1(mask) PR (mask)
#define DIGITS_2(mask) PR (mask), n %= (mask), DIGITS_1 ((mask) / 10)
#define DIGITS_3(mask) PR (mask), n %= (mask), DIGITS_2 ((mask) / 10)
#define DIGITS_4(mask) PR (mask), n %= (mask), DIGITS_3 ((mask) / 10)
#define DIGITS_5(mask) PR (mask), n %= (mask), DIGITS_4 ((mask) / 10)
#define DIGITS_6(mask) PR (mask), n %= (mask), DIGITS_5 ((mask) / 10)
#define DIGITS_7(mask) PR (mask), n %= (mask), DIGITS_6 ((mask) / 10)
#define DIGITS_8(mask) PR (mask), n %= (mask), DIGITS_7 ((mask) / 10)
#define DIGITS_9(mask) PR (mask), n %= (mask), DIGITS_8 ((mask) / 10)
#define DIGITS_10(mask) PR (mask), n %= (mask), DIGITS_9 ((mask) / 10)

/* DIGITS_<11-20> are only used on machines with 64-bit wgints. */

#define DIGITS_11(mask) PR (mask), n %= (mask), DIGITS_10 ((mask) / 10)
#define DIGITS_12(mask) PR (mask), n %= (mask), DIGITS_11 ((mask) / 10)
#define DIGITS_13(mask) PR (mask), n %= (mask), DIGITS_12 ((mask) / 10)
#define DIGITS_14(mask) PR (mask), n %= (mask), DIGITS_13 ((mask) / 10)
#define DIGITS_15(mask) PR (mask), n %= (mask), DIGITS_14 ((mask) / 10)
#define DIGITS_16(mask) PR (mask), n %= (mask), DIGITS_15 ((mask) / 10)
#define DIGITS_17(mask) PR (mask), n %= (mask), DIGITS_16 ((mask) / 10)
#define DIGITS_18(mask) PR (mask), n %= (mask), DIGITS_17 ((mask) / 10)
#define DIGITS_19(mask) PR (mask), n %= (mask), DIGITS_18 ((mask) / 10)

/* Shorthand for casting to wgint. */
#define W wgint

#define xfree(p) do { free ((void *) (p)); p = NULL; } while (0)
/* The current position in the ring. */
static int log_line_current = -1;
static bool trailing_line;
#define ROT_ADVANCE(num) do {                   \
  if (++num >= SAVED_LOG_LINES)                 \
    num = 0;                                    \
} while (0)

#define STATIC_LENGTH 128
static struct log_ln {
    char static_line[STATIC_LENGTH + 1]; /* statically allocated
                                          line. */
    char *malloced_line;          /* malloc'ed line, for lines of output
                                   larger than 80 characters. */
    char *content;                /* this points either to malloced_line
                                   or to the appropriate static_line.
                                   If this is NULL, it means the line
                                   has not yet been used. */
} log_lines[SAVED_LOG_LINES];

#define SCHAR_MAX __SCHAR_MAX__
#define UCHAR_MAX (SCHAR_MAX * 2 + 1)
#define INT_BITS (sizeof (int) * CHAR_BIT)

/* Zero out a value.  */
#define xzero(x) memset (&(x), '\0', sizeof (x))

struct slotvec {
    size_t size;
    char *val;
};
static char slot0[256];
static int nslots = 1;
static struct slotvec slotvec0 = {sizeof slot0, slot0};
static struct slotvec *slotvec = &slotvec0;

#define MIN(a, b) (((a)<(b))?(a):(b))
#define MAX(a, b) (((a)>(b))?(a):(b))
/* MAX(a,b) returns the maximum of A and B.  */
#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

/* MIN(a,b) returns the minimum of A and B.  */
#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

#define INT_MAX __INT_MAX__

#define FALLTHROUGH ((void) 0)
#define N_(msgid) msgid

#define HAVE_LANGINFO_CODESET 1
#define CODESET     10000

#define C_CTYPE_INLINE _GL_EXTERN_INLINE
#define _C_CTYPE_LOWER_A_THRU_F_N(N) \
   case 'a' + (N): case 'b' + (N): case 'c' + (N): case 'd' + (N): \
   case 'e' + (N): case 'f' + (N)
#define _C_CTYPE_LOWER_N(N) \
   _C_CTYPE_LOWER_A_THRU_F_N(N): \
   case 'g' + (N): case 'h' + (N): case 'i' + (N): case 'j' + (N): \
   case 'k' + (N): case 'l' + (N): case 'm' + (N): case 'n' + (N): \
   case 'o' + (N): case 'p' + (N): case 'q' + (N): case 'r' + (N): \
   case 's' + (N): case 't' + (N): case 'u' + (N): case 'v' + (N): \
   case 'w' + (N): case 'x' + (N): case 'y' + (N): case 'z' + (N)
#define _C_CTYPE_UPPER _C_CTYPE_LOWER_N ('A' - 'a')

/*#define _ISbit(bit)	((bit) < 8 ? ((1 << (bit)) << 8) : ((1 << (bit)) >> 8))
enum {
    _ISupper = _ISbit(0),    *//* UPPERCASE.  *//*
    _ISlower = _ISbit(1),    *//* lowercase.  *//*
    _ISalpha = _ISbit(2),    *//* Alphabetic.  *//*
    _ISdigit = _ISbit(3),    *//* Numeric.  *//*
    _ISxdigit = _ISbit(4),    *//* Hexadecimal numeric.  *//*
    _ISspace = _ISbit(5),    *//* Whitespace.  *//*
    _ISprint = _ISbit(6),    *//* Printing.  *//*
    _ISgraph = _ISbit(7),    *//* Graphical.  *//*
    _ISblank = _ISbit(8),    *//* Blank (usually SPC and TAB).  *//*
    _IScntrl = _ISbit(9),    *//* Control character.  *//*
    _ISpunct = _ISbit(10),    *//* Punctuation.  *//*
    _ISalnum = _ISbit(11)    *//* Alphanumeric.  *//*
};*/
#define __isctype(c, type) \
  ((*__ctype_b_loc ())[(int) (c)] & (unsigned short int) type)
#define isalnum(c)    __isctype((c), _ISalnum)
#define isalpha(c)    __isctype((c), _ISalpha)
#define iscntrl(c)    __isctype((c), _IScntrl)
#define isdigit(c)    __isctype((c), _ISdigit)
#define islower(c)    __isctype((c), _ISlower)
#define isgraph(c)    __isctype((c), _ISgraph)
#define isprint(c)    __isctype((c), _ISprint)
#define ispunct(c)    __isctype((c), _ISpunct)
#define isspace(c)    __isctype((c), _ISspace)
#define isupper(c)    __isctype((c), _ISupper)
#define isxdigit(c)    __isctype((c), _ISxdigit)

#define OPENSSL_VERSION_NUMBER  0x1000207fL
#define CONF_MFLAGS_IGNORE_ERRORS       0x1
#define CONF_MFLAGS_IGNORE_RETURN_CODES 0x2
#define CONF_MFLAGS_SILENT              0x4
#define CONF_MFLAGS_NO_DSO              0x8
#define CONF_MFLAGS_IGNORE_MISSING_FILE 0x10
#define CONF_MFLAGS_DEFAULT_SECTION     0x20
#define OPENSSL_NO_SSL2

#define SSL_OP_TLS_ROLLBACK_BUG                         0x00800000L

#define SSL_OP_NO_SSLv2                                 0x01000000L
#define SSL_OP_NO_SSLv3                                 0x02000000L
#define SSL_OP_NO_TLSv1                                 0x04000000L
#define SSL_OP_NO_TLSv1_2                               0x08000000L
#define SSL_OP_NO_TLSv1_1                               0x10000000L

#define SSL_OP_NO_DTLSv1                                0x04000000L
#define SSL_OP_NO_DTLSv1_2                              0x08000000L

#define SSL_OP_NO_SSL_MASK (SSL_OP_NO_SSLv2|SSL_OP_NO_SSLv3|\
        SSL_OP_NO_TLSv1|SSL_OP_NO_TLSv1_1|SSL_OP_NO_TLSv1_2)

/* SSL has been initialized */
static int ssl_true_initialized = 0;
/* Application-wide SSL context.  This is common to all SSL
   connections.  */
static SSL_CTX *ssl_ctx;

#define _GL_ATTRIBUTE_MALLOC __attribute__ ((__malloc__))
#define _GL_ATTRIBUTE_ALLOC_SIZE(args) __attribute__ ((__alloc_size__ args))

static const char *volatile charset_aliases;
#define LIBDIR "/root/mydev/tools/wget/lib"
#define LOCALEDIR "/root/mydev/tools/wget/share/locale"
#define relocate2(pathname, allocatedp) (*(allocatedp) = NULL, (pathname))

#define DIRECTORY_SEPARATOR '/'
#define ISSLASH(C) ((C) == DIRECTORY_SEPARATOR)
#define HAVE_WORKING_O_NOFOLLOW 1

#define __strerror_r strerror_r

typedef int gl_lock_t;
#define gl_lock_define(STORAGECLASS, NAME)
#define gl_lock_define_initialized(STORAGECLASS, NAME)
#define glthread_lock_init(NAME) 0
#define glthread_lock_lock(NAME) 0
#define glthread_lock_unlock(NAME) 0
#define glthread_lock_destroy(NAME) 0

gl_lock_define_initialized(static, strerror_lock)

#define gl_lock_init(NAME) \
   do                                  \
     {                                 \
       if (glthread_lock_init (&NAME)) \
         abort ();                     \
     }                                 \
   while (0)
#define gl_lock_lock(NAME) \
   do                                  \
     {                                 \
       if (glthread_lock_lock (&NAME)) \
         abort ();                     \
     }                                 \
   while (0)
#define gl_lock_unlock(NAME) \
   do                                    \
     {                                   \
       if (glthread_lock_unlock (&NAME)) \
         abort ();                       \
     }                                   \
   while (0)
#define gl_lock_destroy(NAME) \
   do                                     \
     {                                    \
       if (glthread_lock_destroy (&NAME)) \
         abort ();                        \
     }                                    \
   while (0)



#endif //MYTEST_MY_DEFINE_H
