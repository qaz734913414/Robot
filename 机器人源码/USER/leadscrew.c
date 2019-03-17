/**
  ******************************************************************************
  * @file leadscrew.c
  * @author RoboGame Sonny Team
  * @data 7/8
  * @version 1.11
  * @brief ˿�ܿ���
  *        �������� 1.8
  * @note: �����ļ�  "helper.h" "leadscrew.h" 
	*        ����   ����C2 ����F12
  ******************************************************************************
**/
#include "helper.h"
#include "leadscrew.h"

/*˿�ܿ��� len ���� dir ����*/
void set_leadscrew(LENGTH len,LS_DIRECTION dir){
  int i = len * 360 / (1.8 * DIS_PER_RND);
  //���÷���
  if(dir == LS_LEFT){
    GPIO_SetBits(GPIOC,GPIO_Pin_2);
  }else{
    GPIO_ResetBits(GPIOC,GPIO_Pin_2);
  }
  //���������ź�
	for( ; i > 0; i--){
		GPIO_SetBits(GPIOF,GPIO_Pin_12);
		Delay_ums(160);
		GPIO_ResetBits(GPIOF,GPIO_Pin_12);
		Delay_ums(160);
	}
}
