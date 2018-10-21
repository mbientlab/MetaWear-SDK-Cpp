#pragma once

#include <stdint.h>
#include <vector>

#include "metawear/core/metawearboard_fwd.h"

struct FileOperationsDelegate {
    virtual ~FileOperationsDelegate() = 0;
    // define callback interface functions
    virtual void onTransferPercentage(int) = 0;
    virtual void onAllPacketsTranferred() = 0;
    virtual void onFileOpened(size_t) = 0;
    virtual void onError(const std::string &) = 0;
};

class FileOperations {
    uint8_t *binFile;
    int bytesInLastPacket;
    int prevPercentage;
    
    FileOperationsDelegate &fileDelegate;
    const MblMwMetaWearBoard* bootloaderBoard;
public:
    size_t binFileSize;
    int numberOfPackets;
    int writingPacketNumber;
    
    uint8_t *metaDataFile;
    size_t metaDataFileSize;
    
    FileOperations(FileOperationsDelegate &fileDelegate, const MblMwMetaWearBoard* board);
    ~FileOperations();
    
    void openFile(const char* filename);
    void writeNextPacket();
    
    void openZip(const char *filename);
};
