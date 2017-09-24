#ifndef __STM_FLASH_H__
#define __STM_FLASH_H__
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include <stdint.h>
 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK miniSTM32������
//STM32 FLASH ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/13
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 256 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1              //ʹ��FLASHд��(0��������;1��ʹ��)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ
#define STM32_FLASH_END_PAGE (STM32_FLASH_BASE + ((STM32_FLASH_SIZE - 1)<<10))
 

uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);		  //��������
void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read(uint32_t ReadAddr ,uint16_t *pBuffer,uint16_t NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����
void STMFLASH_Write_HalfWordData(uint32_t WriteAddr,uint16_t WriteData);
uint16_t STMFLASH_Read_HalfWordData(uint32_t WriteAddr);
void STMFLASH_Write_OneWordData(uint32_t WriteAddr,uint32_t WriteData);
uint32_t STMFLASH_Read_OneWordData(uint32_t WriteAddr);
#endif

















