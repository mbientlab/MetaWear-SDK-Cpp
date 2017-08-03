#pragma once

#include "task.h"

#include <functional>
#include <memory>
#include <stdint.h>

class ThreadPool {
public:
    static std::shared_ptr<Task> schedule(std::function<void(void)> fn, int64_t delay);
};
