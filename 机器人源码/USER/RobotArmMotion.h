/**
  ******************************************************************************
  * @file RobotArmMotion.h
  * @author RoboGame Sonny Team
  * @data 11/9
  * @version 1.0
  * @brief ��е���˶�
	* @note �����ļ�  "helper.h" "leadscrew.h" "stepperMotor.h" "init.h"
  ******************************************************************************
**/

#ifndef ROBOTARMMOTION_H
#define ROBOTARMMOTION_H

#include "stm32f4xx.h"
#include "leadscrew.h"
#include "stepperMotor.h"
#include "init.h"
#include "motorContral.h"

//ȫ�ֱ���,��ʾС����λ,���Բ���,�������ϵ�RFIDʱ�жϷ������ı���ֵ
extern u8 ready_to_insert;

typedef u8 BOOKNUMBER;//�鱾��ţ���Ӧ��ܴ�������5��λ�ã�ȡֵ0~4
typedef u8 LAYER;//��ܲ�����ȡֵ2~4
#define YES 1
#define NO 0

#define INCLINE_ANG 78 //3�Ŷ����б����
#define GRAB_ANG 95//4�Ŷ��ץ�����
#define LOOSE_ANG 74//4�Ŷ���ɿ�����

void SERVO(ANGLE ang);

//�ѵ�n����ŵ���m��
void Put(BOOKNUMBER n,LAYER m);

//�ӳ�ʼ״̬��ʼȥץ��n����
void Fetch(BOOKNUMBER n);


#endif
