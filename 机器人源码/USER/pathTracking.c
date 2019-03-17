/**
  ******************************************************************************
  * @file pathTracking.c 
  * @author RoboGame Sonny Team
  * @data 11/9
  * @version 1.1
  * @brief ��ȡ��ǰλ����Ϣ 
  *        λ�þ��� MAT_POSITION
  *        λ�ã���ǰ��������
  *        POSITON 
  *        LOCATION ��1 δ����־��  0 �� 9 ��־��
  * @note: �����ļ� "pathTracking.h"
  ******************************************************************************
**/
/**
  ******************************************************************************
  * ѭ��������λ��˵����
  *                               ��ͷ
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
  *                               ��β
  ******************************************************************************
**/
/**
  *ѭ��˼·�� ֱ��ʱ������LINE1 �� LINE4 ��ֵ�ж��Ƿ�ƫ�뺽�ߣ���ƫ�룬ת��̶���ʱ�䣩������
  *         ����LINE4 && LINE1 (����)
  *         ת��ʱ����ѭ��ȷ��ת�����
**/
#include "pathTracking.h"


extern BOOLEAN TURNING = FALSE;
extern BOOLEAN TURN_RIGHT_SUCCESS = FALSE; //ת�������־������ΪTRUE�����ת��
extern BOOLEAN TURN_LEFT_SUCCESS = FALSE;
extern int LOCATION_BACKUP = -1;
extern int LOCATION = -1;
extern int LOCATION_MAT[4] = {1,1,1,1}; // ȫ1��־Ŀǰδ����־��
/*λ�þ����е�ǰλ��*/
extern u32 POSITON = UNCHANGED;
/*λ�þ���*/
extern u8 MAT_POSITION[12] = {0};
/*�޸�λ�þ���*/
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
/*����Ƿ�ת��ɹ�*/
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
/*����ת�������־*/
void setTurningRestart(void){
    TURN_LEFT_SUCCESS = FALSE;
    TURN_RIGHT_SUCCESS = FALSE;
}
/*��ȡ��־�㣬���±�־������*/
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
