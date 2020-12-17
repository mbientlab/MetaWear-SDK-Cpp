#include "dfu_operations.h"

#include <string.h>

DfuOperations::DfuOperations(const MblMwMetaWearBoard* board, const MblMwDfuDelegate *delegate) : dfuRequests(new DFUOperationsDetails(board)), fileRequests(new FileOperations(*this, board)) {
    memcpy(&this->dfuDelegate, delegate, sizeof(MblMwDfuDelegate));
}

DfuOperations::~DfuOperations() {

}

void DfuOperations::cancelDFU() {
    //NSLog(@"cancelDFU");
    dfuRequests->resetSystem();
    dfuDelegate.on_dfu_cancelled(dfuDelegate.context);
}

//-(void)performDFUOnFiles:(NSURL *)softdeviceURL bootloaderURL:(NSURL *)bootloaderURL firmwareType:(MBL_DfuFirmwareTypes)firmwareType
//{
//    isPerformedOldDFU = NO;
//    [self initFirstMBL_FileOperations];
//    [self initSecondMBL_FileOperations];
//    [self initParameters];
//    self.dfuFirmwareType = firmwareType;
//    [fileRequests openFile:softdeviceURL];
//    [fileRequests2 openFile:bootloaderURL];
//    [dfuRequests enableNotification];
//    [dfuRequests startDFU:firmwareType];
//    [dfuRequests writeFilesSizes:(uint32_t)fileRequests.binFileSize bootloaderSize:(uint32_t)fileRequests2.binFileSize];
//}

//-(void)performDFUOnFile:(NSURL *)firmwareURL firmwareType:(MBL_DfuFirmwareTypes)firmwareType
//{
//    isPerformedOldDFU = NO;
//    firmwareFile = firmwareURL;
//    [self initFirstMBL_FileOperations];
//    isStartingSecondFile = NO;
//    [self initParameters];
//    self.dfuFirmwareType = firmwareType;
//    [fileRequests openFile:firmwareURL];
//    [dfuRequests enableNotification];
//    [dfuRequests startDFU:firmwareType];
//    [dfuRequests writeFileSize:(uint32_t)fileRequests.binFileSize];
//}

void DfuOperations::perfromDFUOnZipFile(const char *zipFilename) {
    isVersionCharacteristicExist = true;
    isPerformedOldDFU = false;
    //firmwareFile = firmwareURL;
    //[self initFirstFileOperations];
    //isStartingSecondFile = NO;
    //[self initParameters];
    //self.dfuFirmwareType = firmwareType;
    fileRequests->openZip(zipFilename);
    //[dfuRequests enableNotification];
    dfuRequests->startDFU(APPLICATION);
    dfuRequests->writeFileSize(static_cast<uint32_t>(fileRequests->binFileSize));
}

void DfuOperations::performOldDFUOnFile(const char *firmwareFilename) {
    isVersionCharacteristicExist = false;
    isPerformedOldDFU = true;
    if (firmwareFilename) {
//        [self initFirstMBL_FileOperations];
//        [self initParameters];
        fileRequests->openFile(firmwareFilename);
//        [dfuRequests enableNotification];
        dfuRequests->startOldDFU();
        dfuRequests->writeFileSizeForOldDFU(static_cast<uint32_t>(fileRequests->binFileSize));
    } else {
        const char *errorMessage = "Old DFU only supports Application upload";
        dfuDelegate.on_error(dfuDelegate.context, errorMessage);
        dfuRequests->resetSystem();
    }
}

//-(void)initParameters
//{
//    startTime = [NSDate date];
//    binFileSize = 0;
//    isStartingSecondFile = NO;
//}

//-(void)initFirstMBL_FileOperations
//{
//    fileRequests = [[MBL_FileOperations alloc]initWithDelegate:self
//                                                 blePeripheral:self.bluetoothPeripheral
//                                             bleCharacteristic:self.dfuPacketCharacteristic];
//}
//
//-(void)initSecondMBL_FileOperations
//{
//    fileRequests2 = [[MBL_FileOperations alloc]initWithDelegate:self
//                                                  blePeripheral:self.bluetoothPeripheral
//                                              bleCharacteristic:self.dfuPacketCharacteristic];
//}

void DfuOperations::startSendingFile() {
    //    if (self.dfuFirmwareType == SOFTDEVICE || self.dfuFirmwareType == SOFTDEVICE_AND_BOOTLOADER) {
    //        NSLog(@"waiting 10 seconds before sending file ...");
    //        //Delay of 10 seconds is required in order to update Softdevice in SDK 6.0
    //        dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delayInSeconds * NSEC_PER_SEC));
    //        dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
    //            [dfuRequests enablePacketNotification];
    //            [dfuRequests receiveFirmwareImage];
    //            [fileRequests writeNextPacket];
    //            [dfuDelegate onDFUStarted];
    //        });
    //    }
    //    else {
    dfuRequests->enablePacketNotification();
    dfuRequests->receiveFirmwareImage();
    fileRequests->writeNextPacket();
    dfuDelegate.on_dfu_started(dfuDelegate.context);
    //    }
    //    if (self.dfuFirmwareType == SOFTDEVICE_AND_BOOTLOADER) {
    //        [dfuDelegate onSoftDeviceUploadStarted];
    //    }
}
//
//-(NSString *) responseErrorMessage:(MBL_DfuOperationStatus)errorCode
//{
//    switch (errorCode) {
//        case OPERATION_FAILED_RESPONSE:
//            return @"Operation Failed";
//            break;
//        case OPERATION_INVALID_RESPONSE:
//            return @"Invalid Response";
//            break;
//        case OPERATION_NOT_SUPPORTED_RESPONSE:
//            return @"Operation Not Supported";
//            break;
//        case DATA_SIZE_EXCEEDS_LIMIT_RESPONSE:
//            return @"Data Size Exceeds";
//            break;
//        case CRC_ERROR_RESPONSE:
//            return @"CRC Error";
//            break;
//        default:
//            return @"unknown Error";
//            break;
//    }
//}

void DfuOperations::processRequestedCode() {
//    NSLog(@"processsRequestedCode");
    switch (dfuResponse.requestedCode) {
        case START_DFU_REQUEST:
//            NSLog(@"Requested code is StartDFU now processing response status");
            processStartDFUResponseStatus();
            break;
        case RECEIVE_FIRMWARE_IMAGE_REQUEST:
//            NSLog(@"Requested code is Receive Firmware Image now processing response status");
            processReceiveFirmwareResponseStatus();
            break;
        case VALIDATE_FIRMWARE_REQUEST:
//            NSLog(@"Requested code is Validate Firmware now processing response status");
            processValidateFirmwareResponseStatus();
            break;
        case INITIALIZE_DFU_PARAMETERS_REQUEST:
//            NSLog(@"Requested code is Initialize DFU Parameters now processing response status");
            processInitPacketResponseStatus();
            break;
        default:
//            NSLog(@"invalid Requested code in DFU Response %d",dfuResponse.requestedCode);
            break;
    }
}

void DfuOperations::processStartDFUResponseStatus() {
//    NSLog(@"processStartDFUResponseStatus");
//    NSString *errorMessage = [NSString stringWithFormat:@"Error on StartDFU\n Message: %@",[self responseErrorMessage:dfuResponse.responseStatus]];
    switch (dfuResponse.responseStatus) {
        case OPERATION_SUCCESSFUL_RESPONSE:
//            NSLog(@"successfully received startDFU notification");
            if (isVersionCharacteristicExist) {
                dfuRequests->sendInitPacket(fileRequests->metaDataFile, static_cast<uint32_t>(fileRequests->metaDataFileSize));
            } else {
                startSendingFile();
            }
            break;
        case OPERATION_NOT_SUPPORTED_RESPONSE:
//            if (!isPerformedOldDFU) {
//                NSLog(@"device has old DFU. switching to old DFU ...");
//                [self performOldDFUOnFile:firmwareFile];
//            }
//            else {
//                NSLog(@"Operation not supported");
//                NSLog(@"Firmware Image failed, Error Status: %@",[self responseErrorMessage:dfuResponse.responseStatus]);
//                NSString *errorMessage = [NSString stringWithFormat:@"Error on StartDFU\n Message: %@",[self responseErrorMessage:dfuResponse.responseStatus]];
            dfuDelegate.on_error(dfuDelegate.context, "Error on StartDFU: OPERATION_NOT_SUPPORTED_RESPONSE");
            dfuRequests->resetSystem();
//            }
            break;
            
        default:
//            NSLog(@"StartDFU failed, Error Status: %@",[self responseErrorMessage:dfuResponse.responseStatus]);
            dfuDelegate.on_error(dfuDelegate.context, "Error on StartDFU: Unkown Response");
            dfuRequests->resetSystem();
            break;
    }
}

void DfuOperations::processInitPacketResponseStatus() {
//    NSLog(@"processInitPacketResponseStatus");
    if (dfuResponse.responseStatus == OPERATION_SUCCESSFUL_RESPONSE) {
//        NSLog(@"successfully received initPacket notification");
        startSendingFile();
    }
    else {
        //NSLog(@"unsuccessfull initPacket notification %d",dfuResponse.responseStatus);
//        NSLog(@"Init Packet failed, Error Status: %@",[self responseErrorMessage:dfuResponse.responseStatus]);
//        NSString *errorMessage = [NSString stringWithFormat:@"Error on Init Packet\n Message: %@",[self responseErrorMessage:dfuResponse.responseStatus]];
//        [dfuDelegate onError:errorMessage];
//        [dfuRequests resetSystem];
        dfuDelegate.on_error(dfuDelegate.context, "Error on Init Packet");
        dfuRequests->resetSystem();
    }
}

void DfuOperations::processReceiveFirmwareResponseStatus() {
//    NSLog(@"processReceiveFirmwareResponseStatus");
    if (dfuResponse.responseStatus == OPERATION_SUCCESSFUL_RESPONSE) {
//        NSLog(@"successfully received notification for whole File transfer");
        dfuRequests->validateFirmware();
    }
    else {
//        NSLog(@"Firmware Image failed, Error Status: %@",[self responseErrorMessage:dfuResponse.responseStatus]);
//        NSString *errorMessage = [NSString stringWithFormat:@"Error on Receive Firmware Image\n Message: %@",[self responseErrorMessage:dfuResponse.responseStatus]];
        dfuDelegate.on_error(dfuDelegate.context, "Error on Receive Firmware Image");
        dfuRequests->resetSystem();
    }
}

void DfuOperations::processValidateFirmwareResponseStatus() {
//    NSLog(@"processValidateFirmwareResponseStatus");
    if (dfuResponse.responseStatus == OPERATION_SUCCESSFUL_RESPONSE) {
//        NSLog(@"succesfully received notification for ValidateFirmware");
        dfuRequests->activateAndReset();
//        [self calculateDFUTime];
        dfuDelegate.on_successful_file_transferred(dfuDelegate.context);
    }
    else {
//        NSLog(@"Firmware validate failed, Error Status: %@",[self responseErrorMessage:dfuResponse.responseStatus]);
//        NSString *errorMessage = [NSString stringWithFormat:@"Error on Validate Firmware Request\n Message: %@",[self responseErrorMessage:dfuResponse.responseStatus]];
        dfuDelegate.on_error(dfuDelegate.context, "Error on Validate Firmware Request");
        dfuRequests->resetSystem();
    }
}

void DfuOperations::processPacketNotification() {
    //NSLog(@"received Packet Received Notification");
//    if (isStartingSecondFile) {
//        if (fileRequests2.writingPacketNumber < fileRequests2.numberOfPackets) {
//            [fileRequests2 writeNextPacket];
//        }
//    }
//    else {
        if (fileRequests->writingPacketNumber < fileRequests->numberOfPackets) {
            fileRequests->writeNextPacket();
        }
//    }
}

void DfuOperations::processDFUResponse(const uint8_t *data, uint8_t len) {
//    NSLog(@"processDFUResponse");
    setDFUResponseStruct(data, len);
    if (dfuResponse.responseCode == RESPONSE_CODE) {
        processRequestedCode();
    }
    else if (dfuResponse.responseCode == PACKET_RECEIPT_NOTIFICATION_RESPONSE) {
        processPacketNotification();
    }
}

void DfuOperations::setDFUResponseStruct(const uint8_t *data, uint8_t len) {
    while (len < 3) { } // TODO: Remove me
    dfuResponse.responseCode = data[0];
    dfuResponse.requestedCode = data[1];
    dfuResponse.responseStatus = data[2];
}

//
//-(void)setMBL_DFUOperationsDetails
//{
//    [self.dfuRequests setPeripheralAndOtherParameters:self.bluetoothPeripheral
//                           controlPointCharacteristic:self.dfuControlPointCharacteristic
//                                 packetCharacteristic:self.dfuPacketCharacteristic];
//}
//
//-(void)calculateDFUTime
//{
//    finishTime = [NSDate date];
//    self.uploadTimeInSeconds = [finishTime timeIntervalSinceDate:startTime];
//    NSLog(@"upload time in sec: %u", (int)self.uploadTimeInSeconds);
//}
//
//#pragma mark - MBL_BLEOperations delegates
//
//-(void)onDeviceConnected:(CBPeripheral *)peripheral withPacketCharacteristic:(CBCharacteristic *)dfuPacketCharacteristic andControlPointCharacteristic:(CBCharacteristic *)dfuControlPointCharacteristic
//{
//    self.bluetoothPeripheral = peripheral;
//    self.dfuPacketCharacteristic = dfuPacketCharacteristic;
//    self.dfuControlPointCharacteristic = dfuControlPointCharacteristic;
//    [self setMBL_DFUOperationsDetails];
//    [dfuDelegate onDeviceConnected:peripheral];
//}
//
//-(void)onDeviceDisconnected:(CBPeripheral *)peripheral
//{
//    [dfuDelegate onDeviceDisconnected:peripheral];
//}
//


// FileOperations delegates

void DfuOperations::onTransferPercentage(int32_t percentage) {
//    //NSLog(@"MBL_DFUOperations: onTransferPercentage %d",percentage);
    dfuDelegate.on_transfer_percentage(dfuDelegate.context, percentage);
}
void DfuOperations::onAllPacketsTranferred() {
//    NSLog(@"MBL_DFUOperations: onAllPacketsTransfered");
//    if (isStartingSecondFile) {
//        [dfuDelegate onBootloaderUploadCompleted];
//    }
//    else if (self.dfuFirmwareType == SOFTDEVICE_AND_BOOTLOADER) {
//        isStartingSecondFile = YES;
//        NSLog(@"Firmware type is Softdevice plus Bootloader. now upload bootloader ...");
//        [dfuDelegate onSoftDeviceUploadCompleted];
//        [dfuDelegate onBootloaderUploadStarted];
//        [fileRequests2 writeNextPacket];
//    }
}
void DfuOperations::onFileOpened(size_t) {
//    NSLog(@"onFileOpened file size: %d", (int)fileSizeOfBin);
//    binFileSize += fileSizeOfBin;
}
void DfuOperations::onError(const std::string &errorMessage) {
//    NSLog(@"MBL_DFUOperations: onError");
    dfuDelegate.on_error(dfuDelegate.context, errorMessage.c_str());
}
