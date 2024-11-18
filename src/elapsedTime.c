#include <elapsedTime.h>

#if !(DISABLE_ELAPSED)
struct timeval gTime;      /**< Stores the current time values for elapsed time calculation. */
double gStartTime;         /**< Holds the start time in seconds for the timed section. */
double gEndTime;           /**< Holds the end time in seconds for the timed section. */
#endif

/**
 * @brief Starts the elapsed time tracking.
 * 
 * Captures the current time and stores it in `gStartTime` to mark 
 * the beginning of an elapsed time measurement.
 * 
 * Only operates if `DISABLE_ELAPSED` is not defined, allowing conditional 
 * compilation for performance tracking.
 */
void startElapseTime()
{
#if !(DISABLE_ELAPSED)
    gettimeofday(&gTime, NULL);
    gStartTime = gTime.tv_sec + 1.0e-6 * gTime.tv_usec;
#endif
}

/**
 * @brief Stops the elapsed time tracking and outputs the result.
 * 
 * Calculates the time elapsed since `startElapseTime` and outputs it in seconds 
 * using the provided label.
 * 
 * @param str Label describing the operation or section being timed.
 * 
 * Only operates if `DISABLE_ELAPSED` is not defined, and outputs timing information 
 * through `PRINT_DEBUG` for profiling and debugging.
 */
void stopElapseTime(char *str)
{
#if !(DISABLE_ELAPSED)
    gettimeofday(&gTime, NULL);
    gEndTime = gTime.tv_sec + 1.0e-6 * gTime.tv_usec;
    PRINT_DEBUG("[ELAPSED] %s -> took %f seconds \n\n", str, (double)(gEndTime - gStartTime));
#endif
}
