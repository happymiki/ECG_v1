#include <stdarg.h>
#include "SPI.h"
#include "ESP8266_config.h"


/**
*@brief SPI Initialization
**/
void SPI1_Init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA, ENABLE);

    /* NSS---->GPIO(LED) */
    SPI_SSOutputCmd(SPI1, ENABLE);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    /* SPI1 Config -------------------------------------------------------------*/
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//ȫ˫��ͨ��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//������
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8λ����֡
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//����ʱΪ�͵�ƽ
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//�����ڵ�1�������ر��ɼ�
    SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;//CS��Ϊ���ģʽ
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//64��Ƶ
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//���ֽ���ǰ
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);


    NVIC_IRQChannel_Configuration_Set(SPI1_IRQn, 3, 1, ENABLE);
    SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_TXE,DISABLE);
    SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_ERR,DISABLE);
    SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_RXNE,DISABLE);


    /* Enable SPI1 */
    SPI_Cmd(SPI1, ENABLE);
    SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_TXE);
    SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_RXNE);

}


/**
*@brief SPI Initialization
**/
void SPI2_Init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    SPI_SSOutputCmd(SPI2, ENABLE);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//ȫ˫��ͨ��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//������
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8λ����֡
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//����ʱΪ�͵�ƽ
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//�����ڵ�1�������ر��ɼ�
    SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;//CS��Ϊ���ģʽ
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;//16��Ƶ
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//���ֽ���ǰ
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);

//    NVIC_IRQChannel_Configuration_Set(SPI2_IRQn, 2, 0, ENABLE);
//    SPI_I2S_ITConfig(SPI2,SPI_I2S_IT_TXE,DISABLE);
//    SPI_I2S_ITConfig(SPI2,SPI_I2S_IT_RXNE,ENABLE);


    /* Enable SPI2 */
    SPI_Cmd(SPI2, ENABLE);
    SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_IT_TXE);
    SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_IT_RXNE);
}


/*******************************************************************************
* Function Name  : SPI_FLASH_Init
* Description    : Initializes the peripherals used by the SPI FLASH driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void SPI3_Init(void)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	SPI_InitTypeDef   	SPI_InitStructure;	   

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);	
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
    
	/* ��ʼ��SCK��MISO��MOSI���� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);

	/* ��ʼ��CS���� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_15);

	/* ��ʼ������STM32 SPI */
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;	//SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;							//����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;						//SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low;							//ʱ�����յ�
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;							//���ݲ����ڵ�1��ʱ����
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;								//NSS���ⲿ�ܽŹ���
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;	//������Ԥ��ƵֵΪ2
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;					//���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial=7;								//CRC����ʽΪ7
	SPI_Init(SPI3,&SPI_InitStructure);									//����SPI_InitStruct��ָ���Ĳ�����ʼ������SPI�Ĵ���

	SPI_Cmd(SPI3,ENABLE);	//STM32ʹ��SPI
}

void SPI3_Send_Byte(uint8_t dat)
{
	SPI_I2S_SendData(SPI3,dat);//д1���ֽ�����
	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);//�ȴ����ݼĴ�����
}

uint16_t SPI3_Receive_Data()
{
    return SPI_I2S_ReceiveData(SPI3);
}




uint8_t SPI1_WriteReadOneByte(uint8_t byteData)
{
    int count = 0;
    int COUNT_MAX = 500;
    
     /* �жϷ��ͻ����Ƿ�Ϊ�� */ 
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
    {
        count++;
        if(count>COUNT_MAX)
        {
            count = 0;
            return 0;
        }
    }
    /* �����ֽ� */
    SPI_I2S_SendData(SPI1, byteData);  
    /* �жϽ��ջ����Ƿ�Ϊ��*/
    count = 0;
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
    {
        count++;
        if(count>COUNT_MAX)
        {
            count = 0;
            return 0;
        }
    }
    /* flush data read during the write */ 
    return SPI_I2S_ReceiveData(SPI1); 
}


uint8_t SPI2_WriteReadOneByte(uint8_t byteData)
{
    int count = 0;
    int COUNT_MAX = 500;
    
     /* �жϷ��ͻ����Ƿ�Ϊ�� */ 
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
    {
        count++;
        if(count>COUNT_MAX)
        {
            count = 0;
            return 0;
        }
    }
    /* �����ֽ� */
    SPI_I2S_SendData(SPI2, byteData);  
    /* �жϽ��ջ����Ƿ�Ϊ��*/
    count = 0;
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
    {
        count++;
        if(count>COUNT_MAX)
        {
            count = 0;
            return 0;
        }
    }
    /* flush data read during the write */ 
    return SPI_I2S_ReceiveData(SPI2); 
}

uint8_t SPI3_WriteReadOneByte(uint8_t byteData)
{
    int count = 0;
    int COUNT_MAX = 500;
    
     /* �жϷ��ͻ����Ƿ�Ϊ�� */ 
    while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET)
    {
        count++;
        if(count>COUNT_MAX)
        {
            count = 0;
            return 0;
        }
    }
    /* �����ֽ� */
    SPI_I2S_SendData(SPI3, byteData);
    /* �жϽ��ջ����Ƿ�Ϊ��*/
    count = 0;
    while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET)
    {
        count++;
        if(count>COUNT_MAX)
        {
            count = 0;
            return 0;
        }
    }
    /* flush data read during the write */ 
    return SPI_I2S_ReceiveData(SPI3); 
}


/**
* @brief This function handles SPI1 global interrupt.
*/
void SPI1_IRQHandler(void)
{
    uint8_t SPI_Value=0;
    if( SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_TXE) == SET )
    {
        SPI_I2S_SendData(SPI1,0xFF);      
    }
    if( SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) == SET )   
    {     
       SPI_Value = SPI_I2S_ReceiveData(SPI1);    
    }
    SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_TXE);
    SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_RXNE);
}





void SPI2_IRQHandler(void)
{   
    uint8_t SPI_Value=0;
    if( SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_TXE) == SET )
    {
        SPI_I2S_SendData(SPI2,0xFF);      
    }
    if( SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_RXNE) == SET )   
    {     
       SPI_Value = SPI_I2S_ReceiveData(SPI2);    
    }
    SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_IT_TXE);
    SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_IT_RXNE);
}



