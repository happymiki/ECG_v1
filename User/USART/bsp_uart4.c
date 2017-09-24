/**
  ******************************************************************************
  * @file    bsp_usart3.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   usartӦ��bsp
  ******************************************************************************

  ******************************************************************************
  */
  
#include "bsp_uart4.h"


void UART4_NVIC_Configuration(FunctionalState isEnable)
{
    NVIC_IRQChannel_Configuration_Set(UART4_IRQn, 3, 3, isEnable);
}



 /**
  * @brief  UART4 GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval ��
  */
void UART4_Config(uint32_t BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* config UART4 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
    
	/* UART4 GPIO config */
	/* Configure UART4 Tx (PC.10) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);    
	
	/* Configure UART4 Rx (PC.11) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* UART4 mode config */
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure);
    /* ʹ�ܴ���4�����ж� */
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);	
    USART_Cmd(UART4, ENABLE);
    UART4_NVIC_Configuration(ENABLE);
    USART_DMACmd(UART4,USART_DMAReq_Tx,ENABLE);
    MYDMA_Config(UART4_TX_DMA_Channel,(u32)&UART4->DR,(u32)UART4_DMA_SendBuff,ENABLE,UART4_SEND_DMA_BUF_LENTH);
    NVIC_IRQChannel_Configuration_Set(DMA2_Channel4_5_IRQn, 3, 3, ENABLE);
    DMA_ITConfig(UART4_TX_DMA_Channel,DMA_IT_TC,ENABLE);
}


void ChangeUART4BaudRate(uint32_t BaudRate, FunctionalState ReceiveITState)
{
    USART_InitTypeDef USART_InitStructure;
    /* UART4 mode config */
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure);
    USART_ITConfig(UART4, USART_IT_RXNE, ReceiveITState);	
    USART_Cmd(UART4, ENABLE);
}


void SendUART4OneByte(uint8_t byteData)//���ڷ�����Ϣ,ͨ����ѯ��ʽ����һ���ַ�
{
    USART_ClearFlag(UART4,USART_FLAG_TC);//�����һ�·����жϱ�־λ��������һ���ֽڶ�ʧ�����⡣
	USART_SendData(UART4, byteData);
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
}

void SendUART4BytesBuf(uint8_t* bytesBuf, uint16_t bytesBufLength)
{    
	while(bytesBufLength--)
	{
		SendUART4OneByte(*(bytesBuf++));
	}
}

void UART4_DMA_Send_Data(uint8_t *USART_SendBuff, uint16_t DataSendLength)
{
    PushUART4_DMA_SendDataIntoFIFO(USART_SendBuff, DataSendLength);
}

  /*******************************************************************************
* Function Name  : UART4_IRQHandler
* Description    : This function handles UART4 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART4_IRQHandler(void)
{
	u8 receiveByte = 0;
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
        USART_ClearITPendingBit(UART4,USART_IT_RXNE); //����жϱ�־
		receiveByte = USART_ReceiveData(UART4);//(UART4->DR);		//��ȡ���յ�������
	}
}


/*********************************************END OF FILE**********************/
