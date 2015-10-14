#include "metawear/connection.h"

#define SEND_COMMAND metawear_connection->send_command(board, command, sizeof(command))
#define SEND_COMMAND_BOARD(board) metawear_connection->send_command(board, command, sizeof(command))

extern const MblMwConnection *metawear_connection;
