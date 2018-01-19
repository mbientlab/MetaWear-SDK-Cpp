#pragma once

#include "metawear/core/anonymous_datasignal_fwd.h"
#include "metawear/core/data.h"

struct MblMwAnonymousDataSignal {
    virtual ~MblMwAnonymousDataSignal() = 0;

    virtual void subscribe(void *context, MblMwFnData data_handler) = 0;
    virtual const char* get_identifier() const = 0;
};