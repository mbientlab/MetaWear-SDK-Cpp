#include "metawear/haptic.h"

#include <cstring>

static const uint8_t HAPTIC_MODULE= 0x8;
static const uint8_t HAPTIC_PULSE= 0x1;

static const uint8_t BUZZER_DUTY_CYCLE= 127;

void mbl_mw_haptic_start_motor(uint8_t command[6], float duty_cycle_per, uint16_t pulse_width_ms) {
    uint8_t converted= uint8_t ((duty_cycle_per / 100.f) * 248);
    
    command[0]= HAPTIC_MODULE;
    command[1]= HAPTIC_PULSE;
    command[2]= converted;
    command[5]= 0;

    std::memcpy(command + 3, &pulse_width_ms, 2);
}

void mbl_mw_haptic_start_buzzer(uint8_t command[6], uint16_t pulse_width_ms) {
    command[0]= HAPTIC_MODULE;
    command[1]= HAPTIC_PULSE;
    command[2]= BUZZER_DUTY_CYCLE;
    command[5]= 1;

    std::memcpy(command + 3, &pulse_width_ms, 2);
}
