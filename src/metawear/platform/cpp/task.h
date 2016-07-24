#pragma once

class Task {
public:
    virtual ~Task();
    virtual void cancel()= 0;
};
