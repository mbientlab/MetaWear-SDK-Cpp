#pragma once

#include "metawear/core/anonymous_datasignal_fwd.h"
#include "metawear/core/data.h"

struct MblMwAnonymousDataSignal {
    virtual ~MblMwAnonymousDataSignal() = 0;

    virtual void subscribe(MblMwFnData data_handler) = 0;
    virtual char* get_identifier() const = 0;
};