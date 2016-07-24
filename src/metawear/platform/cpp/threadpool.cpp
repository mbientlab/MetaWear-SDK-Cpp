#include "threadpool.h"

#include <atomic>
#include <chrono>
#include <thread>

using std::atomic_bool;
using std::chrono::milliseconds;
using std::function;
using std::thread;

class TaskImpl : public Task {
public:
    TaskImpl();
    virtual ~TaskImpl();

    virtual void cancel();

    atomic_bool cancelled;
};

TaskImpl::TaskImpl() : cancelled(false) { }

TaskImpl::~TaskImpl() { }

void TaskImpl::cancel() {
    cancelled= true;
}

Task* ThreadPool::schedule(function<void(void)> fn, int64_t delay) {
    TaskImpl* new_task= new TaskImpl();

    thread th([=](void) -> void {
        std::this_thread::sleep_for(milliseconds(delay));
        if (!new_task->cancelled) {
            fn();
        }
        delete new_task;
    });
    th.detach();
    return new_task;
}
