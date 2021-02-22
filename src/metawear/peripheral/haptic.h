/**
 * @copyright MbientLab License 
 * @file haptic.h
 * @brief Communicates with the haptic driver
 */
#pragma once

#include "peripheral_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Drives a motor - Pulls down the HCD pin
 * The MetaWear has a driver for motor or buzzers that are 3C compatible (check the datasheet)
 * This is not a full PWM driver.
 * @param board             Pointer to the board to send the command to
 * @param duty_cycle_per    Strength of the motor, between [0, 100] percent
 * @param pulse_width_ms    How long to run the motor, in milliseconds
 */
METAWEAR_API void mbl_mw_haptic_start_motor(const MblMwMetaWearBoard *board, float duty_cycle_per, uint16_t pulse_width_ms);
/**
 * Drives a buzzer - Pulls down the HCD pin
 * The MetaWear has a driver for motor or buzzers that are 3C compatible (check the datasheet)
 * @param board             Pointer to the board to send the command to
 * @param pulse_width_ms    How long to run the buzzer, in milliseconds
 */
METAWEAR_API void mbl_mw_haptic_start_buzzer(const MblMwMetaWearBoard *board, uint16_t pulse_width_ms);

#ifdef	__cplusplus
}
#endif
