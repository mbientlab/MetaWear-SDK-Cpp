#include "metawear/core/connection.h"

#define SEND_COMMAND send_command_wrapper(board, command, sizeof(command))
#define SEND_COMMAND_BOARD(board) send_command_wrapper(board, command, sizeof(command))

void send_command_wrapper(const MblMwMetaWearBoard* board, const uint8_t* command, uint8_t len);