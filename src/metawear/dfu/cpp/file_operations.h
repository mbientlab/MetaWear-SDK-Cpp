#pragma once

#include <stdint.h>
#include <vector>

#include "metawear/dfu/metabootboard_fwd.h"

struct FileOperationsDelegate {
    // define callback interface functions
    virtual void onTransferPercentage(int) = 0;
    virtual void onAllPacketsTranferred() = 0;
    virtual void onFileOpened(size_t) = 0;
    virtual void onError(const std::string &) = 0;
};

class FileOperations {
    uint8_t *binFile;
    int bytesInLastPacket;
    
    FileOperationsDelegate &fileDelegate;
    const MblMwMetaBootBoard* bootloaderBoard;
public:
    size_t binFileSize;
    int numberOfPackets;
    int writingPacketNumber;
    
    uint8_t *metaDataFile;
    size_t metaDataFileSize;
    
    FileOperations(FileOperationsDelegate &fileDelegate, const MblMwMetaBootBoard* board);
    ~FileOperations();
    
    void openFile(const char* filename);
    void writeNextPacket();
    
    void openZip(const char *filename);
};
