/**
* @copyright MbientLab License
* @file logging_fwd.h
* @brief Forward declaration of the MblMwDataLogger type
*/
#pragma once

/**
 * Logs data from an MblMwDataSignal type letting users retrieve the data at a later time
 */
#ifdef	__cplusplus
struct MblMwDataLogger;
#else
typedef struct MblMwDataLogger MblMwDataLogger;
#endif

/**
* Definition for callback functions that accept an MblMwDataLogger pointer
* @param timer         Timer to be used with the function
*/
typedef void(*MblMwFnDataLoggerPtr)(MblMwDataLogger* logger);
