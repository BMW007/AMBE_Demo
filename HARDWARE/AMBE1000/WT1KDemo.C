#include "WT1KDemo.h"
#include "Setwt1k.h"


/*------------------------------------------------------------------------------------*/
/*硬件连接
	 AMBERST	  PE0						 							 
	 SDO		  PE9
     SOCK      	  PE10
	 PD2		  PE11
	 SDI	   	  PE3
	 SICK		  PE4
	 SISTRB		  PE8
	 SOSTRB		  PE7
	 EPR	 	  PE1
	 DPE	 	  PE2
-----------------------------------------------------------------------------------*/
#define VOICELENGTH  24


HIBYTE pFrameData[34];
BYTE_EX pTXBUF[VOICELENGTH+2];
BYTE_EX pRXBUF[VOICELENGTH+2];


void WT_GPIO_Config(void)	
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE, ENABLE); 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3| GPIO_Pin_4 | GPIO_Pin_8| GPIO_Pin_11| GPIO_Pin_10| GPIO_Pin_7;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure); 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2| GPIO_Pin_9;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
  GPIO_Init(GPIOE, &GPIO_InitStructure); 

}

void AMBEReset(void)       //复位WT1000
{
	BYTE nI;
	AMBERST(1);
	for(nI = 0; nI < 100; nI++)	Delay15uS();
	AMBERST(0);
	for(nI = 0; nI < 100; nI++)	Delay15uS();
	AMBERST(1);
}

//SOSTRB 下降沿之后，第一个SOCK上升沿锁存数据到AMBE1000的SDO管脚

void AMBEReadSerial(void)
{
	BYTE nI, nK ,nTemp;
	BYTE *pDes;
	pDes = pFrameData;

	for(nI = 0; nI < 17; nI ++)
	{
		SOSTRB(0);
		
		SOCK(0);
		
		nTemp = 0;
		for(nK = 0; nK < 8; nK ++)
		{
			SOCK(1);
			Delay500nS();
			SOCK(0);
			
			if(SDO == 1) Sfr_Bit_Set(nTemp,7-nK);
			
		}
		* pDes++ = nTemp;
		SOSTRB(1);
		
		nTemp = 0;
		for(nK = 0; nK < 8; nK ++)
		{
			SOCK(1);
			Delay500nS();
			SOCK(0);
			
			if(SDO == 1) Sfr_Bit_Set(nTemp,7-nK);
			
		}
		* pDes++ = nTemp;
	}
}

//SISTRB 下降沿之后，第二个SICK上升沿锁存SDI管脚的数据到AMBE1000内部

void AMBEWriteSerial(void)
{
	BYTE nI, nK ,nTemp;
	BYTE *pSrc;
	pSrc = pFrameData;

	for(nI = 0; nI < 17; nI++)
 	{
		SISTRB(0);
		SICK(0);
		Delay500nS();
		SICK(1);
		nTemp = *pSrc++;
		for(nK = 0; nK <=7; nK++)
		{
			SICK(0);
			SDI(Sfr_Bit_Test_True(nTemp ,7-nK));
			Delay500nS();
			SICK(1);
		}
		SISTRB(1);
		nTemp = *pSrc++;
		for(nK = 0; nK <=7; nK++)
		{
			SICK(0);
			SDI(Sfr_Bit_Test_True(nTemp ,7-nK));
			Delay500nS();
			SICK(1);
		}
	}	
}


void DelayUs(unsigned int i)
{
	unsigned int j,time;
	time = F_CPU/(1000000UL)-4;
	for(i=i;i>0;i--)
		for(j=time;j>0;j--);
}

void LoadreadBUF(void)
{
	BYTE *pDes;
	BYTE *pSrc;
	BYTE nI;
	pTXBUF[0]=0X13;
	pTXBUF[1]=0XEC;
	
	pDes = pTXBUF + 2;
	pSrc = pFrameData + 10;	

	for(nI = 0; nI < VOICELENGTH; nI++ )
	{
	 	*pDes ++ = *pSrc ++;

	}
}

void LoadFrame(void)
{
	BYTE *pDes;
	BYTE *pSrc;
	BYTE nI;
	pFrameData[0] = 0x13;
	pFrameData[1] = 0xec;
	
	for(nI = 2; nI < 10; nI++)
	{
		pFrameData[nI] = 0x00;
	}  
	
	pDes = pFrameData + 10 ;
	pSrc = pRXBUF + 2;	

	for(nI = 0; nI < VOICELENGTH; nI++ )
	{
	 	*pDes ++ = *pSrc ++;

	}
}

void LoadNoVoice(void)
{
	BYTE nI;
	BYTE * pDes;
	const BYTE * pSrc;
	
	pDes = pFrameData;
	pSrc = pMute9600;
	
	for(nI = 0; nI < 34; nI++)
	{																							 
		*pDes ++ = *pSrc ++;
	}
}

void Delay15uS(void)
{
	DelayUs(15);
}


void Delay20mS(void)
{
	DelayUs(5000);
}
void Delay500nS(void)
{
	DelayUs(1);

}









