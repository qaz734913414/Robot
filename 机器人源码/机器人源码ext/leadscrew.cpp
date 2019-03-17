#include "helper.h"



#define DIS_PER_RND 4 //����ÿתһȦ˿���ƶ�����mm

/*˿�ܿ��� len ���� dir ����*/
void set_leadscrew(u32 len,S_DIRECTION dir){
  int i = len * 360 / (0.09 * DIS_PER_RND);
  //���÷���
  if(dir == S_FRONT){
    GPIO_SetBits(GPIOE,GPIO_Pin_1);
  }else{
    GPIO_ResetBits(GPIOE,GPIO_Pin_1);
  }
  //���������ź�
	for( ; i > 0; i--){
		GPIO_SetBits(GPIOA,GPIO_Pin_1);
		Delay_ums(10);
		GPI1O_ResetBits(GPIOA,GPIO_Pin_1);
		Delay_ums(10);
	}
}