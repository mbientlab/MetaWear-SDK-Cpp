/**
 * @copyright MbientLab License
 * @file datasignal_fwd.h
 * @brief Forward declaration for the MblMwDataSignal type
 */
#pragma once

#ifdef	__cplusplus
struct MblMwDataSignal;
#else
/**
 * A event fired from the MetaWear board that also contains data.  An MblMwDataSignal pointer can be casted as an 
 * MblMwEvent pointer and used with any function that accepts an MblMwEvent pointer.
 */
typedef struct MblMwDataSignal MblMwDataSignal;
#endif
