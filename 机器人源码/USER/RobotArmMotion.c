/**
  ******************************************************************************
  * @file 
  * @author RoboGame Sonny Team
  * @data 11/9
  * @version 1.0
  * @brief  ��е���˶�
  *
  ******************************************************************************
**/

#include "helper.h"
#include "RobotArmMotion.h"
#include "rfid.h"
#include "pathtracking.h"

extern u8 ready_to_insert=0;

//˿���ƶ�����
int leadscrewLeft[5]={130,65,0,71,133},leadscrewRight[5]={126,63,0,64,129};

//����S_FRONT������,S_BACK������
//SetS_mortor1�Ǵ��,2��С��
//SetServo1��2�ֱ�����������ͬ����,3�ǽ���б�ŵĶ��,4�Ǽ���Ķ��

//����ͬ����ͬʱת���Ƕ�ang
void SERVO(ANGLE ang)
{
SERVO1(ang);
SERVO2(180-ang);
}


//����n��������ڶ���,��ͬ��,n��ʾ��n����
void Put_2(BOOKNUMBER n)
{
    setS_Motor1(40,S_BACK);
    setS_Motor2(35,S_BACK);
    setS_Motor1(30,S_BACK);
    SERVO(135);
		Delay_ums(50000);
		if(n==2);
    else if(n>2) set_leadscrew(leadscrewLeft[n],LS_LEFT);
    else if(n<2) set_leadscrew(leadscrewRight[n],LS_RIGHT);//˿�ܹ���
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


//����n�������������
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
    else if(n<2) set_leadscrew(leadscrewRight[n],LS_RIGHT);//˿�ܹ���
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


//����n����������Ĳ�
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
    else if(n<2) set_leadscrew(leadscrewRight[n],LS_RIGHT);//˿�ܹ���
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


//����n������õ�m��
void Put(BOOKNUMBER n,LAYER m){
    if(m==2) Put_2(n);
    else if(m==3) Put_3(n);
    else if(m==4) Put_4(n);
}

//�ɳ�ʼ״̬ȥȡ��n����
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
    else if(n<2) set_leadscrew(leadscrewLeft[n],LS_LEFT);//˿���ƶ�����n���鴦
    setS_Motor1(85,S_FRONT);
    setS_Motor2(100,S_FRONT);
    SERVO(55);
    Delay_ums(20000);
    setS_Motor1(40,S_FRONT);
    setS_Motor2(20,S_FRONT);
    setS_Motor1(84,S_FRONT);
		Delay_mms(30);
    setS_Motor2(57,S_FRONT);
    SERVO4(GRAB_ANG);//ץ��
    //Delay_ums(30000);
}


//�����۳�������,����������
/*
//�ɳ�ʼ״̬ȥȡ��n����
void Fetch(BOOKNUMBER n)
{
    SERVO(90);
    SERVO3(90);
    SERVO4(LOOSE_ANG);
    setS_Motor1(15,S_FRONT);
    SERVO(135);
    if(n==2) ;
    else if(n>2) set_leadscrew(65*(n-2),LS_RIGHT);
    else if(n<2) set_leadscrew(65*(2-n),LS_LEFT);//˿���ƶ�����n���鴦
    setS_Motor1(85,S_FRONT);
    setS_Motor2(100,S_FRONT);
    SERVO(55);
    Delay_ums(20000);
    setS_Motor1(40,S_FRONT);
    setS_Motor2(20,S_FRONT);
    setS_Motor1(83,S_FRONT);
		Delay_mms(30);
    setS_Motor2(57,S_FRONT);
    SERVO4(GRAB_ANG);//ץ��
    Delay_ums(30000);
}
*/
