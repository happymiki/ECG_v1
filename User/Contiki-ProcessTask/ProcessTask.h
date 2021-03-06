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
#include "ILI93xx.h"
#include "filter.h"
#include "waveform.h"
#include "key.h"

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


//在"CommunicationConfig.h"文件中修改 __TERMINAL_XX__宏定义，选择节点或终端模式
#ifdef __TERMINAL_ON__
//    #define __LED_BLINK_ON__            //LED 闪烁
    #define __KEY_MODULE_ON__            // 独立按键扫描
    #define __WIFI_MODULE_ON__          //WIFI模块开启
//    #define __W5500_MODULE_ON__         //W5500模块开启
//    #define __E30TTLUART_MODULE_ON__    //E30无线串口模块
		#define __LCD_MODULE_ON__    				//LCD显示模块
		#define __ECG_MODULE_ON__						// 发送ECG数据到服务器
    #define __COMMUNICAT_PROTOCOL__     //管理发送队列
    #define __COMMUNICAT_PROTOCOL_SENSOR_DATA__  //通过JSON发送所有数据
#else
    #ifdef __TERMINAL_OFF__
    #define __COMMUNICAT_PROTOCOL__     //管理发送队列
    #define __COMMUNICAT_PROTOCOL_SENSOR_DATA__  //通过JSON发送所有数据
    #define __E30TTLUART_MODULE_ON__    //E30无线串口模块
    #endif
#endif

//    #define __CLOCK_TICK_TEST__         //NOP延时时间测试
//    #define __W5500_SEND_TEST_ON__      //W5500模块发送测试开启

PROCESS_NAME(red_blink_process);
PROCESS_NAME(green_blink_process);
PROCESS_NAME(IWDG_Feed_process);
PROCESS_NAME(Communication_Protocol_Load_process);
PROCESS_NAME(Communication_Protocol_Send_process);
PROCESS_NAME(clock_test_process);
PROCESS_NAME(cJSON_test_process);
PROCESS_NAME(CommunicatProtocol_Send_Sensor_Data);
PROCESS_NAME(LCD_display_waveform_process);
PROCESS_NAME(KEY_Scan_process);
PROCESS_NAME(ECG_send_data_process);

PROCESS_NAME(wifi_send_test_process);
PROCESS_NAME(W5500_send_test_process);


#endif

