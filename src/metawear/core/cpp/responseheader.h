#pragma once

#include <functional>
#include <stdint.h>
#include <vector>

struct ResponseHeader {
    uint8_t module_id, register_id, data_id;

    ResponseHeader(uint8_t** state_stream);
    ResponseHeader(uint8_t module_id, uint8_t register_id);
    ResponseHeader(uint8_t module_id, uint8_t register_id, uint8_t data_id);
    ResponseHeader(const ResponseHeader &original);

    void disable_silent();
    void enable_silent();

    bool is_readable() const;
    bool has_data_id() const;
    void serialize(std::vector<uint8_t>& state) const;

    bool operator <(const ResponseHeader& other) const;
    bool operator ==(const ResponseHeader& other) const;
};

namespace std {

template <>
struct hash<ResponseHeader> {
    size_t operator()(const ResponseHeader& key) const;
};

}
