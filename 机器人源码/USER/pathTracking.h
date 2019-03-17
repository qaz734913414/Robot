/**
  ******************************************************************************
  * @file pathTracking.h
  * @author RoboGame Sonny Team
  * @data 11/9
  * @version 1.1
  * @brief ��ȡ��ǰλ����Ϣ 
  *        λ�þ��� MAT_POSITION
  *        λ�ã���ǰ��������
  *        POSITON 
  *        LOCATION ��1 δ����־��  0 �� 9 ��־��
  *
  ******************************************************************************
**/
/**
  ******************************************************************************
  * ѭ��������λ��˵����
  *                               ��ͷ
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
  *                               ��β
  ******************************************************************************
**/
/**
  *ѭ��˼·�� ֱ��ʱ������LINE1 �� LINE4 ��ֵ�ж��Ƿ�ƫ�뺽�ߣ���ƫ�룬ת��̶���ʱ�䣩������
  *         ����LINE4 && LINE1 ������
  *         ת��ʱ����ѭ��ȷ��ת�����
**/
#ifndef PATHTRACKING_H
#define PATHTRACKING_H

#include "stm32f4xx.h"
#include "init.h"
#include "motorContral.h"

extern BOOLEAN TURNING;
extern u32 POSITON;
extern u8 MAT_POSITION[12];
extern BOOLEAN TURN_RIGHT_SUCCESS; //ת�������־������ΪTRUE�����ת��
extern BOOLEAN TURN_LEFT_SUCCESS;
extern int LOCATION;
extern int LOCATION_BACKUP;


/*���������Ϣ�궨��*/
#define UNCHANGED 0
                     // ��Ϊ    ������� �������
#define L_RANGE_1 1  //��ƫ5��   LINE2   LINE7
#define L_RANGE_2 2  //��ƫ10��  LINE1   LINE6
#define L_TURN    4  //��ת      LINE0   LINE5
#define R_RANGE_1 8  //��ƫ5��   LINE3   LINE8
#define R_RANGE_2 16 //��ƫ10��  LINE4   LINE9
#define R_TURN    32 //��ת      LINE5   LINE0

/*����12�����ȡ��ǰ��·ѭ�����*/
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

/*����4�������RFID��־����Ϣ*/
#define RFID_1 (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)) //PA0
#define RFID_2 (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)) //PA1
#define RFID_3 (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)) //PA2
#define RFID_4 (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)) //PA3


/*�޸�λ�þ���*/
void setCurPositionMat_And_Position(void);
void setTurningRestart(void);
void testHadTuringSuccess(void);
void setLocationMat(void);

#endif /* pathTracking.h */
