/**
  ******************************************************************************
  * @file stepperMotor.c
  * @author RoboGame Sonny Team
  * @data 23/7
  * @version 1.11
  * @brief ������������㷨
  *        �������� 1.8 / 20
  * @note: �����ļ�  "helper.h" "stepperMotor.h"
  ******************************************************************************
**/
#include "helper.h"
#include "stepperMotor.h"

/*�������1 ���� ang �Ƕ� dir ����*/
void setS_Motor1(ANGLE ang, S_DIRECTION dir){
  int i = ang *16 /0.18;
  //���÷���
  if(dir == S_FRONT){
    GPIO_ResetBits(GPIOC,GPIO_Pin_0);
  }else{
    GPIO_SetBits(GPIOC,GPIO_Pin_0);
  }
  //���������ź�
	for( ; i > 0; i--){
		GPIO_SetBits(GPIOF,GPIO_Pin_10);
		Delay_ums(30);
		GPIO_ResetBits(GPIOF,GPIO_Pin_10);
		Delay_ums(30);
	}
}

/*�������1 ���� ang �Ƕ� dir ����*/
void setS_Motor1_slow(ANGLE ang, S_DIRECTION dir,int delaytime){
  int i = ang *16 /0.18;
	int delaytime2=delaytime*30;
  //���÷���
  if(dir == S_FRONT){
    GPIO_ResetBits(GPIOC,GPIO_Pin_0);
  }else{
    GPIO_SetBits(GPIOC,GPIO_Pin_0);
  }
  //���������ź�
	for( ; i > 0; i--){
		GPIO_SetBits(GPIOF,GPIO_Pin_10);
		Delay_ums(delaytime2);
		GPIO_ResetBits(GPIOF,GPIO_Pin_10);
		Delay_ums(delaytime2);
	}
}
/*�������2 ���� ang �Ƕ� dir ����*/
void setS_Motor2(ANGLE ang, S_DIRECTION dir){
  int i = ang * 16 /0.067;
  //���÷���
  if(dir == S_FRONT){
    GPIO_SetBits(GPIOC,GPIO_Pin_1);
  }else{
    GPIO_ResetBits(GPIOC,GPIO_Pin_1);
  }
  //���������ź�
	for( ; i > 0; i--){
		GPIO_SetBits(GPIOF,GPIO_Pin_11);
		Delay_ums(15);
		GPIO_ResetBits(GPIOF,GPIO_Pin_11);
		Delay_ums(15);
	};
}
/*��С��ͬʱ�� ang �Ƕ� dir ����,���λ�ò���*/
void setS_Motor(ANGLE ang,S_DIRECTION dir){
	int i=ang/0.18;	
	//���÷���
  if(dir == S_FRONT){
    GPIO_ResetBits(GPIOC,GPIO_Pin_0);
		GPIO_SetBits(GPIOC,GPIO_Pin_1);
  }else{
    GPIO_SetBits(GPIOC,GPIO_Pin_0);
		GPIO_ResetBits(GPIOC,GPIO_Pin_1);
  }
	//���������ź�
	for( ; i > 0; i--){
		GPIO_SetBits(GPIOF,GPIO_Pin_11);
		GPIO_SetBits(GPIOF,GPIO_Pin_10);
		Delay_ums(250);
		GPIO_ResetBits(GPIOF,GPIO_Pin_11);
		Delay_ums(250);
		GPIO_SetBits(GPIOF,GPIO_Pin_11);
		Delay_ums(250);
		GPIO_ResetBits(GPIOF,GPIO_Pin_11);
		GPIO_ResetBits(GPIOF,GPIO_Pin_10);
		Delay_ums(250);
		GPIO_SetBits(GPIOF,GPIO_Pin_11);
		Delay_ums(250);
		GPIO_ResetBits(GPIOF,GPIO_Pin_11);
		Delay_ums(250);
	}
	
}
/*�������3 ���� ang �Ƕ� dir ����*/
void setS_Motor3(ANGLE ang, S_DIRECTION dir){
  int i = ang /0.09;
  //���÷���
  if(dir == S_FRONT){
    GPIO_SetBits(GPIOC,GPIO_Pin_2);
  }else{
    GPIO_ResetBits(GPIOC,GPIO_Pin_2);
  }
  //���������ź�
	for( ; i > 0; i--){
		GPIO_SetBits(GPIOF,GPIO_Pin_12);
		Delay_ums(750);
		GPIO_ResetBits(GPIOF,GPIO_Pin_12);
		Delay_ums(750);
	}
}
/*�������4 ���� ang �Ƕ� dir ����*/
void setS_Motor4(ANGLE ang, S_DIRECTION dir){
  int i = ang /0.09;
  //���÷���
  if(dir == S_FRONT){
    GPIO_SetBits(GPIOC,GPIO_Pin_3);
  }else{
    GPIO_ResetBits(GPIOC,GPIO_Pin_3);
  }
  //���������ź�
	for( ; i > 0; i--){
		GPIO_SetBits(GPIOF,GPIO_Pin_13);
		Delay_ums(750);
		GPIO_ResetBits(GPIOF,GPIO_Pin_13);
		Delay_ums(750);
	}
}
