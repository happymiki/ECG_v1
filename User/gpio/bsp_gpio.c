/**
********************************************************

  ******************************************************************************
  */
  
#include "bsp_gpio.h"   


LightLamp lamps[1] = {
    
        1, 
        "Green", 
        0, 
        LED_Green_On,
        LED_Green_Off
    
};
 /**
  * @brief  ��ʼ��GPIO
  * @param  ��
  * @retval ��
  */
  
void LED_GPIO_Config(void)
{		
    GPIO_InitTypeDef GPIO_InitStructure;/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/

//    RCC_APB2PeriphClockCmd( LED_Red_RCC_Periph | LED_Green_RCC_Periph, ENABLE); /*����GPIOA������ʱ�� ����GPIOD������ʱ��*/

//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;/*��������ģʽΪͨ���������*/   
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; /*������������Ϊ50MHz */  	
//    
//    GPIO_InitStructure.GPIO_Pin = LED_Red_Pin;/*ѡ��Ҫ���Ƶ�GPIO����*/	
//    GPIO_Init(LED_Red_Port, &GPIO_InitStructure );/*���ÿ⺯������ʼ��GPIO*/

    GPIO_InitStructure.GPIO_Pin = LED_Green_Pin;
    GPIO_Init(LED_Green_Port, &GPIO_InitStructure);
    
    LED_Green_Off();
}

void Gate_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
    
    RCC_APB2PeriphClockCmd(Gate_RCC_Periph, ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;/*��������ģʽΪͨ���������*/   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; /*������������Ϊ50MHz */  
    
    GPIO_InitStructure.GPIO_Pin = Gate_Pin;
    GPIO_Init(Gate_Port, &GPIO_InitStructure);
    
    Gate_Off();
}

//void LED_Red_On(void)
//{
//    GPIO_ResetBits(LED_Red_Port, LED_Red_Pin);
//}

//void LED_Red_Off(void)
//{
//    GPIO_SetBits(LED_Red_Port, LED_Red_Pin);
//}

void LED_Green_On(void)
{
    GPIO_ResetBits(LED_Green_Port, LED_Green_Pin);
}

void LED_Green_Off(void)
{
    GPIO_SetBits(LED_Green_Port, LED_Green_Pin);
}

void Gate_On(void)
{
    GPIO_ResetBits(Gate_Port, Gate_Pin);
}

void Gate_Off(void)
{
    GPIO_SetBits(Gate_Port, Gate_Pin);
}

/*********************************************END OF FILE**********************/
