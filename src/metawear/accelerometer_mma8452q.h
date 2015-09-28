/**
 * @copyright MbientLab License 
 * @file accelerometer_mma8452q.h
 * @brief Functions for interacting with the MMA8452Q accelerometer.
 * This sensor is only available on MetaWear R boards.
 */
#pragma once

#include <stdint.h>

#include "dllmarker.h"
#include "sensor_defs.h"
#include "types.h"

#ifdef	__cplusplus
extern "C" {
#endif

const uint8_t MBL_MW_ACC_MMA8452Q_MODULE= 0x3;      ///< Module id for the MMA8452Q accelerometer
const uint8_t MBL_MW_ACC_MMA8452Q_DATA= 0x4;        ///< Register id for MMA8452Q acceleration data

/**
 * Available g-ranges on the MMA8452Q accelerometer
 */
typedef enum {
    MBL_MW_ACC_MMA8452Q_FSR_2G= 0,  ///< +/- 2g
    MBL_MW_ACC_MMA8452Q_FSR_4G,     ///< +/- 4g
    MBL_MW_ACC_MMA8452Q_FSR_8G      ///< +/- 8g
} MblMwAccMma8452qRange;

/**
 * Available output data rates on the MMA8452Q accelerometer
 */
typedef enum {
    MBL_MW_ACC_MMA8452Q_ODR_800HZ= 0,
    MBL_MW_ACC_MMA8452Q_ODR_400HZ,
    MBL_MW_ACC_MMA8452Q_ODR_200HZ,
    MBL_MW_ACC_MMA8452Q_ODR_100HZ,
    MBL_MW_ACC_MMA8452Q_ODR_50HZ,
    MBL_MW_ACC_MMA8452Q_ODR_12_5HZ,
    MBL_MW_ACC_MMA8452Q_ODR_6_25HZ,
    MBL_MW_ACC_MMA8452Q_ODR_1_56HZ
} MblMwAccMma8452qOdr;

/**
 * Container for MMA8452Q configuration
 */
typedef struct MblMwAccMma8452qConfig MblMwAccMma8452qConfig;

/**
 * Retrieves the attributes for the MMA8452Q acceleration data 
 * @return Pointer to the MMA8452Q acceleration data source
 */
METAWEAR_API const DataSource* mbl_mw_acc_mma8452q_get_acceleration_data_source();

/**
 * Instantiates a MblMwAccMma8452qConfig struct with an output data rate of 100Hz and a g-range of +/- 2g.
 * The user is responsible for deallocating the memory by calling mbl_mw_acc_mma8452q_free_config()
 */
METAWEAR_API MblMwAccMma8452qConfig* mbl_mw_acc_mma8452q_create_config();
/**
 * Frees the memory allocated for an MblMwAccMma8452qConfig struct
 * @param config    Pointer to the memory to free
 */
METAWEAR_API void mbl_mw_acc_mma8452q_free_config(MblMwAccMma8452qConfig *config);
/**
 * Sets the output data rate
 * @param config    Pointer to the MblMwAccMma8452qConfig struct to modify
 * @param odr       Output data rate value to set
 */
METAWEAR_API void mbl_mw_acc_mma8452q_set_odr(MblMwAccMma8452qConfig *config, MblMwAccMma8452qOdr odr);
/**
 * Sets the acceleration range 
 * @param config    Pointer to the MblMwAccMma8452qConfig struct to modify
 * @param range     Acceleration range value to set
 */
METAWEAR_API void mbl_mw_acc_mma8452q_set_range(MblMwAccMma8452qConfig *config, MblMwAccMma8452qRange range);
/**
 * Constructs the command that writes the configuration to the MMA8452Q chip
 * @param command   Byte array (lenn 7) for the function to write the command to
 * @param config    Pointer to the MMA8452Q configuration
 */
METAWEAR_API void mbl_mw_acc_mma8452q_write_acceleration_config(uint8_t command[7], const MblMwAccMma8452qConfig *config);

/**
 * Constructs the command that puts the accelerometer in active mode.  When in active mode, the accelerometer cannot be configured
 * @param command   Byte array (len 3) for the function to write the command to
 */
METAWEAR_API void mbl_mw_acc_mma8452q_start(uint8_t command[3]);
/**
 * Constructs the command that puts the accelerometer in standby mode
 * @param command   Byte array (len 3) for the function to write the command to
 */
METAWEAR_API void mbl_mw_acc_mma8452q_stop(uint8_t command[3]);

/**
 * Constructs the command that enables acceleration sampling
 * @param command   Byte array (len 3) for the function to write the command to
 */
METAWEAR_API void mbl_mw_acc_mma8452q_enable_acceleration_sampling(uint8_t command[3]);
/**
 * Constructs the command that disables acceleration sampling
 * @param command   Byte array (len 3) for the function to write the command to
 */
METAWEAR_API void mbl_mw_acc_mma8452q_disable_acceleration_sampling(uint8_t command[3]);

/**
 * Extracts the acceleration data from a response sent by the MMA8452Q chip in milli Gs
 * @param data_g   Pointer to a CartesianShort to write the acceleration data to
 * @param response  Response received from the MMA8452Q chip
 * @return #STATUS_OK if the response contains valid acceleration data, #STATUS_INVALID_RESPONSE otherwise
 */
METAWEAR_API int mbl_mw_acc_mma8452q_get_acceleration_data_g(CartesianFloat *data_g, const uint8_t response[8]);
/**
 * Extracts the acceleration data from a response sent by the MMA8452Q chip in Gs
 * @param data_mg   Pointer to a CartesianShort to write the acceleration data to
 * @param response  Response received from the MMA8452Q chip
 * @return #STATUS_OK if the response contains valid acceleration data, #STATUS_INVALID_RESPONSE otherwise
 */
METAWEAR_API int mbl_mw_acc_mma8452q_get_acceleration_data_mg(CartesianShort *data_mg, const uint8_t response[8]);

#ifdef	__cplusplus
}
#endif
