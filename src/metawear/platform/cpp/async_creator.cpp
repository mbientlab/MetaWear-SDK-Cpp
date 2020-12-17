#include "async_creator.h"

AsyncCreator::~AsyncCreator() {
    if (timeout.use_count()) {
        timeout->cancel();
    }
}

void AsyncCreator::create_next(bool force) {
    if (force && !pending_fns.empty()) {
        pending_fns.pop();
    }
    if ((force && !pending_fns.empty()) || (!force && pending_fns.size() == 1)) {
        pending_fns.front()();
    }
}
