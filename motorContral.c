/**
  ******************************************************************************
  * @file motorContral.c 
  * @author RoboGame Sonny Team
  * @data 11/9
  * @version 1.1
  * @brief ������� 
  *     PWM1(L1)            PWM2(R1)
  *     PWM3(L2)            PWM4(R2)
  * @note: �����ļ� "init.h" "motorContral.h" "helper.h"
  ******************************************************************************
**/
#include "motorContral.h"
#include "init.h"
#include "pathTracking.h"
#include "helper.h"

#define SHUTDOWN_MOTOR
#define L_T_PWM 8 //ѭ��΢�����PWM��������
//43:101
#define TURNING_PWM 40 //ת��ʱ�ĵ��ת��
//493000 :101
#define TIME_TURNING_PWM 80000 //ת�俪ʼʱ����ʼ�ж��Ƿ�ת����ɵ��ӳ�
//37:101
#define INITIAL_PWM 34 //�����ʼת��
#define L_T_TIME_1 10 //ѭ���ڲ���ֵ��ԽСԽ��ȷ���ݴ�ΧԽС
#define L_T_TIME_2 25 //ѭ�������ֵ

/*�����ٶ�*/
extern VELOCITY V_CUR = INITIAL_PWM;
/*�ֵ���ٶȣ���ʼ40��49�ɵ�*/
extern VELOCITY PWM1_CUR = INITIAL_PWM;
extern VELOCITY PWM2_CUR = INITIAL_PWM;
extern VELOCITY PWM3_CUR = INITIAL_PWM;
extern VELOCITY PWM4_CUR = INITIAL_PWM;

/*�������*/
extern u8 TimeOfTurns = 0;

/*��������� �� FRONT �� �� BACK*/
extern DIRECTION motorDirection1 = FRONT;
extern DIRECTION motorDirection2 = FRONT;
extern DIRECTION motorDirection3 = FRONT;
extern DIRECTION motorDirection4 = FRONT;

/*����ǰ������*/
extern DIRECTION direction = FRONT;
extern DIRECTION direction_backup=FRONT;

/*�������*/
BOOLEAN MOTOR = TRUE;

/*����ʱ��*/
u32 TurningTime(u8 time){
    if(time == 0) return(563000);
    else if(time == 1) return(730000);
    else if(time == 2) return(650000);
    else if(time == 3) return(730000);
		return 1;
}
/*���·���*/
void updateDirection(){
  //���ڲ�ʹ��
  if(motorDirection1 == FRONT){
    SET_DIR_L1;
  }else if (motorDirection1 == BACK){
    RESET_DIR_L1;
  }else{
        SHUTDOWN_L1;
    }
  if(motorDirection2 == FRONT){
    SET_DIR_R1;
  }else if (motorDirection2 == BACK){
    RESET_DIR_R1;
  }else{
        SHUTDOWN_R1;
    }
    if(motorDirection3 == FRONT){
    SET_DIR_L2;
  }else if (motorDirection3 == BACK){
    RESET_DIR_L2;
  }else{
        SHUTDOWN_L2;
    }
    if(motorDirection4 == FRONT){
    SET_DIR_R2;
  }else if (motorDirection4 == BACK){
    RESET_DIR_R2;
  }else{
        SHUTDOWN_R2;
    }
}
/*�����ٶ�*/
void changeAll_PWM(VELOCITY L1 ,VELOCITY R1, VELOCITY L2, VELOCITY R2){
  //���ڲ�ʹ��
    PWM1(L1);
    PWM2(R1);
    PWM3(L2);
    PWM4(R2);
}
/*ֱ��*/
void goStraight(DIRECTION dir){
    changeAll_PWM(V_CUR,V_CUR,V_CUR,V_CUR);
    motorDirection1 = dir;
    motorDirection2 = dir;
    motorDirection3 = dir;
    motorDirection4 = dir;
    updateDirection();
}
/*��ת*/
void turnRight(DIRECTION dir){
    TURNING = TRUE;
    setTurningRestart();
    changeAll_PWM(TURNING_PWM,TURNING_PWM,TURNING_PWM,TURNING_PWM);
    if(dir == FRONT){
        motorDirection1 = FRONT;
        motorDirection2 = BACK;
        motorDirection3 = FRONT;
        motorDirection4 = BACK;
    }else{
        motorDirection1 = BACK;
        motorDirection2 = FRONT;
        motorDirection3 = BACK;
        motorDirection4 = FRONT;
    }
    updateDirection();
    //Delay_ums(TurningTime(TimeOfTurns)); //ԭת�䷽ʽ����ʱ��
    Delay_ums(TIME_TURNING_PWM);
    while(1){
				if(direction == FRONT){
					if(LINE3_3){
            break;
					}
				}else{
					if(LINE3_4){
            break;
					}
				}
        continue;
    }
		delay_ms(200);
    goStraight(dir);
    changeAll_PWM(V_CUR,V_CUR,V_CUR,V_CUR);
    Delay_mms(280);
    TURNING = FALSE;
}
/*��ת*/
void turnLeft(DIRECTION dir){
    TURNING = TRUE;
    setTurningRestart();
    changeAll_PWM(TURNING_PWM,TURNING_PWM,TURNING_PWM,TURNING_PWM);
    if(dir == FRONT){
        motorDirection1 = BACK;
        motorDirection2 = FRONT;
        motorDirection3 = BACK;
        motorDirection4 = FRONT;
    }else{
        motorDirection1 = FRONT;
        motorDirection2 = BACK;
        motorDirection3 = FRONT;
        motorDirection4 = BACK;
    }
    updateDirection();
    //Delay_ums(TurningTime(TimeOfTurns));//ԭת�䷽ʽ����ʱ��
    Delay_ums(TIME_TURNING_PWM);
    while(!TURN_LEFT_SUCCESS){
				testHadTuringSuccess();
        continue;
    }
		delay_ms(200);
    goStraight(dir);
    changeAll_PWM(V_CUR,V_CUR,V_CUR,V_CUR);
    Delay_mms(280);
    TURNING = FALSE;
    }
/*���5��*/
void changeL1(DIRECTION dir){
    if(dir == FRONT){
        PWM2_CUR+=L_T_PWM;
        PWM4_CUR+=L_T_PWM;
        PWM1_CUR-=L_T_PWM;
        PWM3_CUR-=L_T_PWM;
        changeAll_PWM(PWM1_CUR,PWM2_CUR,PWM3_CUR,PWM4_CUR);
        Delay_mms(L_T_TIME_1);
    }else{
        PWM1_CUR+=L_T_PWM;
        PWM3_CUR+=L_T_PWM;
        PWM2_CUR-=L_T_PWM;
        PWM4_CUR-=L_T_PWM;
        changeAll_PWM(PWM1_CUR,PWM2_CUR,PWM3_CUR,PWM4_CUR);
        Delay_mms(L_T_TIME_1);
    }
}
/*�ҵ�5��*/
void changeR1(DIRECTION dir){
    if(dir == FRONT){
        PWM1_CUR+=L_T_PWM;
        PWM3_CUR+=L_T_PWM;
        PWM2_CUR-=L_T_PWM;
        PWM4_CUR-=L_T_PWM;
        changeAll_PWM(PWM1_CUR,PWM2_CUR,PWM3_CUR,PWM4_CUR);
        Delay_mms(L_T_TIME_1);
    }else{
        PWM2_CUR+=L_T_PWM;
        PWM4_CUR+=L_T_PWM;
        PWM1_CUR-=L_T_PWM;
        PWM3_CUR-=L_T_PWM;
        changeAll_PWM(PWM1_CUR,PWM2_CUR,PWM3_CUR,PWM4_CUR);
        Delay_mms(L_T_TIME_1);
    }
}
/*���10��*/
void changeL2(DIRECTION dir){
    if(dir == FRONT){
        PWM2_CUR+=L_T_PWM;
        PWM4_CUR+=L_T_PWM;
        PWM1_CUR-=L_T_PWM;
        PWM3_CUR-=L_T_PWM;
        changeAll_PWM(PWM1_CUR,PWM2_CUR,PWM3_CUR,PWM4_CUR);
        Delay_mms(L_T_TIME_2);
    }else{
        PWM1_CUR+=L_T_PWM;
        PWM3_CUR+=L_T_PWM;
        PWM2_CUR-=L_T_PWM;
        PWM4_CUR-=L_T_PWM;
        changeAll_PWM(PWM1_CUR,PWM2_CUR,PWM3_CUR,PWM4_CUR);
        Delay_mms(L_T_TIME_2);
    }
}
/*�ҵ�10��*/
void changeR2(DIRECTION dir){
    if(dir == FRONT){
        PWM1_CUR+=L_T_PWM;
        PWM3_CUR+=L_T_PWM;
        PWM2_CUR-=L_T_PWM;
        PWM4_CUR-=L_T_PWM;
        changeAll_PWM(PWM1_CUR,PWM2_CUR,PWM3_CUR,PWM4_CUR);
        Delay_mms(L_T_TIME_2);
    }else{
        PWM2_CUR+=L_T_PWM;
        PWM4_CUR+=L_T_PWM;
        PWM1_CUR-=L_T_PWM;
        PWM3_CUR-=L_T_PWM;
        changeAll_PWM(PWM1_CUR,PWM2_CUR,PWM3_CUR,PWM4_CUR);
        Delay_mms(L_T_TIME_2);
    }
}
/*�����˶�*/
void run(void){
    if(MOTOR == TRUE){
        TURNING = TRUE;
        switch(POSITON){
        case  L_RANGE_1: //��ƫ5��
            changeL1(direction);
            goStraight(direction);
            break;
        case  L_RANGE_2: //��ƫ10��
            changeL2(direction);
            goStraight(direction);
            break;
        case  L_TURN: //��ת
            if(TimeOfTurns == 1){
                turnLeft(direction);
                TimeOfTurns++;
          }
            else if(TimeOfTurns == 2){
                direction=BACK;
                turnLeft(direction);
                TimeOfTurns++;
            }
            else if(TimeOfTurns == 3){
                turnRight(direction);
                TimeOfTurns++;
            }
            else if(TimeOfTurns == 4) {
                TimeOfTurns++;
                TURNING = TRUE;
                Delay_mms(200);
                TURNING=FALSE;
            }
            else if(TimeOfTurns == 5) stopMotor();
            break;
        case  R_TURN: //��ת
            if(TimeOfTurns == 0){
                turnRight(direction);
                TimeOfTurns++;
            }
            break;
        case  R_RANGE_1://��ƫ5��
            changeR1(direction);
            goStraight(direction);
            break;
        case  R_RANGE_2://��ƫ10��
            changeR2(direction);
            goStraight(direction);
            break;
        default:
            TURNING = FALSE;
            break;
    }
    TURNING = FALSE;
    }else{
    goStraight(STOP);
  }
  
}
/*�������*/
void openMotor(void){
  MOTOR = TRUE;
}
/*�رյ��*/
void stopMotor(void){
  MOTOR = FALSE;
}
/*�Ƿ�����*/
BOOLEAN isMOTOR(void){
	return MOTOR;
}
/*�����ϲ�װ�ó�ʼ��*/
void testArm(void){
    stopMotor();
		goStraight(STOP);
}
