#ifndef __i2c_H
#define __i2c_H
#include "sys.h" 	    	   		  

#define I2C_ANALOG_SDA_GPIO_Port    GPIOC
#define I2C_ANALOG_SDA_GPIO_Pin     GPIO_Pin_4
#define I2C_ANALOG_SDA_RCC_Periph   RCC_APB2Periph_GPIOC
#define I2C_ANALOG_SCL_GPIO_Port    GPIOC
#define I2C_ANALOG_SCL_GPIO_Pin     GPIO_Pin_5
#define I2C_ANALOG_SCL_RCC_Periph   RCC_APB2Periph_GPIOC
//IO��������	 
#define I2C_SCL    PCout(5) //SCL
#define I2C_SDA    PCout(4) //SDA
#define SDA_Read   PCin(4)  //����SDA

//IIC���в�������
void I2C_Analog_Port_Init(void);                //��ʼ��IIC��IO��				 
void I2C_Start(void);				//����IIC��ʼ�ź� �ɹ��򷵻�1
void I2C_Stop(void);	  			//����IICֹͣ�ź�
void I2C_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t I2C_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
uint8_t I2C_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void I2C_Ack(void);					//IIC����ACK�ź�
void I2C_NAck(void);				//IIC������ACK�ź�
	  
#endif

