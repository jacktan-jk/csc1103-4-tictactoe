#include <elapsedTime.h>

#if !(DISABLE_ELAPSED)
struct timeval gTime;      /**< Stores the current time values for elapsed time calculation. */
double gStartTime;         /**< Holds the start time in seconds for the timed section. */
double gEndTime;           /**< Holds the end time in seconds for the timed section. */
#endif

void startElapseTime()
{
#if !(DISABLE_ELAPSED)
    gettimeofday(&gTime, NULL);
    gStartTime = gTime.tv_sec + 1.0e-6 * gTime.tv_usec;
#endif
}

void stopElapseTime(char *str)
{
#if !(DISABLE_ELAPSED)
    gettimeofday(&gTime, NULL);
    gEndTime = gTime.tv_sec + 1.0e-6 * gTime.tv_usec;
    PRINT_DEBUG("[ELAPSED] %s -> took %f seconds \n\n", str, (double)(gEndTime - gStartTime));
#endif
}
