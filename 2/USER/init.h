/**
  ******************************************************************************
  * @file init.h 
  * @author RoboGame Sonny Team
  * @data 8/9
  * @version 1.0
  * @brief 进行所有初始化工作，包括：
  *        循迹模块初始化：GPIOB 0 - 11
  *        底盘电机控制初始化：GPIOF A相 0 - 3 B相 4(8) - 7
  *        底盘电机PWM初始化：GPIOC 6 - 9 PWM1000可调 定时器3
  *		   步进电机方向初始化：GPIOC 0 - 3
  *		   步进电机信号初始化：GPIOF 10 - 13
  *		   舵机信号初始化：GPIOA 6 - 7 GPIOB 14 - 15 TIME12 13 14 CHANNEL 1 TIME12 CHANNEL 2
  *        串口USART1初始化：GPIOA 9 TX 10 RX
  *        基本定时器初始化 :定时器2 1KHz 更新position矩阵值 
  *                       定时器4 50Hz 电机调控
  *                       定时器5 20Hz 更新定位点信息
  ******************************************************************************
**/
#ifndef INIT_H
#define INIT_H

#include "stm32f4xx.h"
#include "delay.h"
/*自定义的boolean类型*/
#define TRUE 1
#define FALSE 0
typedef u32 BOOLEAN;

/*串口USART1中断辅助变量*/
#define BOUND 19200
#define USART1_RX_LEN_MAX 20
extern u8 USART1_RX_BUF[USART1_RX_LEN_MAX];     //接收缓冲,最大USART1_RX_LEN_MAX个字节.
extern u8 USART1_RX_LEN;     //接收缓冲实际利用空间
extern u16 USART1_RX_STA;       //接收状态标记	

/*全部初始化*/
void init(void);

/*中断服务程序*/
void USART1_IRQHandler(void);

/*系统时钟函数 精度100ms*/
extern u32 system_time_ ;

#endif /* init.h */
