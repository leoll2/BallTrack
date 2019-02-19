/* 4-pin touchscreen */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4_discovery.h"

#include "adc.h"
#include "touch.h"
#include "serial.h"

typedef enum{X, Y} Axis;
Axis selected_axis = X;


/* Adjust pins to reflect changes in the selected axis.
 * Specifically, set PB0 and PB1 to be analog input and digital output
 * respectively, or viceversa. */
void setup_gpio(Axis to_read)
{
	GPIO_InitTypeDef GPIO_InitStructANA;	// config of the pin used as analog input
	GPIO_InitTypeDef GPIO_InitStructOUT;	// config of the pin used as output
	GPIO_InitTypeDef GPIO_InitStructHZ;		// config of the pin used in high-impedance

	// ANA
	GPIO_InitStructANA.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructANA.GPIO_PuPd = GPIO_PuPd_NOPULL;

	// OUT
	GPIO_InitStructOUT.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructOUT.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructOUT.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructOUT.GPIO_PuPd = GPIO_PuPd_NOPULL;

	// HZ
	GPIO_InitStructHZ.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructHZ.GPIO_PuPd = GPIO_PuPd_NOPULL;

	switch(to_read) {
		case X:
			GPIO_InitStructANA.GPIO_Pin = GPIO_Pin_0;
			GPIO_InitStructOUT.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_13;
			GPIO_InitStructHZ.GPIO_Pin = GPIO_Pin_14;
			break;
		case Y:
			GPIO_InitStructANA.GPIO_Pin = GPIO_Pin_1;
			GPIO_InitStructOUT.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_14;
			GPIO_InitStructHZ.GPIO_Pin = GPIO_Pin_13;
			break;
	}

	GPIO_Init(GPIOB, &GPIO_InitStructANA);
	GPIO_Init(GPIOB, &GPIO_InitStructOUT);
	GPIO_Init(GPIOB, &GPIO_InitStructHZ);
}


void select_axis(Axis ax)
{
	ADC_Disable();
	setup_gpio(ax);
	selected_axis = ax;

	switch(ax) {
		case X:
			/* PB1	-> 0
			 * PB0 	-> in
			 * PB13	-> 3V
			 * PB14 -> HZ */
			ADC_ChooseChannel(8);
			GPIO_ResetBits(GPIOB, GPIO_Pin_1);
			GPIO_SetBits(GPIOB, GPIO_Pin_13);
			break;
		case Y:
			/* PB1	-> in
			 * PB0 	-> 0
			 * PB13	-> HZ
			 * PB14 -> 3V */
			ADC_ChooseChannel(9);
			GPIO_ResetBits(GPIOB, GPIO_Pin_0);
			GPIO_SetBits(GPIOB, GPIO_Pin_14);
			break;
		default:
			break;
	}
	ADC_Enable();

	/* A bunch of dummy reads to discard. This is necessary because the ADC needs time to
	 * catch-up with the new configuration, otherwise the readings are very inaccurate.
	 * Number 100 has been obtained empirically. */
	for(uint8_t i = 0; i < 100; ++i) {
		uint16_t dummy = ADC_Read();
	}
}


uint16_t read_x(void)
{
	if (!(selected_axis == X))
		select_axis(X);
	return 0xFFF - ADC_Read();
}


uint16_t read_y(void)
{
	if (!(selected_axis == Y))
		select_axis(Y);
	return ADC_Read();
}


/* Initialize the touchscreen. */
void Touch_Config(void)
{
	// Initialize ADC
	ADC_Config();
	// Activate clock for GPIOB
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	// Select X axis by default
	select_axis(X);
}


/* Read the coordinate from the touchscreen */
Coordinate Touch_Read(void)
{
	Coordinate ret;
	ret.x = read_x();
	ret.y = read_y();
	return ret;
}


/* validate_read() provides an anomaly-detection mechanism for touchscreen readings.
 * For unknown reasons, the ADC sporadically returns wrong values. Such outliers
 * are often identifiable because they widely differ from the previous samples.
 * In that case, the current sample is simply discarded, and no control action is
 * generated. In order not to jeopardize the stability of the system, an upper
 * bound is defined on the number of consecutive measurements that can be skipped.
 * Experiments show that this policy works sufficiently well in most cases.
 * */
bool Touch_ValidateRead(Coordinate pos, Coordinate pos_prev)
{
	static const uint16_t max_allowed_deviation = 700;
	static const uint8_t max_consecutive_skips = 3;
	static uint8_t skipped = 0;
	char s[32];

	if ((abs(pos.x - pos_prev.x) > max_allowed_deviation) ||
		(abs(pos.y - pos_prev.y) > max_allowed_deviation))
	{
		if (++skipped <= max_consecutive_skips) {
			sprintf(s, "Skip read!\r\n", pos.x, pos.y);
			console_out(s);
			return false;
		}
	}
	skipped = 0;
	return true;
}

