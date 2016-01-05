#include "connection_def.h"
#include "event_private.h"
#include "event_register.h"
#include "metawearboard_def.h"

#include <cstring>
#include <vector>

using std::memcpy;
using std::vector;

void send_command_wrapper(const MblMwMetaWearBoard* board, const uint8_t* command, uint8_t len) {
    if (!board->event_config.empty()) {
        board->event_owner.front()->num_expected_cmds++;
        vector<uint8_t> event_entry= {MBL_MW_MODULE_EVENT, ORDINAL(EventRegister::ENTRY),
                board->event_config.at(0), board->event_config.at(1), board->event_config.at(2), command[0], command[1], 
                (uint8_t) (len - 2)};

        if (board->data_token != nullptr) {
            uint8_t event_data[2]= {
                    (uint8_t) (0x01 | (board->data_token->data_length << 1) | (board->data_token->data_offset << 4)), 
                    board->data_token->dest_offset
            };
            event_entry.insert(event_entry.end(), event_data, event_data + sizeof(event_data));
        }
        board->event_owner.front()->commands.emplace_back(event_entry);

        vector<uint8_t> event_parameters(command + 2, command + len);
        uint8_t prefix[2]= {MBL_MW_MODULE_EVENT, ORDINAL(EventRegister::CMD_PARAMETERS)};
        event_parameters.insert(event_parameters.begin(), prefix, prefix + sizeof(prefix));
        board->event_owner.front()->commands.emplace_back(event_parameters);
    } else {
        board->send_command(command, len);
    }
}
