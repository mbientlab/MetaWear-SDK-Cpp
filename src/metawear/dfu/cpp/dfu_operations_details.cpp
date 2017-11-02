#include "dfu_operations_details.h"

#include <string.h>
#include <cmath>

#include "dfu_utility.h"
#include "metawear/core/cpp/metawearboard_def.h"

DFUOperationsDetails::DFUOperationsDetails(const MblMwMetaWearBoard* board) : bootloaderBoard(board) {
}

void DFUOperationsDetails::startDFU(MblDfuFirmwareTypes firmwareType) {
    dfuFirmwareType = firmwareType;
    uint8_t value[] = { START_DFU_REQUEST, (uint8_t)firmwareType };
    bootloaderBoard->write_gatt_char(&DFU_CONTROL_POINT_CHAR, MBL_MW_GATT_CHAR_WRITE_WITH_RESPONSE, value, sizeof(value)/sizeof(value[0]));
}

void DFUOperationsDetails::startOldDFU() {
    uint8_t value[] = { START_DFU_REQUEST };
    bootloaderBoard->write_gatt_char(&DFU_CONTROL_POINT_CHAR, MBL_MW_GATT_CHAR_WRITE_WITH_RESPONSE, value, sizeof(value)/sizeof(value[0]));
}

void DFUOperationsDetails::writeFileSize(uint32_t firmwareSize) {
    uint32_t fileSizeCollection[3];
    switch (dfuFirmwareType) {
        case SOFTDEVICE:
            fileSizeCollection[0] = firmwareSize;
            fileSizeCollection[1] = 0;
            fileSizeCollection[2] = 0;
            break;
        case BOOTLOADER:
            fileSizeCollection[0] = 0;
            fileSizeCollection[1] = firmwareSize;
            fileSizeCollection[2] = 0;
            break;
        case APPLICATION:
            fileSizeCollection[0] = 0;
            fileSizeCollection[1] = 0;
            fileSizeCollection[2] = firmwareSize;
            break;
        default:
            break;
    }
    bootloaderBoard->write_gatt_char(&DFU_PACKET_CHAR, MBL_MW_GATT_CHAR_WRITE_WITHOUT_RESPONSE, (uint8_t *)&fileSizeCollection, sizeof(fileSizeCollection));
}

void DFUOperationsDetails::writeFileSizeForOldDFU(uint32_t firmwareSize) {
    bootloaderBoard->write_gatt_char(&DFU_PACKET_CHAR, MBL_MW_GATT_CHAR_WRITE_WITHOUT_RESPONSE, (uint8_t *)&firmwareSize, sizeof(firmwareSize));
}

void DFUOperationsDetails::enablePacketNotification() {
    uint8_t value[] = { PACKET_RECEIPT_NOTIFICATION_REQUEST, (uint8_t)MBL_PACKETS_NOTIFICATION_INTERVAL, 0} ;
    bootloaderBoard->write_gatt_char(&DFU_CONTROL_POINT_CHAR, MBL_MW_GATT_CHAR_WRITE_WITH_RESPONSE, value, sizeof(value)/sizeof(value[0]));
}
void DFUOperationsDetails::receiveFirmwareImage() {
    uint8_t value[] = { RECEIVE_FIRMWARE_IMAGE_REQUEST };
    bootloaderBoard->write_gatt_char(&DFU_CONTROL_POINT_CHAR, MBL_MW_GATT_CHAR_WRITE_WITH_RESPONSE, value, sizeof(value)/sizeof(value[0]));
}

void DFUOperationsDetails::validateFirmware() {
    uint8_t value[] = { VALIDATE_FIRMWARE_REQUEST };
    bootloaderBoard->write_gatt_char(&DFU_CONTROL_POINT_CHAR, MBL_MW_GATT_CHAR_WRITE_WITH_RESPONSE, value, sizeof(value)/sizeof(value[0]));
}

void DFUOperationsDetails::activateAndReset() {
    uint8_t value[] = { ACTIVATE_AND_RESET_REQUEST };
    bootloaderBoard->write_gatt_char(&DFU_CONTROL_POINT_CHAR, MBL_MW_GATT_CHAR_WRITE_WITH_RESPONSE, value, sizeof(value)/sizeof(value[0]));
}

void DFUOperationsDetails::resetSystem() {
    uint8_t value[] = { RESET_SYSTEM };
    bootloaderBoard->write_gatt_char(&DFU_CONTROL_POINT_CHAR, MBL_MW_GATT_CHAR_WRITE_WITH_RESPONSE, value, sizeof(value)/sizeof(value[0]));
}

void DFUOperationsDetails::sendInitPacket(uint8_t *fileData, int fileDataLength) {
    int numberOfPackets = std::ceil((double)fileDataLength / (double)MBL_PACKET_SIZE);
    int bytesInLastPacket = fileDataLength % 20;
    //NSLog(@"metaDataFile length: %lu and number of packets: %d",(unsigned long)[fileData length], numberOfPackets);
    
    //send initPacket with parameter value set to Receive Init Packet [0] to dfu Control Point Characteristic
    uint8_t initPacketStart[] = {INITIALIZE_DFU_PARAMETERS_REQUEST, START_INIT_PACKET};
    bootloaderBoard->write_gatt_char(&DFU_CONTROL_POINT_CHAR, MBL_MW_GATT_CHAR_WRITE_WITH_RESPONSE, (uint8_t *)&initPacketStart, sizeof(initPacketStart));

    // send init Packet data to dfu Packet Characteristic
    // for longer .dat file the data need to be chopped into 20 bytes
    for (int index = 0; index < numberOfPackets-1; index++) {
        //chopping data into 20 bytes packet
        uint8_t *packetData = &fileData[index * MBL_PACKET_SIZE];
        //writing 20 bytes packet to peripheral
        bootloaderBoard->write_gatt_char(&DFU_PACKET_CHAR, MBL_MW_GATT_CHAR_WRITE_WITHOUT_RESPONSE, packetData, MBL_PACKET_SIZE);
    }
    //chopping data for last packet that can be less than 20 bytes
    uint8_t *packetData = &fileData[(numberOfPackets - 1) * MBL_PACKET_SIZE];
    //writing last packet
    bootloaderBoard->write_gatt_char(&DFU_PACKET_CHAR, MBL_MW_GATT_CHAR_WRITE_WITHOUT_RESPONSE, packetData, bytesInLastPacket);
    
    //send initPacket with parameter value set to Init Packet Complete [1] to dfu Control Point Characteristic
    uint8_t initPacketEnd[] = { INITIALIZE_DFU_PARAMETERS_REQUEST, END_INIT_PACKET };
    bootloaderBoard->write_gatt_char(&DFU_CONTROL_POINT_CHAR, MBL_MW_GATT_CHAR_WRITE_WITH_RESPONSE, (uint8_t *)&initPacketEnd, sizeof(initPacketEnd));
}
