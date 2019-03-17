/**
  ******************************************************************************
  * @file init.c 
  * @author RoboGame Sonny Team
  * @data 11/9
  * @version 1.1
  * @brief �������г�ʼ��������������
  *        ѭ��ģ���ʼ����GPIOB 0 - 11
  *        ���̵�����Ƴ�ʼ����GPIOF A�� 0 - 3 B�� 4(8) - 7
  *        ���̵��PWM��ʼ����GPIOC 6 - 9 PWM1000�ɵ� ��ʱ��3
  *	       ������������ʼ����GPIOC 0 - 3
  *	       ��������źų�ʼ����GPIOF 10 - 13
  *	       ����źų�ʼ����GPIOA 6 - 7 GPIOB 14 - 15 TIME12 13 14 CHANNEL 1 TIME12 CHANNEL 2
  *        ����USART1��ʼ����GPIOA 9 TX 10 RX
  *        ������ʱ����ʼ�� :��ʱ��2 1KHz ����position����ֵ 
  *                       ��ʱ��4 50Hz �������
  *                       ��ʱ��5 1kHz ����ת�����
  * @update:��ת����򵥶��ߵ�һ����ʱ��5ȥ����ȡ��ת����ӳ��жϡ���Ϊ���ӳ�һ������̽ͷ����ȷ��
  * @update:���ⲿ�ж�ȡ������Ϊ��4��λ GPIO A 0 ��3 ��ȡRFIDֵ ��ȡ�������Ͻ�ѭ���С��ѿ���ɾ����ʱ��5��
  * @note: �����ļ� "init.h" "pathTracking.h" "motorContral.h" "rfid.h"
  ******************************************************************************
**/
/**
  ******************************************************************************
  *�ж����ȼ�˵��
  *00 00:�ⲿ�жϣ�������ͣ
  *01 00:���̵������
  *01 01:ѭ��
  *03 03:����
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
  *ʱ��˵��
  *APB1����ʱ��Ϊ4��Ƶ��42M��
  *APB2����ʱ��Ϊ2��Ƶ��84M
  *����TIM1��TIM8~TIM11��ʱ��ΪAPB2ʱ�ӵ�������168M
  *TIM2~TIM7��TIM12~TIM14��ʱ��ΪAPB1��ʱ�ӵ�������84M
  *��ʱ��ʽ  Tout = ((arr+1)*(psc+1))/Tclk
  ******************************************************************************
**/
/*��ʱ��3ͨ��1����4���PWM*/
void TIM3_Init(void){
    //ԭʼƵ�� 84M ����Ƶ�� 50kHz
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM3ʱ��ʹ��    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//ʹ��PORTAʱ��	
    
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3); //GPIOC6����Ϊ��ʱ��3
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM3); //GPIOC7����Ϊ��ʱ��3
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM3); //GPIOC8����Ϊ��ʱ��3
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM3); //GPIOC9����Ϊ��ʱ��3
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 |GPIO_Pin_7 |GPIO_Pin_8|GPIO_Pin_9;           //GPIOC 6-9
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //
    GPIO_Init(GPIOC,&GPIO_InitStructure);              //��ʼ��PC6-9
      
    TIM_TimeBaseStructure.TIM_Prescaler=83;  //��ʱ����Ƶ 1MHz
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_Period=49;   //����װ���ֵ 
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
    
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//��ʼ����ʱ��3
    
    //��ʼ��TIM3 Channel 1-4 PWMģʽ	 
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
     TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ե�

    TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //��ʼ������TIM3 OC1
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //��ʼ������TIM3 OC2
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //��ʼ������TIM3 OC3
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //��ʼ������TIM3 OC4

    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR3�ϵ�Ԥװ�ؼĴ���
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR4�ϵ�Ԥװ�ؼĴ���

    TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPEʹ�� 

    TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
}
/*��ʱ�� 13 14 ͨ��1 ����������PWM*/
void TIM12_13_14_Init(void){
    //ԭʼƵ�� 84M ����Ƶ�� 1kHz
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);  	//TIM12ʱ��ʹ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,ENABLE);  	//TIM13ʱ��ʹ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  	//TIM14ʱ��ʹ��        
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTAʱ��	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//ʹ��PORTBʱ��	

    GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_TIM12); //GPIOB14����Ϊ��ʱ��12
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_TIM12); //GPIOB15����Ϊ��ʱ��12
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM13); //GPIOA6����Ϊ��ʱ��13
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM14); //GPIOA7����Ϊ��ʱ��14

    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 |GPIO_Pin_7;           //GPIOA 6-7
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //
    GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��PA6-9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 |GPIO_Pin_15;      //GPIOB 14-15
    GPIO_Init(GPIOB,&GPIO_InitStructure);              //��ʼ��PB 14-15

    TIM_TimeBaseStructure.TIM_Prescaler=1555;  //��ʱ����Ƶ 53984Hz
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_Period=179;   //����װ���ֵ  ����Ƶ��300Hz
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 

    TIM_TimeBaseInit(TIM12,&TIM_TimeBaseStructure);//��ʼ����ʱ��12
    TIM_TimeBaseInit(TIM13,&TIM_TimeBaseStructure);//��ʼ����ʱ��13
    TIM_TimeBaseInit(TIM14,&TIM_TimeBaseStructure);//��ʼ����ʱ��14

    //��ʼ��TIM12 13 14 Channel 1 TIM12 Channel 2 PWMģʽ	 
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
     TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ե�

    TIM_OC1Init(TIM12, &TIM_OCInitStructure);  //��ʼ������TIM12 OC1
    TIM_OC1Init(TIM13, &TIM_OCInitStructure);  //��ʼ������TIM13 OC1
    TIM_OC1Init(TIM14, &TIM_OCInitStructure);  //��ʼ������TIM14 OC1
    TIM_OC2Init(TIM12, &TIM_OCInitStructure);  //��ʼ������TIM12 OC2

    TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Enable);  //ʹ��TIM12��CCR1�ϵ�Ԥװ�ؼĴ���
    TIM_OC1PreloadConfig(TIM13, TIM_OCPreload_Enable);  //ʹ��TIM13��CCR1�ϵ�Ԥװ�ؼĴ���
    TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);  //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
    TIM_OC2PreloadConfig(TIM12, TIM_OCPreload_Enable);  //ʹ��TIM12��CCR2�ϵ�Ԥװ�ؼĴ���
  
    TIM_ARRPreloadConfig(TIM12,ENABLE);//ARPEʹ�� 
    TIM_ARRPreloadConfig(TIM13,ENABLE);//ARPEʹ�� 
    TIM_ARRPreloadConfig(TIM14,ENABLE);//ARPEʹ�� 

    TIM_Cmd(TIM12, ENABLE);  //ʹ��TIM12
    TIM_Cmd(TIM13, ENABLE);  //ʹ��TIM13
    TIM_Cmd(TIM14, ENABLE);  //ʹ��TIM14
}
/*��ʱ��2 ����position����ֵ 1kHz*/
void TIM2_Init(void){
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///ʹ��TIM2ʱ��
    
    TIM_TimeBaseInitStructure.TIM_Period =199; 	//�Զ���װ��ֵ
    TIM_TimeBaseInitStructure.TIM_Prescaler=839;  //��ʱ����Ƶ 100KHz
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
    
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//��ʼ��TIM2
    
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //����ʱ��2�����ж�
    TIM_Cmd(TIM2,ENABLE); //ʹ�ܶ�ʱ��2
    
    NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //��ʱ��2�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //�����ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //����жϱ�־λ
}
/*��ʱ��2�жϷ�����*/
void TIM2_IRQHandler(void){
    if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //����ж�
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
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //����жϱ�־λ
}
/*��ʱ��4 �������*/
void TIM4_Init(void){
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///ʹ��TIM4ʱ��
    
    TIM_TimeBaseInitStructure.TIM_Period =499; 	//�Զ���װ��ֵ
    TIM_TimeBaseInitStructure.TIM_Prescaler=8339;  //��ʱ����Ƶ 10KHz
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
    
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//��ʼ��TIM4
    
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //����ʱ��4�����ж�

    TIM_Cmd(TIM4,ENABLE); //ʹ�ܶ�ʱ��4
    
    NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //��ʱ��4�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //�����ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //����жϱ�־λ	
}
/*��ʱ��4�жϷ�����*/
void TIM4_IRQHandler(void){
    if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //����ж�
    {
        if(TURNING == FALSE){
            run();
        }
    }
    TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //����жϱ�־λ
}
/*ѭ��ģ���ʼ��*/
void init_PathTracking(void){
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1 |GPIO_Pin_2 |GPIO_Pin_3 |
                                GPIO_Pin_4 |GPIO_Pin_5 |GPIO_Pin_6 |GPIO_Pin_7 |
                                GPIO_Pin_8 |GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//Ĭ�����ͣ��ź���Ч
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/*�������ģ���ʼ��*/
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
/*�������������Ƴ�ʼ��*/
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
/*������������źų�ʼ��*/
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
/*��������źų�ʼ��*/
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
/*����USART1��ʼ��*/
void init_USART1(void){
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
    //����1��Ӧ���Ÿ���ӳ��
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
    
    //USART1�˿�����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	//�ٶ�50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
    GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
    USART_InitStructure.USART_BaudRate = BOUND;//����������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    USART_Init(USART1, &USART_InitStructure); //��ʼ������1
    
    USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
    
    USART_ClearFlag(USART1, USART_FLAG_TC);
    
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}
/*����USART1�жϸ�������*/
extern u8 USART1_RX_BUF[USART1_RX_LEN_MAX] = {0,0,0,0,0,0,0,0,0xFF};     //���ջ���,���USART1_RX_LEN_MAX���ֽ�.
extern u8 USART1_RX_LEN = USART1_RX_LEN_MAX;//���ջ���ʵ�����ÿռ�
extern u16 USART1_RX_STA = 0;       //����״̬���	
/*USART1�жϴ�����*/
void USART1_IRQHandler(void){
    int i = 0;
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
    {
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);      //��������жϱ�־λ
        if(USART1_RX_STA == 0){
            //��ʼ���������ռ�			 
            for(i = 0; i < USART1_RX_LEN_MAX; i++){
                USART1_RX_BUF[i] = 0x00;
            }				                          
            USART1_RX_BUF[8] = 0xFF;//ȷ����9λ��00	
        }
        USART1_RX_BUF[USART1_RX_STA] = USART_ReceiveData(USART1); //��������
        USART1_RX_STA++; //�л�����һ�ֽ�
        if(USART1_RX_STA == USART1_RX_LEN) //���ݽ�����ɣ���λ������
            USART1_RX_STA = 0;
   }
    if(USART_GetITStatus(USART1,USART_IT_TXE)!=RESET){      //�ж��Ƿ�Ϊ�����ж�
        USART_ClearITPendingBit(USART1,USART_IT_TXE);       //��������жϱ�־λ
    }
    if(USART_GetITStatus(USART1,USART_IT_TC)!=RESET){	    //�ж��Ƿ�Ϊ��������ж�
        USART_ClearITPendingBit(USART1,USART_IT_TC);	    //�����������жϱ�־λ
    } 
} 
/*�ⲿ�ж�PB0��ʼ��*/
void init_Location_RFID(void)
{
    // PA0 Ϊ��λ
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/*ȫ����ʼ��*/
void init(void){
    TIM2_Init();
    TIM3_Init();
    TIM4_Init();
    TIM12_13_14_Init();
}
/*Ԥ��ʼ��*/
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
