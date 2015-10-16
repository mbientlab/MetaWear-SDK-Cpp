#include "connection_def.h"

#include <cstring>

MblMwConnection metawear_connection;

void mbl_mw_connection_init(const MblMwConnection* conn) {
    std::memcpy(&metawear_connection, conn, sizeof(metawear_connection));
}
