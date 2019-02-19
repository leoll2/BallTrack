#include "mock.h"

uint32_t servo0_pulse = 0,
	servo1_pulse = 0;

void Servo_SetPulse(ServoID servo_id, uint32_t pulse) {
	switch(servo_id) {
		case SERVO_X:
			servo1_pulse = pulse;
			break;
		case SERVO_Y:
			servo0_pulse = pulse;
			break;
	}
}
