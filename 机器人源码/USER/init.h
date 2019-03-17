/**
  ******************************************************************************
  * @file init.h 
  * @author RoboGame Sonny Team
  * @data 11/9
  * @version 1.1
  * @brief �������г�ʼ��������������
  *        ѭ��ģ���ʼ����GPIOB 0 - 11
  *        ���̵�����Ƴ�ʼ����GPIOF A�� 0 - 3 B�� 4(8) - 7
  *        ���̵��PWM��ʼ����GPIOC 6 - 9 PWM1000�ɵ� ��ʱ��3
  *		     ������������ʼ����GPIOC 0 - 3
  *		     ��������źų�ʼ����GPIOF 10 - 13
  *		     ����źų�ʼ����GPIOA 6 - 7 GPIOB 14 - 15 TIME12 13 14 CHANNEL 1 TIME12 CHANNEL 2
  *        ����USART1��ʼ����GPIOA 9 TX 10 RX
  *        ������ʱ����ʼ�� :��ʱ��2 1KHz ����position����ֵ 
  *                       ��ʱ��4 50Hz �������
  *                       ��ʱ��5 20Hz ����ת�����
  ******************************************************************************
**/
#ifndef INIT_H
#define INIT_H

#include "stm32f4xx.h"
#include "delay.h"
/*�Զ����boolean����*/
#define TRUE 1
#define FALSE 0
typedef u32 BOOLEAN;

/*����USART1�жϸ�������*/
#define BOUND 19200
#define USART1_RX_LEN_MAX 20
extern u8 USART1_RX_BUF[USART1_RX_LEN_MAX];     //���ջ���,���USART1_RX_LEN_MAX���ֽ�.
extern u8 USART1_RX_LEN;     //���ջ���ʵ�����ÿռ�
extern u16 USART1_RX_STA;       //����״̬���	

/*��ǩ�ø�������*/
extern u8 rightTagOrder[10];//��ǩ����ȷ˳��
extern u8 nowTagNum ;//��ǰ�Ѷ�ȡ���ı�ǩ��

/*PWM1~4 �����µ�PWMֵ*/
#define PWM1(PWM1_VAL) (PWM1_CUR = PWM1_VAL, TIM_SetCompare1(TIM3, (PWM1_VAL)))
#define PWM2(PWM2_VAL) (PWM2_CUR = PWM2_VAL, TIM_SetCompare2(TIM3, (PWM2_VAL)))
#define PWM3(PWM3_VAL) (PWM3_CUR = PWM3_VAL, TIM_SetCompare3(TIM3, (PWM3_VAL)))
#define PWM4(PWM4_VAL) (PWM4_CUR = PWM4_VAL, TIM_SetCompare4(TIM3, (PWM4_VAL)))
#define SERVO1(SERVO1_VAL) (TIM_SetCompare1(TIM13, (SERVO1_VAL)))
#define SERVO2(SERVO2_VAL) (TIM_SetCompare1(TIM14, (SERVO2_VAL)))
#define SERVO3(SERVO3_VAL) (TIM_SetCompare1(TIM12, (SERVO3_VAL)))
#define SERVO4(SERVO4_VAL) (TIM_SetCompare2(TIM12, (SERVO4_VAL)))

/*ȫ����ʼ��*/
void init(void);
/*Ԥ��ʼ��*/
void preinit(void);

/*�жϷ������*/
void TIM4_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM5_IRQHandler(void);
void USART1_IRQHandler(void);
void EXTI0_IRQHandler(void);

#endif /* init.h */
