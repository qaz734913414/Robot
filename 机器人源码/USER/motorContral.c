/**
  ******************************************************************************
  * @file motorContral.c 
  * @author RoboGame Sonny Team
  * @data 11/9
  * @version 1.1
  * @brief 电机控制 
  *     PWM1(L1)            PWM2(R1)
  *     PWM3(L2)            PWM4(R2)
  * @note: 依赖文件 "init.h" "motorContral.h" "helper.h"
  ******************************************************************************
**/
#include "motorContral.h"
#include "init.h"
#include "pathTracking.h"
#include "helper.h"

#define SHUTDOWN_MOTOR
#define L_T_PWM 8 //循迹微调电机PWM增减数额
//43:101
#define TURNING_PWM 40 //转弯时的电机转速
//493000 :101
#define TIME_TURNING_PWM 80000 //转弯开始时到开始判断是否转弯完成的延迟
//37:101
#define INITIAL_PWM 34 //电机初始转速
#define L_T_TIME_1 10 //循迹内层阈值，越小越精确，容错范围越小
#define L_T_TIME_2 25 //循迹外层阈值

/*整机速度*/
extern VELOCITY V_CUR = INITIAL_PWM;
/*分电机速度，初始40，49可调*/
extern VELOCITY PWM1_CUR = INITIAL_PWM;
extern VELOCITY PWM2_CUR = INITIAL_PWM;
extern VELOCITY PWM3_CUR = INITIAL_PWM;
extern VELOCITY PWM4_CUR = INITIAL_PWM;

/*拐弯次数*/
extern u8 TimeOfTurns = 0;

/*电机朝向，正 ＝ FRONT 反 ＝ BACK*/
extern DIRECTION motorDirection1 = FRONT;
extern DIRECTION motorDirection2 = FRONT;
extern DIRECTION motorDirection3 = FRONT;
extern DIRECTION motorDirection4 = FRONT;

/*整机前进方向*/
extern DIRECTION direction = FRONT;
extern DIRECTION direction_backup=FRONT;

/*电机开关*/
BOOLEAN MOTOR = TRUE;

/*拐弯时间*/
u32 TurningTime(u8 time){
    if(time == 0) return(563000);
    else if(time == 1) return(730000);
    else if(time == 2) return(650000);
    else if(time == 3) return(730000);
		return 1;
}
/*更新方向*/
void updateDirection(){
  //仅内部使用
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
/*更新速度*/
void changeAll_PWM(VELOCITY L1 ,VELOCITY R1, VELOCITY L2, VELOCITY R2){
  //仅内部使用
    PWM1(L1);
    PWM2(R1);
    PWM3(L2);
    PWM4(R2);
}
/*直行*/
void goStraight(DIRECTION dir){
    changeAll_PWM(V_CUR,V_CUR,V_CUR,V_CUR);
    motorDirection1 = dir;
    motorDirection2 = dir;
    motorDirection3 = dir;
    motorDirection4 = dir;
    updateDirection();
}
/*右转*/
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
    //Delay_ums(TurningTime(TimeOfTurns)); //原转弯方式，按时间
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
/*左转*/
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
    //Delay_ums(TurningTime(TimeOfTurns));//原转弯方式，按时间
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
/*左调5度*/
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
/*右调5度*/
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
/*左调10度*/
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
/*右调10度*/
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
/*整体运动*/
void run(void){
    if(MOTOR == TRUE){
        TURNING = TRUE;
        switch(POSITON){
        case  L_RANGE_1: //左偏5度
            changeL1(direction);
            goStraight(direction);
            break;
        case  L_RANGE_2: //左偏10度
            changeL2(direction);
            goStraight(direction);
            break;
        case  L_TURN: //左转
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
        case  R_TURN: //右转
            if(TimeOfTurns == 0){
                turnRight(direction);
                TimeOfTurns++;
            }
            break;
        case  R_RANGE_1://右偏5度
            changeR1(direction);
            goStraight(direction);
            break;
        case  R_RANGE_2://右偏10度
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
/*开启电机*/
void openMotor(void){
  MOTOR = TRUE;
}
/*关闭电机*/
void stopMotor(void){
  MOTOR = FALSE;
}
/*是否电机打开*/
BOOLEAN isMOTOR(void){
	return MOTOR;
}
/*测试上层装置初始化*/
void testArm(void){
    stopMotor();
		goStraight(STOP);
}
