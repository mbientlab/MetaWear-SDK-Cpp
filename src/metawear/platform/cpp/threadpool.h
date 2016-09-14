#pragma once

#include "task.h"

#include <functional>
#include <stdint.h>

class ThreadPool {
public:
    static Task* schedule(std::function<void(void)> fn, int64_t delay);
};
