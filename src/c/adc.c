/*
 * adc.c
 *
 * Manage STM32F4 Discovery built-in ADCs.
 *
 *  Created on: 10/feb/2019
 *      Author: Leonardo Lai
 */

#include "stm32f4xx.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4_discovery.h"

#include "adc.h"

#define ADC_PERIPH 	RCC_APB2Periph_ADC1 	// ADC1 on APB2
#define ADC_ID 		ADC1


/* Configure and enable analog-digital converter, in order to read analog inputs.*/
void ADC_Config(void)
{
    // Enable clock for ADC
    RCC_APB2PeriphClockCmd(ADC_PERIPH, ENABLE);

    // Init ADC
    ADC_InitTypeDef ADC_InitStruct;
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_ExternalTrigConv = DISABLE;
    ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStruct.ADC_NbrOfConversion = 1;
    ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;
    ADC_Init(ADC_ID, &ADC_InitStruct);
}


/* Select the ADC channel.
 * For ADC1, channels 8 and 9 correspond to pin PB0 and PB1, respectively. */
void ADC_ChooseChannel(uint8_t ch)
{
	switch(ch) {
		case 8:
			ADC_RegularChannelConfig(ADC_ID, ADC_Channel_8, 1, ADC_SampleTime_3Cycles);
			break;
		case 9:
			ADC_RegularChannelConfig(ADC_ID, ADC_Channel_9, 1, ADC_SampleTime_3Cycles);
			break;
		default:
			break;
	}
}


/* Enable the ADC */
void ADC_Enable(void)
{
	ADC_Cmd(ADC_ID, ENABLE);
}


/* Disable the ADC */
void ADC_Disable(void)
{
	ADC_Cmd(ADC_ID, DISABLE);
}


/* Read a value from the ADC. */
uint16_t ADC_Read(void)
{
    // Start conversion
    ADC_SoftwareStartConv(ADC_ID);
    // Wait conversion to finish
    while (!ADC_GetFlagStatus(ADC_ID, ADC_FLAG_EOC));

    return ADC_GetConversionValue(ADC_ID);
}
