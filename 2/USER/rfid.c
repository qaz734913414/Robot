/**
  ******************************************************************************
  * @file rfid.c 
  * @author RoboGame Sonny Team
  * @data 7/9
  * @version 1.0
  * @brief ����rfid
  *        ͼ���ǩ��λ���� readLocationTag()
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
#define DELAY_TIME 200
/**
  *book��ȡ��������˵����
  *ǰ���ֽ���Ϣ�ֱ�Ϊ 0x01 0x02 0x03,���ʾͼ��λ�ڵ�һ��ܵڶ���������λ�á�
**/
u8 Data[3] = {0xFF,0xFF,0xFF};   //RFID��ȡ��������
u8 UID[8] = {0}; //64bits UIDֵ
u8 rightTagOrder[10] = {0,1,2,3,4,9,8,7,6,5}; //��ǩ����ȷ˳��
extern u8 nowTagNum = 0; //��ǰ�Ѷ�ȡ���ı�ǩ��
extern u8 locationTag = 0xFF;//��ǰ��λ���ǩֵ

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
    u32 start_time = system_time_;
    u32 now_time = start_time;
    while(!success){
        delay_ms(19);
				//Delay_mms(2);
        for(i = 0; i < INVENTORYS_SIZE; i++){ //��ȡUID��
            delay_ms(7);
						//Delay_mms(1);
            USART_SendData(USART1,INVENTORYS[i]);
        }
				delay_ms(19);
				//Delay_mms(2);
        if(USART1_RX_BUF[8] == 0x00){
            success = 1;
        }else{
            success = 0;
            USART1_RX_STA = 0;
        }
        now_time = system_time_;
        if(start_time - now_time > 15){
            break;
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
    u32 start_time = system_time_;
    u32 now_time = start_time;
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
        delay_ms(19);
				//Delay_mms(2);
        for(i = 0; i < READ_SIZE; i++){//��ȡ��ǩ��Ϣ
            delay_ms(7);
						//Delay_mms(1);
            USART_SendData(USART1,READ[i]);
        }
				delay_ms(19);
				//Delay_mms(2);
        if(USART1_RX_BUF[8] == 0x00){
            success = 1;
        }else{
            success = 0;
            USART1_RX_STA = 0;
        }
        now_time = system_time_;
        if(now_time - start_time > 15){
            break;
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

/*������λ����Ϣ*/
void readLocationTag(void)
{
    getUID();
    openRedLight();
    readRFID();
    openGreenLight();
    locationTag = Data[0];
    switch(locationTag){
        case 0:
				case 0x16:
        // 0000
						delay_ms(200);
            GPIO_ResetBits(GPIOB,GPIO_Pin_0);
            GPIO_ResetBits(GPIOB,GPIO_Pin_1);
            GPIO_ResetBits(GPIOB,GPIO_Pin_2);
            GPIO_ResetBits(GPIOB,GPIO_Pin_3);
            Delay_mms(DELAY_TIME);
            resetLocationTag(); 
            break;
        case 1:
				case 0x17:
        // 0001
						delay_ms(200);
            GPIO_ResetBits(GPIOB,GPIO_Pin_0);
            GPIO_ResetBits(GPIOB,GPIO_Pin_1);
            GPIO_ResetBits(GPIOB,GPIO_Pin_2);
            GPIO_SetBits(GPIOB,GPIO_Pin_3);
            Delay_mms(DELAY_TIME);
            resetLocationTag();
            break;
        case 2:
				case 0x18:
        // 0010
						delay_ms(200);
            GPIO_ResetBits(GPIOB,GPIO_Pin_0);
            GPIO_ResetBits(GPIOB,GPIO_Pin_1);
            GPIO_SetBits(GPIOB,GPIO_Pin_2);
            GPIO_ResetBits(GPIOB,GPIO_Pin_3);
            Delay_mms(DELAY_TIME);
            resetLocationTag(); 
            break;
        case 3:
				case 0x19:
        // 0011
						Delay_ms(200);
            GPIO_ResetBits(GPIOB,GPIO_Pin_0);
            GPIO_ResetBits(GPIOB,GPIO_Pin_1);
            GPIO_SetBits(GPIOB,GPIO_Pin_2);
            GPIO_SetBits(GPIOB,GPIO_Pin_3);
            Delay_mms(DELAY_TIME);
            resetLocationTag();  
            break;
        case 4:
				case 0x1A:
        // 0100
						Delay_ms(200);
            GPIO_ResetBits(GPIOB,GPIO_Pin_0);
            GPIO_SetBits(GPIOB,GPIO_Pin_1);
            GPIO_ResetBits(GPIOB,GPIO_Pin_2);
            GPIO_ResetBits(GPIOB,GPIO_Pin_3);
            Delay_mms(DELAY_TIME);
            resetLocationTag();
            break;
        case 5:
				case 0x1B:
        // 0101
						Delay_ms(200);
            GPIO_ResetBits(GPIOB,GPIO_Pin_0);
            GPIO_SetBits(GPIOB,GPIO_Pin_1);
            GPIO_ResetBits(GPIOB,GPIO_Pin_2);
            GPIO_SetBits(GPIOB,GPIO_Pin_3);
            Delay_mms(DELAY_TIME);
            resetLocationTag(); 
            break;
        case 6:
				case 0x1C:
        // 0110
						Delay_ms(200);
            GPIO_ResetBits(GPIOB,GPIO_Pin_0);
            GPIO_SetBits(GPIOB,GPIO_Pin_1);
            GPIO_SetBits(GPIOB,GPIO_Pin_2);
            GPIO_ResetBits(GPIOB,GPIO_Pin_3);
            Delay_mms(DELAY_TIME);
            resetLocationTag(); 
            break;
        case 7:
				case 0x1D:
        // 0111
						Delay_ms(200);
            GPIO_ResetBits(GPIOB,GPIO_Pin_0);
            GPIO_SetBits(GPIOB,GPIO_Pin_1);
            GPIO_SetBits(GPIOB,GPIO_Pin_2);
            GPIO_SetBits(GPIOB,GPIO_Pin_3);
            Delay_mms(DELAY_TIME);
            resetLocationTag();
            break;
        case 8:
				case 0x1E:
        // 1000
						Delay_ms(200);
            GPIO_SetBits(GPIOB,GPIO_Pin_0);
            GPIO_ResetBits(GPIOB,GPIO_Pin_1);
            GPIO_ResetBits(GPIOB,GPIO_Pin_2);
            GPIO_ResetBits(GPIOB,GPIO_Pin_3);
            Delay_mms(DELAY_TIME);
            resetLocationTag();
            break;
        case 9:
				case 0x1F:
        // 1001
						Delay_ms(200);
            GPIO_SetBits(GPIOB,GPIO_Pin_0);
            GPIO_ResetBits(GPIOB,GPIO_Pin_1);
            GPIO_ResetBits(GPIOB,GPIO_Pin_2);
            GPIO_SetBits(GPIOB,GPIO_Pin_3);
            Delay_mms(DELAY_TIME);
            resetLocationTag();
            break;
        default:
            resetLocationTag();
            break;
    }
}
void resetLocationTag(void){
    GPIO_SetBits(GPIOB,GPIO_Pin_0);
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
	GPIO_SetBits(GPIOB,GPIO_Pin_2);
	GPIO_SetBits(GPIOB,GPIO_Pin_3);
}
