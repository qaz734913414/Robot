/**
  ******************************************************************************
  * @file rfid.h 
  * @author RoboGame Sonny Team
  * @data 11/9
  * @version 1.1
  * @brief ����rfid
  *        ����ȡ�麯�� readBooks()
  ******************************************************************************
**/
#ifndef RFID_H
#define RFID_H

#include "stm32f4xx.h"

#define BOOK_NUM 5 //������
extern u8 locationTag;//��ǰ��λ���ǩֵ
extern u8 RecentBookNumber;//��ǰ�ŵ��ڼ�����
extern u8 SortedBooks[5][3];//[i][j]��i��ʾ����˳��,jΪ0��ʾ��Ӧ��ǩ��,jΪ1��ʾ���ڻ���������ϵ�λ��,jΪ2��ʾҪ�ŵ�����

/*��ȡ5��ͼ��RFID��Ϣ*/
void readBooks(void);

#endif /* rfid.h */
