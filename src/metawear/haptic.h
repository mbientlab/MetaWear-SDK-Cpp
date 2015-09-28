/**
 * @copyright MbientLab License 
 * @file haptic.h
 * @brief Communicates with the haptic driver
 */

#pragma once

#include <stdint.h>
#include "dllmarker.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Drives a motor
 * @param   command         Byte array (len 6) for the function to write the command to
 * @param   duty_cycle_per  Strength of the motor, between [0, 100] percent
 * @param   pulse_width_ms  How long to run the motor, in milliseconds
 */
METAWEAR_API void mbl_mw_haptic_start_motor(uint8_t command[6], float duty_cycle_per, uint16_t pulse_width_ms);
/**
 * Drives a buzzer
 * @param   command         Byte array (len 6) for the function to write the command to
 * @param   pulse_width_ms  How long to run the buzzer, in milliseconds
 */
METAWEAR_API void mbl_mw_haptic_start_buzzer(uint8_t command[6], uint16_t pulse_width_ms);

#ifdef	__cplusplus
}
#endif
