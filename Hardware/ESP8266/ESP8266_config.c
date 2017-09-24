/**
  ******************************************************************************
  * @file    wifi_config.c
  * @author  fire
  * @version V1.0
  * @date    2014-xx-xx
  * @brief   WiFiģ��ӿ���������
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� ISO-STM32 ������
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
	*/

#include "ESP8266_config.h"
#include "bsp_SysTick.h"
#include "bsp_gpio.h"
#include "bsp_usart1.h"
#include "bsp_usart2.h"
#include "SPI.h"

struct  STRUCT_USARTx_Fram strEsp8266_Fram_Record = { 0 };
struct  STRUCT_USART1_1_Fram str1_1esp8266 = {0} ;


/**
  * @brief  WiFi_Config wifi ��ʼ��
  * @param  ��
  * @retval ��
  */
void WiFi_Config( void )
{
    USART2_Config(115200);
	WiFi_RST_INIT();
}

void ESP8266_WIFI_GPIO_Config( void )
{		
    /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /*����GPIOA������ʱ��*/
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE ); 

    /* ����WiFiģ���Ƭѡ��CH������	��λ������RST������*/
    /*ѡ��Ҫ���Ƶ�GPIOA0��CH�����ź�GPIOA1��RST������*/															   
    GPIO_InitStructure.GPIO_Pin = WIFI_RST_GPIO_Pin;	

    /*��������ģʽΪͨ���������*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

    /*������������Ϊ50MHz */   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

    /*���ÿ⺯������ʼ��GPIOA*/
    GPIO_Init( WIFI_RST_GPIO_Port, &GPIO_InitStructure );	 

//    /*����WiFiģ��ĸ�λ��������	*/
    GPIO_SetBits( WIFI_RST_GPIO_Port, WIFI_RST_GPIO_Pin );	
}

/*********************************************************end of file**************************************************/
