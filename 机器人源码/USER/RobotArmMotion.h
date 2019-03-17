/**
  ******************************************************************************
  * @file RobotArmMotion.h
  * @author RoboGame Sonny Team
  * @data 11/9
  * @version 1.0
  * @brief 机械臂运动
	* @note 依赖文件  "helper.h" "leadscrew.h" "stepperMotor.h" "init.h"
  ******************************************************************************
**/

#ifndef ROBOTARMMOTION_H
#define ROBOTARMMOTION_H

#include "stm32f4xx.h"
#include "leadscrew.h"
#include "stepperMotor.h"
#include "init.h"
#include "motorContral.h"

//全局变量,表示小车就位,可以插书,读到地上的RFID时中断服务程序改变其值
extern u8 ready_to_insert;

typedef u8 BOOKNUMBER;//书本序号，对应书架从左至右5个位置，取值0~4
typedef u8 LAYER;//书架层数，取值2~4
#define YES 1
#define NO 0

#define INCLINE_ANG 78 //3号舵机倾斜度数
#define GRAB_ANG 95//4号舵机抓书度数
#define LOOSE_ANG 74//4号舵机松开度数

void SERVO(ANGLE ang);

//把第n本书放到第m层
void Put(BOOKNUMBER n,LAYER m);

//从初始状态开始去抓第n本书
void Fetch(BOOKNUMBER n);


#endif
