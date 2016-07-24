/**
 * @copyright MbientLab License
 * @file datasignal_fwd.h
 * @brief Forward declaration for the MblMwDataSignal type
 */
#pragma once

/**
 * A event fired from the MetaWear board that also contains data.  An MblMwDataSignal pointer can be casted as an 
 * MblMwEvent pointer and used with any function that accepts an MblMwEvent pointer.
 */
#ifdef	__cplusplus
struct MblMwDataSignal;
#else
typedef struct MblMwDataSignal MblMwDataSignal;
#endif
