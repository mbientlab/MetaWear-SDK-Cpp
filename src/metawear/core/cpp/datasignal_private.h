#pragma once

#include <vector>

#include "metawear/core/datasignal_fwd.h"
#include "metawear/core/data.h"
#include "datainterpreter.h"
#include "event_private.h"

struct MblMwDataSignal : public MblMwEvent {
    MblMwDataSignal(uint8_t** state_stream, MblMwMetaWearBoard *owner);
    MblMwDataSignal(const MblMwEvent& event);
    MblMwDataSignal(const ResponseHeader& header, MblMwMetaWearBoard *owner, DataInterpreter interpreter, uint8_t n_channels, 
            uint8_t channel_size, uint8_t is_signed, uint8_t offset);
    MblMwDataSignal(const ResponseHeader& header, MblMwMetaWearBoard *owner, DataInterpreter interpreter, FirmwareConverter converter, 
            uint8_t n_channels, uint8_t channel_size, uint8_t is_signed, uint8_t offset);
    virtual ~MblMwDataSignal();

    virtual void read() const;
    virtual void read(const void* parameters) const;
    virtual void subscribe();
    virtual void unsubscribe();
    virtual void serialize(std::vector<uint8_t>& state) const;

    uint8_t length() const;
    uint8_t get_data_ubyte() const;

    void set_channel_attr(uint8_t n_channels, uint8_t channel_size);
    void make_signed();
    void make_unsigned();

    MblMwFnData handler;
    DataInterpreter interpreter;
    FirmwareConverter converter;
    uint8_t n_channels;
    uint8_t channel_size;
    uint8_t is_signed;
    uint8_t offset;
};
