/*
 * mock.h
 *
 *  Created on: 12/feb/2019
 *      Author: leona
 */


#ifndef MOCK_H_
#define MOCK_H_

#include <stdint.h>
#include "servo.h"

extern uint32_t servo0_pulse, servo1_pulse;

void Servo_SetPulse(ServoID servo_id, uint32_t pulse);


#endif /* MOCK_H_ */
