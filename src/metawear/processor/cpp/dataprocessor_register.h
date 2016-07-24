#pragma once

enum class DataProcessorRegister : uint8_t {
    ADD= 2,
    NOTIFY,
    STATE,
    PARAMETER,
    REMOVE,
    NOTIFY_ENABLE,
    REMOVE_ALL
};
