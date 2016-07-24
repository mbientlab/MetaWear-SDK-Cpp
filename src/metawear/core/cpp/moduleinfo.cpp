#include "moduleinfo.h"
#include "register.h"

ModuleInfo::ModuleInfo(const uint8_t *response, uint8_t size) : id(response[0]), present(size > 2) {
    if (present) {
        implementation = response[2];
        revision = response[3];
    } else {
        implementation= 0xff;
        revision= 0xff;
    }

    if (size > 4) {
        extra.assign(response + 4, response + size);
    }
}

ModuleInfo::ModuleInfo(uint8_t** state_stream) {
    id = **state_stream;
    implementation = *(++(*state_stream));
    revision = *(++(*state_stream));
    present = implementation != 0xff && revision != 0xff;

    if (present) {
        uint8_t extra_size = *(++(*state_stream));
        ++(*state_stream);
        if (extra_size) {
            extra.assign(*state_stream, *state_stream + extra_size);
            *state_stream += extra_size;
        }
    } else {
        ++(*state_stream);
    }
    
}

void ModuleInfo::serialize(std::vector<uint8_t>& state) const {
    state.push_back(id);
    state.push_back(implementation);
    state.push_back(revision);

    if (present) {
        state.push_back((uint8_t)extra.size());
        state.insert(state.end(), extra.begin(), extra.end());
    }
}
