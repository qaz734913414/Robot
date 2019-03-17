/**
  ******************************************************************************
  * @file init.h 
  * @author RoboGame Sonny Team
  * @data 8/9
  * @version 1.0
  * @brief �������г�ʼ��������������
  *        ѭ��ģ���ʼ����GPIOB 0 - 11
  *        ���̵�����Ƴ�ʼ����GPIOF A�� 0 - 3 B�� 4(8) - 7
  *        ���̵��PWM��ʼ����GPIOC 6 - 9 PWM1000�ɵ� ��ʱ��3
  *		   ������������ʼ����GPIOC 0 - 3
  *		   ��������źų�ʼ����GPIOF 10 - 13
  *		   ����źų�ʼ����GPIOA 6 - 7 GPIOB 14 - 15 TIME12 13 14 CHANNEL 1 TIME12 CHANNEL 2
  *        ����USART1��ʼ����GPIOA 9 TX 10 RX
  *        ������ʱ����ʼ�� :��ʱ��2 1KHz ����position����ֵ 
  *                       ��ʱ��4 50Hz �������
  *                       ��ʱ��5 20Hz ���¶�λ����Ϣ
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

/*ȫ����ʼ��*/
void init(void);

/*�жϷ������*/
void USART1_IRQHandler(void);

/*ϵͳʱ�Ӻ��� ����100ms*/
extern u32 system_time_ ;

#endif /* init.h */
