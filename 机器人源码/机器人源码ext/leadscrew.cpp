#include "helper.h"



#define DIS_PER_RND 4 //步进每转一圈丝杠移动距离mm

/*丝杠控制 len 距离 dir 方向*/
void set_leadscrew(u32 len,S_DIRECTION dir){
  int i = len * 360 / (0.09 * DIS_PER_RND);
  //设置方向
  if(dir == S_FRONT){
    GPIO_SetBits(GPIOE,GPIO_Pin_1);
  }else{
    GPIO_ResetBits(GPIOE,GPIO_Pin_1);
  }
  //构造脉冲信号
	for( ; i > 0; i--){
		GPIO_SetBits(GPIOA,GPIO_Pin_1);
		Delay_ums(10);
		GPI1O_ResetBits(GPIOA,GPIO_Pin_1);
		Delay_ums(10);
	}
}