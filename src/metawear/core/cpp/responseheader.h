#pragma once

#include <functional>
#include <stdint.h>

#include "metawear/core/module.h"
#include "register.h"

struct ResponseHeader {
    uint8_t module_id, register_id, data_id;

    ResponseHeader();
    ResponseHeader(uint8_t module_id, uint8_t register_id);
    ResponseHeader(uint8_t module_id, uint8_t register_id, uint8_t data_id);
    ResponseHeader(const ResponseHeader &original);

    bool operator ==(const ResponseHeader& other) const; 
};

namespace std {

template <>
struct hash<ResponseHeader> {
    size_t operator()(const ResponseHeader& key) const;
};

}