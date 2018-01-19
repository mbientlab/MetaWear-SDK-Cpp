/**
 * @copyright MbientLab License
 * @file dataprocessor_fwd.h
 * @brief Forward declaration for the MblMwDataProcessor type
 */
#pragma once

/**
 * Data signal from the on board data processor.  An MblMwDataProcessor pointer can be casted as an 
 * MblMwDataSignal pointer and used with any function tht accepts an MblMwdDataSignal.
 */
#ifdef __cplusplus
struct MblMwDataProcessor; 
#else
typedef struct MblMwDataProcessor MblMwDataProcessor;
#endif

/**
 * Definition for callback functions that accept an MblMwDataProcessor pointer
 * @param context           Pointer to the context the enclosing function was called with
 * @param processor         Processor to be used with the function
 */
typedef void (*MblMwFnDataProcessor)(void *context, MblMwDataProcessor* processor);
