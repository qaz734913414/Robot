/**
  ******************************************************************************
  * @file 
  * @author RoboGame Sonny Team
  * @data 28/7
  * @version 0.2
  * @brief  
  *
  ******************************************************************************
**/

#include "helper.h"


#define INCLINE_ANG 95 //3�Ŷ����б����
#define GRAB_ANG 90//4�Ŷ��ץ�����
#define LOOSE_ANG 120//4�Ŷ���ɿ�����


//����S_FRONT������,S_BACK������
//SetS_mortor1�Ǵ��,2��С��
//SetServo1��2�ֱ�����������ͬ����,3�ǽ���б�ŵĶ��,4�Ǽ���Ķ��

//ͬ���������ͬʱ��
SetServo(u32 ang)
{
SetServo1(ang);
SetServo2(180-ang);//ʵ�ʷ�Χ����180�ȿ��ܻ������?
}

//�������ڶ���,��ͬ��,n��ʾ��n����,S_FRONT��ʾ����,S_BACK����
Put_2(u8 n)
{
SetServo(160);//1
Delay_ms(3);
SetS_mortor1(74,S_BACK);//2
if(n==2) ;
else if(n>2) set_leadscrew(65*(n-2),S_BACK);
else if(n<2) set_leadscrew(65*(2-n),S_FRONT);//˿�ܹ���
SetS_mortor2(28,S_BACK);//3
SetS_mortor1(58,S_FRONT);//4
SetServo(11);//5
Delay_ms(3);
SetS_mortor2(50,S_BACK);//6
SetS_mortor1(15,S_FRONT);//7
SetS_mortor2(14,S_BACK);//8
SetS_mortor1(8,S_FRONT);//9
SetServo3(INCLINE_ANG);
Delay_ms(3);//�鱾��б
SetServo4(LOOSE_ANG);
Delay_ms(3);//����
}

Put_3(u8 n)
{
SetServo(160);//1
Delay_ms(3);
SetS_mortor1(126,S_BACK);//2
if(n==2) ;
else if(n>2) set_leadscrew(65*(n-2),S_BACK);
else if(n<2) set_leadscrew(65*(2-n),S_FRONT);//˿�ܹ���
SetServo(20);//3
Delay_ms(3);
SetS_mortor2(13,S_BACK);//4
SetS_mortor1(40,S_FRONT);//5
SetS_mortor2(22,S_BACK);//6
SetServo(24);//7//23.5ȡ��,һ����������?
SetS_mortor1(14,S_FRONT);//8//13.5ȡ��
SetServo3(INCLINE_ANG);
Delay_ms(3);
SetServo4(LOOSE_ANG);
Delay_ms(3);
}

Put_4(u8 n)
{
SetServo(160);//1
Delay_ms(3);
SetS_mortor1(136,S_BACK);//2
if(n==2);
else if(n>2) set_leadscrew(65*(n-2),S_BACK);
else if(n<2) set_leadscrew(65*(2-n),S_FRONT);//˿�ܹ���
SetS_mortor2(108,S_BACK);//3
SetServo(113);//4
Delay_ms(3);
SetS_mortor1(41,S_FRONT);//5
SetS_mortor2(14,S_FRONT);//6
SetServo3(INCLINE_ANG);
Delay_ms(3);
SetServo4(LOOSE_ANG);
Delay_ms(3);
}

�ӵڶ����ȥȡ��n����(n=0,1,2,3,4),��ͬ
Fetch_2(u8 n)
{
SetS_mortor2(92,S_FRONT);//1
SetServo3(90);//��е�ֹ���
Delay_ms(3);
SetServo(106);//2
Delay_ms(3);
if(n==2) ;
else if(n>2) set_leadscrew(65*(n-2),S_FRONT);
else if(n<2) set_leadscrew(65*(2-n),S_BACK);//˿���ƶ�����n���鴦
SetS_mortor1(7,S_BACK);//3
SetServo4(GRAB_ANG);//ץ��
Delay_ms(3);
}

Fetch_3(u8 n)
{
SetS_mortor2(35,S_FRONT);//1
SetServo3(90);//��е�ֹ���
Delay_ms(3);
SetServo(106);//2
Delay_ms(3);
if(n==2) ;
else if(n>2) set_leadscrew(65*(n-2),S_FRONT);
else if(n<2) set_leadscrew(65*(2-n),S_BACK)//˿���ƶ�����n���鴦
SetS_mortor1(73,S_FRONT);//3//72.5
SetServo4(GRAB_ANG);//ץ��
Delay_ms(3);
}

Fetch_4(u8 n)
{
SetS_mortor1(41,S_BACK);//1
SetServo3(90);//��е�ֹ���
Delay_ms(3);
SetServo(106);//2
Delay_ms(3);
SetS_mortor2(92,S_FRONT);//3
if(n==2) ;
else if(n>2) set_leadscrew(65*(n-2),S_FRONT);
else if(n<2) set_leadscrew(65*(2-n),S_BACK)//˿���ƶ�����n���鴦
SetS_mortor1(136,S_FRONT);//4
SetServo4(GRAB_ANG);//ץ��
Delay_ms(3);
}