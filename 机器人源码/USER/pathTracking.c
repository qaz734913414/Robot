/**
  ******************************************************************************
  * @file pathTracking.c 
  * @author RoboGame Sonny Team
  * @data 11/9
  * @version 1.1
  * @brief 读取当前位置信息 
  *        位置矩阵 MAT_POSITION
  *        位置：当前动作命令
  *        POSITON 
  *        LOCATION －1 未到标志点  0 － 9 标志点
  * @note: 依赖文件 "pathTracking.h"
  ******************************************************************************
**/
/**
  ******************************************************************************
  * 循迹传感器位置说明：
  *                               车头
  *       
  *
  *
  *              |LINE1|   |LINE2|   |LINE3|   |LINE4|      
  * |LINE0|                                                 |LINE5|
  *              |LINE9|   |LINE8|   |LINE7|   |LINE6|      
  *
  *
  *
  *     
  *                               车尾
  ******************************************************************************
**/
/**
  *循迹思路： 直行时，根据LINE1 ～ LINE4 的值判断是否偏离航线，若偏离，转向固定（时间），优先
  *         考察LINE4 && LINE1 (正向)
  *         转弯时，依循迹确定转弯结束
**/
#include "pathTracking.h"


extern BOOLEAN TURNING = FALSE;
extern BOOLEAN TURN_RIGHT_SUCCESS = FALSE; //转弯结束标志量，若为TRUE则结束转弯
extern BOOLEAN TURN_LEFT_SUCCESS = FALSE;
extern int LOCATION_BACKUP = -1;
extern int LOCATION = -1;
extern int LOCATION_MAT[4] = {1,1,1,1}; // 全1标志目前未到标志点
/*位置矩阵中当前位置*/
extern u32 POSITON = UNCHANGED;
/*位置矩阵*/
extern u8 MAT_POSITION[12] = {0};
/*修改位置矩阵*/
void setCurPositionMat_And_Position(void){
    MAT_POSITION[0] = LINE1_1;
    MAT_POSITION[1] = LINE1_2; 
    MAT_POSITION[2] = LINE1_3;
    MAT_POSITION[3] = LINE1_4;
    MAT_POSITION[4] = LINE2_1;
    MAT_POSITION[5] = LINE2_2;
    MAT_POSITION[6] = LINE2_3;
    MAT_POSITION[7] = LINE2_4;
    MAT_POSITION[8] = LINE3_1;
    MAT_POSITION[9] = LINE3_2;
    // MAT_POSITION[10] = LINE3_3;
    // MAT_POSITION[11] = LINE3_4;

    if(direction == FRONT){
        if(MAT_POSITION[0]){
            POSITON = L_TURN;
        }else if(MAT_POSITION[5]){
            POSITON = R_TURN;
        }else if(MAT_POSITION[1]){
            POSITON = L_RANGE_2;
        }else if(MAT_POSITION[4]){
            POSITON = R_RANGE_2;
        }else if(MAT_POSITION[2]){
            POSITON = L_RANGE_1;
        }else if(MAT_POSITION[3]){
            POSITON = R_RANGE_1;
        }else{
            POSITON = UNCHANGED;
        }
    }else{
        if(MAT_POSITION[0]){
            POSITON = R_TURN;
        }else if(MAT_POSITION[5]){
            POSITON = L_TURN;
        }else if(MAT_POSITION[9]){
            POSITON = R_RANGE_2;
        }else if(MAT_POSITION[6]){
            POSITON = L_RANGE_2;
        }else if(MAT_POSITION[8]){
            POSITON = R_RANGE_1;
        }else if(MAT_POSITION[7]){
            POSITON = L_RANGE_1;
        }else{
            POSITON = UNCHANGED;
        }
    }
}
/*检测是否转弯成功*/
void testHadTuringSuccess(void){
    MAT_POSITION[0] = LINE1_1;
    MAT_POSITION[1] = LINE1_2; 
    MAT_POSITION[2] = LINE1_3;
    MAT_POSITION[3] = LINE1_4;
    MAT_POSITION[4] = LINE2_1;
    MAT_POSITION[5] = LINE2_2;
    MAT_POSITION[6] = LINE2_3;
    MAT_POSITION[7] = LINE2_4;
    MAT_POSITION[8] = LINE3_1;
    MAT_POSITION[9] = LINE3_2;
    MAT_POSITION[10] = LINE3_3;
    MAT_POSITION[11] = LINE3_4;

    if(direction == FRONT){
        if(MAT_POSITION[10]){
            TURN_LEFT_SUCCESS = TRUE;
            TURN_RIGHT_SUCCESS = TRUE;
        }
    }else{
        if(MAT_POSITION[11]){
            TURN_LEFT_SUCCESS = TRUE;
            TURN_RIGHT_SUCCESS = TRUE;
        }
    }
}
/*重置转弯结束标志*/
void setTurningRestart(void){
    TURN_LEFT_SUCCESS = FALSE;
    TURN_RIGHT_SUCCESS = FALSE;
}
/*读取标志点，更新标志点数组*/
void setLocationMat(void){
    LOCATION_MAT[0] = RFID_1;
    LOCATION_MAT[1] = RFID_2;
    LOCATION_MAT[2] = RFID_3;
    LOCATION_MAT[3] = RFID_4;
    if(LOCATION_MAT[0]){
        if(!LOCATION_MAT[2]&&!LOCATION_MAT[1]){
            // 1001
            LOCATION = 9;
        }else{
            LOCATION = -1;
        }
    }else{
        if(LOCATION_MAT[1]){
            if(LOCATION_MAT[2]){
                if(LOCATION_MAT[3]){
                    // 0111
                    LOCATION = 7;
                }else{
                    // 0110
                    LOCATION = 6;
                }
            }else{
                if(LOCATION_MAT[3]){
                    // 0101
                    LOCATION = 5;
                }else{
                    // 0100
                    LOCATION = 4;
                }
            }
        }else{
						if(LOCATION_MAT[2]){
								if(LOCATION_MAT[3]){
												// 0011
												LOCATION = 3;
										}else{
												// 0010
												LOCATION = 2;
										}
						}else{
										if(LOCATION_MAT[3]){
												// 0001
												LOCATION = 1;
										}else{
												// 0000
												LOCATION = 0;
										}
								}
        }
    }
}
