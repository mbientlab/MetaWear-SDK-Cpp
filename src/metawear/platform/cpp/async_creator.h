#pragma once

#include "metawear/platform/cpp/concurrent_queue.h"
#include "metawear/platform/cpp/task.h"

#include <functional>

struct AsyncCreator {
    virtual ~AsyncCreator();

    ConcurrentQueue<std::function<void (void)>> pending_fns;
    Task* timeout;

    void create_next(bool force);    
};
