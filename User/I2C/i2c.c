#include "i2c.h"
#include "delay.h"  

//��ʼ��IIC
void I2C_Analog_Port_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(I2C_ANALOG_SDA_RCC_Periph | I2C_ANALOG_SCL_RCC_Periph, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = I2C_ANALOG_SDA_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_ANALOG_SDA_GPIO_Port, &GPIO_InitStructure);
 
    GPIO_InitStructure.GPIO_Pin = I2C_ANALOG_SCL_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_ANALOG_SCL_GPIO_Port, &GPIO_InitStructure);
    
	I2C_SCL=1;
	I2C_SDA=1;

}
void SDA_OUT(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = I2C_ANALOG_SDA_GPIO_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //��©���
  GPIO_Init(I2C_ANALOG_SDA_GPIO_Port,&GPIO_InitStructure);
}
void SDA_IN(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = I2C_ANALOG_SDA_GPIO_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(I2C_ANALOG_SDA_GPIO_Port,&GPIO_InitStructure);
}
//����IIC��ʼ�ź�
void I2C_Start(void)
{
	SDA_OUT();     //sda�����
	I2C_SDA=1;	  	  
	I2C_SCL=1;
	Delay_NOP_us(4);
 	I2C_SDA=0;//START:when CLK is high,DATA change form high to low 
	Delay_NOP_us(4);
	I2C_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void I2C_Stop(void)
{
	SDA_OUT();//sda�����
	I2C_SCL=0;
	I2C_SDA=0;//STOP:when CLK is high DATA change form low to high
 	Delay_NOP_us(4);
	I2C_SCL=1; 
	I2C_SDA=1;//����I2C���߽����ź�
	Delay_NOP_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t I2C_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	I2C_SDA=1;Delay_NOP_us(1);	   
	I2C_SCL=1;Delay_NOP_us(1);
	//
	SDA_IN();      //SDA����Ϊ����	 
	while(SDA_Read)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			I2C_Stop();
			return 1;
		}
	}
	I2C_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void I2C_Ack(void)
{
	I2C_SCL=0;
	SDA_OUT();
	I2C_SDA=0;
	Delay_NOP_us(2);
	I2C_SCL=1;
	Delay_NOP_us(2);
	I2C_SCL=0;
}
//������ACKӦ��		    
void I2C_NAck(void)
{
	I2C_SCL=0;
	SDA_OUT();
	I2C_SDA=1;
	Delay_NOP_us(2);
	I2C_SCL=1;
	Delay_NOP_us(2);
	I2C_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void I2C_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	SDA_OUT(); 	    
    I2C_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        I2C_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		Delay_NOP_us(2);   //��TEA5767��������ʱ���Ǳ����
		I2C_SCL=1;
		Delay_NOP_us(2); 
		I2C_SCL=0;	
		Delay_NOP_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t I2C_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        I2C_SCL=0; 
        Delay_NOP_us(2);
		I2C_SCL=1;
        receive<<=1;
        if(SDA_Read)receive++;   
		Delay_NOP_us(1); 
    }					 
    if (!ack)
        I2C_NAck();//����nACK
    else
        I2C_Ack(); //����ACK   
    return receive;
}


