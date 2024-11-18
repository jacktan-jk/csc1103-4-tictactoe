/**
 * @file elapsedTime.h
 * @author jacktan-jk
 * @brief Provides functionality for measuring elapsed time for profiling purposes.
 * @version 1.0
 * @date 2024-11-13
 * 
 * @copyright Copyright (c) 2024
 * 
 * This header file declares functions and macros to track the start and stop times 
 * of operations, enabling performance measurement. The code is conditionally compiled 
 * based on the `DISABLE_ELAPSED` macro, allowing profiling code to be included or excluded 
 * as needed.
 */

#ifndef ELAPSED_TIME_H
#define ELAPSED_TIME_H

#include <macros.h>
#include <sys/time.h>

/**
 * @brief Starts the elapsed time tracking.
 * 
 * Captures the current time and stores it in `gStartTime` to mark 
 * the beginning of an elapsed time measurement.
 * 
 * Only operates if `DISABLE_ELAPSED` is not defined, allowing conditional 
 * compilation for performance tracking.
 */
void startElapseTime();

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
void stopElapseTime(char *str);

#endif // ELAPSED_TIME_H
