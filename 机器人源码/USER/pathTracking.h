/**
  ******************************************************************************
  * @file pathTracking.h
  * @author RoboGame Sonny Team
  * @data 11/9
  * @version 1.1
  * @brief 读取当前位置信息 
  *        位置矩阵 MAT_POSITION
  *        位置：当前动作命令
  *        POSITON 
  *        LOCATION －1 未到标志点  0 － 9 标志点
  *
  ******************************************************************************
**/
/**
  ******************************************************************************
  * 循迹传感器位置说明：
  *                               车头
  *       
  *
  *
  *              |LINE1|   |LINE2|   |LINE3|   |LINE4|      
  * |LINE0|                                                 |LINE5|
  *              |LINE9|   |LINE8|   |LINE7|   |LINE6|      
  *
  *
  *
  *     
  *                               车尾
  ******************************************************************************
**/
/**
  *循迹思路： 直行时，根据LINE1 ～ LINE4 的值判断是否偏离航线，若偏离，转向固定（时间），优先
  *         考察LINE4 && LINE1 （正向）
  *         转弯时，依循迹确定转弯结束
**/
#ifndef PATHTRACKING_H
#define PATHTRACKING_H

#include "stm32f4xx.h"
#include "init.h"
#include "motorContral.h"

extern BOOLEAN TURNING;
extern u32 POSITON;
extern u8 MAT_POSITION[12];
extern BOOLEAN TURN_RIGHT_SUCCESS; //转弯结束标志量，若为TRUE则结束转弯
extern BOOLEAN TURN_LEFT_SUCCESS;
extern int LOCATION;
extern int LOCATION_BACKUP;


/*方向控制信息宏定义*/
#define UNCHANGED 0
                     // 行为    正向情况 反向情况
#define L_RANGE_1 1  //左偏5度   LINE2   LINE7
#define L_RANGE_2 2  //左偏10度  LINE1   LINE6
#define L_TURN    4  //左转      LINE0   LINE5
#define R_RANGE_1 8  //右偏5度   LINE3   LINE8
#define R_RANGE_2 16 //右偏10度  LINE4   LINE9
#define R_TURN    32 //右转      LINE5   LINE0

/*以下12个宏读取当前线路循迹结果*/
#define LINE1_1 (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)) //PB0
#define LINE1_2 (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)) //PB1
#define LINE1_3 (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)) //PB2
#define LINE1_4 (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)) //PB3
#define LINE2_1 (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)) //PB4
#define LINE2_2 (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)) //PB5
#define LINE2_3 (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)) //PB6
#define LINE2_4 (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)) //PB7
#define LINE3_1 (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)) //PB8
#define LINE3_2 (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)) //PB9
#define LINE3_3 (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)) //PB10
#define LINE3_4 (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)) //PB11

/*以下4个宏读区RFID标志点信息*/
#define RFID_1 (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)) //PA0
#define RFID_2 (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)) //PA1
#define RFID_3 (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)) //PA2
#define RFID_4 (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)) //PA3


/*修改位置矩阵*/
void setCurPositionMat_And_Position(void);
void setTurningRestart(void);
void testHadTuringSuccess(void);
void setLocationMat(void);

#endif /* pathTracking.h */
