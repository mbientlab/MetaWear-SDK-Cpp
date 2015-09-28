/**
 * @copyright MbientLab License 
 * @file gyro_bmi160.h
 * @brief Functions for interacting with the BMI160 gyro.
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

const uint8_t MBL_MW_GYRO_BMI160_MODULE= 0x13;      ///< Module id for the BMI160 gyro
const uint8_t MBL_MW_GYRO_BMI160_DATA= 0x5;         ///< Register id for BMI160 rotation data

/**
 * Available output data rates on the BMI160 gyro
 */
typedef enum {
    MBL_MW_GYRO_BMI160_ODR_25HZ= 6,
    MBL_MW_GYRO_BMI160_ODR_50HZ,
    MBL_MW_GYRO_BMI160_ODR_100HZ,
    MBL_MW_GYRO_BMI160_ODR_200HZ,
    MBL_MW_GYRO_BMI160_ODR_400HZ,
    MBL_MW_GYRO_BMI160_ODR_800HZ,
    MBL_MW_GYRO_BMI160_ODR_1600HZ,
    MBL_MW_GYRO_BMI160_ODR_3200HZ
} MblMwGyroBmi160Odr;

/**
 * Available degrees per second ranges on the BMI160 gyro
 */
typedef enum {
    MBL_MW_GYRO_BMI160_FSR_2000DPS= 0,      ///< +/-2000 degrees per second
    MBL_MW_GYRO_BMI160_FSR_1000DPS,         ///< +/-1000 degrees per second
    MBL_MW_GYRO_BMI160_FSR_500DPS,          ///< +/-500 degrees per second
    MBL_MW_GYRO_BMI160_FSR_250DPS,          ///< +/-250 degrees per second
    MBL_MW_GYRO_BMI160_FSR_125DPS           ///< +/-125 degrees per second
} MblMwGyroBmi160Range;

/**
 * Container representing the BMI160 gyro configuration 
 */
typedef struct MblMwGyroBmi160Config MblMwGyroBmi160Config;

/**
 * Retrieves the attributes for BMI16MblMwGyroBmi160Config rotation data 
 * @return Pointer to the BMI160 gyro rotation data source
 */
METAWEAR_API const DataSource* mbl_mw_gyro_bmi160_get_rotation_data_source();

/**
 * Instantiates an MblMwGyroBmi160Config struct with an output data rate of 100Hz and dps range of +/-2000 degrees per second
 * The user is responsible for deallocating the memory by calling mbl_mw_gyro_bmi160_free_config()
 * @return Pointer to the newly instantiated MblMwGyroBmi160Config struct
 */
METAWEAR_API MblMwGyroBmi160Config* mbl_mw_gyro_bmi160_create_config();
/**
 * Frees memory allocated for a MblMwGyroBmi160Config struct
 * @param config        Pointer to the memory to free
 */
METAWEAR_API void mbl_mw_gyro_bmi160_free_config(MblMwGyroBmi160Config *config);
/**
 * Sets the output data rate 
 * @param config    Pointer to the MblMwAccBmi160Config struct to modify
 * @param odr       Output data rate value to assign
 */
METAWEAR_API void mbl_mw_gyro_bmi160_set_odr(MblMwGyroBmi160Config *config, MblMwGyroBmi160Odr odr);
/**
 * Sets the rotation range
 * @param config    Pointer to the MblMwGyroBmi160Config struct to modify
 * @param range     New rotation range
 */
METAWEAR_API void mbl_mw_gyro_bmi160_set_range(MblMwGyroBmi160Config *config, MblMwGyroBmi160Range range);
/**
 * Constructs the command that writes the configuration to the board
 * @param command   Byte array (len 4) for the function to write the command to
 * @param config    Pointer to the gyro configuration 
 */
METAWEAR_API void mbl_mw_gyro_bmi160_write_config(uint8_t command[4], const MblMwGyroBmi160Config *config);

/**
 * Constructs the command that puts the gyro in active mode.  While in active mode, the gyro cannot be configured
 * @param command   Byte array (len 3) for the function to write the command to
 */
METAWEAR_API void mbl_mw_gyro_bmi160_start(uint8_t command[3]);
/**
 * Constructs the command that puts the gyro in standby mode.
 * @param command   Byte array (len 3) for the function to write the command to
 */
METAWEAR_API void mbl_mw_gyro_bmi160_stop(uint8_t command[3]);

/**
 * Constructs the command that enables rotation sampling
 * @param command   Byte array (len 4) for the function to write the command to
 */
METAWEAR_API void mbl_mw_gyro_bmi160_enable_rotation_sampling(uint8_t command[4]);
/**
 * Constructs the command that disables rotation sampling
 * @param command   Byte array (len 4) for the function to write the command to
 */
METAWEAR_API void mbl_mw_gyro_bmi160_disable_rotation_sampling(uint8_t command[4]);

/**
 * Extracts the rotation data from a response sent by the BMI160 gyro in degrees per second (dps)
 * @param data_dps  Pointer to a CartesianFloat to write the rotation data to
 * @param config    Pointer to the MblMwGyroBmi160Config struct representing the current gyro configuration
 * @param response  Response received from the BMI160 gyro
 * @return #STATUS_OK if the response contains valid rotation data, #STATUS_INVALID_RESPONSE otherwise
 */
METAWEAR_API int mbl_mw_gyro_bmi160_get_rotation_data(CartesianFloat *data_dps, const MblMwGyroBmi160Config *config, uint8_t response[8]);

#ifdef	__cplusplus
}
#endif
