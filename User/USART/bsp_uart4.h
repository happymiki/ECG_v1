#ifndef __UART4_H
#define	__UART4_H

#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include "NVIC_CONFIG.H"
#include "dma.h"



void UART4_Config(uint32_t BaudRate);
void SendUART4OneByte(uint8_t byteData);
void SendUART4BytesBuf(uint8_t* bytesBuf, uint16_t bytesBufLength);
void UART4_DMA_Send_Data(uint8_t *USART_SendBuff, uint16_t DataSendLength);//DMA���ʹ�
void ChangeUART4BaudRate(uint32_t BaudRate, FunctionalState ReceiveITState);

#endif /* __UART4_H */
