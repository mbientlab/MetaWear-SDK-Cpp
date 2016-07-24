#pragma once

#include "metawear/core/cpp/datasignal_private.h"

struct MblMwGpioPinNotifySignal : public MblMwDataSignal {
    MblMwGpioPinNotifySignal(ResponseHeader header, MblMwMetaWearBoard* owner);
    MblMwGpioPinNotifySignal(uint8_t** state_stream, MblMwMetaWearBoard *owner);

    virtual void unsubscribe();
};

struct MblMwGpioAnalogSignal : public MblMwDataSignal {
    MblMwGpioAnalogSignal(ResponseHeader header, MblMwMetaWearBoard* owner);
    MblMwGpioAnalogSignal(uint8_t** state_stream, MblMwMetaWearBoard *owner);

    virtual void read() const;
    virtual void read(const void* parameters) const;
};

void init_gpio_module(MblMwMetaWearBoard *board);
