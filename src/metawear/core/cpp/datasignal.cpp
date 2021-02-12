#include "metawear/core/datasignal.h"

#include "datainterpreter.h"
#include "datasignal_private.h"
#include "metawearboard_def.h"

#include "settings_private.h"
#include "metawear/sensor/cpp/accelerometer_private.h"
#include "metawear/sensor/cpp/ambientlight_ltr329_private.h"
#include "metawear/sensor/cpp/barometer_bosch_private.h"
#include "metawear/sensor/cpp/colordetector_tcs34725_private.h"
#include "metawear/sensor/cpp/gpio_private.h"
#include "metawear/sensor/cpp/gyro_bosch_private.h"
#include "metawear/sensor/cpp/humidity_bme280_private.h"
#include "metawear/sensor/cpp/magnetometer_bmm150_private.h"
#include "metawear/sensor/cpp/multichanneltemperature_private.h"
#include "metawear/sensor/cpp/proximity_tsl2671_private.h"
#include "metawear/sensor/cpp/sensor_fusion_private.h"
#include "metawear/sensor/cpp/serialpassthrough_private.h"
#include "metawear/sensor/cpp/switch_private.h"

#include <stdexcept>

using std::out_of_range;
using std::stringstream;
using std::vector;

uint8_t MblMwDataSignal::count_subscribers(const MblMwDataSignal* signal) {
    auto root= dynamic_cast<MblMwDataSignal*>(signal->owner->module_events.at(signal->header));
    uint8_t count= root->handler == nullptr ? 0 : 1;

    for(auto it: root->components) {
        if (it->handler != nullptr) {
            count++;
        } 
    }
    return count;
}

MblMwDataSignal::MblMwDataSignal(uint8_t** state_stream, MblMwMetaWearBoard *owner) : MblMwEvent(state_stream, owner), handler(nullptr) {
    interpreter = static_cast<DataInterpreter>(**state_stream);
    converter = static_cast<FirmwareConverter>(*(++(*state_stream)));
    n_channels = *(++(*state_stream));
    channel_size = *(++(*state_stream));
    is_signed = *(++(*state_stream));
    offset = *(++(*state_stream));

    (*state_stream)++;
}

MblMwDataSignal::MblMwDataSignal(const MblMwEvent& event) : MblMwDataSignal(event.header, event.owner, DataInterpreter::UINT32, 0, 0, 0, 0) {
}

MblMwDataSignal::MblMwDataSignal(const ResponseHeader& header, MblMwMetaWearBoard *owner, DataInterpreter interpreter, FirmwareConverter converter, 
        uint8_t n_channels, uint8_t channel_size, uint8_t is_signed, uint8_t offset) : 
        MblMwEvent(header, owner), handler(nullptr), interpreter(interpreter), converter(converter), n_channels(n_channels), 
        channel_size(channel_size), is_signed(is_signed), offset(offset) {
    // Initialize readable signals to silent reads
    if (this->header.is_readable()) {
        this->header.enable_silent();
    }
}

MblMwDataSignal::MblMwDataSignal(const ResponseHeader& header, MblMwMetaWearBoard *owner, DataInterpreter interpreter, 
        uint8_t n_channels, uint8_t channel_size, uint8_t is_signed, uint8_t offset) : 
        MblMwDataSignal(header, owner, interpreter, FirmwareConverter::DEFAULT, n_channels, channel_size, is_signed, offset) {
}

MblMwDataSignal::~MblMwDataSignal() {
    for(auto it: components) {
        delete it;
    }
}

void MblMwDataSignal::subscribe() {
    if (this->header.is_readable()) {
        this->header.disable_silent();
    } else {
        if (count_subscribers(this) == 1) {
            uint8_t command[3] = { header.module_id, header.register_id, 1 };
            SEND_COMMAND_BOARD(owner);
        }
    }
}

void MblMwDataSignal::unsubscribe() {
    if (this->header.is_readable()) {
        this->header.enable_silent();
    } else {
        if (!count_subscribers(this)) {
            uint8_t command[3] = { header.module_id, header.register_id, 0 };
            SEND_COMMAND_BOARD(owner);
        }
    }
}

void MblMwDataSignal::read() const {
    if (header.has_data_id()) {
        uint8_t command[2]= { header.module_id, header.register_id };
        SEND_COMMAND_BOARD(owner);
    } else {
        uint8_t command[3]= { header.module_id, header.register_id, header.data_id };
        SEND_COMMAND_BOARD(owner);
    }
}

void MblMwDataSignal::read(const void* parameters) const {
    read();
}

void MblMwDataSignal::serialize(vector<uint8_t>& state) const {
    MblMwEvent::serialize(state);

    state.push_back(static_cast<uint8_t>(interpreter));
    state.push_back(static_cast<uint8_t>(converter));
    state.push_back(n_channels);
    state.push_back(channel_size);
    state.push_back(is_signed);
    state.push_back(offset);
}

void MblMwDataSignal::create_uri(stringstream& uri) const {
    switch(header.module_id) {
    case MBL_MW_MODULE_SWITCH:
        return create_switch_uri(this, uri);
    case MBL_MW_MODULE_ACCELEROMETER:
        return create_acc_uri(this, uri);
    case MBL_MW_MODULE_TEMPERATURE:
        return create_temp_uri(this, uri);
    case MBL_MW_MODULE_GPIO:
        return create_gpio_uri(this, uri);
    case MBL_MW_MODULE_DATA_PROCESSOR:
        return create_dataprocessor_state_uri(this, uri);
    case MBL_MW_MODULE_I2C:
        return create_serialpassthrough_uri(this, uri);
    case MBL_MW_MODULE_SETTINGS:
        return create_settings_uri(this, uri);
    case MBL_MW_MODULE_BAROMETER:
        return create_barometer_uri(this, uri);
    case MBL_MW_MODULE_GYRO:
        return create_gyro_uri(this, uri);
    case MBL_MW_MODULE_AMBIENT_LIGHT:
        return create_als_uri(this, uri);
    case MBL_MW_MODULE_MAGNETOMETER:
        return create_magnetometer_uri(this, uri);
    case MBL_MW_MODULE_HUMIDITY:
        return create_humidity_uri(this, uri);
    case MBL_MW_MODULE_COLOR_DETECTOR:
        return create_colordetector_uri(this, uri);
    case MBL_MW_MODULE_PROXIMITY:
        return create_proximity_uri(this, uri);
    case MBL_MW_MODULE_SENSOR_FUSION:
        return create_sensor_fusion_uri(this, uri);
    }
}

uint8_t MblMwDataSignal::length() const {
    return n_channels * channel_size;
}

uint8_t MblMwDataSignal::get_data_ubyte() const {
    return ((length() - 1) << 5) | offset;
}

void MblMwDataSignal::set_channel_attr(uint8_t n_channels, uint8_t channel_size) {
    this->n_channels= n_channels;
    this->channel_size= channel_size;
}

void MblMwDataSignal::make_signed() {
    is_signed= 1;

    switch (interpreter) {
    case DataInterpreter::UINT32:
        interpreter = DataInterpreter::INT32;
        break;
    case DataInterpreter::BMI160_ROTATION_UNSIGNED_SINGLE_AXIS:
        interpreter = DataInterpreter::BMI160_ROTATION_SINGLE_AXIS;
        break;
    case DataInterpreter::BOSCH_ACCELERATION_UNSIGNED_SINGLE_AXIS:
        interpreter = DataInterpreter::BOSCH_ACCELERATION_SINGLE_AXIS;
        break;
    case DataInterpreter::MMA8452Q_ACCELERATION_UNSIGNED_SINGLE_AXIS:
        interpreter = DataInterpreter::MMA8452Q_ACCELERATION_SINGLE_AXIS;
        break;
    case DataInterpreter::BMM150_B_FIELD_UNSIGNED_SINGLE_AXIS:
        interpreter = DataInterpreter::BMM150_B_FIELD_SINGLE_AXIS;
        break;
    case DataInterpreter::BOSCH_PRESSURE:
        interpreter = DataInterpreter::BOSCH_ALTITUDE;
        break;
    default:
        break;
    }
}

void MblMwDataSignal::make_unsigned() {
    is_signed= 0;

    switch (interpreter) {
    case DataInterpreter::INT32:
        interpreter = DataInterpreter::UINT32;
        break;
    case DataInterpreter::BMI160_ROTATION_SINGLE_AXIS:
        interpreter = DataInterpreter::BMI160_ROTATION_UNSIGNED_SINGLE_AXIS;
        break;
    case DataInterpreter::BOSCH_ACCELERATION_SINGLE_AXIS:
        interpreter = DataInterpreter::BOSCH_ACCELERATION_UNSIGNED_SINGLE_AXIS;
        break;
    case DataInterpreter::MMA8452Q_ACCELERATION_SINGLE_AXIS:
        interpreter = DataInterpreter::MMA8452Q_ACCELERATION_UNSIGNED_SINGLE_AXIS;
        break;
    case DataInterpreter::BMM150_B_FIELD_SINGLE_AXIS:
        interpreter = DataInterpreter::BMM150_B_FIELD_UNSIGNED_SINGLE_AXIS;
        break;
    case DataInterpreter::BOSCH_ALTITUDE:
        interpreter = DataInterpreter::BOSCH_PRESSURE;
        break;
    default:
        break;
    }
}

MblMwDataSignal* mbl_mw_datasignal_get_component(const MblMwDataSignal* signal, uint8_t index) {
    try {
        return signal->components.at(index);
    } catch (const out_of_range&) {
        return nullptr;
    }
}

void mbl_mw_datasignal_subscribe(MblMwDataSignal *signal, void *context, MblMwFnData received_data) {
    signal->context= context;
    signal->handler= received_data;
    signal->subscribe();
}

void mbl_mw_datasignal_unsubscribe(MblMwDataSignal *signal) {
    signal->context= nullptr;
    signal->handler= nullptr;
    signal->unsubscribe();
}

void mbl_mw_datasignal_read(const MblMwDataSignal* signal) {
    signal->read();
}

void mbl_mw_datasignal_read_with_parameters(const MblMwDataSignal* signal, const void* parameters) {
    signal->read(parameters);
}

int32_t mbl_mw_datasignal_is_readable(const MblMwDataSignal* signal) {
    return signal->header.is_readable();
}
