/**
  ******************************************************************************
  * @file rfid.c 
  * @author RoboGame Sonny Team
  * @data 11/9
  * @version 1.1
  * @brief 设置rfid
  *        开放取书函数 readBooks()
  * @note: 依赖文件 "rfid.h" "inti.h" "helper.h"
  ******************************************************************************
**/
#include "rfid.h"
#include "init.h"
#include "helper.h"
/**
  * 发送：
  * 命令头 + 长度字 + 设备标识符 + 命令码 + 命令参数 + 校验字
  * 命 令 头:2BYTE,0xAABB
  * 长 度 字:2BYTE,指明后续的字节数,从设备标识到校验字,第一字节有效,第二字节保留为0 
  * 设备标识:2BYTE,模块只响应设备标识与自身设备标识一致或设备标识为0x0000(广播)的命令
  * 命 令 码:2BYTE,标识命令功能的编码
  * 命令参数:命令报文(可以为空)
  * 校 验 字:1BYTE,设备标识到命令参数最后一字节的逐字节异或
  * 注:除命令头及校验字之外,若数据中有一个字节为 0xAA,则其后应紧跟一个 0x00,长度字不变.
  * 接收：
  * 命令头 + 长度字 + 设备标识符 + 命令码 + 状态字 + 命令参数 + 校验字
  * 命 令 头:2BYTE,0xAABB
  * 长 度 字:2BYTE,指明后续的字节数,从设备标识到校验字,第一字节有效,第二字节保留为0 
  * 设备标识:2BYTE,模块设备标识
  * 命 令 码:2BYTE,标识命令功能的编码
  * 状 态 字:1BYTE,00 = 成功; 非零 = 失败 
  * 命令参数:命令报文(可以为空)
  * 校 验 字:1BYTE,设备标识到命令参数最后一字节的逐字节异或
  * 注:除命令头及校验字之外,若数据中有一个字节为 0xAA,则其后应紧跟一个 0x00,长度字不变.
**/
static u8 OPEN[10] = {0xAA,0xBB,0x06,0x00,0x00,0x00,0x0C,0x01,0x11,0x1C};//开启天线
static u8 SETMODE[10] = {0xAA,0xBB,0x06,0x00,0x00,0x00,0x08,0x01,0x31,0x38};//设置工作模式为ISO15693
static u8 INVENTORYS[9] = {0xAA,0xBB,0x05,0x00,0x00,0x00,0x01,0x10,0x11};//读取单卡UID
static u8 READ[20] = {0xAA,0xBB,0x10,0x00,0x00,0x00,0x05,0x10,0x02};//读取数据
static u8 REDLIGHT[10] = {0xAA,0xBB,0x06,0x00,0x00,0x00,0x07,0x01,0x01,0x07};//红灯亮
static u8 GREENLIGHT[10] = {0xAA,0xBB,0x06,0x00,0x00,0x00,0x07,0x01,0x02,0x04};//绿灯亮
static u8 TURNOFFLIGHT[10] = {0xAA,0xBB,0x06,0x00,0x00,0x00,0x07,0x01,0x00,0x06};//关闭所有指示灯
#define OPEN_SIZE 10
#define SETMODE_SIZE 10
#define INVENTORYS_SIZE 9
#define READ_SIZE 20
#define LIGHT_SIZE 10
/**
  *book读取到的内容说明：
  *前三字节信息分别为 0x01 0x02 0x03,则表示图书位于第一书架第二层第三格的位置。
**/
u8 book[5][3] = {0}; //保存读取到的书的位置信息，共5本书，每本占3字节
u8 countBook = 0;   //计数指针，指示当前读到第（countBook ＋ 1）本书
u8 Data[3] = {0};   //RFID读取到的内容
u8 UID[8] = {0}; //64bits UID值
extern u8 locationTag = 0xFF;//当前定位点标签值
extern u8 RecentBookNumber = 0;//当前放到第几本书
extern u8 SortedBooks[5][3]={1,0,2,2,1,2,9,2,2,8,3,2,7,4,2};//[i][j]中i表示放书顺序,j为0表示对应标签号,j为1表示书在机器人书架上的位置,j为2表示要放到几层

//将book数组中的书本信息排序后放入SortedBooks数组
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
        book[k][0]=0;//书本k退出排序
    }
}
/*初始化电子标签读写器*/
void openRFID(void)
{
    int i = 0;
    int success = 0;//测试是否读取成功
    USART1_RX_LEN = 10;
    while(!success){
        Delay_mms(10);
        for(i = 0; i < OPEN_SIZE; i++){     //打开读卡器天线
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
        for(i = 0; i < SETMODE_SIZE; i++){      //设置工作模式ISO159693
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
/*读取电子标签UID值*/
void getUID(void)
{
    int i = 0;
    int success = 0;//测试是否读取成功
    USART1_RX_LEN = 19;
    while(!success){
        Delay_mms(10);
        for(i = 0; i < INVENTORYS_SIZE; i++){ //读取UID号
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
        UID[i] = USART1_RX_BUF[i + 10];//写入UID值：第10-17个字节
    }										 
}
/*读取电子标签信息*/
void readRFID(void)
{
    //READ 命令参数：
    //1BYTE model + 8BYTE UID + 1BYTE 起始块号 + 1BYTE 块数  + 校验字
    //      9byte   10-17byte         18byte       19byte   20byte
    int i = 0;
    int success = 0;//测试是否读取成功
    //写入READ 命令报文 第10 － 20 byte
    for(i = 9; i < 17; i++){
        READ[i] = UID[i - 9]; //写入10-17byte,UID
    }
    READ[17] = 0x00;//起始块号
    READ[18] = 0x01;//块数
    //计算校验位
    READ[19] = READ[4];
    for(i = 5; i < 19; i++){
        READ[19] = READ[19] ^ READ[i];
    }								
    USART1_RX_LEN = 14;
    while(!success){
        Delay_mms(10);
        for(i = 0; i < READ_SIZE; i++){//读取书签信息
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
    //READ 输出参数:
    //1BYTE 书架号 + 1BYTE 层号 + 1BYTE 格号 + 1BYTE 0x00
    //     byte10      byte11      byte12        byte13
    for(i = 0; i < 3; i++){
        Data[i] = USART1_RX_BUF[i + 9];
    }								 
}
/*亮红色指示灯*/
void openRedLight(void)
{
    int i = 0;
    for(i = 0; i < LIGHT_SIZE; i++){//开红灯
        Delay_mms(1);
        USART_SendData(USART1,REDLIGHT[i]);
    } 
    Delay_mms(10);
    USART1_RX_STA = 0;
}
/*亮绿色指示灯*/
void openGreenLight(void)
{
    int i = 0;
    for(i = 0; i < LIGHT_SIZE; i++){//开绿灯
        Delay_mms(1);
        USART_SendData(USART1,GREENLIGHT[i]);
    } 
    Delay_mms(10);
    USART1_RX_STA = 0;
}
/*关闭指示灯*/
void turnOffAllLight(void){
    int i = 0;
    for(i = 0; i < LIGHT_SIZE; i++){//关灯
        Delay_mms(1);
        USART_SendData(USART1,TURNOFFLIGHT[i]);
    } 
    Delay_mms(10);
    USART1_RX_STA = 0;
}
/*保存单本书信息并写入book数组*/
void saveBook()
{
    //5本书中将有2本处于书架第二层,2本处于书架第三层,1本处于书架第四层。
    int i = 0,j = 0,exit = 0;
    for(i = 0; i < 5; i++){ //遍历book数组查重
        for(j = 0; j < 3; j++){
            if(book[i][j] != Data[j]) //如果不重，保持exit ＝ 0，检查是否与下一本重复
                break;
            else if(j == 2){//如果已经重复，跳出循环
                exit = 1;
                goto STOP_SEARCHING;
                        }
            else{
                continue;
                        }
        }
    }
STOP_SEARCHING:
    if(!exit){//均未重复，加入新书，并更新计数指针
        for(i = 0; i < 3; i++){										
            book[countBook][i] = Data[i];
        }
        countBook++;
                if(countBook == 4) Sort();
    }
}
/*读取5本图书RFID信息*/
void readBooks(void){
    openRFID();
    while(countBook < BOOK_NUM){
        turnOffAllLight();
        Delay_mms(500);
        getUID();
        openRedLight();//红灯显示读区UID成功
        Delay_mms(100);
        readRFID();
        saveBook();
        openGreenLight();//绿灯显示一本书信息读区写入成功
        Delay_mms(500);
    }	
}
