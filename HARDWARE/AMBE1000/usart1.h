#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>

void USART1_Config(void);
void UART1Test(void);
void UART1SendData(unsigned char *SendData,unsigned int Len);
void UART1GetByte(unsigned char* GetData);
#endif /* __USART1_H */



