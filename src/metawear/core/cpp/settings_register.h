#pragma once

enum class SettingsRegister : uint8_t {
    DEVICE_NAME = 1,
    AD_INTERVAL,
    TX_POWER,
    START_ADVERTISING = 5,
    SCAN_RESPONSE = 7,
    PARTIAL_SCAN_RESPONSE,
    CONNECTION_PARAMS,
    DISCONNECT_EVENT,
    BATTERY_STATE= 0xc
};
