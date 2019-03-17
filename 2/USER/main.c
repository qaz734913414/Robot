/**
  ******************************************************************************
  * @file main.c 
  * @author RoboGame Sonny Team
  * @data 23/7
  * @version 0.2
  * @brief ������
  * @note: �����ļ� "init.h" "delay.h" "usart.h"
  ******************************************************************************
**/
#include "stm32f4xx.h"
#include "helper.h"
#include "init.h"
#include "rfid.h"


int main(void)
{
	init();
	openRFID();
  resetLocationTag();
	while(1){
		readLocationTag();
		;
	}
}
