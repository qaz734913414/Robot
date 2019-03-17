/**
  ******************************************************************************
  * @file motorContral.h 
  * @author RoboGame Sonny Team
  * @data 11/9
  * @version 1.1
  * @brief ������� 
  *     PWM1(L1)            PWM2(R1)
  *     PWM3(L2)            PWM4(R2)
  ******************************************************************************
**/
#ifndef MOTORCONTRAL_H
#define MOTORCONTRAL_H
#include "stm32f4xx.h"
#include "init.h"

typedef u32 VELOCITY;
typedef unsigned DIRECTION;
#define FRONT 1
#define BACK 0
#define STOP 2

/*�������*/
extern u8 TimeOfTurns;

/*�ֵ���ٶȣ���ʼ500��1000�ɵ�*/
extern VELOCITY PWM1_CUR;
extern VELOCITY PWM2_CUR;
extern VELOCITY PWM3_CUR;
extern VELOCITY PWM4_CUR;

/*�����ٶ�*/
extern VELOCITY V_CUR;

/*��������� �� FRONT �� �� BACK*/
extern DIRECTION motorDirection1;
extern DIRECTION motorDirection2;
extern DIRECTION motorDirection3;
extern DIRECTION motorDirection4;

/*����ǰ������*/
extern DIRECTION direction;
extern DIRECTION direction_backup;

/*������ƽӿڣ�SETΪ��תģʽ��RESETΪ��תģʽ���Գ�ͷΪ��*/
#define SET_DIR_L1 (GPIO_SetBits(GPIOF,GPIO_Pin_0),GPIO_ResetBits(GPIOF,GPIO_Pin_8))
#define SET_DIR_R1 (GPIO_SetBits(GPIOF,GPIO_Pin_1),GPIO_ResetBits(GPIOF,GPIO_Pin_5))
#define SET_DIR_L2 (GPIO_SetBits(GPIOF,GPIO_Pin_2),GPIO_ResetBits(GPIOF,GPIO_Pin_6))
#define SET_DIR_R2 (GPIO_SetBits(GPIOF,GPIO_Pin_3),GPIO_ResetBits(GPIOF,GPIO_Pin_7))
#define RESET_DIR_L1 (GPIO_ResetBits(GPIOF,GPIO_Pin_0),GPIO_SetBits(GPIOF,GPIO_Pin_8))
#define RESET_DIR_R1 (GPIO_ResetBits(GPIOF,GPIO_Pin_1),GPIO_SetBits(GPIOF,GPIO_Pin_5))
#define RESET_DIR_L2 (GPIO_ResetBits(GPIOF,GPIO_Pin_2),GPIO_SetBits(GPIOF,GPIO_Pin_6))
#define RESET_DIR_R2 (GPIO_ResetBits(GPIOF,GPIO_Pin_3),GPIO_SetBits(GPIOF,GPIO_Pin_7))
#define SHUTDOWN_L1 (GPIO_SetBits(GPIOF,GPIO_Pin_0),GPIO_SetBits(GPIOF,GPIO_Pin_8))
#define SHUTDOWN_R1 (GPIO_SetBits(GPIOF,GPIO_Pin_1),GPIO_SetBits(GPIOF,GPIO_Pin_5))
#define SHUTDOWN_L2 (GPIO_SetBits(GPIOF,GPIO_Pin_2),GPIO_SetBits(GPIOF,GPIO_Pin_6))
#define SHUTDOWN_R2 (GPIO_SetBits(GPIOF,GPIO_Pin_3),GPIO_SetBits(GPIOF,GPIO_Pin_7))

/*�����˶�*/
void run(void);
void turnRight(DIRECTION dir);
void turnLeft(DIRECTION dir);
void goStraight(DIRECTION dir);
void openMotor(void);
void stopMotor(void);
BOOLEAN isMOTOR(void);
void testArm(void);

#endif /* motorContral.h */
