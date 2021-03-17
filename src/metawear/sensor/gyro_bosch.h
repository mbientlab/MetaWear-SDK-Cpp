/**
 * @copyright MbientLab License 
 * @file gyro_bosch.h
 * @brief Functions for interacting with the BMI160 and BMI270 gyro.
 * @details This sensor is only available on MMR, MMS, MetaWear RG and RPro boards.
 */
#pragma once

#include "sensor_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

const uint8_t MBL_MW_MODULE_GYRO_TYPE_BMI160 = 0;            ///< Constant identifying the BMI160 accelerometer module type
const uint8_t MBL_MW_MODULE_GYRO_TYPE_BMI270 = 1;            ///< Constant identifying the BMI270 accelerometer module type

//@{
/** Indices for component values of the rotation data signal, used with mbl_mw_datasignal_get_component */
const uint8_t MBL_MW_GYRO_ROTATION_X_AXIS_INDEX = 0, 
        MBL_MW_GYRO_ROTATION_Y_AXIS_INDEX = 1, 
        MBL_MW_GYRO_ROTATION_Z_AXIS_INDEX = 2;
//@}

/**
 * Available output data rates on the BMI160 gyro
 */
typedef enum {
    MBL_MW_GYRO_BOSCH_ODR_25Hz= 6,
    MBL_MW_GYRO_BOSCH_ODR_50Hz,
    MBL_MW_GYRO_BOSCH_ODR_100Hz,
    MBL_MW_GYRO_BOSCH_ODR_200Hz,
    MBL_MW_GYRO_BOSCH_ODR_400Hz,
    MBL_MW_GYRO_BOSCH_ODR_800Hz,
    MBL_MW_GYRO_BOSCH_ODR_1600Hz,
    MBL_MW_GYRO_BOSCH_ODR_3200Hz
} MblMwGyroBoschOdr;

/**
 * Available degrees per second ranges on the BMI160 gyro
 */
typedef enum {
    MBL_MW_GYRO_BOSCH_RANGE_2000dps= 0,      ///< +/-2000 degrees per second
    MBL_MW_GYRO_BOSCH_RANGE_1000dps,         ///< +/-1000 degrees per second
    MBL_MW_GYRO_BOSCH_RANGE_500dps,          ///< +/-500 degrees per second
    MBL_MW_GYRO_BOSCH_RANGE_250dps,          ///< +/-250 degrees per second
    MBL_MW_GYRO_BOSCH_RANGE_125dps           ///< +/-125 degrees per second
} MblMwGyroBoschRange;

/**
 * Retrieves the data signal representing BMI160 rotation data
 * This signal is timestamp,x,y,z gyro data
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's BMI160 rotation data signal
 * MblMwCartesianFloat is return signal data type
 */
METAWEAR_API MblMwDataSignal* mbl_mw_gyro_bmi160_get_rotation_data_signal(const MblMwMetaWearBoard *board);
/**
 * Retrieves the data signal representing BMI270 rotation data
 * This signal is timestamp,x,y,z gyro data
 * @param board     Pointer to the board to retrieve the signal from
 * @return Pointer to the board's BMI160 rotation data signal
 * MblMwCartesianFloat is return signal data type
 */
METAWEAR_API MblMwDataSignal* mbl_mw_gyro_bmi270_get_rotation_data_signal(const MblMwMetaWearBoard *board);
/**
 * @deprecated As of v0.8.0 and will be removed in v1.0.0.  Use mbl_mw_gyro_bmi160_get_packed_rotation_data_signal instead.
 */
METAWEAR_API MblMwDataSignal* mbl_mw_gyro_bmi160_get_high_freq_rotation_data_signal(const MblMwMetaWearBoard *board);
/**
 * Variant of rotation data that packs multiple data samples into 1 BLE packet to increase the
 * data throughput.  This data signal cannot be used with data processing or logging, only with streaming.
 * This signal is timestamp,x,y,z,x,y,z,x,y,z gyro data (it packs three acc data points in one timestamp)
 * @return Pointer to the data singal
 * [MblMwCartesianFloat, MblMwCartesianFloat, MblMwCartesianFloat] is return signal data type
 */
METAWEAR_API MblMwDataSignal* mbl_mw_gyro_bmi160_get_packed_rotation_data_signal(const MblMwMetaWearBoard *board);
/**
 * Variant of rotation data that packs multiple data samples into 1 BLE packet to increase the
 * data throughput.  This data signal cannot be used with data processing or logging, only with streaming.
 * This signal is timestamp,x,y,z,x,y,z,x,y,z acc gyro (it packs three acc data points in one timestamp)
 * @return Pointer to the data singal
 * [MblMwCartesianFloat, MblMwCartesianFloat, MblMwCartesianFloat] is return signal data type
 */
METAWEAR_API MblMwDataSignal* mbl_mw_gyro_bmi270_get_packed_rotation_data_signal(const MblMwMetaWearBoard *board);
/**
 * Sets the output data rate for the BMI160 gyroscope
 * The ODR sets the output data frequency in Hz.
 * See MblMwGyroBoschOdr for allowed values.
 * @param board     Pointer to the board to modify
 * @param odr       Output data rate value to assign
 */
METAWEAR_API void mbl_mw_gyro_bmi160_set_odr(MblMwMetaWearBoard *board, MblMwGyroBoschOdr odr);
/**
 * Sets the rotation range
 * The range is in units of degrees per second (dps) for Bosch sensors
 * See MblMwGyroBoschRange for allowed values.
 * @param board     Pointer to the board to modify
 * @param range     New rotation range
 */
METAWEAR_API void mbl_mw_gyro_bmi160_set_range(MblMwMetaWearBoard *board, MblMwGyroBoschRange range);
/**
 * Writes the configuration to the sendor
 * Applies the ODR and RANGE values set in set_range() and set_odr().
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_gyro_bmi160_write_config(const MblMwMetaWearBoard *board);
/**
 * Pulls the current gyro output data rate and data range from the sensor
 * @param board         Calling object
 * @param context       Pointer to additional data for the callback function
 * @param completed     Callback function that is executed when the task is finished
 */
METAWEAR_API void mbl_mw_gyro_bmi160_read_config(const MblMwMetaWearBoard* board, void *context, MblMwFnBoardPtrInt completed);
/**
 * Sets the output data rate for the BMI270 gyroscope
 * The ODR sets the output data frequency in Hz.
 * See MblMwGyroBoschOdr for allowed values.
 * @param board     Pointer to the board to modify
 * @param odr       Output data rate value to assign
 */
METAWEAR_API void mbl_mw_gyro_bmi270_set_odr(MblMwMetaWearBoard *board, MblMwGyroBoschOdr odr);
/**
 * Sets the rotation range for the BMI270 gyroscope
 * The range is in units of degrees per second (dps) for Bosch sensors
 * See MblMwGyroBoschRange for allowed values.
 * @param board     Pointer to the board to modify
 * @param range     New rotation range
 */
METAWEAR_API void mbl_mw_gyro_bmi270_set_range(MblMwMetaWearBoard *board, MblMwGyroBoschRange range);
/**
 * Writes the configuration to the sendor
 * Applies the ODR and RANGE values set in set_range() and set_odr().
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_gyro_bmi270_write_config(const MblMwMetaWearBoard *board);
/**
 * Pulls the current gyro output data rate and data range from the sensor
 * @param board         Calling object
 * @param context       Pointer to additional data for the callback function
 * @param completed     Callback function that is executed when the task is finished
 */
METAWEAR_API void mbl_mw_gyro_bmi270_read_config(const MblMwMetaWearBoard* board, void *context, MblMwFnBoardPtrInt completed);
/**
 * Switches the gyro to active mode.  
 * While in active mode, the gyro cannot be configured
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_gyro_bmi160_start(const MblMwMetaWearBoard *board);
/**
 * Switches the gyro to standby mode.
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_gyro_bmi160_stop(const MblMwMetaWearBoard *board);
/**
 * Switches the gyro to active mode.  
 * While in active mode, the gyro cannot be configured
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_gyro_bmi270_start(const MblMwMetaWearBoard *board);
/**
 * Switches the gyro to standby mode.
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_gyro_bmi270_stop(const MblMwMetaWearBoard *board);
/**
 * Enables rotation sampling
 * The board will start gathering data from the gyroscope
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_gyro_bmi160_enable_rotation_sampling(const MblMwMetaWearBoard *board);
/**
 * Disables rotation sampling
 * The board will stop gathering data from the gyroscope
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_gyro_bmi160_disable_rotation_sampling(const MblMwMetaWearBoard *board);
/**
 * Enables rotation sampling
 * The board will start gathering data from the gyroscope
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_gyro_bmi270_enable_rotation_sampling(const MblMwMetaWearBoard *board);
/**
 * Disables rotation sampling
 * The board will stop gathering data from the gyroscope
 * @param board     Pointer to the board to send the command to
 */
METAWEAR_API void mbl_mw_gyro_bmi270_disable_rotation_sampling(const MblMwMetaWearBoard *board);
/**
 * Manual compensation for the BMI270 gyro
 * The offset compensation field for each axis has a width of 10 bit using two’s complement notation. 
 * The offset resolution (LSB) is 61 mdps and the offset range is +- 31 dps.
 * @param board     Pointer to the board to send the command to
 * @param x_offset  Offset compensation for Gyroscope X-axis 
 * @param y_offset  Offset compensation for Gyroscope Y-axis 
 * @param z_offset  Offset compensation for Gyroscope Z-axis
 */
METAWEAR_API void mbl_mw_gyro_bmi270_acc_offsets(const MblMwMetaWearBoard* board, uint8_t x_offset, uint8_t y_offset, uint8_t z_offset);

#ifdef	__cplusplus
}
#endif
