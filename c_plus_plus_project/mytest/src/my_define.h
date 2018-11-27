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
# define SIZEOF_WGINT SIZEOF_LONG_LONG
#elif HAVE_INT64_T
typedef int64_t wgint;
# define SIZEOF_WGINT 8
#elif SIZEOF_OFF_T >= 8
/* In case off_t is typedeffed to a large non-standard type that our
     tests don't find. */
  typedef off_t wgint;
# define SIZEOF_WGINT SIZEOF_OFF_T
#else
/* Fall back to using long, which is always available and in most
   cases large enough. */
typedef long wgint;
# define SIZEOF_WGINT SIZEOF_LONG
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

typedef unsigned long (*hashfun_t) (const void *);
typedef int (*testfun_t) (const void *, const void *);

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
#define	STDIN_FILENO	0	/* Standard input.  */
#define	STDOUT_FILENO	1	/* Standard output.  */
#define	STDERR_FILENO	2	/* Standard error output.  */

#endif //MYTEST_MY_DEFINE_H
