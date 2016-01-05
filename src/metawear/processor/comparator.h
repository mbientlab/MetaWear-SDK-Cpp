#pragma once

#include "metawear/core/dataprocessor_fwd.h"
#include "metawear/core/datasignal_fwd.h"
#include "metawear/core/dllmarker.h"
#include "metawear/core/status.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MBL_MW_COMPARATOR_OP_EQ = 0,
    MBL_MW_COMPARATOR_OP_NEQ,
    MBL_MW_COMPARATOR_OP_LT,
    MBL_MW_COMPARATOR_OP_LTE,
    MBL_MW_COMPARATOR_OP_GT,
    MBL_MW_COMPARATOR_OP_GTE,
} MblMwComparatorOperation;

METAWEAR_API void mbl_mw_dataprocessor_create_comparator(MblMwDataSignal *source, MblMwComparatorOperation op, 
        float reference, MblMwFnDataProcessor processor_created);
METAWEAR_API void mbl_mw_dataprocessor_create_comparator_signed(MblMwDataSignal *source, MblMwComparatorOperation op, 
        float reference, MblMwFnDataProcessor processor_created);
METAWEAR_API void mbl_mw_dataprocessor_create_comparator_unsigned(MblMwDataSignal *source, MblMwComparatorOperation op, 
        float reference, MblMwFnDataProcessor processor_created);
METAWEAR_API int32_t mbl_mw_dataprocessor_comparator_modify(MblMwDataProcessor *comparator, MblMwComparatorOperation op, 
        float reference);
METAWEAR_API int32_t mbl_mw_dataprocessor_comparator_modify_signal(MblMwDataProcessor *comparator, MblMwComparatorOperation op, 
        MblMwDataSignal *reference_signal);

#ifdef __cplusplus
}
#endif
