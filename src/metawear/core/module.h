/**
 * @copyright MbientLab License
 * @file module.h
 */

#pragma once

/**
 * Sensors or peripherals supported by the firmware.  Different board skews have different module combinations, 
 * use mbl_mw_metawearboard_lookup_module to check if a module is present on your board
 */
typedef enum {
    MBL_MW_MODULE_SWITCH = 1,
    MBL_MW_MODULE_LED,
    MBL_MW_MODULE_ACCELEROMETER,
    MBL_MW_MODULE_TEMPERATURE,
    MBL_MW_MODULE_GPIO,
    MBL_MW_MODULE_NEO_PIXEL,
    MBL_MW_MODULE_IBEACON,
    MBL_MW_MODULE_HAPTIC,
    MBL_MW_MODULE_DATA_PROCESSOR,
    MBL_MW_MODULE_EVENT,
    MBL_MW_MODULE_LOGGING,
    MBL_MW_MODULE_TIMER,
    MBL_MW_MODULE_I2C,
    MBL_MW_MODULE_MACRO = 0xf,
    MBL_MW_MODULE_CONDUCTANCE,
    MBL_MW_MODULE_SETTINGS,
    MBL_MW_MODULE_BAROMETER,
    MBL_MW_MODULE_GYRO,
    MBL_MW_MODULE_AMBIENT_LIGHT,
    MBL_MW_MODULE_MAGNETOMETER,
    MBL_MW_MODULE_HUMIDITY,
    MBL_MW_MODULE_COLOR_DETECTOR,
    MBL_MW_MODULE_PROXIMITY,
    MBL_MW_MODULE_SENSOR_FUSION,
    MBL_MW_MODULE_DEBUG = 0xfe
} MblMwModule;
