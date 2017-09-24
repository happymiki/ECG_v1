#ifndef __DMA_H
#define	__DMA_H	   
#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include "stdlib.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "Uint8PacketQueueManger.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK miniSTM32������
//DMA ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/8
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
							    					    
#define USART1_TX_DMA_Channel             DMA1_Channel4                                                   
#define USART2_TX_DMA_Channel             DMA1_Channel7
#define USART3_TX_DMA_Channel             DMA1_Channel2
#define UART4_TX_DMA_Channel              DMA2_Channel5

#define USART1_DMA_TX_FLAG                DMA1_FLAG_TC4
#define USART2_DMA_TX_FLAG                DMA1_FLAG_TC7
#define USART3_DMA_TX_FLAG                DMA1_FLAG_TC2
#define UART4_DMA_TX_FLAG                 DMA2_FLAG_TC5

#define USART1_DMA_IRQHandler             DMA1_Channel4_IRQHandler
#define USART2_DMA_IRQHandler             DMA1_Channel7_IRQHandler
#define USART3_DMA_IRQHandler             DMA1_Channel2_IRQHandler
#define UART4_DMA_IRQHandler              DMA2_Channel4_5_IRQHandler


#define USART1_SEND_DMA_BUF_LENTH       500
#define USART2_SEND_DMA_BUF_LENTH       500
#define USART3_SEND_DMA_BUF_LENTH       500
#define UART4_SEND_DMA_BUF_LENTH        500

extern uint8_t USART1_DMA_SendBuff[];
extern uint8_t USART2_DMA_SendBuff[];
extern uint8_t USART3_DMA_SendBuff[];
extern uint8_t UART4_DMA_SendBuff[];

void MYDMA_Config(DMA_Channel_TypeDef* Channel_x,u32 PeripheralBaseAddr,u32 MemoryBaseAddr,FunctionalState isMemoryToPeripheral,u16 BufferSize);
void MYDMA_Enable(DMA_Channel_TypeDef* DMA_CHx, u16 bufferSize);

void USART1_TXD_DMA_Enable(u16 bufferSize);
void USART2_TXD_DMA_Enable(u16 bufferSize);
void USART3_TXD_DMA_Enable(u16 bufferSize);
void UART4_TXD_DMA_Enable(u16 bufferSize);


void PushUSART1_DMA_SendDataIntoFIFO(uint8_t *USART_SendBuff, uint16_t DataSendLength);
void PushUSART2_DMA_SendDataIntoFIFO(uint8_t *USART_SendBuff, uint16_t DataSendLength);
void PushUSART3_DMA_SendDataIntoFIFO(uint8_t *USART_SendBuff, uint16_t DataSendLength);
void PushUART4_DMA_SendDataIntoFIFO(uint8_t *USART_SendBuff, uint16_t DataSendLength);

void Free_USART_DMA_PacketNoedItem(Uint8PacketNode* uint8PacketNodePointer);

#endif




