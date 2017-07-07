#pragma once

#include <memory>

#include "metawear/dfu/metabootboard_fwd.h"
#include "dfu_operations.h"

struct MblMwMetaBootBoard {
    MblMwMetaBootBoard(const MblMwBtleConnection *connection, const MblMwDfuDelegate *delegate);
    ~MblMwMetaBootBoard();
    
    MblMwBtleConnection btleConn;
    std::unique_ptr<DfuOperations> operations;

    inline void write_gatt_char(const MblMwGattChar* gattChar, const uint8_t* value, uint8_t len) const {
        btleConn.write_gatt_char(this, MBL_MW_GATT_CHAR_WRITE_WITHOUT_RESPONSE, gattChar, value, len);
    }
};

