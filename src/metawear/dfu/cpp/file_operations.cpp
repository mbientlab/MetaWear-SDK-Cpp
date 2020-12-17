#include <cmath>
#include <fstream>

#include "file_operations.h"
#include "dfu_utility.h"

#include "metawear/core/cpp/metawearboard_def.h"

#include "miniz.h"
#include "json.hpp"

FileOperationsDelegate::~FileOperationsDelegate() {

}

FileOperations::FileOperations(FileOperationsDelegate &fileDelegate, const MblMwMetaWearBoard* board) : binFile(0), fileDelegate(fileDelegate), bootloaderBoard(board), metaDataFile(0) {

}

FileOperations::~FileOperations() {
    if (binFile) {
        mz_free(binFile);
    }
    if (metaDataFile) {
        mz_free(metaDataFile);
    }
}

void FileOperations::openFile(const char* filename)
{
    std::ifstream file;
    file.exceptions(std::ifstream::badbit | std::ifstream::failbit | std::ifstream::eofbit);
    //Need to use binary mode; otherwise CRLF line endings count as 2 for
    //`length` calculation but only 1 for `file.read` (on some platforms),
    //and we get undefined  behaviour when trying to read `length` characters.
    file.open(filename, std::ifstream::in | std::ifstream::binary);
    file.seekg(0, std::ios::end);
    std::streampos length(file.tellg());
    if (length) {
        file.seekg(0, std::ios::beg);
        binFileSize = static_cast<std::size_t>(length);
        binFile = static_cast<uint8_t *>(malloc(binFileSize));
        // Load all the data to our local vector
        file.read((char *)binFile, binFileSize);
        
        // Compute how we need to slice up the file into BLE sized chunks
        numberOfPackets = std::ceil((double)binFileSize / (double)MBL_PACKET_SIZE);
        bytesInLastPacket = binFileSize % MBL_PACKET_SIZE;
        if (bytesInLastPacket == 0) {
            bytesInLastPacket = MBL_PACKET_SIZE;
        }
        writingPacketNumber = 0;
        prevPercentage = -1;
        // Let the delegate know we opened it!
        fileDelegate.onFileOpened(binFileSize);
    } else {
        fileDelegate.onError("0 length file");
    }
}

void FileOperations::writeNextPacket()
{
    int percentage = 0;
    for (int index = 0; index < MBL_PACKETS_NOTIFICATION_INTERVAL; index++) {
        if (writingPacketNumber > numberOfPackets - 2) {
            //NSLog(@"writing last packet");
            uint8_t *nextPacketData = &binFile[writingPacketNumber * MBL_PACKET_SIZE];
            //NSLog(@"writing packet number %d ...",self.writingPacketNumber+1);
            //NSLog(@"packet data: %@",nextPacketData);
            bootloaderBoard->write_gatt_char(&DFU_PACKET_CHAR, MBL_MW_GATT_CHAR_WRITE_WITHOUT_RESPONSE, nextPacketData, bytesInLastPacket);
            writingPacketNumber++;
            fileDelegate.onTransferPercentage(100);
            fileDelegate.onAllPacketsTranferred();
            break;
        }
        uint8_t *nextPacketData = &binFile[writingPacketNumber * MBL_PACKET_SIZE];
        //NSLog(@"writing packet number %d ...",self.writingPacketNumber+1);
        //NSLog(@"packet data: %@",nextPacketData);
        bootloaderBoard->write_gatt_char(&DFU_PACKET_CHAR, MBL_MW_GATT_CHAR_WRITE_WITHOUT_RESPONSE, nextPacketData, MBL_PACKET_SIZE);
        percentage = (((double)(writingPacketNumber * 20) / (double)(binFileSize)) * 100);
        if (percentage != prevPercentage) {
            fileDelegate.onTransferPercentage(percentage);
            prevPercentage = percentage;
        }
        writingPacketNumber++;
    }
}

void FileOperations::openZip(const char *filename)
{
    int i;
    mz_bool status;
    size_t uncomp_size;
    mz_zip_archive zip_archive = { 0 };
    void *p;
    std::string metadataFilename, firmwareFilename;
    
    status = mz_zip_reader_init_file(&zip_archive, filename, MZ_ZIP_FLAG_DO_NOT_SORT_CENTRAL_DIRECTORY);
    if (!status) {
        fileDelegate.onError("mz_zip_reader_init_file() failed!");
        return;
    }
    
    for (i = 0; i < (int)mz_zip_reader_get_num_files(&zip_archive); i++) {
        mz_zip_archive_file_stat file_stat;
        if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat)) {
            fileDelegate.onError("mz_zip_reader_file_stat() failed!");
            mz_zip_reader_end(&zip_archive);
            return;
        }
        if (std::string(file_stat.m_filename) == "manifest.json") {
            // Try to extract the manifest to the heap
            p = mz_zip_reader_extract_file_to_heap(&zip_archive, file_stat.m_filename, &uncomp_size, 0);
            if (!p) {
                fileDelegate.onError("mz_zip_reader_extract_file_to_heap() failed!");
                mz_zip_reader_end(&zip_archive);
                return;
            }
            // Pull out the firmware and data filenames
            auto manifest = nlohmann::json::parse(std::string((const char *)p, uncomp_size));
            auto it = manifest.find("manifest");
            if (it != manifest.end()) {
                auto it1 = it->find("application");
                if (it1 != it->end()) {
                    auto it2 = it1->find("bin_file");
                    if (it2 != it1->end()) {
                        firmwareFilename = *it2;
                    }
                    auto it3 = it1->find("dat_file");
                    if (it3 != it1->end()) {
                        metadataFilename = *it3;
                    }
                }
            }
            // We're done.
            mz_free(p);
        }
    }
    
    if (metadataFilename.empty() || firmwareFilename.empty()) {
        fileDelegate.onError("error parsing manifest");
        return;
    }

    // Open the Metadata file
    metaDataFile = static_cast<uint8_t *>(mz_zip_reader_extract_file_to_heap(&zip_archive, metadataFilename.c_str(), &metaDataFileSize, 0));
    if (!metaDataFile) {
        fileDelegate.onError("mz_zip_reader_extract_file_to_heap() failed!");
        mz_zip_reader_end(&zip_archive);
        return;
    }
    
    // Process the firmware file
    binFile = static_cast<uint8_t *>(mz_zip_reader_extract_file_to_heap(&zip_archive, firmwareFilename.c_str(), &binFileSize, 0));
    if (!binFile) {
        fileDelegate.onError("mz_zip_reader_extract_file_to_heap() failed!");
        mz_zip_reader_end(&zip_archive);
        return;
    }
    // Compute how we need to slice up the file into BLE sized chunks
    numberOfPackets = std::ceil((double)binFileSize / (double)MBL_PACKET_SIZE);
    bytesInLastPacket = binFileSize % MBL_PACKET_SIZE;
    if (bytesInLastPacket == 0) {
        bytesInLastPacket = MBL_PACKET_SIZE;
    }
    writingPacketNumber = 0;
    // Let the delegate know we opened it!
    fileDelegate.onFileOpened(binFileSize);
    
    mz_zip_reader_end(&zip_archive);
}
