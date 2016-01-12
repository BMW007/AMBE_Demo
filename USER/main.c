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


#define VOICELENGTH  24                       //���巢�͵���������


extern HIBYTE pFrameData[];                   
extern BYTE_EX pRXBUF[];                      //���ڽ��ջ�����
extern BYTE_EX pTXBUF[];					  //���ڷ��ͻ�����
uint8_t bReceived;                            //���ڽ���һ������������1
uint8_t noReceived;

int main(void)
{
	BYTE nI;
	 LED_Init();
	USART1_Config();                           //���ڳ�ʼ��
	WT_GPIO_Config();                          //����WT1000��IO���ų�ʼ��
	AMBEReset();                               //��λWT1K��WT1027
	
	SOCK(0);
	SOSTRB(1);
	PD2(0);                                     //����ģʽ��PD2�����������߽ӵ�
	
	pFrameData[0] = 0x13;
	pFrameData[1] = 0xec;
	for(nI = 2; nI < 34; nI++)
	{
		pFrameData[nI] = 0;                       //��ʼ��pFrameData����
	}
	
	while(!EPR);
	AMBEReadSerial();
	
	while(1)                                     
	{
		LED0=~LED0;
		while(!EPR);                             //��EPR�ź�

		AMBEReadSerial();                        //���ж�WT1K��������
		LoadreadBUF();                           //�������������������ݷŵ����ͻ�����pTXBUF��
		UART1SendData(pTXBUF,VOICELENGTH+2);     //���ڷ���
		while(!DPE);                             //��DPE�ź�
		if(bReceived ==1)                        //�����жϽ��յ�һ������������bReceived=1;�յ������ݷ��ڽ��ջ�����pRXBUF
		{	
			noReceived=0;
			LoadFrame();                           //�����ջ�����pRXBUF�����������Ƶ�pFrameData��
			AMBEWriteSerial();                     //����дWT1K
			bReceived = 0;
		}
		else
		{	
			noReceived++;
			if(noReceived==4)                       //����4��û�յ�����������WT1Kд��������
			{
				noReceived=3;
				LoadNoVoice();
				AMBEWriteSerial();
			}
		}
	}
}

  


/************************ (C) COPYRIGHT wmb007 *****END OF FILE****/




















