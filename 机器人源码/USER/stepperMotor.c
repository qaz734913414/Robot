/**
  ******************************************************************************
  * @file stepperMotor.c
  * @author RoboGame Sonny Team
  * @data 23/7
  * @version 1.11
  * @brief 步进电机控制算法
  *        电机步距角 1.8 / 20
  * @note: 依赖文件  "helper.h" "stepperMotor.h"
  ******************************************************************************
**/
#include "helper.h"
#include "stepperMotor.h"

/*步进电机1 控制 ang 角度 dir 方向*/
void setS_Motor1(ANGLE ang, S_DIRECTION dir){
  int i = ang *16 /0.18;
  //设置方向
  if(dir == S_FRONT){
    GPIO_ResetBits(GPIOC,GPIO_Pin_0);
  }else{
    GPIO_SetBits(GPIOC,GPIO_Pin_0);
  }
  //构造脉冲信号
	for( ; i > 0; i--){
		GPIO_SetBits(GPIOF,GPIO_Pin_10);
		Delay_ums(30);
		GPIO_ResetBits(GPIOF,GPIO_Pin_10);
		Delay_ums(30);
	}
}

/*步进电机1 控制 ang 角度 dir 方向*/
void setS_Motor1_slow(ANGLE ang, S_DIRECTION dir,int delaytime){
  int i = ang *16 /0.18;
	int delaytime2=delaytime*30;
  //设置方向
  if(dir == S_FRONT){
    GPIO_ResetBits(GPIOC,GPIO_Pin_0);
  }else{
    GPIO_SetBits(GPIOC,GPIO_Pin_0);
  }
  //构造脉冲信号
	for( ; i > 0; i--){
		GPIO_SetBits(GPIOF,GPIO_Pin_10);
		Delay_ums(delaytime2);
		GPIO_ResetBits(GPIOF,GPIO_Pin_10);
		Delay_ums(delaytime2);
	}
}
/*步进电机2 控制 ang 角度 dir 方向*/
void setS_Motor2(ANGLE ang, S_DIRECTION dir){
  int i = ang * 16 /0.067;
  //设置方向
  if(dir == S_FRONT){
    GPIO_SetBits(GPIOC,GPIO_Pin_1);
  }else{
    GPIO_ResetBits(GPIOC,GPIO_Pin_1);
  }
  //构造脉冲信号
	for( ; i > 0; i--){
		GPIO_SetBits(GPIOF,GPIO_Pin_11);
		Delay_ums(15);
		GPIO_ResetBits(GPIOF,GPIO_Pin_11);
		Delay_ums(15);
	};
}
/*大小臂同时动 ang 角度 dir 方向,相对位置不变*/
void setS_Motor(ANGLE ang,S_DIRECTION dir){
	int i=ang/0.18;	
	//设置方向
  if(dir == S_FRONT){
    GPIO_ResetBits(GPIOC,GPIO_Pin_0);
		GPIO_SetBits(GPIOC,GPIO_Pin_1);
  }else{
    GPIO_SetBits(GPIOC,GPIO_Pin_0);
		GPIO_ResetBits(GPIOC,GPIO_Pin_1);
  }
	//构造脉冲信号
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
/*步进电机3 控制 ang 角度 dir 方向*/
void setS_Motor3(ANGLE ang, S_DIRECTION dir){
  int i = ang /0.09;
  //设置方向
  if(dir == S_FRONT){
    GPIO_SetBits(GPIOC,GPIO_Pin_2);
  }else{
    GPIO_ResetBits(GPIOC,GPIO_Pin_2);
  }
  //构造脉冲信号
	for( ; i > 0; i--){
		GPIO_SetBits(GPIOF,GPIO_Pin_12);
		Delay_ums(750);
		GPIO_ResetBits(GPIOF,GPIO_Pin_12);
		Delay_ums(750);
	}
}
/*步进电机4 控制 ang 角度 dir 方向*/
void setS_Motor4(ANGLE ang, S_DIRECTION dir){
  int i = ang /0.09;
  //设置方向
  if(dir == S_FRONT){
    GPIO_SetBits(GPIOC,GPIO_Pin_3);
  }else{
    GPIO_ResetBits(GPIOC,GPIO_Pin_3);
  }
  //构造脉冲信号
	for( ; i > 0; i--){
		GPIO_SetBits(GPIOF,GPIO_Pin_13);
		Delay_ums(750);
		GPIO_ResetBits(GPIOF,GPIO_Pin_13);
		Delay_ums(750);
	}
}
