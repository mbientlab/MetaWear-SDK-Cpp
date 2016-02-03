#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/peripheral/haptic.h"

#include <cstring>

static const uint8_t HAPTIC_MODULE= 0x8;
static const uint8_t HAPTIC_PULSE= 0x1;
static const uint8_t BUZZER_DUTY_CYCLE= 127;

void mbl_mw_haptic_start_motor(const MblMwMetaWearBoard *board, float duty_cycle_per, uint16_t pulse_width_ms) {
    uint8_t converted= uint8_t ((duty_cycle_per / 100.f) * 248);
    uint8_t command[6]= {HAPTIC_MODULE, HAPTIC_PULSE, converted, 0, 0, 0};

    std::memcpy(command + 3, &pulse_width_ms, 2);
    SEND_COMMAND;
}

void mbl_mw_haptic_start_buzzer(const MblMwMetaWearBoard *board, uint16_t pulse_width_ms) {
    uint8_t command[6]= {HAPTIC_MODULE, HAPTIC_PULSE, BUZZER_DUTY_CYCLE, 0, 0, 1};

    std::memcpy(command + 3, &pulse_width_ms, 2);
    SEND_COMMAND;
}
