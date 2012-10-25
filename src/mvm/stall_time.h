 /* Niels Widger
 * Time-stamp: <24 Oct 2012 at 19:56:49 by nwidger on macros.local>
 */

#include <unistd.h>

#if defined(_POSIX_TIMERS) && !defined(__MACH__)

    /* use the newer Posix high-resolution time facilities */
    /* must be linked on all machines with -lrt */

    #include <time.h>

    #define PER_SECOND  1000000000
    #define time_type   struct timespec
    #define fraction_field  tv_nsec
    #define get_time(where) clock_gettime(CLOCK_REALTIME, where)

#else

    /* use the older (BSD) high-resolution time facilities */

    #include <sys/time.h>

    #define PER_SECOND  1000000
    #define time_type   struct timeval
    #define fraction_field  tv_usec
    #define get_time(where) gettimeofday(where, NULL)

#endif

double stall_time(time_type *before, time_type *done);
