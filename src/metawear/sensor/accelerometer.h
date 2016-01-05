#pragma once

#include <stdint.h>

#include "metawear/core/datasignal_fwd.h"
#include "metawear/core/dllmarker.h"
#include "metawear/core/metawearboard_fwd.h"

#ifdef	__cplusplus
extern "C" {
#endif

METAWEAR_API const MblMwDataSignal* mbl_mw_acc_get_acceleration_data_signal(const MblMwMetaWearBoard *board);

METAWEAR_API void mbl_mw_acc_set_odr(MblMwMetaWearBoard *board, float odr);
METAWEAR_API void mbl_mw_acc_set_range(MblMwMetaWearBoard *board, float range);
METAWEAR_API void mbl_mw_acc_write_acceleration_config(const MblMwMetaWearBoard* board);

METAWEAR_API void mbl_mw_acc_start(const MblMwMetaWearBoard *board);
METAWEAR_API void mbl_mw_acc_stop(const MblMwMetaWearBoard *board);

METAWEAR_API void mbl_mw_acc_enable_acceleration_sampling(const MblMwMetaWearBoard *board);
METAWEAR_API void mbl_mw_acc_disable_acceleration_sampling(const MblMwMetaWearBoard *board);

#ifdef	__cplusplus
}
#endif
