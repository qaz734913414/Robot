/**
  ******************************************************************************
  * @file leadscrew.h
  * @author RoboGame Sonny Team
  * @data 7/8
  * @version 1.11
  * @brief ˿�ܿ���
  *        �������� 1.8
	*        ����   ����C2 ����F12
  ******************************************************************************
**/

#ifndef LEADSCREW_H
#define LEADSCREW_H

#include "stm32f4xx.h"

typedef u32 LENGTH;
typedef unsigned LS_DIRECTION;
//LS_FRONTΪ���ң�LS_BACK����
#define  LS_LEFT 1
#define  LS_RIGHT 0
#define DIS_PER_RND 4 //����ÿתһȦ˿���ƶ�����mm



//˿������dir�ƶ�����len
void set_leadscrew(LENGTH len,LS_DIRECTION dir);


#endif /* leadscrew.h */
