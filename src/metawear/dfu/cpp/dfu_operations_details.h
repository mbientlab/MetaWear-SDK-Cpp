#pragma once

#include <stdint.h>

#include "metawear/dfu/metabootboard_fwd.h"
#include "dfu_utility.h"

class DFUOperationsDetails {
    //@property (nonatomic) MBL_DfuFirmwareTypes dfuFirmwareType;
    const MblMwMetaBootBoard* bootloaderBoard;
    MblDfuFirmwareTypes dfuFirmwareType;
    
public:
    DFUOperationsDetails(const MblMwMetaBootBoard* board);
    
    //void enableNotification();
    void startDFU(MblDfuFirmwareTypes firmwareType);
    void startOldDFU();
    void writeFileSize(uint32_t firmwareSize);
    //void writeFilesSizes:(uint32_t)softdeviceSize bootloaderSize:(uint32_t)bootloaderSize;
    void writeFileSizeForOldDFU(uint32_t firmwareSize);
    void enablePacketNotification();
    void receiveFirmwareImage();
    void validateFirmware();
    void activateAndReset();
    void resetSystem();
    
    //Init Packet is included in new DFU in SDK 7.0
    void sendInitPacket(uint8_t *fileData, int fileDataLength);
};
