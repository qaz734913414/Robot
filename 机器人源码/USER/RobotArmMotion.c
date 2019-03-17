/**
  ******************************************************************************
  * @file 
  * @author RoboGame Sonny Team
  * @data 11/9
  * @version 1.0
  * @brief  机械臂运动
  *
  ******************************************************************************
**/

#include "helper.h"
#include "RobotArmMotion.h"
#include "rfid.h"
#include "pathtracking.h"

extern u8 ready_to_insert=0;

//丝杠移动距离
int leadscrewLeft[5]={130,65,0,71,133},leadscrewRight[5]={126,63,0,64,129};

//假设S_FRONT是向下,S_BACK是向上
//SetS_mortor1是大臂,2是小臂
//SetServo1和2分别是左右两个同轴舵机,3是将书斜放的舵机,4是夹书的舵机

//两个同轴舵机同时转到角度ang
void SERVO(ANGLE ang)
{
SERVO1(ang);
SERVO2(180-ang);
}


//将第n本书放至第二层,下同理,n表示第n本书
void Put_2(BOOKNUMBER n)
{
    setS_Motor1(40,S_BACK);
    setS_Motor2(35,S_BACK);
    setS_Motor1(30,S_BACK);
    SERVO(135);
		Delay_ums(50000);
		if(n==2);
    else if(n>2) set_leadscrew(leadscrewLeft[n],LS_LEFT);
    else if(n<2) set_leadscrew(leadscrewRight[n],LS_RIGHT);//丝杠归中
		setS_Motor2(15,S_BACK);
		setS_Motor1(40,S_FRONT);
    SERVO3(INCLINE_ANG);
    Delay_ums(40000);
    SERVO4(LOOSE_ANG);
		SERVO(40);
		setS_Motor1(20,S_BACK);
    setS_Motor2(20,S_FRONT);
    setS_Motor1(55,S_BACK);
    setS_Motor2(90,S_BACK);
    setS_Motor1(80,S_BACK);
    setS_Motor2(60,S_BACK);
    setS_Motor1(56,S_BACK);
		setS_Motor2(13,S_FRONT);
    SERVO(90);
    SERVO3(90);
		RecentBookNumber++;
		if(LOCATION_BACKUP!=SortedBooks[RecentBookNumber][0]) 
		{
			openMotor();
			ready_to_insert=NO;
		}
}


//将第n本书放至第三层
void Put_3(BOOKNUMBER n)
{
    setS_Motor1(30,S_BACK);
    setS_Motor2(30,S_BACK);
    setS_Motor1(35,S_BACK);
    setS_Motor2(40,S_BACK);
    SERVO(75);
    setS_Motor1(51,S_BACK);
		setS_Motor2(20,S_BACK);
    setS_Motor1(30,S_BACK);
    SERVO(70);
		setS_Motor2(50,S_BACK);
    setS_Motor1(40,S_BACK);
    if(n==2) ;
    else if(n>2) set_leadscrew(leadscrewLeft[n],LS_LEFT);
    else if(n<2) set_leadscrew(leadscrewRight[n],LS_RIGHT);//丝杠归中
    SERVO(110);
    Delay_ums(50000);
		setS_Motor1(35,S_FRONT);
		setS_Motor2(20,S_FRONT);
    setS_Motor1(40,S_FRONT);
		setS_Motor2(20,S_FRONT);
    //SERVO3(INCLINE_ANG);
		Delay_ums(40000);
    SERVO4(LOOSE_ANG);
		setS_Motor2(25,S_BACK);
		setS_Motor1(50,S_BACK);		
    setS_Motor2(40,S_BACK);
    setS_Motor1(70,S_BACK);
    SERVO(90);
    SERVO3(90);
		RecentBookNumber++;
		if(LOCATION_BACKUP!=SortedBooks[RecentBookNumber][0]) 
		{
			openMotor();
			ready_to_insert=NO;
		}
}


//将第n本书放至第四层
void Put_4(BOOKNUMBER n)
{
    setS_Motor1(30,S_BACK);
    setS_Motor2(30,S_BACK);
    setS_Motor1(35,S_BACK);
    setS_Motor2(40,S_BACK);
    SERVO(75);
    setS_Motor1(51,S_BACK);
		setS_Motor2(15,S_BACK);
    setS_Motor1(30,S_BACK);
    SERVO(65);
    setS_Motor2(60,S_BACK);
		setS_Motor1(40,S_BACK);
    SERVO(70);
    if(n==2) ;
    else if(n>2) set_leadscrew(leadscrewLeft[n],LS_LEFT);
    else if(n<2) set_leadscrew(leadscrewRight[n],LS_RIGHT);//丝杠归中
    setS_Motor2(40,S_BACK);
    setS_Motor1(90,S_FRONT);
    //SERVO3(INCLINE_ANG);
    Delay_ums(40000);
    SERVO4(LOOSE_ANG);	
    setS_Motor1(130,S_BACK);
    SERVO(90);
    SERVO3(90);
		RecentBookNumber++;
		if(LOCATION_BACKUP!=SortedBooks[RecentBookNumber][0]) 
		{
			openMotor();
			ready_to_insert=NO;
		}
}


//将第n本书放置第m层
void Put(BOOKNUMBER n,LAYER m){
    if(m==2) Put_2(n);
    else if(m==3) Put_3(n);
    else if(m==4) Put_4(n);
}

//由初始状态去取第n本书
void Fetch(BOOKNUMBER n)
{
		while(!ready_to_insert){;}
    SERVO(90);
    SERVO3(90);
    SERVO4(LOOSE_ANG);
    setS_Motor1(15,S_FRONT);
    SERVO(135);
    if(n==2) ;
    else if(n>2) set_leadscrew(leadscrewRight[n],LS_RIGHT);
    else if(n<2) set_leadscrew(leadscrewLeft[n],LS_LEFT);//丝杠移动到第n本书处
    setS_Motor1(85,S_FRONT);
    setS_Motor2(100,S_FRONT);
    SERVO(55);
    Delay_ums(20000);
    setS_Motor1(40,S_FRONT);
    setS_Motor2(20,S_FRONT);
    setS_Motor1(84,S_FRONT);
		Delay_mms(30);
    setS_Motor2(57,S_FRONT);
    SERVO4(GRAB_ANG);//抓书
    //Delay_ums(30000);
}


//假如大臂出了问题,用下面这种
/*
//由初始状态去取第n本书
void Fetch(BOOKNUMBER n)
{
    SERVO(90);
    SERVO3(90);
    SERVO4(LOOSE_ANG);
    setS_Motor1(15,S_FRONT);
    SERVO(135);
    if(n==2) ;
    else if(n>2) set_leadscrew(65*(n-2),LS_RIGHT);
    else if(n<2) set_leadscrew(65*(2-n),LS_LEFT);//丝杠移动到第n本书处
    setS_Motor1(85,S_FRONT);
    setS_Motor2(100,S_FRONT);
    SERVO(55);
    Delay_ums(20000);
    setS_Motor1(40,S_FRONT);
    setS_Motor2(20,S_FRONT);
    setS_Motor1(83,S_FRONT);
		Delay_mms(30);
    setS_Motor2(57,S_FRONT);
    SERVO4(GRAB_ANG);//抓书
    Delay_ums(30000);
}
*/
