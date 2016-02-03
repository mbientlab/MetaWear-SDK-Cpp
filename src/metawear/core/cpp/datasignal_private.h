#pragma once

#include "metawear/core/datasignal_fwd.h"
#include "datainterpreter.h"
#include "event_private.h"

typedef float (*FnDataSignalFloat)(MblMwDataSignal* source, float value);
typedef void (*FnDataSignal)(MblMwDataSignal* signal);

struct MblMwDataSignal : public MblMwEvent {
    MblMwDataSignal(const MblMwEvent& event);
    MblMwDataSignal(const ResponseHeader& header, MblMwMetaWearBoard *owner, DataInterpreter interpreter, uint8_t n_channels, 
            uint8_t channel_size, uint8_t is_signed, uint8_t offset);
    MblMwDataSignal(const ResponseHeader& header, MblMwMetaWearBoard *owner, DataInterpreter interpreter, uint8_t n_channels,
            uint8_t channel_size, uint8_t is_signed, uint8_t offset, FnDataSignalFloat number_to_firmware, 
            FnDataSignal subscribe, FnDataSignal unsubscribe);
    virtual ~MblMwDataSignal();

    uint8_t length();
    uint8_t get_data_ubyte();
    void set_channel_attr(uint8_t n_channels, uint8_t channel_size);

    DataInterpreter interpreter;
    uint8_t n_channels;
    uint8_t channel_size;
    uint8_t is_signed;
    uint8_t offset;
    FnDataSignalFloat number_to_firmware;
    FnDataSignal subscribe;
    FnDataSignal unsubscribe;
};

float number_to_firmware_default(MblMwDataSignal* source, float value);
