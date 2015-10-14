#include "connection_def.h"

#include <cstring>

const MblMwConnection *metawear_connection;

void mbl_mw_connection_init(const MblMwConnection* conn) {
    metawear_connection= conn;
}
