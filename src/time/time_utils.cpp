#include "time/time_utils.h"

#include <time.h>
#include <sys/time.h>
#include <signal.h>

long get_current_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

long get_zero_clock_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec - tv.tv_sec % 86400 - 28800;
}

