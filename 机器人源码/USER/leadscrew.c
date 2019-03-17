/**
  ******************************************************************************
  * @file leadscrew.c
  * @author RoboGame Sonny Team
  * @data 7/8
  * @version 1.11
  * @brief 丝杠控制
  *        电机步距角 1.8
  * @note: 依赖文件  "helper.h" "leadscrew.h" 
	*        引脚   方向C2 脉冲F12
  ******************************************************************************
**/
#include "helper.h"
#include "leadscrew.h"

/*丝杠控制 len 距离 dir 方向*/
void set_leadscrew(LENGTH len,LS_DIRECTION dir){
  int i = len * 360 / (1.8 * DIS_PER_RND);
  //设置方向
  if(dir == LS_LEFT){
    GPIO_SetBits(GPIOC,GPIO_Pin_2);
  }else{
    GPIO_ResetBits(GPIOC,GPIO_Pin_2);
  }
  //构造脉冲信号
	for( ; i > 0; i--){
		GPIO_SetBits(GPIOF,GPIO_Pin_12);
		Delay_ums(160);
		GPIO_ResetBits(GPIOF,GPIO_Pin_12);
		Delay_ums(160);
	}
}
