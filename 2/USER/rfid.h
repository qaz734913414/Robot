/**
  ******************************************************************************
  * @file rfid.h 
  * @author RoboGame Sonny Team
  * @data 7/9
  * @version 1.0
  * @brief 设置rfid
  *        图书标签定位函数 readLocationTag()
  ******************************************************************************
**/
#ifndef RFID_H
#define RFID_H

#include "stm32f4xx.h"

extern u8 locationTag;//当前定位点标签值

/*读区定位点信息*/
void readLocationTag(void);
void openRFID(void);
void resetLocationTag(void);
#endif /* rfid.h */
