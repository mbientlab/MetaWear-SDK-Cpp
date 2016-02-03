/**
 * @copyright MbientLab License 
 * @file accelerometer_bmi160.h
 * @brief Functions for interacting with the BMI160 accelerometer.
 * @details This sensor is only available on MetaWear RG and RPro boards.
 */
#pragma once

#include <stdint.h>

#include "metawear/core/datasignal_fwd.h"
#include "metawear/core/dllmarker.h"
#include "metawear/core/metawearboard_fwd.h"

#ifdef	__cplusplus
extern "C" {
#endif

const uint8_t MBL_MW_MODULE_ACC_TYPE_BMI160 = 1;            ///< Constant identifying the BMI160 accelerometer module type

/**
 * Available g-ranges on the BMI160 accelerometer
 */
typedef enum {
    MBL_MW_ACC_BMI160_FSR_2G= 0,    ///< +/- 2g
    MBL_MW_ACC_BMI160_FSR_4G,       ///< +/- 4g
    MBL_MW_ACC_BMI160_FSR_8G,       ///< +/- 8g
    MBL_MW_ACC_BMI160_FSR_16G       ///< +/- 16g
} MblMwAccBmi160Range;

/**
 * Available ouput data rates on the BMI160 accelerometer
 */
typedef enum {
    MBL_MW_ACC_BMI160_ODR_0_78125HZ= 0,
    MBL_MW_ACC_BMI160_ODR_1_5625HZ,
    MBL_MW_ACC_BMI160_ODR_3_125HZ,
    MBL_MW_ACC_BMI160_ODR_6_25HZ,
    MBL_MW_ACC_BMI160_ODR_12_5HZ,
    MBL_MW_ACC_BMI160_ODR_25HZ,
    MBL_MW_ACC_BMI160_ODR_50HZ,
    MBL_MW_ACC_BMI160_ODR_100HZ,
    MBL_MW_ACC_BMI160_ODR_200HZ,
    MBL_MW_ACC_BMI160_ODR_400HZ,
    MBL_MW_ACC_BMI160_ODR_800HZ,
    MBL_MW_ACC_BMI160_ODR_1600HZ
} MblMwAccBmi160Odr;

/**
 * Retrieves the data signal representing BMI160 acceleration data
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's BMI160 acceleration data signal
 */
METAWEAR_API MblMwDataSignal* mbl_mw_acc_bmi160_get_acceleration_data_signal(const MblMwMetaWearBoard *board);

/**
 * Sets the output data rate
 * @param board     Pointer to the board to modify
 * @param odr       Output data rate value to assign
 */
METAWEAR_API void mbl_mw_acc_bmi160_set_odr(MblMwMetaWearBoard *board, MblMwAccBmi160Odr odr);
/**
 * Sets the acceleration range
 * @param board     Pointer to the board to modify
 * @param range     Acceleration range to assign
 */
METAWEAR_API void mbl_mw_acc_bmi160_set_range(MblMwMetaWearBoard *board, MblMwAccBmi160Range range);
/**
 * Writes the acceleration settings to the sensor
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_acc_bmi160_write_acceleration_config(const MblMwMetaWearBoard *board);

/**
 * Switches the accelerometer to active mode.  When in active mode, the accelerometer cannot be configured
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_acc_bmi160_start(const MblMwMetaWearBoard *board);
/**
 * Switches the accelerometer to standby mode
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_acc_bmi160_stop(const MblMwMetaWearBoard *board);

/**
 * Enables acceleration sampling
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_acc_bmi160_enable_acceleration_sampling(const MblMwMetaWearBoard *board);
/**
 * Disables acceleration sampling
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_acc_bmi160_disable_acceleration_sampling(const MblMwMetaWearBoard *board);

#ifdef	__cplusplus
}
#endif
