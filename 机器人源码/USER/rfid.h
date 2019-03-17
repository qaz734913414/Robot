/**
  ******************************************************************************
  * @file rfid.h 
  * @author RoboGame Sonny Team
  * @data 11/9
  * @version 1.1
  * @brief 设置rfid
  *        开放取书函数 readBooks()
  ******************************************************************************
**/
#ifndef RFID_H
#define RFID_H

#include "stm32f4xx.h"

#define BOOK_NUM 5 //书总数
extern u8 locationTag;//当前定位点标签值
extern u8 RecentBookNumber;//当前放到第几本书
extern u8 SortedBooks[5][3];//[i][j]中i表示放书顺序,j为0表示对应标签号,j为1表示书在机器人书架上的位置,j为2表示要放到几层

/*读取5本图书RFID信息*/
void readBooks(void);

#endif /* rfid.h */
