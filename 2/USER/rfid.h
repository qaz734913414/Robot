/**
  ******************************************************************************
  * @file rfid.h 
  * @author RoboGame Sonny Team
  * @data 7/9
  * @version 1.0
  * @brief ����rfid
  *        ͼ���ǩ��λ���� readLocationTag()
  ******************************************************************************
**/
#ifndef RFID_H
#define RFID_H

#include "stm32f4xx.h"

extern u8 locationTag;//��ǰ��λ���ǩֵ

/*������λ����Ϣ*/
void readLocationTag(void);
void openRFID(void);
void resetLocationTag(void);
#endif /* rfid.h */
