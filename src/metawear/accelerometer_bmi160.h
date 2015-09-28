/**
 * @copyright MbientLab License 
 * @file accelerometer_bmi160.h
 * @brief Functions for interacting with the BMI160 accelerometer.
 * This sensor is only available on MetaWear RG and RPro boards.
 */
#pragma once

#include <stdint.h>

#include "dllmarker.h"
#include "sensor_defs.h"
#include "types.h"

#ifdef	__cplusplus
extern "C" {
#endif

const uint8_t MBL_MW_ACC_BMI160_MODULE= 0x3;    ///< Module id for the BMI160 accelerometer
const uint8_t MBL_MW_ACC_BMI160_DATA= 0x4;      ///< Register id for the BMI160 acceleration data

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
 * Container for the accelerometer configuration
 */
typedef struct MblMwAccBmi160Config MblMwAccBmi160Config;

/**
 * Retrieves the attributes for the BMI160 acceleration data
 * @return Pointer to the BMI160 acceleration data source
 */
METAWEAR_API const DataSource* mbl_mw_acc_bmi160_get_acceleration_data_source();

/**
 * Instantiates an MblMwAccBmi160Config struct with an ODR of 100Hz and FSR of +/- 2g.  The user is responsible for 
 * deallocating the memory by calling mbl_mw_acc_bmi160_free_config()
 */
METAWEAR_API MblMwAccBmi160Config* mbl_mw_acc_bmi160_create_config();
/**
 * Frees the memory allocated for a MblMwAccBmi160Config struct
 * @param config    Pointer to memory to free
 */
METAWEAR_API void mbl_mw_acc_bmi160_free_config(MblMwAccBmi160Config *config);
/**
 * Sets the output data rate attribute of an MblMwAccBmi160Config struct
 * @param config    Pointer to the MblMwAccBmi160Config struct to modify
 * @param odr       Output data rate value to assign
 */
METAWEAR_API void mbl_mw_acc_bmi160_set_odr(MblMwAccBmi160Config *config, MblMwAccBmi160Odr odr);
/**
 * Sets the max range of an MblMwAccBmi160Config struct
 * @param config    Pointer to the MblMwAccBmi160Config struct to modify
 * @param range     Acceleration range to assign
 */
METAWEAR_API void mbl_mw_acc_bmi160_set_range(MblMwAccBmi160Config *config, MblMwAccBmi160Range range);
/**
 * Constructs the command that writes the BMI160 configuration to the chip
 * @param command   Byte array (len 4) for the function to write the command to
 * @param config    Pointer to the MblMwAccBmi160Config struct to write to the board
 */
METAWEAR_API void mbl_mw_acc_bmi160_write_acceleration_config(uint8_t command[4], const MblMwAccBmi160Config *config );

/**
 * Constructs the command that puts the BMI160 in active mode.  When in active mode, the chip cannot be configured
 * @param command   Byte array (len 3) for the function to write the command to
 */
METAWEAR_API void mbl_mw_acc_bmi160_start(uint8_t command[3]);
/**
 * Constructs the command that puts the BMI160 in standby mode
 * @param command   Byte array (len 3) for the function to write the command to
 */
METAWEAR_API void mbl_mw_acc_bmi160_stop(uint8_t command[3]);

/**
 * Constructs the command that enables acceleration sampling
 * @param command   Byte array (len 4) for the function to write the command to 
 */
METAWEAR_API void mbl_mw_acc_bmi160_enable_acceleration_sampling(uint8_t command[4]);
/**
 * Constructs the command that disables acceleration sampling
 * @param command   Byte array (len 4) for the function to write the command to
 */
METAWEAR_API void mbl_mw_acc_bmi160_disable_acceleration_sampling(uint8_t command[4]);

/**
 * Extracts the acceleration data from a response sent by the BMI160 chip in milli Gs
 * @param data_mg   Pointer to a CartesianShort to write the acceleration data to
 * @param config    Pointer to the MblMwAccBmi160Config struct representing the current BMI160 configuration
 * @param response  Response received from the BMI160 chip
 * @return #STATUS_OK if the response contains valid BMI160 acceleration data, #STATUS_INVALID_RESPONSE otherwise
 */
METAWEAR_API int mbl_mw_acc_bmi160_get_acceleration_data_mg(CartesianShort *data_mg, const MblMwAccBmi160Config *config, uint8_t response[8]);
/**
 * Extracts the acceleration data from a response sent by the BMI160 chip in Gs
 * @param data_g    Pointer to a CartesianFloat to write the acceleration data to
 * @param config    Pointer to the MblMwAccBmi160Config struct representing the current BMI160 configuration
 * @param response  Response received from the BMI160 chip
 * @return #STATUS_OK if the response contains valid BMI160 acceleration data, #STATUS_INVALID_RESPONSE otherwise
 */
METAWEAR_API int mbl_mw_acc_bmi160_get_acceleration_data_g(CartesianFloat *data_g, const MblMwAccBmi160Config *config, uint8_t response[8]);

#ifdef	__cplusplus
}
#endif
