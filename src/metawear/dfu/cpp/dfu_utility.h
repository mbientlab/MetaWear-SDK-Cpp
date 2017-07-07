#pragma once

//
//  MBL_Utility.h
//  nRFDeviceFirmwareUpdate
//
//  Created by Nordic Semiconductor on 22/05/14.
//  Copyright (c) 2014 Nordic Semiconductor. All rights reserved.
//

#include <stdint.h>

//#import <Foundation/Foundation.h>

//@interface MBL_Utility : NSObject

//extern NSString * const MBL_dfuServiceUUIDString;
//extern NSString * const MBL_dfuControlPointCharacteristicUUIDString;
//extern NSString * const MBL_dfuPacketCharacteristicUUIDString;

//extern NSString* const MBL_FIRMWARE_TYPE_SOFTDEVICE;
//extern NSString* const MBL_FIRMWARE_TYPE_BOOTLOADER;
//extern NSString* const MBL_FIRMWARE_TYPE_APPLICATION;
//extern NSString* const MBL_FIRMWARE_TYPE_BOTH_SOFTDEVICE_BOOTLOADER;
//
//

extern int MBL_PACKETS_NOTIFICATION_INTERVAL;
extern int const MBL_PACKET_SIZE;

struct DFUResponse
{
    uint8_t responseCode;
    uint8_t requestedCode;
    uint8_t responseStatus;
};

//typedef enum {
//    HEX,
//    ZIP
//} MblDfuFileExtension;

typedef enum {
    START_INIT_PACKET = 0x00,
    END_INIT_PACKET = 0x01
} MblInitPacketParam;

/**
 * Enumeration of DFU opcodes
 */
typedef enum {
    START_DFU_REQUEST = 0x01,
    INITIALIZE_DFU_PARAMETERS_REQUEST = 0x02,
    RECEIVE_FIRMWARE_IMAGE_REQUEST = 0x03,
    VALIDATE_FIRMWARE_REQUEST = 0x04,
    ACTIVATE_AND_RESET_REQUEST = 0x05,
    RESET_SYSTEM = 0x06,
    PACKET_RECEIPT_NOTIFICATION_REQUEST = 0x08,
    RESPONSE_CODE = 0x10,
    PACKET_RECEIPT_NOTIFICATION_RESPONSE = 0x11
} MblDfuOperations;

/**
 * Enumeration of DFU status codes
 */
typedef enum {
    OPERATION_SUCCESSFUL_RESPONSE = 0x01,
    OPERATION_INVALID_RESPONSE = 0x02,
    OPERATION_NOT_SUPPORTED_RESPONSE = 0x03,
    DATA_SIZE_EXCEEDS_LIMIT_RESPONSE = 0x04,
    CRC_ERROR_RESPONSE = 0x05,
    OPERATION_FAILED_RESPONSE = 0x06
} MblDfuOperationStatus;

/**
 * Enumeration of DFU firmware types
 */
typedef enum {
    SOFTDEVICE = 0x01,
    BOOTLOADER = 0x02,
    SOFTDEVICE_AND_BOOTLOADER = 0x03,
    APPLICATION = 0x04
} MblDfuFirmwareTypes;

//+ (NSArray *) getFirmwareTypes;
//+ (NSString *) stringFileExtension:(MBL_enumFileExtension)fileExtension;
//+ (NSString *) getDFUHelpText;
//+ (NSString *) getEmptyUserFilesText;
//+ (NSString *) getDFUAppFileHelpText;

