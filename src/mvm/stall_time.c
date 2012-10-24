 /* Niels Widger
 * Time-stamp: <24 Oct 2012 at 19:37:48 by nwidger on macros.local>
 */

#define _POSIX_C_SOURCE 200112L
#define _ISOC99_SOURCE
#define __EXTENSIONS__

#include <stdio.h>

#include "stall_time.h"

double stall_time(time_type *before, time_type *done) {
    int secs, fraction;
    double delta;

    secs = done->tv_sec - before->tv_sec;
    fraction = done->fraction_field - before->fraction_field;
    if (fraction < 0) {
        secs--;
        fraction += PER_SECOND;
    }
    
    delta = ((double)secs) + ((double)fraction)/((double)PER_SECOND);
    return delta;
}
