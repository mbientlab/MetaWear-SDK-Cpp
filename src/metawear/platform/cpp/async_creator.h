#pragma once

#include "metawear/platform/cpp/concurrent_queue.h"
#include "metawear/platform/cpp/task.h"

#include <functional>
#include <memory>

struct AsyncCreator {
    virtual ~AsyncCreator();

    ConcurrentQueue<std::function<void (void)>> pending_fns;
    std::shared_ptr<Task> timeout;

    void create_next(bool force);    
};
