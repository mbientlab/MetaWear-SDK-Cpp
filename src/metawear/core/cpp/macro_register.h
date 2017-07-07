#pragma once

enum class MacroRegister : uint8_t {
    BEGIN = 2,
    ADD_COMMAND,
    END,
    EXECUTE,
    ERASE_ALL = 8,
    ADD_PARTIAL
};
