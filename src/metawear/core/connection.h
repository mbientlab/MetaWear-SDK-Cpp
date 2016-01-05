/**
 * @copyright MbientLab License 
 * @file connection.h
 * @brief Methods and types for communicating with the metawear board
 */
#pragma once

#include <stdint.h>

#include "dllmarker.h"
#include "metawearboard_fwd.h"

#ifdef __cplusplus
extern "C" {
#endif

/** High QWORD of the MetaWear gatt service UUID */
const uint64_t METAWEAR_GATT_SERVICE_UUID_HIGH= 0x326a900085cb9195;
/** Low QWORD of the MetaWear gatt service uuid */
const uint64_t METAWEAR_GATT_SERVICE_UUID_LOW=  0xd9dd464cfbbae75a;

/** High QWORD of the MetaWear command gatt characteristic */
const uint64_t METAWEAR_GATT_CHAR_COMMAND_UUID_HIGH= 0x326a900185cb9195;
/** Low QWORD of the MetaWear command gatt characteristic */
const uint64_t METAWEAR_GATT_CHAR_COMMAND_UUID_LOW=  0xd9dd464cfbbae75a;

/** High QWORD of the MetaWear notification gatt characteristic */
const uint64_t METAWEAR_GATT_CHAR_NOTIFY_UUID_HIGH= 0x326a900685cb9195;
/** Low QWORD of the MetaWear notification gatt characteristic */
const uint64_t METAWEAR_GATT_CHAR_NOTIFY_UUID_LOW=  0xd9dd464cfbbae75a;

#ifdef __cplusplus
}
#endif
