#include "metawear/platform/memory.h"

#include <cstdlib>

using std::free;

void mbl_mw_memory_free(void* ptr) {
    free(ptr);
}
