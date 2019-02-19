/*
 * adc.h
 *
 *  Created on: 10/feb/2019
 *      Author: leona
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdlib.h>


void ADC_Config(void);
void ADC_ChooseChannel(uint8_t ch);
void ADC_Enable(void);
void ADC_Disable(void);
uint16_t ADC_Read(void);

#endif /* ADC_H_ */
