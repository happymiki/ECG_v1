#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"

#define ADC_CH1  			1  		//ͨ��1		 
#define ADC_CH_TEMP  	16 		//�¶ȴ�����ͨ��

void Adc_Init(void);
u16  Get_Adc(u8 ch); 
u16 Get_Adc_Average(u8 ch,u8 times); 
int Get_Temp(void);					//��ȡ�¶�ֵ

#endif 
