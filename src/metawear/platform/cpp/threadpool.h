#pragma once

#include "task.h"
#include <functional>

class ThreadPool {
public:
    static Task* schedule(std::function<void(void)> fn, int64_t delay);
};
