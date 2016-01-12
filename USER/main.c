//#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
//#include "usart.h"

/****************************Copyright(c)*****************************
**                      http://www.indusic.com
**--------------File Info---------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2014-11-18
** Last Version:        V1.00
** Descriptions:        
**
**----------------------------------------------------------------------------------------------------*/
//#include "stm32f10x.h"
#include "WT1KDemo.h"
#include "usart1.h"
#include "stm32f10x_usart.h"
#include <stdarg.h>
#include "stm32f10x.h"
#include "misc.h"


#define VOICELENGTH  24                       //定义发送的语音长度


extern HIBYTE pFrameData[];                   
extern BYTE_EX pRXBUF[];                      //串口接收缓冲区
extern BYTE_EX pTXBUF[];					  //串口发送缓冲区
uint8_t bReceived;                            //串口接收一包完整数据置1
uint8_t noReceived;

int main(void)
{
	BYTE nI;
	 LED_Init();
	USART1_Config();                           //串口初始化
	WT_GPIO_Config();                          //控制WT1000的IO引脚初始化
	AMBEReset();                               //复位WT1K和WT1027
	
	SOCK(0);
	SOSTRB(1);
	PD2(0);                                     //串行模式下PD2必须下拉或者接地
	
	pFrameData[0] = 0x13;
	pFrameData[1] = 0xec;
	for(nI = 2; nI < 34; nI++)
	{
		pFrameData[nI] = 0;                       //初始化pFrameData数组
	}
	
	while(!EPR);
	AMBEReadSerial();
	
	while(1)                                     
	{
		LED0=~LED0;
		while(!EPR);                             //读EPR信号

		AMBEReadSerial();                        //串行读WT1K编码数据
		LoadreadBUF();                           //将独到的语音编码数据放到发送缓冲区pTXBUF中
		UART1SendData(pTXBUF,VOICELENGTH+2);     //串口发送
		while(!DPE);                             //读DPE信号
		if(bReceived ==1)                        //串口中断接收到一包完整的数据bReceived=1;收到的数据放在接收缓冲区pRXBUF
		{	
			noReceived=0;
			LoadFrame();                           //将接收缓冲区pRXBUF的语音数据移到pFrameData中
			AMBEWriteSerial();                     //串行写WT1K
			bReceived = 0;
		}
		else
		{	
			noReceived++;
			if(noReceived==4)                       //连续4包没收到完整数据向WT1K写静音数据
			{
				noReceived=3;
				LoadNoVoice();
				AMBEWriteSerial();
			}
		}
	}
}

  


/************************ (C) COPYRIGHT wmb007 *****END OF FILE****/




















