	/********************************************/
	#define BOOL		unsigned char
	#define BYTE		unsigned char 
	#define HIBYTE		unsigned char 
	#define BYTE_EX		unsigned char 
	#include "stm32f10x_gpio.h"
	#include "stm32f10x_usart.h"
	#include "stm32f10x.h"

/************** Bit Opration ***************/
    /*-------------------------------------------------------------------------
     *      Sfr_Bit_Set(sfr,bit);              set bit 'bit' in sfr 'sfr' to 1.
     *      Sfr_Bit_Clear(sfr,bit);            set bit 'bit' in sfr 'sfr' to 0.
     *      if ( Sfr_Bit_Test_True(sfr,bit) )  true if bit 'bit' in sfr 'sfr' is 1. 
     *      if ( Sfr_Bit_Test_False(sfr,bit) ) false if bit 'bit' in sfr 'sfr' is 0. 
     *-------------------------------------------------------------------------*/
/************** Bit Opration ***************/

#define Sfr_Bit_Set(sfr,bit)        (sfr |= (1 << (bit)))
#define lSfr_Bit_Set(sfr,bit)       (sfr |= (1 << (bit)))
#define Sfr_Bit_Clear(sfr,bit)      (sfr &= ~(1 << (bit)))
#define Sfr_Bit_Test_True(sfr,bit)  (sfr >>(bit) & 0x01)
#define Sfr_Bit_Test_False(sfr,bit) (!(sfr & (1 << bit)))



#define  AMBERST(sw)	(sw)?GPIO_SetBits(GPIOE,GPIO_Pin_0):\
							 GPIO_ResetBits(GPIOE,GPIO_Pin_0)							 
							 
#define  SDO				GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_9)	

#define  SOCK(sw)			(sw)?GPIO_SetBits(GPIOE,GPIO_Pin_10):\
							 GPIO_ResetBits(GPIOE,GPIO_Pin_10)

#define  PD2(sw)			(sw)?GPIO_SetBits(GPIOE,GPIO_Pin_11):\
							 GPIO_ResetBits(GPIOE,GPIO_Pin_11)

#define  SDI(sw)			(sw)?GPIO_SetBits(GPIOE,GPIO_Pin_3):\
							 GPIO_ResetBits(GPIOE,GPIO_Pin_3)

#define  SICK(sw)		(sw)?GPIO_SetBits(GPIOE,GPIO_Pin_4):\
							 GPIO_ResetBits(GPIOE,GPIO_Pin_4)

#define  SISTRB(sw)		(sw)?GPIO_SetBits(GPIOE,GPIO_Pin_8):\
							 GPIO_ResetBits(GPIOE,GPIO_Pin_8)

#define  SOSTRB(sw)		(sw)?GPIO_SetBits(GPIOE,GPIO_Pin_7):\
							 GPIO_ResetBits(GPIOE,GPIO_Pin_7)
							 

#define EPR				GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)

#define DPE				GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)



void AMBEReadParallel(void);
void AMBEReadSerial(void);
void AMBEWriteSerial(void);
void Delay15uS(void);
void WT_GPIO_Config(void);
void LoadFrame(void);
void DelayUs(unsigned int i);
void Delay15uS(void);
void Delay20mS(void);
void Delay500nS(void);
void AMBEReset(void);
void LoadFrame(void);
void LoadTXBUF(void);
void LoadreadBUF(void);
void LoadNoVoice(void);
void SetBPS(void);

//#define ENABLE		NVIC_RESETPRIMASK();
//#define DISABLE		NVIC_SETPRIMASK();

#define INTERRUPT_INT0	0
#define INTERRUPT_T0	1
#define INTERRUPT_INT1	2
#define INTERRUPT_T1	3
#define INTERRUPT_UART0 4
#define INTERRUPT_T2	5
#define INTERRUPT_UART1	16

#define F_CPU	10000000UL
















