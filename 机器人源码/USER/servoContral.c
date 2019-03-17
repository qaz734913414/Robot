/**
  ******************************************************************************
  * @file servoContral.c
  * @author RoboGame Sonny Team
  * @data 28/7
  * @version 1.11
  * @brief ������� 
  *
  ******************************************************************************
**/
#include "servoContral.h"
#include "helper.h"
#define MAX_SERVO 180
/*���1 ���� ang / 180 */
void setServo1(u32 ang){
    int i = ang;
    GPIO_SetBits(GPIOA,GPIO_Pin_4);
    for( ;i > 0 ;i-- )
        Delay_mms(100);
    i = MAX_SERVO - ang;
    GPIO_ResetBits(GPIOA,GPIO_Pin_4);
    for( ;i > 0 ;i--)
        Delay_mms(100);
    GPIO_SetBits(GPIOA,GPIO_Pin_4);
}
/*���2 ���� ang / 180 */
void setServo2(u32 ang){
    int i = ang;
    GPIO_SetBits(GPIOA,GPIO_Pin_5);
    for( ;i > 0 ;i-- )
        Delay_mms(100);
    i = MAX_SERVO - ang;
    GPIO_ResetBits(GPIOA,GPIO_Pin_5);
    for( ;i > 0 ;i--)
        Delay_mms(100);
    GPIO_SetBits(GPIOA,GPIO_Pin_5);
}
/*���3 ���� ang / 180 */
void setServo3(u32 ang){
    int i = ang;
    GPIO_SetBits(GPIOA,GPIO_Pin_6);
    for( ;i > 0 ;i-- )
        Delay_mms(100);
    i = MAX_SERVO - ang;
    GPIO_ResetBits(GPIOA,GPIO_Pin_6);
    for( ;i > 0 ;i--)
        Delay_mms(100);
    GPIO_SetBits(GPIOA,GPIO_Pin_6);
}
/*���1 ���� ang / 180 */
void setServo4(u32 ang){
    int i = ang;
    GPIO_SetBits(GPIOA,GPIO_Pin_7);
    for( ;i > 0 ;i-- )
        Delay_mms(100);
    i = MAX_SERVO - ang;
    GPIO_ResetBits(GPIOA,GPIO_Pin_7);
    for( ;i > 0 ;i--)
        Delay_mms(100);
    GPIO_SetBits(GPIOA,GPIO_Pin_7);
}
