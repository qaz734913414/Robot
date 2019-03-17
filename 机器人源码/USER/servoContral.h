/**
  ******************************************************************************
  * @file servoContral.h
  * @author RoboGame Sonny Team
  * @data 28/7
  * @version 1.11
  * @brief ¶æ»ú¿ØÖÆ 
  *
  ******************************************************************************
**/
#ifndef SERVOCONTRAL_H
#define SERVOCONTRAL_H
#include "stm32f4xx.h"

#define MAX_SERVO 180
void setServo1(u32 ang);
void setServo2(u32 ang);
void setServo3(u32 ang);
void setServo4(u32 ang);
#endif /* servoContral.h */
