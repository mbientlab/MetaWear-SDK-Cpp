#include "moduleinfo.h"

ModuleInfo::ModuleInfo(const uint8_t *response, uint8_t size) : id(response[0]), present(size > 2) {
    if (present) {
        implementation = response[2];
        revision = response[3];
    } else {
        implementation= -1;
        revision= -1;
    }

    if (size > 4) {
        extra.assign(response + 4, response + size);
    }
}
