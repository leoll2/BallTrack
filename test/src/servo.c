/* ACE RC S1903 servomotors
 *
 * Few details available at:
 * 		https://servodatabase.com/servo/ace-rc/s1903
 */

#include <stdint.h>
#include "mock.h"
#include "servo.h"


/* PWM pulse bounds are constrained by physical limits and geometry
 * of the platform. The following values are obtained empirically. */

// Servomotor #0 (long side of 'ball and plate') -> limits 950, 1600
const uint16_t servo0_rest_pulse_ms = 1310; // pulse to stay in rest position
const uint16_t servo0_max_offset_ms = 290; 	// pulse range (from rest position)

// Servomotor 1 (short side of 'ball and plate') -> limits 1200, 1850
const uint16_t servo1_rest_pulse_ms = 1550; // pulse to stay in rest position
const uint16_t servo1_max_offset_ms = 300; 	// pulse range (from rest position)



/* Set up GPIO pins used for PWM (PB6 and PB7) */
void PWM_ConfigPins(void)
{
	/*
	GPIO_InitTypeDef GPIO_InitStructure;

	// Enable TIM4 clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	// Enable GPIOB
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	// Shared configuration of PB6 and PB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	// Init PB6 and PB7 (timer 4 channels 1 and 2)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// Choose the desired alternate function for PB6 and PB7 (timer)
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);
	*/
}


/* Set up PWM, timer T4 channels 1 and 2 (PB6 and PB7 pins, respectively) */
void Servo_Config(uint32_t period, uint16_t prescaler)
{
	/*
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	PWM_ConfigPins();

	// Setup timer defaults
	TIM_TimeBaseStructure.TIM_Period = period;
	TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	// Configure timer for PWM
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	// PWM1: set on compare
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	// Setup channel 1 (servomotor 0)
	TIM_OCInitStructure.TIM_Pulse = servo0_rest_pulse_ms;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	// Setup channel 2 (servomotor 1)
	TIM_OCInitStructure.TIM_Pulse = servo1_rest_pulse_ms;
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

	// Enable TIM4 preload register on ARR (auto reload register)
	TIM_ARRPreloadConfig(TIM4, ENABLE);

	// Start timer
	TIM_Cmd(TIM4, ENABLE);
	*/
}


/* Positions the specified servo to the angle corresponding to the specified PWM pulse. */
/*void Servo_SetPulse(ServoID servo_id, uint32_t pulse)
{
	TIM_OCInitTypeDef TIM_OCInitStructure;

	// Configure timer for PWM
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = pulse;

	switch(servo_id) {
		case SERVO_X:
			TIM_OC2Init(TIM4, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
			break;
		case SERVO_Y:
			TIM_OC1Init(TIM4, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
			break;
		default:
			return;
	}
}*/


/* Position the specified servo to the angle represented as percentage with respect to maximum offset.
 * That is, 0 corresponds to rest position, -100 and 100 to min and max angles, respectively
 * 32 bit percentage should prevent overflow in saturation condition. */
void Servo_SetPosition(ServoID servo_id, int32_t angle_pct)
{
	uint32_t step;

	// Saturation (can't go beyond 100% of allowed offset)
	if (angle_pct > 100)
		angle_pct = 100;
	else if (angle_pct < -100)
		angle_pct = -100;

	switch(servo_id) {
		case SERVO_X:
			step = servo1_rest_pulse_ms + (angle_pct * servo1_max_offset_ms / 100);
			break;
		case SERVO_Y:
			step = servo0_rest_pulse_ms + (angle_pct * servo0_max_offset_ms / 100);
			break;
		default:
			return;
	}
	Servo_SetPulse(servo_id, step);
}

