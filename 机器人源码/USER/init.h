/**
  ******************************************************************************
  * @file init.h 
  * @author RoboGame Sonny Team
  * @data 11/9
  * @version 1.1
  * @brief 进行所有初始化工作，包括：
  *        循迹模块初始化：GPIOB 0 - 11
  *        底盘电机控制初始化：GPIOF A相 0 - 3 B相 4(8) - 7
  *        底盘电机PWM初始化：GPIOC 6 - 9 PWM1000可调 定时器3
  *		     步进电机方向初始化：GPIOC 0 - 3
  *		     步进电机信号初始化：GPIOF 10 - 13
  *		     舵机信号初始化：GPIOA 6 - 7 GPIOB 14 - 15 TIME12 13 14 CHANNEL 1 TIME12 CHANNEL 2
  *        串口USART1初始化：GPIOA 9 TX 10 RX
  *        基本定时器初始化 :定时器2 1KHz 更新position矩阵值 
  *                       定时器4 50Hz 电机调控
  *                       定时器5 20Hz 管理转弯结束
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

/*标签用辅助变量*/
extern u8 rightTagOrder[10];//标签的正确顺序
extern u8 nowTagNum ;//当前已读取到的标签数

/*PWM1~4 设置新的PWM值*/
#define PWM1(PWM1_VAL) (PWM1_CUR = PWM1_VAL, TIM_SetCompare1(TIM3, (PWM1_VAL)))
#define PWM2(PWM2_VAL) (PWM2_CUR = PWM2_VAL, TIM_SetCompare2(TIM3, (PWM2_VAL)))
#define PWM3(PWM3_VAL) (PWM3_CUR = PWM3_VAL, TIM_SetCompare3(TIM3, (PWM3_VAL)))
#define PWM4(PWM4_VAL) (PWM4_CUR = PWM4_VAL, TIM_SetCompare4(TIM3, (PWM4_VAL)))
#define SERVO1(SERVO1_VAL) (TIM_SetCompare1(TIM13, (SERVO1_VAL)))
#define SERVO2(SERVO2_VAL) (TIM_SetCompare1(TIM14, (SERVO2_VAL)))
#define SERVO3(SERVO3_VAL) (TIM_SetCompare1(TIM12, (SERVO3_VAL)))
#define SERVO4(SERVO4_VAL) (TIM_SetCompare2(TIM12, (SERVO4_VAL)))

/*全部初始化*/
void init(void);
/*预初始化*/
void preinit(void);

/*中断服务程序*/
void TIM4_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM5_IRQHandler(void);
void USART1_IRQHandler(void);
void EXTI0_IRQHandler(void);

#endif /* init.h */
