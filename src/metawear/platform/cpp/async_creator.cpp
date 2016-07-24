#include "async_creator.h"

AsyncCreator::~AsyncCreator() {
}

void AsyncCreator::create_next(bool force) {
    if (force) {
        pending_fns.pop();
    }
    if ((force && !pending_fns.empty()) || (!force && pending_fns.size() == 1)) {
        pending_fns.front()();
    }
}
