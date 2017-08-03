#include "threadpool.h"

#include <atomic>
#include <chrono>
#include <thread>

using std::atomic_bool;
using std::chrono::milliseconds;
using std::function;
using std::make_shared;
using std::shared_ptr;
using std::thread;

const int64_t INDEFINITE_TIMEOUT= 0;

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

shared_ptr<Task> ThreadPool::schedule(function<void(void)> fn, int64_t delay) {
    shared_ptr<TaskImpl> task(new TaskImpl());
    if (delay != INDEFINITE_TIMEOUT) {
        thread th([=]() -> void {
            std::this_thread::sleep_for(milliseconds(delay));
            if (!task->cancelled) {
                fn();
            }
        });
        th.detach();
    }

    return task;
}
