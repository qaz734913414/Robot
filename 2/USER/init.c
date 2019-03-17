/**
  ******************************************************************************
  * @file init.c 
  * @author RoboGame Sonny Team
  * @data 9/9
  * @version 1.0
  * @brief 进行所有初始化工作，包括：
  *        串口USART1初始化：GPIOA 9 TX 10 RX
  *        中断输出口 ： GPIOB 0
  * @note: 依赖文件 "init.h" "rfid.h"
  ******************************************************************************
**/
#include "init.h"
#include "rfid.h"
#include "delay.h"
extern u32 system_time_  = 0;

/**
  ******************************************************************************
  *时钟说明
  *APB1总线时钟为4分频即42M，
  *APB2总线时钟为2分频即84M
  *所以TIM1、TIM8~TIM11的时钟为APB2时钟的两倍即168M
  *TIM2~TIM7、TIM12~TIM14的时钟为APB1的时钟的两倍即84M
  *延时公式  Tout = ((arr+1)*(psc+1))/Tclk
  ******************************************************************************
*/
/*中断产生端口初始化*/
void init_int(void){
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/*串口USART1初始化*/
void init_USART1(void){
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
    //串口1对应引脚复用映射
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
    
    //USART1端口配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	//速度50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
    GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
    USART_InitStructure.USART_BaudRate = BOUND;//波特率设置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART1, &USART_InitStructure); //初始化串口1
    
    USART_Cmd(USART1, ENABLE);  //使能串口1 
    
    USART_ClearFlag(USART1, USART_FLAG_TC);
    
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}
/*串口USART1中断辅助变量*/
extern u8 USART1_RX_BUF[USART1_RX_LEN_MAX] = {0,0,0,0,0,0,0,0,0xFF};     //接收缓冲,最大USART1_RX_LEN_MAX个字节.
extern u8 USART1_RX_LEN = USART1_RX_LEN_MAX;//接收缓冲实际利用空间
extern u16 USART1_RX_STA = 0;       //接收状态标记	
/*USART1中断处理函数*/
void USART1_IRQHandler(void){
    int i = 0;
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
    {
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);      //清楚接收中断标志位
        if(USART1_RX_STA == 0){
            //初始化缓冲区空间			 
            for(i = 0; i < USART1_RX_LEN_MAX; i++){
                USART1_RX_BUF[i] = 0x00;
            }				                          
            USART1_RX_BUF[8] = 0xFF;//确保第9位非00	
        }
        USART1_RX_BUF[USART1_RX_STA] = USART_ReceiveData(USART1); //接收数据
        USART1_RX_STA++; //切换到下一字节
        if(USART1_RX_STA == USART1_RX_LEN) //数据接收完成，复位缓冲区
            USART1_RX_STA = 0;
   }
    if(USART_GetITStatus(USART1,USART_IT_TXE)!=RESET){      //判断是否为发送中断
        USART_ClearITPendingBit(USART1,USART_IT_TXE);       //清除发送中断标志位
    }
    if(USART_GetITStatus(USART1,USART_IT_TC)!=RESET){	    //判断是否为发送完成中断
        USART_ClearITPendingBit(USART1,USART_IT_TC);	    //清除发送完成中断标志位
    } 
} 

void TIM4_Init(void){
    //10Hz
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///使能TIM4时钟
    
    TIM_TimeBaseInitStructure.TIM_Period =999; 	//自动重装载值
    TIM_TimeBaseInitStructure.TIM_Prescaler=8339;  //定时器分频 10KHz
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
    
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//初始化TIM4
    
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //允许定时器4更新中断

    TIM_Cmd(TIM4,ENABLE); //使能定时器4
    
    NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //定时器4中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //子优先级0
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //清除中断标志位	
}
/*定时器4中断服务函数*/
void TIM4_IRQHandler(void){
    if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //溢出中断
    {
        system_time_++;
    }
    TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //清除中断标志位
}
/*全部初始化*/
void init(void){
    init_int();
    init_USART1();
    GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	delay_init(SystemCoreClock/1000000);
}
