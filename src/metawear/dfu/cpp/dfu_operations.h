#pragma once

//
//  MBL_DFUOperations.h
//  nRFDeviceFirmwareUpdate
//
//  Created by Nordic Semiconductor on 18/06/14.
//  Copyright (c) 2014 Nordic Semiconductor. All rights reserved.
//
//
//#import <Foundation/Foundation.h>
//#import <CoreBluetooth/CoreBluetooth.h>
#include <iostream>
#include <memory>

#include "dfu_operations_details.h"
#include "dfu_utility.h"
#include "file_operations.h"
#include "metawear/core/metawearboard.h"
//#import "MBL_BLEOperations.h"



//
//
//@class MBL_DFUOperations;
//
////define protocol for the delegate
//@protocol MBL_DFUOperationsDelegate
//
////define protocol functions that can be used in any class using this delegate
//-(void)onDeviceConnected:(CBPeripheral *)peripheral;
//-(void)onDeviceDisconnected:(CBPeripheral *)peripheral;
//-(void)onDFUStarted;
//-(void)onDFUCancelled;
//-(void)onSoftDeviceUploadStarted;
//-(void)onBootloaderUploadStarted;
//-(void)onSoftDeviceUploadCompleted;
//-(void)onBootloaderUploadCompleted;
//-(void)onTransferPercentage:(int)percentage;
//-(void)onSuccessfulFileTranferred;
//-(void)onError:(NSString *)errorMessage;
//
//@end

class DfuOperations : public FileOperationsDelegate {
    //@property (nonatomic) CBPeripheral *bluetoothPeripheral;
    //@property (nonatomic) CBCharacteristic *dfuPacketCharacteristic;
    //@property (nonatomic) CBCharacteristic *dfuControlPointCharacteristic;
    //
    //@property (nonatomic) MBL_BLEOperations *bleOperations;
    std::unique_ptr<DFUOperationsDetails> dfuRequests;
    std::unique_ptr<FileOperations> fileRequests;
    //@property (nonatomic) MBL_FileOperations *fileRequests2;
    //@property (nonatomic) MBL_DfuFirmwareTypes dfuFirmwareType;
    //@property (nonatomic) NSUInteger binFileSize;
    //@property (nonatomic) NSUInteger uploadTimeInSeconds;
    //@property (nonatomic) NSURL *firmwareFile;
    struct DFUResponse dfuResponse;
    
    MblMwDfuDelegate dfuDelegate;
    
    bool isVersionCharacteristicExist;
    bool isPerformedOldDFU;
    
    void setDFUResponseStruct(const uint8_t *data, uint8_t len);
    void processRequestedCode();
    void processStartDFUResponseStatus();
    void processReceiveFirmwareResponseStatus();
    void processValidateFirmwareResponseStatus();
    void processInitPacketResponseStatus();
    
    void processPacketNotification();
    
    void startSendingFile();

    // FileOperationsDelegate
    void onTransferPercentage(int32_t);
    void onAllPacketsTranferred();
    void onFileOpened(size_t);
    void onError(const std::string &);
    
public:
    DfuOperations(const MblMwMetaWearBoard* board, const MblMwDfuDelegate *delegate);
    virtual ~DfuOperations();
    
    //define public methods
    //void setCentralManager:(CBCentralManager *)manager;
    //void connectDevice:(CBPeripheral *)peripheral;
    //void performDFUOnFile:(NSURL *)firmwareURL firmwareType:(MBL_DfuFirmwareTypes)firmwareType;
    //void performDFUOnFiles:(NSURL *)softdeviceURL bootloaderURL:(NSURL *)bootloaderURL firmwareType:(MBL_DfuFirmwareTypes)firmwareType;
    void perfromDFUOnZipFile(const char *zipFilename);
    void performOldDFUOnFile(const char *firmwareFilename);
    
    void cancelDFU();
    
    
    void processDFUResponse(const uint8_t *data, uint8_t len);
};
