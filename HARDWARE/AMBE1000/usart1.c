/******************** 鑫盛电子工作室 ********************
 * 文件名  ：usart1.c
 * 描述    ：配置USART1         
 * 实验平台：MINI STM32开发板 基于STM32F103VET6
 * 硬件连接：------------------------
 *          | PA9  - USART1(Tx)      |
 *          | PA10 - USART1(Rx)      |
 *           ------------------------
 * 库版本  ：ST3.0.0  
 * 淘宝店：http://shop66177872.taobao.com
**********************************************************************************/

#include "usart1.h"
#include "stm32f10x_usart.h"
#include <stdarg.h>
#include "stm32f10x.h"
#include "WT1KDemo.h"
#include "misc.h"


extern BYTE_EX pRXBUF[];
extern BYTE_EX pTXBUF[];


void USART1_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* 使能 USART1 时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); 

	/* USART1 使用IO端口配置 */    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);    
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);   //初始化GPIOA
	  
	/* USART1 工作模式配置 */
	USART_InitStructure.USART_BaudRate = 9600;	//波特率设置：原来为19200， 2015年1月11日 由小明改为9600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No ;  //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//接收与发送都使能
	USART_Init(USART1, &USART_InitStructure);  //初始化USART1
	USART_Cmd(USART1, ENABLE);// USART1使能

	  NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
}

void UART1SendData(unsigned char *SendData,unsigned int Len)
{
	unsigned int  nI;
	for(nI = 0; nI < Len; nI++)
	{
		USART_SendData(USART1,pTXBUF[nI]);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	   
	}

}

extern volatile uint8_t bReceived;
unsigned char nRxLength = 0;
unsigned char Rx_buff[30]={0};
unsigned char nRxByteCnt=0;
#define IDLE	1
#define HEAD	2
#define DATA	3
#define VOICELENGTH 24
int Len = VOICELENGTH+2;

void USART1_IRQHandler(void)
{
	static unsigned char RX_STATE = IDLE;
	unsigned char temp,nI;
	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	
	temp = USART_ReceiveData(USART1); 
	
	if(RX_STATE == IDLE){
		if(temp == 0x13)
		{
			RX_STATE = HEAD;
			Rx_buff[0] = temp;
			nRxByteCnt = 1 ;
		}else
		{
			RX_STATE = IDLE;
			return ;
		}
	}else if(RX_STATE == HEAD){
		if(temp == 0xEC)
		{
			RX_STATE = DATA;
			Rx_buff[1] = temp;
			nRxByteCnt = 2;
		}else
		{
			RX_STATE = IDLE;
			return ;
		}		
	}else if(RX_STATE == DATA){
		Rx_buff[nRxByteCnt] = temp;	
		nRxByteCnt ++;
		if(nRxByteCnt == Len)
		{
			nRxByteCnt = 0;
			RX_STATE = IDLE;
			bReceived =1;
				for(nI = 0; nI < Len; nI++)
				{
					pRXBUF[nI] = Rx_buff[nI];
				}
		}
	}
	
}


























