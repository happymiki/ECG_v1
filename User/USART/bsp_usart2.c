/******************** (C) COPYRIGHT 2012 WildFire Team **************************

**********************************************************************************/
#include <stdarg.h>
#include "bsp_usart2.h"
#include "ESP8266_config.h"

USART2_Receive_Handler USART2ReceiveHandler;


/*
 * ��������USART2_Config
 * ����  ��USART2 GPIO ����,����ģʽ����
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
void USART2_Config(uint32_t BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* config USART2 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* USART2 GPIO config */
    /* Configure USART2 Tx (PA.02) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
        
    /* Configure USART2 Rx (PA.03) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate = BaudRate;               
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure); 

	/* ʹ�ܴ���2�����ж� */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	/* ʹ�ܴ���2���߿����ж� */
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	USART_ClearFlag(USART2,USART_FLAG_TC);//�����һ�·����жϱ�־λ��������һ���ֽڶ�ʧ�����⡣
	USART_Cmd(USART2, ENABLE);
    SetUSART2_NVIC_ISENABLE(ENABLE);
	USART2ReceiveHandler = ReceiveUSART2WifiCmdDelegate;
    USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
    MYDMA_Config(USART2_TX_DMA_Channel,(u32)&USART2->DR,(u32)USART2_DMA_SendBuff,ENABLE,USART2_SEND_DMA_BUF_LENTH);
    NVIC_IRQChannel_Configuration_Set(DMA1_Channel7_IRQn, 3, 1, ENABLE);
    DMA_ITConfig(USART2_TX_DMA_Channel,DMA_IT_TC,ENABLE);
}

void ChangeUSART2ReceiveMode()
{
    USART_ITConfig(USART2, USART_IT_IDLE, DISABLE);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    SetUSART2_NVIC_ISENABLE(ENABLE);
    USART2ReceiveHandler = ReceiveUSART2PacketDelegate;
}
/*
 * ��������itoa
 * ����  ������������ת�����ַ���
 * ����  ��-radix =10 ��ʾ10���ƣ��������Ϊ0
 *         -value Ҫת����������
 *         -buf ת������ַ���
 *         -radix = 10
 * ���  ����
 * ����  ����
 * ����  ����USART2_printf()����
 */
static char *itoa( int value, char *string, int radix )
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */


/*
 * ��������USART2_printf
 * ����  ����ʽ�������������C���е�printf��������û���õ�C��
 * ����  ��-USARTx ����ͨ��������ֻ�õ��˴���2����USART2
 *		     -Data   Ҫ���͵����ڵ����ݵ�ָ��
 *			   -...    ��������
 * ���  ����
 * ����  ���� 
 * ����  ���ⲿ����
 *         ����Ӧ��USART2_printf( USART2, "\r\n this is a demo \r\n" );
 *            		 USART2_printf( USART2, "\r\n %d \r\n", i );
 *            		 USART2_printf( USART2, "\r\n %s \r\n", j );
 */
void USART2_printf( USART_TypeDef* USARTx, char *Data, ... )
{
	const char *s;
  int d;   
  char buf[16];

  va_list ap;
  va_start(ap, Data);

	while ( *Data != 0)     // �ж��Ƿ񵽴��ַ���������
	{				                          
		if ( *Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //�س���
					USART_SendData(USARTx, 0x0d);
					Data ++;
					break;

				case 'n':							          //���з�
					USART_SendData(USARTx, 0x0a);	
					Data ++;
					break;
				
				default:
					Data ++;
				    break;
			}			 
		}
		else if ( *Data == '%')
		{									  //
			switch ( *++Data )
			{				
				case 's':										  //�ַ���
					s = va_arg(ap, const char *);
          for ( ; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
          }
					Data++;
          break;

        case 'd':										//ʮ����
          d = va_arg(ap, int);
          itoa(d, buf, 10);
          for (s = buf; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
          }
					Data++;
          break;
				 default:
						Data++;
				    break;
			}		 
		} /* end of else if */
		else USART_SendData(USARTx, *Data++);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
	}
}

void SetUSART2_NVIC_ISENABLE(FunctionalState isEnable)
{
    NVIC_IRQChannel_Configuration_Set(USART2_IRQn, 1, 0, isEnable);
}

void SendUSART2OneByte(uint8_t byteData)
{
	USART_ClearFlag(USART2,USART_FLAG_TC);//�����һ�·����жϱ�־λ��������һ���ֽڶ�ʧ�����⡣
	USART_SendData(USART2, byteData);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
}
void SendUSART2BytesBuf(uint8_t* bytesBuf, uint16_t bytesBufLength)
{
	while(bytesBufLength--)
	{
		SendUSART2OneByte(*(bytesBuf++));
	}
}
void USART2_DMA_Send_Data(uint8_t *USART_SendBuff, uint16_t DataSendLength)
{
    PushUSART2_DMA_SendDataIntoFIFO(USART_SendBuff, DataSendLength);
}


void ReceiveUSART2PacketDelegate(void)                	//�����жϷ������
{
	u8 receiveByte = 0;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
        USART_ClearITPendingBit(USART2,USART_IT_RXNE); //����жϱ�־
		receiveByte = USART_ReceiveData(USART2);//(USART1->DR);		//��ȡ���յ�������
//        sendUart1OneByte(receiveByte);
#ifdef __TERMINAL_ON__
        PushTianProtocolReceiveByteIntoFIFO(receiveByte);
#endif
	}
}

void ReceiveUSART2WifiCmdDelegate(void)
{
    char ch;
    //pStr = ESP8266_ReceiveString ( DISABLE );
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        ch  = USART_ReceiveData( USART2 );

        if( strEsp8266_Fram_Record .InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) )   //Ԥ��1���ֽ�д������
        {
            strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ++ ]  = ch;
        } 
    }

    if(USART_GetITStatus( USART2, USART_IT_IDLE ) == SET)    //����֡�������
    {
        strEsp8266_Fram_Record .InfBit .FramFinishFlag = 1;

        ch = USART_ReceiveData( USART2 ); //�������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR)
    }	
}

/**
  * @brief  This function handles USART2 Handler.
  * @param  None
  * @retval None
  */
void USART2_IRQHandler( void )
{	
    if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)//����ORE�жϵ���������Ҳ�жϵļ���
    {
        USART_ReceiveData(USART2);
    }
    USART2ReceiveHandler();   
}




/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/



