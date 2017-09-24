#ifndef __E30TTLUART_H
#define __E30TTLUART_H
#include "bsp_usart3.h"

#define E30TTLUART_SendOneByte          SendUSART3OneByte
#define E30TTLUART_ChangeBaudRate       ChangeUSART3BaudRate
#define E30TTLUART_UART_SendBytesBuf    USART3_DMA_Send_Data
#define E30TTLUART_UART_AppointSendBytesBuf SendUSART3BytesBuf     //ָ������ʱ��Ҫ�ķ��ͺ���������DMA��ʽ


#define E30TTLUART_M0_Port GPIOB
#define E30TTLUART_M0_Pin  GPIO_Pin_8
#define E30TTLUART_M0_RCC_Periph RCC_APB2Periph_GPIOB

#define E30TTLUART_M1_Port GPIOB
#define E30TTLUART_M1_Pin  GPIO_Pin_9
#define E30TTLUART_M1_RCC_Periph RCC_APB2Periph_GPIOB

#define E30TTLUART_AUX_Port GPIOB
#define E30TTLUART_AUX_Pin  GPIO_Pin_7
#define E30TTLUART_AUX_RCC_Periph RCC_APB2Periph_GPIOB

////IO��������											   
#define	E30TTLUART_M0_OUT    PBout(8)
#define	E30TTLUART_M1_OUT    PBout(9)
#define	E30TTLUART_AUX_IN    PBin(7)  //���ݶ˿�	PA0 


#define E30TTLUART_MODE_0 0
#define E30TTLUART_MODE_1 1
#define E30TTLUART_MODE_2 2
#define E30TTLUART_MODE_3 3

void E30TTLUART_Init(void);//��ʼ��E30TTLģ������
void E30TTLUART_Config(uint16_t E30TTLUART_Address,uint8_t E30TTLUART_Channel,FunctionalState isPoint2Point);//����ģ��
void E30TTLUART_MultiCountConfig(uint16_t E30TTLUART_Address,uint8_t E30TTLUART_Channel,FunctionalState isPoint2Point,uint8_t reConfigTimes);//���������Ч
void E30TTLUART_TransparentSendBytesData(uint8_t* bytesBuf, uint16_t bytesBufLength);
void E30TTLUART_Point2PointSendBytesData(uint16_t address,uint8_t channel,uint8_t* bytesBuf, uint16_t bytesBufLength);
void E30TTLUART_AppointSendBytesData(uint8_t *USART3_SendBuff, uint16_t DataSendLength);
extern void (*E30TTLUART_SendBytesData)(uint8_t *USART3_SendBuff, uint16_t DataSendLength);//����API,�ṩ���ͽӿڣ����������Ƿ�͸��
#endif















