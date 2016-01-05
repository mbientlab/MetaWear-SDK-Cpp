/**
* @copyright MbientLab License
* @file datasignal_fwd.h
* @brief Forward declarations for datasignal types
*/
#pragma once

#ifdef	__cplusplus
struct MblMwDataSignal;
#else
/**
 * Represents a data producer
 */
typedef struct MblMwDataSignal MblMwDataSignal;
#endif

typedef void(*MblMwFnDataSignal)(MblMwDataSignal* new_signal);