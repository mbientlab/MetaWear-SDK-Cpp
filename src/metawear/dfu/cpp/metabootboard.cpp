#include <cstring>

#include "metawear/dfu/metabootboard.h"

#include "metawear/dfu/cpp/metabootboard_def.h"

MblMwMetaBootBoard::MblMwMetaBootBoard(const MblMwBtleConnection *connection, const MblMwDfuDelegate *delegate) {
    memcpy(&btleConn, connection, sizeof(MblMwBtleConnection));
    operations.reset(new DfuOperations(this, delegate));
}

MblMwMetaBootBoard::~MblMwMetaBootBoard() {
}

MblMwMetaBootBoard* mbl_mw_metabootboard_create(const MblMwBtleConnection *connection, const MblMwDfuDelegate *delegate) {
    return new MblMwMetaBootBoard(connection, delegate);
}

void mbl_mw_metabootboard_free(MblMwMetaBootBoard *board) {
    delete board;
}

static bool has_suffix(const std::string &str, const std::string &suffix)
{
    return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

void mbl_mw_metabootboard_perform_dfu(MblMwMetaBootBoard *board, const char *filename) {
    if (has_suffix(filename, "zip")) {
        board->operations->perfromDFUOnZipFile(filename);
    } else {
        board->operations->performOldDFUOnFile(filename);
    }
}

void mbl_mw_dfu_control_point_char_changed(MblMwMetaBootBoard *board, const uint8_t *value, uint8_t len) {
    board->operations->processDFUResponse(value, len);
}
