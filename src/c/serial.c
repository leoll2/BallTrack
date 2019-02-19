/* RS-232 serial communication */

#include <stdio.h>

#include "ee.h"
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"


/* Configure the serial communication between the Discovery board and another machine
 * (typically another computer, for debugging purposes).*/
void USART_Config(void)
{
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	STM_EVAL_COMInit(COM1, &USART_InitStructure);
}


/* Send a message via serial communication. */
void console_out(char *msg)
{
	EE_UINT8 i = 0;

	while (msg[i] != '\0') {
		USART_SendData(EVAL_COM1, (uint8_t)msg[i++]);
		/* Loop until the end of transmission */
		while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET);
	}
}
