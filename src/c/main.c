/* code.c
 *
 * Ball and plate control system
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "ee.h"
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#include "controller.h"
#include "field.h"
#include "pilot.h"
#include "serial.h"
#include "servo.h"
#include "touch.h"
#include "types/geometry.h"
#include "types/control.h"

char s[128];		// buffer for serial messages


/* Handle SysTick interrupts */
ISR2(systick_handler)
{
	CounterTick(counter);
}


TASK(TaskController)
{
	static Box b_prev = {N_BOX_X, N_BOX_Y}, 	// previous box
			   b_targ = {N_BOX_X, N_BOX_Y}; 	// target box
	static Coordinate ref_pos = {4096, 4096}; 	// target reference point
	Position pos;		 						// (x,y) reading from ADC
	Actuation u; 								// output of the controller
	Box b; 										// box corresponding to current position
	bool reset_state = false; 					// reset control variables (due to target change)

	// Read coordinates from touchscreen
	pos = Touch_Read();
	if (!Touch_ValidateRead(pos, Controller_GetPosPrev1()))
		return;

	// Compute box and coordinates of the current target
	b = Field_GetBoxFromCoord(pos);
	if (!Field_IsEqualBox(b, target_box) && !Field_IsEqualBox(b, b_prev)) {
		b_targ = Pilot_GetNextBox(b, Pilot_GetBestAction(b));
		ref_pos = Field_GetMiddleCoordOfBox(b_targ);
		reset_state = true;
	}
	b_prev = b;

	// Compute controller output
	u = Controller_GetOutput(pos, ref_pos, reset_state);

	// Actuate the servomotors
	Servo_SetPosition(SERVO_X, u.x);
	Servo_SetPosition(SERVO_Y, u.y);

	// Print debug information
	console_out("###############################\r\n");
	sprintf(s, "Pos: (%i, %i)\r\n", pos.x, pos.y);
	console_out(s);
	sprintf(s, "Box: (%i,%i) TargetBox: (%i,%i)\r\n", b.i, b.j, b_targ.i, b_targ.j);
	console_out(s);
	sprintf(s, "u: (%i,%i)\r\n", u.x, u.y);
	console_out(s);
}


int main(void)
{
	// Setup the microcontroller system
	SystemInit();

	// Initialize Erika system
	EE_system_init();

	// Initialize systick
	EE_systick_set_period(MILLISECONDS_TO_TICKS(1, SystemCoreClock));
	EE_systick_enable_int();
	EE_systick_start();

	// Initialize USART
	USART_Config();

	// Initialize Touchscreen (including ADC)
	Touch_Config();

	// Setup PWM
	uint32_t prescalar = (uint16_t)(((SystemCoreClock / 2) / 1000000) - 1);
	Servo_Config(20000, prescalar);

	// Set servos at rest
	Servo_SetPosition(SERVO_X, 0);
	Servo_SetPosition(SERVO_Y, 0);

	// Program periodical alarms
	SetRelAlarm(AlarmController, 10, 50);

	console_out("Initialization complete\r\n");

	// Endless loop
	for (;;);
}

