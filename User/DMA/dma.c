#include "dma.h"
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


uint8_t USART1_DMA_SendBuff[USART1_SEND_DMA_BUF_LENTH];
bool USART1_DMA_Sendding = false;
Uint8PacketQueue USART1_DMASendPacketQueue = {NULL, NULL, CreatUint8PacketNode, Free_USART_DMA_PacketNoedItem};
Uint8PacketQueue* USART1_DMASendPacketQueueHandle = &USART1_DMASendPacketQueue;

uint8_t USART2_DMA_SendBuff[USART2_SEND_DMA_BUF_LENTH];
bool USART2_DMA_Sendding = false;
Uint8PacketQueue USART2_DMASendPacketQueue = {NULL, NULL, CreatUint8PacketNode, Free_USART_DMA_PacketNoedItem};
Uint8PacketQueue* USART2_DMASendPacketQueueHandle = &USART2_DMASendPacketQueue;

uint8_t USART3_DMA_SendBuff[USART3_SEND_DMA_BUF_LENTH];
bool USART3_DMA_Sendding = false;
Uint8PacketQueue USART3_DMASendPacketQueue = {NULL, NULL, CreatUint8PacketNode, Free_USART_DMA_PacketNoedItem};
Uint8PacketQueue* USART3_DMASendPacketQueueHandle = &USART3_DMASendPacketQueue;

uint8_t UART4_DMA_SendBuff[UART4_SEND_DMA_BUF_LENTH];
bool UART4_DMA_Sendding = false;
Uint8PacketQueue UART4_DMASendPacketQueue = {NULL, NULL, CreatUint8PacketNode, Free_USART_DMA_PacketNoedItem};
Uint8PacketQueue* UART4_DMASendPacketQueueHandle = &UART4_DMASendPacketQueue;
   
//DMA�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//Channel_x:DMAͨ��CHx
//PeripheralBaseAddr:�����ַ
//MemoryBaseAddr:�洢����ַ
//BufferSize:���ݴ����� 
void MYDMA_Config(DMA_Channel_TypeDef* Channel_x,u32 PeripheralBaseAddr,u32 MemoryBaseAddr,FunctionalState isMemoryToPeripheral,u16 BufferSize)
{
    DMA_InitTypeDef DMA_InitStructure;
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	                //ʹ��DMA����
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);	                //ʹ��DMA����
    DMA_DeInit(Channel_x);                                          //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = PeripheralBaseAddr;  //DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = MemoryBaseAddr;  		//DMA�ڴ����ַ
    DMA_InitStructure.DMA_BufferSize = BufferSize;  			    //DMAͨ����DMA����Ĵ�С
    
    if(isMemoryToPeripheral == ENABLE)
    {
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;    //�����ַ�Ĵ�������
        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;             //�ڴ��ַ�Ĵ�������
    }
    else
    {
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                  //���ݴ��䷽�򣬴������ȡ���͵��ڴ�
        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;    //�����ַ�Ĵ�������
        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;             //�ڴ��ַ�Ĵ�������
    }
    
    
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //�������ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;     //�ڴ����ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                       //��������������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;               //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                        //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(Channel_x, &DMA_InitStructure);                        //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ������ʶ�ļĴ���
	DMA_Cmd(Channel_x, ENABLE);                                         //ʹ����ָʾ��ͨ�� ��ÿ�η�������ʱʹ��
}
//����һ��DMA����
void MYDMA_Enable(DMA_Channel_TypeDef* DMA_CHx, u16 bufferSize)
{ 
	DMA_Cmd(DMA_CHx, DISABLE );  //�ر�USART1 TX DMA1 ��ָʾ��ͨ��      
 	DMA_SetCurrDataCounter(DMA_CHx,bufferSize);//DMAͨ����DMA����Ĵ�С
 	DMA_Cmd(DMA_CHx, ENABLE);  //ʹ��USART1 TX DMA1 ��ָʾ��ͨ�� 
}	  


void USART1_TXD_DMA_Enable(u16 bufferSize)
{
 	MYDMA_Enable(USART1_TX_DMA_Channel,bufferSize);//DMAͨ����DMA����Ĵ�С
}

void USART2_TXD_DMA_Enable(u16 bufferSize)
{
 	MYDMA_Enable(USART2_TX_DMA_Channel,bufferSize);//DMAͨ����DMA����Ĵ�С
}

void USART3_TXD_DMA_Enable(u16 bufferSize)
{
 	MYDMA_Enable(USART3_TX_DMA_Channel,bufferSize);//DMAͨ����DMA����Ĵ�С
}

void UART4_TXD_DMA_Enable(u16 bufferSize)
{
 	MYDMA_Enable(UART4_TX_DMA_Channel,bufferSize);//DMAͨ����DMA����Ĵ�С
}

void Free_USART_DMA_PacketNoedItem(Uint8PacketNode* uint8PacketNodePointer)
{
    if(!uint8PacketNodePointer)return;
    if(uint8PacketNodePointer->packetBlock)
    {
        free(uint8PacketNodePointer->packetBlock);
        uint8PacketNodePointer->packetBlock = NULL;
    }
    free(uint8PacketNodePointer);
}

void USART_DMA_SendPacketNode(Uint8PacketQueue* USART_DMASendPacketQueueHandle,uint8_t* USART_DMA_SendBuff,bool* USART_DMA_Sendding,void (*USART_TXD_DMA_Enable)(u16 bufferSize))
{
    if(USART_DMASendPacketQueueHandle->head)
    {
        uint8_t* packet;
        uint16_t packetLength;
        Uint8PacketNode* uint8PacketNodePointer;
        uint8PacketNodePointer = Uint8PacketQueuePop(USART_DMASendPacketQueueHandle);
        packet = uint8PacketNodePointer->packet;
        packetLength = uint8PacketNodePointer->packetLength;
        memcpy(USART_DMA_SendBuff,packet,packetLength);
        USART_DMASendPacketQueueHandle->FreePacketNoedItem(uint8PacketNodePointer);
        *USART_DMA_Sendding = true;
        USART_TXD_DMA_Enable(packetLength);

    }
}


/*���ڷ�װ���ṩ����push������FIFO�Ľӿ�
*
*/
void PushUSART1_DMA_SendDataIntoFIFO(uint8_t *USART_SendBuff, uint16_t DataSendLength)//���ڷ�װ���ṩ����push��FIFO�Ľӿ�
{
    Uint8PacketQueuePushStreamData(USART1_DMASendPacketQueueHandle, USART_SendBuff, DataSendLength);
    if(USART1_DMA_Sendding == false)
    {
        USART_DMA_SendPacketNode(USART1_DMASendPacketQueueHandle, USART1_DMA_SendBuff, &USART1_DMA_Sendding, USART1_TXD_DMA_Enable);
    }
}

/*���ڷ�װ���ṩ����push������FIFO�Ľӿ�
*
*/
void PushUSART2_DMA_SendDataIntoFIFO(uint8_t *USART_SendBuff, uint16_t DataSendLength)//���ڷ�װ���ṩ����push��FIFO�Ľӿ�
{
    Uint8PacketQueuePushStreamData(USART2_DMASendPacketQueueHandle, USART_SendBuff, DataSendLength);
    if(USART2_DMA_Sendding == false)
    {
        USART_DMA_SendPacketNode(USART2_DMASendPacketQueueHandle, USART2_DMA_SendBuff, &USART2_DMA_Sendding, USART2_TXD_DMA_Enable);
    }
}


/*���ڷ�װ���ṩ����push������FIFO�Ľӿ�
*
*/
void PushUSART3_DMA_SendDataIntoFIFO(uint8_t *USART_SendBuff, uint16_t DataSendLength)//���ڷ�װ���ṩ����push��FIFO�Ľӿ�
{
    Uint8PacketQueuePushStreamData(USART3_DMASendPacketQueueHandle, USART_SendBuff, DataSendLength);
    if(USART3_DMA_Sendding == false)
    {
        USART_DMA_SendPacketNode(USART3_DMASendPacketQueueHandle, USART3_DMA_SendBuff, &USART3_DMA_Sendding, USART3_TXD_DMA_Enable);
    }
}


/*���ڷ�װ���ṩ����push������FIFO�Ľӿ�
*
*/
void PushUART4_DMA_SendDataIntoFIFO(uint8_t *USART_SendBuff, uint16_t DataSendLength)//���ڷ�װ���ṩ����push��FIFO�Ľӿ�
{
    Uint8PacketQueuePushStreamData(UART4_DMASendPacketQueueHandle, USART_SendBuff, DataSendLength);
    if(UART4_DMA_Sendding == false)
    {
        USART_DMA_SendPacketNode(UART4_DMASendPacketQueueHandle, UART4_DMA_SendBuff, &UART4_DMA_Sendding, UART4_TXD_DMA_Enable);
    }
}

void USART1_DMA_IRQHandler(void)
{
    if(DMA_GetFlagStatus(USART1_DMA_TX_FLAG) == SET)
    {
        USART1_DMA_Sendding = false;
        DMA_ClearFlag(USART1_DMA_TX_FLAG);
        if(USART1_DMASendPacketQueueHandle->head)
        {
            USART_DMA_SendPacketNode(USART1_DMASendPacketQueueHandle, USART1_DMA_SendBuff, &USART1_DMA_Sendding, USART1_TXD_DMA_Enable);
        }
    }
}

void USART2_DMA_IRQHandler(void)
{
    if(DMA_GetFlagStatus(USART2_DMA_TX_FLAG) == SET)
    {
        USART2_DMA_Sendding = false;
        DMA_ClearFlag(USART2_DMA_TX_FLAG);
        if(USART2_DMASendPacketQueueHandle->head)
        {
            USART_DMA_SendPacketNode(USART2_DMASendPacketQueueHandle, USART2_DMA_SendBuff, &USART2_DMA_Sendding, USART2_TXD_DMA_Enable);
        }
    }
}

void USART3_DMA_IRQHandler(void)
{
    if(DMA_GetFlagStatus(USART3_DMA_TX_FLAG) == SET)
    {
        USART3_DMA_Sendding = false;
        DMA_ClearFlag(USART3_DMA_TX_FLAG);
        if(USART3_DMASendPacketQueueHandle->head)
        {
            USART_DMA_SendPacketNode(USART3_DMASendPacketQueueHandle, USART3_DMA_SendBuff, &USART3_DMA_Sendding, USART3_TXD_DMA_Enable);
        }
    }
}
void UART4_DMA_IRQHandler(void)
{
    if(DMA_GetFlagStatus(UART4_DMA_TX_FLAG) == SET)
    {
        UART4_DMA_Sendding = false;
        DMA_ClearFlag(UART4_DMA_TX_FLAG);
        if(UART4_DMASendPacketQueueHandle->head)
        {
            USART_DMA_SendPacketNode(UART4_DMASendPacketQueueHandle, UART4_DMA_SendBuff, &UART4_DMA_Sendding, UART4_TXD_DMA_Enable);
        }
    }
}










