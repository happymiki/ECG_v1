#ifndef __USART3_H
#define	__USART3_H

#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include "NVIC_CONFIG.H"
#include "dma.h"
#include "CommunicationProtocol.h"
#include "CommunicationConfig.h"


void USART3_Config(uint32_t BaudRate);
void SendUSART3OneByte(uint8_t byteData);
void SendUSART3BytesBuf(uint8_t* bytesBuf, uint16_t bytesBufLength);
void USART3_DMA_Send_Data(uint8_t *USART_SendBuff, uint16_t DataSendLength);//USART3 DMA���ʹ�
void ChangeUSART3BaudRate(uint32_t BaudRate, FunctionalState ReceiveITState);

#endif /* __USART3_H */
