/**
  ******************************************************************************
  * @file rfid.c 
  * @author RoboGame Sonny Team
  * @data 11/9
  * @version 1.1
  * @brief ����rfid
  *        ����ȡ�麯�� readBooks()
  * @note: �����ļ� "rfid.h" "inti.h" "helper.h"
  ******************************************************************************
**/
#include "rfid.h"
#include "init.h"
#include "helper.h"
/**
  * ���ͣ�
  * ����ͷ + ������ + �豸��ʶ�� + ������ + ������� + У����
  * �� �� ͷ:2BYTE,0xAABB
  * �� �� ��:2BYTE,ָ���������ֽ���,���豸��ʶ��У����,��һ�ֽ���Ч,�ڶ��ֽڱ���Ϊ0 
  * �豸��ʶ:2BYTE,ģ��ֻ��Ӧ�豸��ʶ�������豸��ʶһ�»��豸��ʶΪ0x0000(�㲥)������
  * �� �� ��:2BYTE,��ʶ����ܵı���
  * �������:�����(����Ϊ��)
  * У �� ��:1BYTE,�豸��ʶ������������һ�ֽڵ����ֽ����
  * ע:������ͷ��У����֮��,����������һ���ֽ�Ϊ 0xAA,�����Ӧ����һ�� 0x00,�����ֲ���.
  * ���գ�
  * ����ͷ + ������ + �豸��ʶ�� + ������ + ״̬�� + ������� + У����
  * �� �� ͷ:2BYTE,0xAABB
  * �� �� ��:2BYTE,ָ���������ֽ���,���豸��ʶ��У����,��һ�ֽ���Ч,�ڶ��ֽڱ���Ϊ0 
  * �豸��ʶ:2BYTE,ģ���豸��ʶ
  * �� �� ��:2BYTE,��ʶ����ܵı���
  * ״ ̬ ��:1BYTE,00 = �ɹ�; ���� = ʧ�� 
  * �������:�����(����Ϊ��)
  * У �� ��:1BYTE,�豸��ʶ������������һ�ֽڵ����ֽ����
  * ע:������ͷ��У����֮��,����������һ���ֽ�Ϊ 0xAA,�����Ӧ����һ�� 0x00,�����ֲ���.
**/
static u8 OPEN[10] = {0xAA,0xBB,0x06,0x00,0x00,0x00,0x0C,0x01,0x11,0x1C};//��������
static u8 SETMODE[10] = {0xAA,0xBB,0x06,0x00,0x00,0x00,0x08,0x01,0x31,0x38};//���ù���ģʽΪISO15693
static u8 INVENTORYS[9] = {0xAA,0xBB,0x05,0x00,0x00,0x00,0x01,0x10,0x11};//��ȡ����UID
static u8 READ[20] = {0xAA,0xBB,0x10,0x00,0x00,0x00,0x05,0x10,0x02};//��ȡ����
static u8 REDLIGHT[10] = {0xAA,0xBB,0x06,0x00,0x00,0x00,0x07,0x01,0x01,0x07};//�����
static u8 GREENLIGHT[10] = {0xAA,0xBB,0x06,0x00,0x00,0x00,0x07,0x01,0x02,0x04};//�̵���
static u8 TURNOFFLIGHT[10] = {0xAA,0xBB,0x06,0x00,0x00,0x00,0x07,0x01,0x00,0x06};//�ر�����ָʾ��
#define OPEN_SIZE 10
#define SETMODE_SIZE 10
#define INVENTORYS_SIZE 9
#define READ_SIZE 20
#define LIGHT_SIZE 10
/**
  *book��ȡ��������˵����
  *ǰ���ֽ���Ϣ�ֱ�Ϊ 0x01 0x02 0x03,���ʾͼ��λ�ڵ�һ��ܵڶ���������λ�á�
**/
u8 book[5][3] = {0}; //�����ȡ�������λ����Ϣ����5���飬ÿ��ռ3�ֽ�
u8 countBook = 0;   //����ָ�룬ָʾ��ǰ�����ڣ�countBook �� 1������
u8 Data[3] = {0};   //RFID��ȡ��������
u8 UID[8] = {0}; //64bits UIDֵ
extern u8 locationTag = 0xFF;//��ǰ��λ���ǩֵ
extern u8 RecentBookNumber = 0;//��ǰ�ŵ��ڼ�����
extern u8 SortedBooks[5][3]={1,0,2,2,1,2,9,2,2,8,3,2,7,4,2};//[i][j]��i��ʾ����˳��,jΪ0��ʾ��Ӧ��ǩ��,jΪ1��ʾ���ڻ���������ϵ�λ��,jΪ2��ʾҪ�ŵ�����

//��book�����е��鱾��Ϣ��������SortedBooks����
void Sort()
{
    int i,j,k;
    for(i=0;i<5;i++){
        k=0;	
        for(j=0;j<5;j++){
            if(book[k][0]>book[j][0]) continue;
            if(book[k][0]<book[j][0]) k=j;
            else{
                if(book[k][0]==1){
                    if(book[j][2]>book[k][2]) k=j;
                }
                else if(book[k][0]==2){
                    if(book[j][2]<book[k][2]) k=j;
                }
            }
        }
        SortedBooks[i][0]=10-5*book[k][0]+book[k][2]-1;
				if(SortedBooks[i][0]>4) SortedBooks[i][0]++;
        SortedBooks[i][1]=k;
        SortedBooks[i][2]=book[k][1];
        book[k][0]=0;//�鱾k�˳�����
    }
}
/*��ʼ�����ӱ�ǩ��д��*/
void openRFID(void)
{
    int i = 0;
    int success = 0;//�����Ƿ��ȡ�ɹ�
    USART1_RX_LEN = 10;
    while(!success){
        Delay_mms(10);
        for(i = 0; i < OPEN_SIZE; i++){     //�򿪶���������
            Delay_mms(1);
            USART_SendData(USART1,OPEN[i]);
        } 
        Delay_mms(10);
        if(USART1_RX_BUF[8] == 0x00){
            success = 1;
        }else{
            success = 0;
            USART1_RX_STA = 0;										 
        }
            }
    success = 0;
    USART1_RX_LEN = 10;
    while(!success){
        Delay_mms(10);
        for(i = 0; i < SETMODE_SIZE; i++){      //���ù���ģʽISO159693
            Delay_mms(1);
            USART_SendData(USART1,SETMODE[i]);
        }
        Delay_mms(10);
        if(USART1_RX_BUF[8] == 0x00){
            success = 1;
        }else{
            success = 0;
            USART1_RX_STA = 0;
        }			                                
    }							
}
/*��ȡ���ӱ�ǩUIDֵ*/
void getUID(void)
{
    int i = 0;
    int success = 0;//�����Ƿ��ȡ�ɹ�
    USART1_RX_LEN = 19;
    while(!success){
        Delay_mms(10);
        for(i = 0; i < INVENTORYS_SIZE; i++){ //��ȡUID��
            Delay_mms(1);
            USART_SendData(USART1,INVENTORYS[i]);
        }
        Delay_mms(10);
        if(USART1_RX_BUF[8] == 0x00){
            success = 1;
        }else{
            success = 0;
            USART1_RX_STA = 0;
        }
    }
    for(i = 0; i < 8; i++){
        UID[i] = USART1_RX_BUF[i + 10];//д��UIDֵ����10-17���ֽ�
    }										 
}
/*��ȡ���ӱ�ǩ��Ϣ*/
void readRFID(void)
{
    //READ ���������
    //1BYTE model + 8BYTE UID + 1BYTE ��ʼ��� + 1BYTE ����  + У����
    //      9byte   10-17byte         18byte       19byte   20byte
    int i = 0;
    int success = 0;//�����Ƿ��ȡ�ɹ�
    //д��READ ����� ��10 �� 20 byte
    for(i = 9; i < 17; i++){
        READ[i] = UID[i - 9]; //д��10-17byte,UID
    }
    READ[17] = 0x00;//��ʼ���
    READ[18] = 0x01;//����
    //����У��λ
    READ[19] = READ[4];
    for(i = 5; i < 19; i++){
        READ[19] = READ[19] ^ READ[i];
    }								
    USART1_RX_LEN = 14;
    while(!success){
        Delay_mms(10);
        for(i = 0; i < READ_SIZE; i++){//��ȡ��ǩ��Ϣ
            Delay_mms(1);
            USART_SendData(USART1,READ[i]);
        }
        Delay_mms(10);
        if(USART1_RX_BUF[8] == 0x00){
            success = 1;
        }else{
            success = 0;
            USART1_RX_STA = 0;
        }
    }
    //READ �������:
    //1BYTE ��ܺ� + 1BYTE ��� + 1BYTE ��� + 1BYTE 0x00
    //     byte10      byte11      byte12        byte13
    for(i = 0; i < 3; i++){
        Data[i] = USART1_RX_BUF[i + 9];
    }								 
}
/*����ɫָʾ��*/
void openRedLight(void)
{
    int i = 0;
    for(i = 0; i < LIGHT_SIZE; i++){//�����
        Delay_mms(1);
        USART_SendData(USART1,REDLIGHT[i]);
    } 
    Delay_mms(10);
    USART1_RX_STA = 0;
}
/*����ɫָʾ��*/
void openGreenLight(void)
{
    int i = 0;
    for(i = 0; i < LIGHT_SIZE; i++){//���̵�
        Delay_mms(1);
        USART_SendData(USART1,GREENLIGHT[i]);
    } 
    Delay_mms(10);
    USART1_RX_STA = 0;
}
/*�ر�ָʾ��*/
void turnOffAllLight(void){
    int i = 0;
    for(i = 0; i < LIGHT_SIZE; i++){//�ص�
        Delay_mms(1);
        USART_SendData(USART1,TURNOFFLIGHT[i]);
    } 
    Delay_mms(10);
    USART1_RX_STA = 0;
}
/*���浥������Ϣ��д��book����*/
void saveBook()
{
    //5�����н���2��������ܵڶ���,2��������ܵ�����,1��������ܵ��Ĳ㡣
    int i = 0,j = 0,exit = 0;
    for(i = 0; i < 5; i++){ //����book�������
        for(j = 0; j < 3; j++){
            if(book[i][j] != Data[j]) //������أ�����exit �� 0������Ƿ�����һ���ظ�
                break;
            else if(j == 2){//����Ѿ��ظ�������ѭ��
                exit = 1;
                goto STOP_SEARCHING;
                        }
            else{
                continue;
                        }
        }
    }
STOP_SEARCHING:
    if(!exit){//��δ�ظ����������飬�����¼���ָ��
        for(i = 0; i < 3; i++){										
            book[countBook][i] = Data[i];
        }
        countBook++;
                if(countBook == 4) Sort();
    }
}
/*��ȡ5��ͼ��RFID��Ϣ*/
void readBooks(void){
    openRFID();
    while(countBook < BOOK_NUM){
        turnOffAllLight();
        Delay_mms(500);
        getUID();
        openRedLight();//�����ʾ����UID�ɹ�
        Delay_mms(100);
        readRFID();
        saveBook();
        openGreenLight();//�̵���ʾһ������Ϣ����д��ɹ�
        Delay_mms(500);
    }	
}
