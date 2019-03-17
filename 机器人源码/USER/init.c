/**
  ******************************************************************************
  * @file init.c 
  * @author RoboGame Sonny Team
  * @data 11/9
  * @version 1.1
  * @brief 进行所有初始化工作，包括：
  *        循迹模块初始化：GPIOB 0 - 11
  *        底盘电机控制初始化：GPIOF A相 0 - 3 B相 4(8) - 7
  *        底盘电机PWM初始化：GPIOC 6 - 9 PWM1000可调 定时器3
  *	       步进电机方向初始化：GPIOC 0 - 3
  *	       步进电机信号初始化：GPIOF 10 - 13
  *	       舵机信号初始化：GPIOA 6 - 7 GPIOB 14 - 15 TIME12 13 14 CHANNEL 1 TIME12 CHANNEL 2
  *        串口USART1初始化：GPIOA 9 TX 10 RX
  *        基本定时器初始化 :定时器2 1KHz 更新position矩阵值 
  *                       定时器4 50Hz 电机调控
  *                       定时器5 1kHz 管理转弯结束
  * @update:将转弯程序单独踢到一个定时器5去做，取消转弯后延迟判断。改为先延迟一点再又探头数据确定
  * @update:将外部中断取消，改为由4个位 GPIO A 0 －3 读取RFID值 读取程序整合进循迹中。已可以删掉定时器5。
  * @note: 依赖文件 "init.h" "pathTracking.h" "motorContral.h" "rfid.h"
  ******************************************************************************
**/
/**
  ******************************************************************************
  *中断优先级说明
  *00 00:外部中断，读到即停
  *01 00:底盘电机调速
  *01 01:循迹
  *03 03:串口
  ******************************************************************************
**/
#include "init.h"
#include "init.h"
#include "pathTracking.h"
#include "motorContral.h"
#include "rfid.h"
#include "RobotArmMotion.h"
/**
  ******************************************************************************
  *时钟说明
  *APB1总线时钟为4分频即42M，
  *APB2总线时钟为2分频即84M
  *所以TIM1、TIM8~TIM11的时钟为APB2时钟的两倍即168M
  *TIM2~TIM7、TIM12~TIM14的时钟为APB1的时钟的两倍即84M
  *延时公式  Tout = ((arr+1)*(psc+1))/Tclk
  ******************************************************************************
**/
/*定时器3通道1——4输出PWM*/
void TIM3_Init(void){
    //原始频率 84M 控制频率 50kHz
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM3时钟使能    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//使能PORTA时钟	
    
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3); //GPIOC6复用为定时器3
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM3); //GPIOC7复用为定时器3
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM3); //GPIOC8复用为定时器3
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM3); //GPIOC9复用为定时器3
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 |GPIO_Pin_7 |GPIO_Pin_8|GPIO_Pin_9;           //GPIOC 6-9
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //
    GPIO_Init(GPIOC,&GPIO_InitStructure);              //初始化PC6-9
      
    TIM_TimeBaseStructure.TIM_Prescaler=83;  //定时器分频 1MHz
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseStructure.TIM_Period=49;   //重新装入初值 
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
    
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//初始化定时器3
    
    //初始化TIM3 Channel 1-4 PWM模式	 
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
     TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低

    TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //初始化外设TIM3 OC1
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //初始化外设TIM3 OC2
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //初始化外设TIM3 OC3
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //初始化外设TIM3 OC4

    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR3上的预装载寄存器
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR4上的预装载寄存器

    TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPE使能 

    TIM_Cmd(TIM3, ENABLE);  //使能TIM3
}
/*定时器 13 14 通道1 输出舵机控制PWM*/
void TIM12_13_14_Init(void){
    //原始频率 84M 控制频率 1kHz
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);  	//TIM12时钟使能
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,ENABLE);  	//TIM13时钟使能
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  	//TIM14时钟使能        
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTB时钟	

    GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_TIM12); //GPIOB14复用为定时器12
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_TIM12); //GPIOB15复用为定时器12
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM13); //GPIOA6复用为定时器13
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM14); //GPIOA7复用为定时器14

    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 |GPIO_Pin_7;           //GPIOA 6-7
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //
    GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PA6-9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 |GPIO_Pin_15;      //GPIOB 14-15
    GPIO_Init(GPIOB,&GPIO_InitStructure);              //初始化PB 14-15

    TIM_TimeBaseStructure.TIM_Prescaler=1555;  //定时器分频 53984Hz
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseStructure.TIM_Period=179;   //重新装入初值  工作频率300Hz
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 

    TIM_TimeBaseInit(TIM12,&TIM_TimeBaseStructure);//初始化定时器12
    TIM_TimeBaseInit(TIM13,&TIM_TimeBaseStructure);//初始化定时器13
    TIM_TimeBaseInit(TIM14,&TIM_TimeBaseStructure);//初始化定时器14

    //初始化TIM12 13 14 Channel 1 TIM12 Channel 2 PWM模式	 
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
     TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低

    TIM_OC1Init(TIM12, &TIM_OCInitStructure);  //初始化外设TIM12 OC1
    TIM_OC1Init(TIM13, &TIM_OCInitStructure);  //初始化外设TIM13 OC1
    TIM_OC1Init(TIM14, &TIM_OCInitStructure);  //初始化外设TIM14 OC1
    TIM_OC2Init(TIM12, &TIM_OCInitStructure);  //初始化外设TIM12 OC2

    TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Enable);  //使能TIM12在CCR1上的预装载寄存器
    TIM_OC1PreloadConfig(TIM13, TIM_OCPreload_Enable);  //使能TIM13在CCR1上的预装载寄存器
    TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
    TIM_OC2PreloadConfig(TIM12, TIM_OCPreload_Enable);  //使能TIM12在CCR2上的预装载寄存器
  
    TIM_ARRPreloadConfig(TIM12,ENABLE);//ARPE使能 
    TIM_ARRPreloadConfig(TIM13,ENABLE);//ARPE使能 
    TIM_ARRPreloadConfig(TIM14,ENABLE);//ARPE使能 

    TIM_Cmd(TIM12, ENABLE);  //使能TIM12
    TIM_Cmd(TIM13, ENABLE);  //使能TIM13
    TIM_Cmd(TIM14, ENABLE);  //使能TIM14
}
/*定时器2 更新position矩阵值 1kHz*/
void TIM2_Init(void){
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///使能TIM2时钟
    
    TIM_TimeBaseInitStructure.TIM_Period =199; 	//自动重装载值
    TIM_TimeBaseInitStructure.TIM_Prescaler=839;  //定时器分频 100KHz
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
    
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//初始化TIM2
    
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //允许定时器2更新中断
    TIM_Cmd(TIM2,ENABLE); //使能定时器2
    
    NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //定时器2中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
}
/*定时器2中断服务函数*/
void TIM2_IRQHandler(void){
    if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //溢出中断
    {
        if(TURNING == FALSE){
            setCurPositionMat_And_Position();
        }
        setLocationMat();
        if(LOCATION == SortedBooks[RecentBookNumber][0]&&ready_to_insert==NO){
            stopMotor();
            ready_to_insert=YES;
        }
				if(isMOTOR() == FALSE){
					LOCATION_BACKUP = LOCATION;
				}
    }
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
}
/*定时器4 电机调控*/
void TIM4_Init(void){
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///使能TIM4时钟
    
    TIM_TimeBaseInitStructure.TIM_Period =499; 	//自动重装载值
    TIM_TimeBaseInitStructure.TIM_Prescaler=8339;  //定时器分频 10KHz
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
    
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//初始化TIM4
    
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //允许定时器4更新中断

    TIM_Cmd(TIM4,ENABLE); //使能定时器4
    
    NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //定时器4中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //子优先级0
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //清除中断标志位	
}
/*定时器4中断服务函数*/
void TIM4_IRQHandler(void){
    if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //溢出中断
    {
        if(TURNING == FALSE){
            run();
        }
    }
    TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //清除中断标志位
}
/*循迹模块初始化*/
void init_PathTracking(void){
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1 |GPIO_Pin_2 |GPIO_Pin_3 |
                                GPIO_Pin_4 |GPIO_Pin_5 |GPIO_Pin_6 |GPIO_Pin_7 |
                                GPIO_Pin_8 |GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//默认拉低，信号无效
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/*电机控制模块初始化*/
void init_MotorContral(void){
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1 |GPIO_Pin_2 |GPIO_Pin_3 |GPIO_Pin_8 |GPIO_Pin_5 |GPIO_Pin_6 |GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
    SET_DIR_L1;
    SET_DIR_L2;
    SET_DIR_R1;
    SET_DIR_R2;
}
/*步进电机方向控制初始化*/
void init_StepperMotorDir(void){
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1 |GPIO_Pin_2 |GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}
/*步进电机脉冲信号初始化*/
void init_StepperMotorSignal(void){
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 |GPIO_Pin_11 |GPIO_Pin_12 |GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
}
/*舵机控制信号初始化*/
void init_servoContral(void){
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5 |GPIO_Pin_6 |GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
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
/*外部中断PB0初始化*/
void init_Location_RFID(void)
{
    // PA0 为高位
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//下拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/*全部初始化*/
void init(void){
    TIM2_Init();
    TIM3_Init();
    TIM4_Init();
    TIM12_13_14_Init();
}
/*预初始化*/
void preinit(void){
    init_PathTracking();
    init_MotorContral();
    init_StepperMotorDir();
    init_StepperMotorSignal();
    init_servoContral();
    init_USART1();
    init_Location_RFID();
		delay_init(SystemCoreClock/1000000);
}
