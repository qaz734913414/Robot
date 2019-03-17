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


#define INCLINE_ANG 95 //3号舵机倾斜度数
#define GRAB_ANG 90//4号舵机抓书度数
#define LOOSE_ANG 120//4号舵机松开度数


//假设S_FRONT是向下,S_BACK是向上
//SetS_mortor1是大臂,2是小臂
//SetServo1和2分别是左右两个同轴舵机,3是将书斜放的舵机,4是夹书的舵机

//同轴两个舵机同时动
SetServo(u32 ang)
{
SetServo1(ang);
SetServo2(180-ang);//实际范围不是180度可能会出问题?
}

//放书至第二层,下同理,n表示第n本书,S_FRONT表示向右,S_BACK向左
Put_2(u8 n)
{
SetServo(160);//1
Delay_ms(3);
SetS_mortor1(74,S_BACK);//2
if(n==2) ;
else if(n>2) set_leadscrew(65*(n-2),S_BACK);
else if(n<2) set_leadscrew(65*(2-n),S_FRONT);//丝杠归中
SetS_mortor2(28,S_BACK);//3
SetS_mortor1(58,S_FRONT);//4
SetServo(11);//5
Delay_ms(3);
SetS_mortor2(50,S_BACK);//6
SetS_mortor1(15,S_FRONT);//7
SetS_mortor2(14,S_BACK);//8
SetS_mortor1(8,S_FRONT);//9
SetServo3(INCLINE_ANG);
Delay_ms(3);//书本倾斜
SetServo4(LOOSE_ANG);
Delay_ms(3);//松手
}

Put_3(u8 n)
{
SetServo(160);//1
Delay_ms(3);
SetS_mortor1(126,S_BACK);//2
if(n==2) ;
else if(n>2) set_leadscrew(65*(n-2),S_BACK);
else if(n<2) set_leadscrew(65*(2-n),S_FRONT);//丝杠归中
SetServo(20);//3
Delay_ms(3);
SetS_mortor2(13,S_BACK);//4
SetS_mortor1(40,S_FRONT);//5
SetS_mortor2(22,S_BACK);//6
SetServo(24);//7//23.5取整,一定得是整数?
SetS_mortor1(14,S_FRONT);//8//13.5取整
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
else if(n<2) set_leadscrew(65*(2-n),S_FRONT);//丝杠归中
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

从第二层回去取第n本书(n=0,1,2,3,4),下同
Fetch_2(u8 n)
{
SetS_mortor2(92,S_FRONT);//1
SetServo3(90);//机械手归中
Delay_ms(3);
SetServo(106);//2
Delay_ms(3);
if(n==2) ;
else if(n>2) set_leadscrew(65*(n-2),S_FRONT);
else if(n<2) set_leadscrew(65*(2-n),S_BACK);//丝杠移动到第n本书处
SetS_mortor1(7,S_BACK);//3
SetServo4(GRAB_ANG);//抓书
Delay_ms(3);
}

Fetch_3(u8 n)
{
SetS_mortor2(35,S_FRONT);//1
SetServo3(90);//机械手归中
Delay_ms(3);
SetServo(106);//2
Delay_ms(3);
if(n==2) ;
else if(n>2) set_leadscrew(65*(n-2),S_FRONT);
else if(n<2) set_leadscrew(65*(2-n),S_BACK)//丝杠移动到第n本书处
SetS_mortor1(73,S_FRONT);//3//72.5
SetServo4(GRAB_ANG);//抓书
Delay_ms(3);
}

Fetch_4(u8 n)
{
SetS_mortor1(41,S_BACK);//1
SetServo3(90);//机械手归中
Delay_ms(3);
SetServo(106);//2
Delay_ms(3);
SetS_mortor2(92,S_FRONT);//3
if(n==2) ;
else if(n>2) set_leadscrew(65*(n-2),S_FRONT);
else if(n<2) set_leadscrew(65*(2-n),S_BACK)//丝杠移动到第n本书处
SetS_mortor1(136,S_FRONT);//4
SetServo4(GRAB_ANG);//抓书
Delay_ms(3);
}