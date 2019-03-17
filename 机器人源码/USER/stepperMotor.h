/**
  ******************************************************************************
  * @file stepperMotor.h
  * @author RoboGame Sonny Team
  * @data 23/7
  * @version 1.11
  * @brief 步进电机控制算法
  *        电机步距角 1.8 / 20
  * 
  ******************************************************************************
**/
#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H

#include "stm32f4xx.h"

typedef u32 ANGLE;
typedef unsigned S_DIRECTION;
#define  S_FRONT 1
#define  S_BACK 0

void setS_Motor1(ANGLE ang, S_DIRECTION dir);
void setS_Motor2(ANGLE ang, S_DIRECTION dir);
void setS_Motor3(ANGLE ang, S_DIRECTION dir);
void setS_Motor4(ANGLE ang, S_DIRECTION dir);
void setS_Motor(ANGLE ang,S_DIRECTION dir);
#endif /* stepperMotor.h */
