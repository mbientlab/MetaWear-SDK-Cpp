#pragma once

#include "metawear/core/cpp/datasignal_private.h"

struct MblMwI2cSignal : public MblMwDataSignal {
    MblMwI2cSignal(ResponseHeader header, MblMwMetaWearBoard* owner, uint8_t length);
    MblMwI2cSignal(uint8_t** state_stream, MblMwMetaWearBoard *owner);

    virtual void read() const;
    virtual void read(const void* parameters) const;
};

struct MblMwSpiSignal : public MblMwDataSignal {
    MblMwSpiSignal(ResponseHeader header, MblMwMetaWearBoard* owner, uint8_t length);
    MblMwSpiSignal(uint8_t** state_stream, MblMwMetaWearBoard *owner);

    virtual void read() const;
    virtual void read(const void* parameters) const;
};

void init_serialpassthrough_module(MblMwMetaWearBoard *board);
