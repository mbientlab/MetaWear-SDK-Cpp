#pragma once

#define GET_EVENT(header) return board->module_events.count(header) ? board->module_events.at(header) : nullptr
#define GET_DATA_SIGNAL(header) GET_DATA_SIGNAL_BOARD(board, header)
#define GET_DATA_SIGNAL_BOARD(board, header) return board->module_events.count(header) ? dynamic_cast<MblMwDataSignal*>(board->module_events.at(header)) : nullptr

#define SERIALIZE_MODULE_CONFIG(config_type, module) auto config = (config_type*)board->module_config.at(module);\
state.insert(state.end(), (uint8_t*)config, ((uint8_t*)config) + sizeof(config_type))

#define DESERIALIZE_MODULE_CONFIG(config_type, module) if (board->module_config.count(module)) {\
    free(board->module_config.at(module));\
}\
void* config = malloc(sizeof(config_type));\
memcpy(config, *state_stream, sizeof(config_type));\
board->module_config[module] = config;\
*state_stream += sizeof(config_type)
