#pragma once

#include <stdint.h>
#include <vector>

struct ModuleInfo {
    uint8_t id;
    bool present;

    std::vector<uint8_t> extra;
    uint8_t implementation;
    uint8_t revision;

    ModuleInfo(const uint8_t *response, uint8_t size);
    ModuleInfo(uint8_t** state_stream);

    void serialize(std::vector<uint8_t>& state) const;
};
