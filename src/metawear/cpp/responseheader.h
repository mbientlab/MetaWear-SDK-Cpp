#pragma once

#include <functional>
#include <stdint.h>

#include "register.h"

struct ResponseHeader {
    uint8_t module_id, register_id, data_id;

    ResponseHeader(uint8_t module_id, uint8_t register_id);
    ResponseHeader(uint8_t module_id, uint8_t register_id, uint8_t data_id);
    ResponseHeader(const ResponseHeader &original);

    bool operator ==(const ResponseHeader& other) const; 
};

const ResponseHeader SWITCH_RESPONSE_HEADER(ORDINAL(Module::SWITCH), ORDINAL(SwitchRegister::STATE)), 
        BMI160_ACCEL_RESPONSE_HEADER(ORDINAL(Module::ACCELEROMETER), ORDINAL(AccelerometerBmi160Register::DATA_INTERRUPT)),
        MMA8452Q_ACCEL_RESPONSE_HEADER(ORDINAL(Module::ACCELEROMETER), ORDINAL(AccelerometerMma8452qRegister::DATA_VALUE)),
        LTR329_ILLUMINANCE_RESPONSE_HEADER(ORDINAL(Module::AMBIENT_LIGHT), ORDINAL(AmbientLightLtr329Register::OUTPUT)),
        BARO_PRESSURE_RESPONSE_HEADER(ORDINAL(Module::BAROMETER), ORDINAL(BarometerBmp280Register::PRESSURE)),
        BARO_ALTITUDE_RESPONSE_HEADER(ORDINAL(Module::BAROMETER), ORDINAL(BarometerBmp280Register::ALTITUDE)),
        GYRO_ROT_RESPONSE_HEADER(ORDINAL(Module::GYRO), ORDINAL(GyroBmi160Register::DATA));

namespace std {

template <>
struct hash<ResponseHeader> {
    size_t operator()(const ResponseHeader& key) const;
};

}
