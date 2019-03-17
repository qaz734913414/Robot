/**
  ******************************************************************************
  * @file leadscrew.h
  * @author RoboGame Sonny Team
  * @data 7/8
  * @version 1.11
  * @brief 丝杠控制
  *        电机步距角 1.8
	*        引脚   方向C2 脉冲F12
  ******************************************************************************
**/

#ifndef LEADSCREW_H
#define LEADSCREW_H

#include "stm32f4xx.h"

typedef u32 LENGTH;
typedef unsigned LS_DIRECTION;
//LS_FRONT为向右，LS_BACK向左
#define  LS_LEFT 1
#define  LS_RIGHT 0
#define DIS_PER_RND 4 //步进每转一圈丝杠移动距离mm



//丝杠向方向dir移动距离len
void set_leadscrew(LENGTH len,LS_DIRECTION dir);


#endif /* leadscrew.h */
