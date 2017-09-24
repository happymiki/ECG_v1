#ifndef __PROCESS_TASK_H__
#define __PROCESS_TASK_H__
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "adc.h"
#include "bsp_gpio.h"
#include "bsp_usart1.h"
#include "bsp_usart2.h"
#include "bsp_usart3.h"
#include "bsp_uart4.h"
#include "bsp_uart5.h"
#include "bsp_SysTick.h"
#include "dma.h"
#include "iwdg.h"
#include "SPI.h"
#include "delay.h"
#include "timers.h"
#include "timer4_cap.h"
#include "NVIC_CONFIG.H"
#include "cJSON.h"
#include "stm_flash.h"
#include "PWM.h" 
#include "CommunicationProtocol.h"
#include "CommunicationConfig.h"

#include "ESP8266_config.h"
#include "ESP8266_function.h"
#include "E30TTLUART.h"
#include "W5500.h"

#include "contiki-conf.h"
#include <stdint.h>
#include <stdio.h>
#include <debug-uart.h>
#include <process.h>
#include <procinit.h>
#include <etimer.h>
#include <autostart.h>
#include <clock.h>
#include "contiki_delay.h"


//��"CommunicationConfig.h"�ļ����޸� __TERMINAL_XX__�궨�壬ѡ��ڵ���ն�ģʽ
#ifdef __TERMINAL_ON__
//    #define __LED_BLINK_ON__            //LED ��˸
    #define __WIFI_MODULE_ON__          //WIFIģ�鿪��
//    #define __W5500_MODULE_ON__         //W5500ģ�鿪��
    #define __E30TTLUART_MODULE_ON__    //E30���ߴ���ģ��
    #define __COMMUNICAT_PROTOCOL__     //�����Ͷ���
    #define __COMMUNICAT_PROTOCOL_SENSOR_DATA__  //ͨ��JSON������������
#else
    #ifdef __TERMINAL_OFF__
    #define __COMMUNICAT_PROTOCOL__     //�����Ͷ���
    #define __COMMUNICAT_PROTOCOL_SENSOR_DATA__  //ͨ��JSON������������
    #define __E30TTLUART_MODULE_ON__    //E30���ߴ���ģ��
    #endif
#endif

//    #define __CLOCK_TICK_TEST__         //NOP��ʱʱ�����
//    #define __W5500_SEND_TEST_ON__      //W5500ģ�鷢�Ͳ��Կ���

PROCESS_NAME(red_blink_process);
PROCESS_NAME(green_blink_process);
PROCESS_NAME(IWDG_Feed_process);
PROCESS_NAME(Communication_Protocol_Load_process);
PROCESS_NAME(Communication_Protocol_Send_process);
PROCESS_NAME(clock_test_process);
PROCESS_NAME(cJSON_test_process);
PROCESS_NAME(CommunicatProtocol_Send_Sensor_Data);

PROCESS_NAME(wifi_send_test_process);
PROCESS_NAME(W5500_send_test_process);

#endif

