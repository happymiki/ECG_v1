/**
********************************************************

  ******************************************************************************
  */
  
#include "PWM.h"   


void TIM1_PWM_Config(uint16_t TIM_Period,uint16_t TIM_Prescaler)//�Զ���װ����ֵ  Ԥ��Ƶֵ(��1������ʵֵ)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	//��ʱ����������
	TIM_TimeBaseStructure.TIM_Period = TIM_Period; 				//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler = TIM_Prescaler; 				//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 			//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 		//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	TIM_Cmd(TIM1, ENABLE);  								//ʹ��TIM1
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); 					//ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);						//MOE �����ʹ��	
}
void TIM1_PWM_Init(void)
{
	TIM1_PWM_Config(1000,36-1);
}
void TIM1_CH1_PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);   //ʹ��GPIO����ʱ��ʹ��
	//���ø�����Ϊ�����������,���TIM1 CH1��PWM���岨��
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  		//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 				//TIM_CH1
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
	//�Ƚϲ���Ĵ������� ����TIM1_CH1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 		//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; 						//���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  				//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  		//CH1Ԥװ��ʹ��	
}

void TIM1_CH4_PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);   //ʹ��GPIO����ʱ��ʹ��
	//���ø�����Ϊ�����������,���TIM1 CH1��PWM���岨��
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  		//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 				//TIM_CH1
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
	//�Ƚϲ���Ĵ������� ����TIM1_CH1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 		//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; 						//���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  		//CH4Ԥװ��ʹ��	
}

void Set_TIM1_CH1_Percentage(uint8_t percentage)
{
	TIM_SetCompare1(TIM1,1000 * percentage / 100);
}

void Set_TIM1_CH4_Percentage(uint8_t percentage)
{
	TIM_SetCompare4(TIM1,1000 * percentage / 100);
}
/*********************************************END OF FILE**********************/
