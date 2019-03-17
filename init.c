/**
  ******************************************************************************
  * @file init.c 
  * @author RoboGame Sonny Team
  * @data 9/9
  * @version 1.0
  * @brief �������г�ʼ��������������
  *        ����USART1��ʼ����GPIOA 9 TX 10 RX
  *        �ж������ �� GPIOB 0
  * @note: �����ļ� "init.h" "rfid.h"
  ******************************************************************************
**/
#include "init.h"
#include "rfid.h"
#include "delay.h"
extern u32 system_time_  = 0;

/**
  ******************************************************************************
  *ʱ��˵��
  *APB1����ʱ��Ϊ4��Ƶ��42M��
  *APB2����ʱ��Ϊ2��Ƶ��84M
  *����TIM1��TIM8~TIM11��ʱ��ΪAPB2ʱ�ӵ�������168M
  *TIM2~TIM7��TIM12~TIM14��ʱ��ΪAPB1��ʱ�ӵ�������84M
  *��ʱ��ʽ  Tout = ((arr+1)*(psc+1))/Tclk
  ******************************************************************************
*/
/*�жϲ����˿ڳ�ʼ��*/
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

void TIM4_Init(void){
    //10Hz
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///ʹ��TIM4ʱ��
    
    TIM_TimeBaseInitStructure.TIM_Period =999; 	//�Զ���װ��ֵ
    TIM_TimeBaseInitStructure.TIM_Prescaler=8339;  //��ʱ����Ƶ 10KHz
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
    
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//��ʼ��TIM4
    
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //����ʱ��4�����ж�

    TIM_Cmd(TIM4,ENABLE); //ʹ�ܶ�ʱ��4
    
    NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //��ʱ��4�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //��ռ���ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //�����ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //����жϱ�־λ	
}
/*��ʱ��4�жϷ�����*/
void TIM4_IRQHandler(void){
    if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //����ж�
    {
        system_time_++;
    }
    TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //����жϱ�־λ
}
/*ȫ����ʼ��*/
void init(void){
    init_int();
    init_USART1();
    GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	delay_init(SystemCoreClock/1000000);
}
