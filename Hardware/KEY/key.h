#ifndef __KEY_H__
#define __KEY_H__
#include "sys.h"

//#define KEY0 PCin(5)   	//PC5
//#define KEY1 PAin(15)	//PA15 


#define KEY0  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) //��ȡ����0
#define KEY1  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) //��ȡ����1

#define KEY0_PRES 	1	// KEY0����
#define KEY1_PRES	  2	// KEY1����

void KEY_Init(void);// IO��ʼ��
u8 KEY_Scan(u8);  	// ����ɨ�躯��	

#endif
