#include <elapsedTime.h>

#if !(DISABLE_ELAPSED)
    struct timeval gTime;
    double gStartTime;
    double gEndTime;
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
