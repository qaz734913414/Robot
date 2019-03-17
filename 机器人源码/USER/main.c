/**
  ******************************************************************************
  * @file main.c 
  * @author RoboGame Sonny Team
  * @data 11/9
  * @version 1.11
  * @brief ������ ���赥�����Ի�е�� ���ȵ���testArm
  * @note: �����ļ� all
  ******************************************************************************
**/
#include "stm32f4xx.h"
#include "stepperMotor.h"
#include "motorContral.h"
#include "servoContral.h"
#include "leadscrew.h"
#include "helper.h"
#include "init.h"
#include "RobotArmMotion.h"
#include "rfid.h"


int main(void)
{
		preinit();
		//readBooks();
		init();
		//testArm();
		Delay_mms(500);
		//Fetch(2);
		//Put(2,4);
		//Fetch(3);
		//Put(3,3);
		//Fetch(1);
		//Put(1,2);
		while(RecentBookNumber<5){
		Fetch(SortedBooks[RecentBookNumber][1]);
		Put(SortedBooks[RecentBookNumber][1],SortedBooks[RecentBookNumber][2]);
	}
	
	
	while(1){
		;
	}
	
}
