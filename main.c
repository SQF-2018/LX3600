#include "stm32f10x.h"
#include "IOCONFIG.H"
#include "QJBL.H"
#include "LCDDRIVER.H"
#include "ADC.H"
#include "AD7606.H"
#include "RTC.H"
#include "CH376_SPI.H"
#include "U_DISK.H"
#include "BLUETOOTH.H"
#include "MB85RCXX.H"
#include "KEY.H"
#include "LCD.H"
#include "FUNCTION.H"
#include "PRINTER.H"
#include "FILE_SYS.H"

#define DEBUG 1 //���Է����־,0��������,1�ڵ��Է���״̬
/*******************************************************************************************************************************
���Է�������
*******************************************************************************************************************************/
void DEBUG_SETTING(void)
{
 DBGMCU_Config(DBGMCU_TIM2_STOP,ENABLE);//TIM2Ϊ10ms��ʱ�������ڰ���������
 DBGMCU_Config(DBGMCU_TIM3_STOP,ENABLE);//TIM3Ϊ1ms��ʱ��������ͨѶ���̼�ʱ��
 DBGMCU_Config(DBGMCU_TIM4_STOP,ENABLE);//TIM4����RTC��IRQ���벶��У׼RTC
 DBGMCU_Config(DBGMCU_TIM8_STOP,ENABLE);//TIM8�����ⲿAD7606��ʱ��������
 DBGMCU_Config(DBGMCU_IWDG_STOP,ENABLE);
}

/*******************************************************************************************************************************
IO�ڳ�ʼ��
PA0 :NC            PB0 :LCD-/BUSY    PC0 :FM-SCL     PD0 :NC               PE0 :RTC-SCL 
PA1 :RS485TXD-EN   PB1 :ENCODE-A     PC1 :FM-SDA     PD1 :NC               PE1 :RTC-SDA 
PA2 :UART-TXD      PB2 :BOOT1        PC2 :NC         PD2 :NC               PE2 :BELL     
PA3 :UART-RXD      PB3 :USB-SPISCK   PC3 :NC         PD3 :NC               PE3 :POWER-SWITCH      
PA4 :LCD-SPI/CS    PB4 :USB-SPIMISO  PC4 :LCD-/RST   PD4 :USB-/INT         PE4 :JDQ-POWER-CTRL     
PA5 :LCD-SPISCK    PB5 :USB-SPIMOSI  PC5 :LCD-/INT   PD5 :USB-BUSY         PE5 :RUNLED     
PA6 :LCD-SPIMISO   PB6 :PRINT-TXD    PC6 :X9C103-UD  PD6 :USB-/CS          PE6 :NC     
PA7 :LCD-SPIMOSI   PB7 :PRINT-BUSY   PC7 :JDQ8       PD7 :USB-/ACT         PE7 :ENCODE-B     
PA8 :JDQ5          PB8 :NC           PC8 :JDQ7       PD8 :AD-BUSY          PE8 :KEY-SET  
PA9 :JDQ4          PB9 :RTC-IRQ      PC9 :JDQ6       PD9 :AD-JDQ1          PE9 :KEY-ESC  
PA10:JDQ3          PB10:NC           PC10:BT-MTSR    PD10:AD-JDQ2          PE10:KEY-DOWN 
PA11:JDQ2          PB11:AD-CONVST    PC11:BT-MRST    PD11:AD-JDQ3          PE11:KEY-ENTER 
PA12:JDQ1          PB12:AD-/CS       PC12:BT-MODE    PD12:AD-JDQ4          PE12:KEY-LEFT 
PA13:TMS/SWDIO     PB13:AD-SPISCK    PC13:NC         PD13:COOL-FAN         PE13:KEY-UP
PA14:TCK/SWCLK     PB14:AD-SPIMISO   PC14:NC         PD14:JDQ_MASTER_CTRL  PE14:KEY-RIGHT
PA15:BT-CONNECT    PB15:AD-RESET     PC15:NC         PD15:X9C103-INC       PE15:NC      
*******************************************************************************************************************************/
void GPIO_Configuration(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;//����IO�ڳ�ʼ���ṹ��

 /*----------------------------------------�ͷ�JTAG��--------------------------------------------*/
 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//����SW-DP���ر�JTAG-DP�����

 //��������ڵĳ�ʼ��״̬
 RS485TXD_EN_ON;     //����RS485���ݷ���
 RUNLED_OFF;         //Ϩ������ָʾ��
 FM_SDA_1;           //�ͷ�����洢����IIC����
 FM_SCL_H;
 RTC_SDA_1;          //�ͷ�RTC��IIC����
 RTC_SCL_H;
 COOL_FAN_OFF;       //���ȹر�
 JDQ_POWER_CTRL_ON;  //������Դ���Ƽ̵�������
 JDQ_MASTER_CTRL_OFF;//�ͷ���������̵���
 JDQ1_OFF;           //�ͷź���Դ������Ƽ̵���
 JDQ2_OFF;
 JDQ3_OFF;
 JDQ4_OFF;
 JDQ5_OFF;
 JDQ6_OFF;
 JDQ7_OFF;
 JDQ8_OFF;
 AD_JDQ1_OFF;        //�ͷŵ�ѹ������Ƽ̵���
 AD_JDQ2_OFF; 
 AD_JDQ3_OFF; 
 AD_JDQ4_OFF; 
 X9C103_INC_H;       //���ֵ�λ��X9C103���ֵ�ǰ״̬������
 X9C103_UD_L;
 LCD_RST_L;          //Һ����������λ
 LCD_CS_H;           //Һ����Ƭѡ��Ч
 BT_MODE_L;          //����ģ�������ͨ����ģʽ
 BELL_ON;            //��������������
 CH376_CS_H;         //CH376Ƭѡ�ź���Ч
 AD_CS_H;            //AD7606Ƭѡ�ź���Ч
 AD_RESET_L;         //ADоƬ��λ���ŵ͵�ƽ��Ϊ���50ns�ߵ�ƽ��λ������׼��

 /*-------------------------------------------����PA��-----------------------------------------------*/
 //�������������
 GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3 | GPIO_Pin_6;//UART-RXD,LCD-SPIMISO                            
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
 GPIO_Init(GPIOA,&GPIO_InitStructure);
 GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;  //BT-CONNECT                              
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//��������
 GPIO_Init(GPIOA,&GPIO_InitStructure);
 //���ÿ�����������0
 //PA0���0(Ϊ����߿��������ܣ�STM32�ֲ���˵�����õ�IO��Ӧ����Ϊ����ڣ������0��ƽ)
 GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);
 //ͨ��IO�������
 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 
                               | GPIO_Pin_11 | GPIO_Pin_12;
                                 //PA0,RS485TXD-EN,LCD-SPI/CS,JDQ5,JDQ4,JDQ3,JDQ2,JDQ1
 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;//�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�����ٶ�50MHZ
 GPIO_Init(GPIOA,&GPIO_InitStructure);
 //����IO���츴�����
 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_7;//UART-TXD,LCD-SPISCK,LCD-SPIMOSI
 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; //���츴�����
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�����ٶ�50MHZ
 GPIO_Init(GPIOA,&GPIO_InitStructure);
 GPIO_PinLockConfig(GPIOA,GPIO_Pin_All);//����PA������

 /*-------------------------------------------����PB��-----------------------------------------------*/
 //�������������
 GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_9 | GPIO_Pin_14;
                                //LCD-/BUSY,ENCODE-A,USB_SPIMISO,RTC-IRQ,AD-SPIMISO
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
 GPIO_Init(GPIOB,&GPIO_InitStructure);
 GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;   //PRINT-BUSY
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//��������
 GPIO_Init(GPIOB,&GPIO_InitStructure);
 //���ÿ�����������0
 //PB8,PB10���0
 GPIO_WriteBit(GPIOB,GPIO_Pin_8 | GPIO_Pin_10,Bit_RESET);//PB8,PB10���0
 //ͨ��IO�������
 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_15;//PB8,PB10,AD-/CS,AD-RESET
 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;//�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�����ٶ�50MHZ
 GPIO_Init(GPIOB,&GPIO_InitStructure);
 //����IO���츴�����
 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_11 | GPIO_Pin_13;
                                 //USB-SPISCK,USB-SPIMOSI,PRINT-TXD,AD-CONVST,AD-SPISCK
 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; //���츴�����
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�����ٶ�50MHZ
 GPIO_Init(GPIOB,&GPIO_InitStructure);
 GPIO_PinLockConfig(GPIOB,GPIO_Pin_All);//����PB������

 /*-------------------------------------------����PC��-----------------------------------------------*/
 //�������������
 GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_11;//LCD-/INT,BT-MRST
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
 GPIO_Init(GPIOC,&GPIO_InitStructure);
 //���ÿ�����������0
 //PC2,PC3,PC13,PC14,PC15���0
 GPIO_WriteBit(GPIOC,GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15,Bit_RESET);
 //ͨ��IO�������
 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_3  | GPIO_Pin_4  | GPIO_Pin_6 | GPIO_Pin_7 
                               | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
                               //PC2,PC3,LCD-/RST,X9C103-UD,JDQ8,JDQ7,JDQ6,BT-MODE,PC13,PC14,PC15
 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;//�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�����ٶ�50MHZ
 GPIO_Init(GPIOC,&GPIO_InitStructure);
 //����IO���츴�����
 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;//BT-MTSR
 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; //���츴�����
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�����ٶ�50MHZ
 GPIO_Init(GPIOC,&GPIO_InitStructure);
 //ͨ��IO��©���
 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1;//FM-SCL,FM-SDA
 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;       //��©���
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //�����ٶ�50MHZ
 GPIO_Init(GPIOC,&GPIO_InitStructure);
 GPIO_PinLockConfig(GPIOC,~GPIO_Pin_1);//������PC1�����PC������

 /*-------------------------------------------����PD��-----------------------------------------------*/
 //���������
 GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4 | GPIO_Pin_7;//USB-/INT,USB-/ACT
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
 GPIO_Init(GPIOD,&GPIO_InitStructure);
 GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_8;//USB-BUSY,AD-BUSY
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//��������
 GPIO_Init(GPIOD,&GPIO_InitStructure);
 //���ÿ�����������0
 //PD0,PD1,PD2,PD3���0
 GPIO_WriteBit(GPIOD,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2| GPIO_Pin_3,Bit_RESET);
 //ͨ��IO�������
 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2| GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_9
                               | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
                               //PD0,PD1,PD2,PD3,USB-/CS,AD-JDQ1,AD-JDQ2,AD-JDQ3,AD-JDQ4,COOL-FAN,JDQ-MASTER-CTRL,X9C103-INC
 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;//�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�����ٶ�50MHZ
 GPIO_Init(GPIOD,&GPIO_InitStructure);
 GPIO_PinLockConfig(GPIOD,GPIO_Pin_All);//����PD������

 /*-------------------------------------------����PE��-----------------------------------------------*/
 //�������������
 GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11
                              | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
							  //POWER-SWITCH,ENCODE-B,KEY-SET,KEY-ESC,KEY-DOWN,KEY-ENTER,KEY-LEFT,KEY-UP,KEY-RIGHT
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
 GPIO_Init(GPIOE,&GPIO_InitStructure);
 //���ÿ�����������0
 //PE6,PE15���0
 GPIO_WriteBit(GPIOE,GPIO_Pin_6 | GPIO_Pin_15,Bit_RESET);
 //ͨ��IO�������
 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_4| GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_15;
                               //BELL,JDQ-POWER-CTRL,RUNLED,PE6,PE15
 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;//�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�����ٶ�50MHZ
 GPIO_Init(GPIOE,&GPIO_InitStructure);
 //ͨ��IO��©���
 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1;//RTC-SCL,RTC-SDA
 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;       //��©���
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //�����ٶ�50MHZ
 GPIO_Init(GPIOE,&GPIO_InitStructure);
 GPIO_PinLockConfig(GPIOE,~GPIO_Pin_1);//������PE1�����PE������
}

/*******************************************************************************************************************************
ϵͳ�жϹ���
*******************************************************************************************************************************/
void NVIC_Configuration(void)
{
 NVIC_InitTypeDef NVIC_InitStructure;//����NVIC��ʼ������
 
 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//������ռ���ȼ��ж�1λ�������ȼ�3λ

#ifdef  VECT_TAB_RAM //RAM�����г��� 
 NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);  //�����ж�������ʼ��ַΪ0x20000000 
#else //FLASH�����г��� 
 NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);//�����ж�������ʼ��ַΪ0x08000000   
#endif

 /*---------------�ⲿ�ж���8�жϳ�ʼ��--------------------------*/
 //PD8�ⲿ�ж�AD-BUSY
 NVIC_InitStructure.NVIC_IRQChannel                   = EXTI9_5_IRQn;//ͨ��
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;         //��ռ��
 NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;         //��Ӧ��
 NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;    //����
 NVIC_Init(&NVIC_InitStructure);
 /*-------------------TIM3�жϳ�ʼ��-----------------------------*/
 //10ms��ʱ��
 NVIC_InitStructure.NVIC_IRQChannel                   = TIM3_IRQn;//ͨ��
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;        //��ռ��
 NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;        //��Ӧ��
 NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;   //����
 NVIC_Init(&NVIC_InitStructure);
 /*-------------------TIM4�жϳ�ʼ��-----------------------------*/
 //1ms��ʱ��
 NVIC_InitStructure.NVIC_IRQChannel                   = TIM4_IRQn;//ͨ��
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;        //��ռ��
 NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;        //��Ӧ��
 NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;   //����
 NVIC_Init(&NVIC_InitStructure);
 /*-------------------USART1�жϳ�ʼ��(��ӡ������)---------------*/
 NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;//ͨ��
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;          //��ռ��
 NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;          //��Ӧ��
 NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;     //����
 NVIC_Init(&NVIC_InitStructure);
 /*-------------------USART2�жϳ�ʼ��(�ⲿͨѶ����)-------------*/
 NVIC_InitStructure.NVIC_IRQChannel                   = USART2_IRQn;//ͨ��
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;          //��ռ��
 NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;          //��Ӧ��
 NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;     //����
 NVIC_Init(&NVIC_InitStructure);
 /*-------------------UART4�жϳ�ʼ��(��������)------------------*/
 NVIC_InitStructure.NVIC_IRQChannel                   = UART4_IRQn;//ͨ��
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;         //��ռ��
 NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;         //��Ӧ��
 NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;    //����
 NVIC_Init(&NVIC_InitStructure); 
}

/*******************************************************************************************************************************
����ϵͳʱ��,ʹ�ܸ�����ʱ��
*******************************************************************************************************************************/
void RCC_Configuration(void)
{
 SystemInit();//����ϵͳʱ�ӣ��Լ�FLASH�ȴ�����
 RCC_ClockSecuritySystemCmd(ENABLE);//ʹ��ʱ�Ӱ�ȫϵͳ
 //��������IO�ں�AFIOʱ�ӣ�����USART1,SPI1ʱ��  
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC  | 
                        RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO   | 
                        RCC_APB2Periph_SPI1  | RCC_APB2Periph_USART1, ENABLE );
 //����USART2,UART4ʱ�ӣ�TIM2��TIM3��TIM4ʱ�ӣ�SPI2��SPI3ʱ��
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_UART4 | RCC_APB1Periph_TIM2 | 
                        RCC_APB1Periph_TIM3   | RCC_APB1Periph_TIM4  | RCC_APB1Periph_SPI2 | 
                        RCC_APB1Periph_SPI3, ENABLE );

 //LSI������
 RCC_LSICmd(ENABLE);//��LSI
 while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET);//�ȴ�ֱ��LSI�ȶ�
}

/*******************************************************************************************************************************
���ö������Ź�IWDG
*******************************************************************************************************************************/
void IWDG_Configuration(void)
{
 IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//ʹ�ܶԼĴ��� IWDG_PR �� IWDG_RLR ��д����
 IWDG_SetPrescaler(IWDG_Prescaler_64);//8��Ƶ һ������1.6ms(40KHZ),2.133ms(40KHZ),1.067ms(60KHZ)
 IWDG_SetReload(4095);//�12λ��4096*1.6=6553.6ms,4096*2.133=8736.768ms,4096*1.067=4370.432ms
 IWDG_ReloadCounter();//��װ�� IWDG ������
 IWDG_Enable();       //ʹ�ܶ������Ź�����(the LSI oscillator will be enabled by hardware)
}

/********************************************************************************************************************************
���ö�ʱ��3����ʱ����10ms
********************************************************************************************************************************/
void TIM3_Configuration(void)
{
 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//���嶨ʱ�����ݽṹ��
 TIM_OCInitTypeDef        TIM_OCInitStructure;  //���嶨ʱ���Ƚ�������ݽṹ��
 TIM_DeInit(TIM3);                              //��λTIM3��ʱ��
 //���ö�ʱ��3����ʱ����10ms
 TIM_TimeBaseStructure.TIM_Period        = 0x47;              //������ֵ71(0x47),0~71������72��      
 TIM_TimeBaseStructure.TIM_Prescaler     = 0x270f;            //Ԥ��Ƶ10000(0x270f)
 //TIM3�ڵ���APB1���ߣ��ٶ�Ϊ36MHZ��2=72MHZ��Ԥ��Ƶ��ֵ����������ֵ=720000       
 TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;               // ʱ�ӷָ�  
 TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;//�����������ϼ���
 TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
 //���TIM3����жϱ�־
 TIM_ClearFlag(TIM3, TIM_FLAG_Update);
 //����TIM3����ж�
 TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);  
 //����TIM3����
 TIM_Cmd(TIM3, ENABLE);
}

/*******************************************************************************************************************************
���ö�ʱ��4����ʱ����1ms
*******************************************************************************************************************************/
void TIM4_Configuration(void)
{
 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//���嶨ʱ�����ݽṹ��
 TIM_DeInit(TIM4);                              //��λTIM4��ʱ��
 //���ö�ʱ��4����ʱ����1ms
 TIM_TimeBaseStructure.TIM_Period        = 0x47;              //������ֵ71(0x47),0~71������72��      
 TIM_TimeBaseStructure.TIM_Prescaler     = 0x3e7;             //Ԥ��Ƶ1000(0x3e7)
 //��ΪTIM4�ڵ���APB1���ߣ��ٶ�Ϊ36MHZ��2=72MHZ��Ԥ��Ƶ��ֵ����������ֵ=72000       
 TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;               // ʱ�ӷָ�  
 TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;//�����������ϼ���
 TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
 //���TIM4����жϱ�־
 TIM_ClearFlag(TIM4, TIM_FLAG_Update);
 //����TIM4����ж�
 TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);  
 //����TIM4����
 TIM_Cmd(TIM4, ENABLE);
}

/*******************************************************************************************************************************
��ʱ��3��ʱ����10ms�жϷ������
*******************************************************************************************************************************/
void TIM3_IRQHandler(void)
{
 static unsigned char hundred_msecond_count = 0;//һ�ٺ��붨ʱ��������
 static unsigned char half_second_count = 0;//�����Ӷ�ʱ��������
 static unsigned char ch376_act_old,ch376_act_new,ch376_act_hold_time = 0;
 static unsigned char hc05_link_old,hc05_link_new,hc05_link_hold_time = 0;
 unsigned char tmp = 0;
 /*-----------------------------------��������---------------------------------------------------*/
 KEY1 = KEY2;
 if(KUP_STAT)    tmp |= 0x20;
 if(KDOWN_STAT)  tmp |= 0x10;
 if(KLEFT_STAT)  tmp |= 0x08;
 if(KRIGHT_STAT) tmp |= 0x04;
 if(KENTER_STAT) tmp |= 0x02;
 if(KESC_STAT)   tmp |= 0x01;
 KEY2 = tmp;
 if(HARDWARE_WORK_STATE)//Ӳ������״̬�쳣��������
  {
   Alarm_BEEP();//��������������
  }
 else BELL_OFF;//��������ֹͣ
 Key_In();
 /*----------------------------------U�̳�ʼ����ʱ��ʱ-------------------------------------------*/
 if(U_DISK_MOUNT_TIME < 0xff) U_DISK_MOUNT_TIME += 1;
 /*-------------------------------���CH376��ACT����״̬��ȥ������-------------------------------*/
 ch376_act_old = ch376_act_new;
 ch376_act_new = CH376_ACT_STAT;   //ȡ�õ�ǰACT����״̬
 if(ch376_act_old == ch376_act_new)//ǰ�����μ���ACT����״̬��ͬ����ʼ��ʱȥ����
  {
   if(ch376_act_hold_time < 10) ch376_act_hold_time += 1;
   else
    {
     if(ch376_act_new) U_DISK_CONNECT = NO;//ACT���Ÿߵ�ƽ����U�̲���
     else U_DISK_CONNECT = YES;            //ACT���ŵ͵�ƽ����U�̲���
    }
  }
 else ch376_act_hold_time = 0;
 /*-----------------------�������ģ��PIO9��״̬���ж���������״̬-------------------------------*/
 if(BLUETOOTH_MODLE_INIT == YES)//����ģ���ʼ���ɹ�
  {
   hc05_link_old = hc05_link_new;
   hc05_link_new = BT_LINK_STAT;     //ȡ�õ�ǰPIO9����״̬
   if(hc05_link_old == hc05_link_new)//ǰ�����μ���PIO9����״̬��ͬ����ʼ��ʱȥ����
    {
     if(hc05_link_hold_time < 10) hc05_link_hold_time += 1;
     else
      {
       if(hc05_link_new) BLUETOOTH_CONNECT = YES;//PIO9���Ÿߵ�ƽ������������
       else BLUETOOTH_CONNECT = NO;              //PIO9���ŵ͵�ƽ������δ����
      }
    }
   else hc05_link_hold_time = 0;
  }
 /*-----------------------------------100ms��ʱ����ʱ--------------------------------------------*/
 if(hundred_msecond_count < 9) hundred_msecond_count += 1;
 else
  {
   hundred_msecond_count = 0;  //�ٺ������������
   HUNDRED_MSECOND_SIG   = YES;//�ٺ��붨ʱ����ʱ��־��λ
  }
 /*----------------------------------�����Ӷ�ʱ����ʱ--------------------------------------------*/
 if(half_second_count < 49) half_second_count += 1;
 else
  {
   half_second_count = 0;  //�����Ӽ���������
   HALF_SECOND_SIG   = YES;//�����Ӷ�ʱ����ʱ��־��λ
  }
 /*----------------------------------����������ʱ��ʱ--------------------------------------------*/
 if(TEST_PROCESS_TIME < 0xffff) TEST_PROCESS_TIME += 1;
 /*--------------------------------����У׼������ʱ��ʱ------------------------------------------*/
 if(CALI_PROCESS_TIME < 0xffff) CALI_PROCESS_TIME += 1;
 /*-------------------------------�����ʱ��3�жϱ�־--------------------------------------------*/
 TIM_ClearFlag(TIM3, TIM_FLAG_Update);//���UIF�����жϱ�־
}

/*******************************************************************************************************************************
��ʱ��4��ʱ����1ms�жϷ������
*******************************************************************************************************************************/
void TIM4_IRQHandler(void)
{
 unsigned short rotary_time_avg;
 unsigned char new_a,new_b,j;
 static unsigned char old_ab,rotary_time[5],i = 0;
 static unsigned short unrotary_time  = 0;
 /*-------------------��ת�����������������----------------------------------------------------------------*/
 //��ȡ������A��B�൱ǰ״̬
 if(ENCODE_A) new_a = 1;
 else new_a = 0;
 if(ENCODE_B) new_b = 1;
 else new_b = 0;
 //���������������λ���ʱ
 if(ROTARY_PHASE_TIME_EN == ON)
  {
   if(ROTARY_PHASE_TIME < 0xff) ROTARY_PHASE_TIME += 1;
   ROTARY_STOP_TIME = 0;  //�������������ط���������
  }
 //����ת������A��B���ƽ��ͬʱ����ʾ��ת��������ת��λ�����ǿ����ж���ת����
 if(new_a == new_b)
  {
   ROTARY_PHASE_TIME_EN = OFF; //��ת��λ�󣬽�ֹ��λ���ʱ
   if(ROTARY_STOP_TIME >= 1)   //������ֵ������ط�������1ms
    {
     if(ROTARY_PHASE_TIME >= 1)//��λ����ڵ���1msΪ��Ч��λ��
      {
	   //���A��B��ǰ��ƽ״̬���ϴβ�ͬ�����Խ����¼�ֵ��ֵ��������Ϊ��ת������ת�������˻أ���ִ���µļ�ֵ
	   if(new_a != old_ab)
	    {
	     if(ENCODE_AB_IRQ == 'A') ROTARY_YESKEY = WHEEL_ADD;//���A���ȳ��������أ���Ϊ˳ʱ����ת��һ����
	     else ROTARY_YESKEY = WHEEL_SUB;                    //����Ϊ��ʱ����ת��һ����
		 old_ab = new_a;                                    //ִ���µļ�ֵ��ֵ�����±���A��B���ƽ״̬
		 rotary_time[i] = ROTARY_PHASE_TIME;                //����˴���ת��������ת�ٶ�ʱ��
	     if(i<4) i += 1;
	     else
	      {
		   rotary_time_avg = 0;
		   for(j=0;j<5;j++)
		    {
		     rotary_time_avg += rotary_time[i];
		    }
		   rotary_time_avg /= 5;                            //������5����ת��������ת�ٶ�ƽ��ֵ
		   if(unrotary_time < 400)                          //�����ת������ͣתʱ��С��400ms��������5�˶����ٶ�Ϊһ����ֵ����ı����Ȩֵ
		    {
			 if(rotary_time_avg <= 5) FAST_WEIGHTED_VALUE = 20;//��ת�ٶ�ƽ��ֵÿ����6ms���ڣ���ȨֵΪ20
	         else if(rotary_time_avg <= 14)                 //��ת�ٶ�ƽ��ֵÿ����15ms���ڣ���ȨֵΪ10 
			  {
			   FAST_WEIGHTED_VALUE = 10;
			   SLOW_WEIGHTED_VALUE = 10;
			  }
	         else                                           //��ת�ٶ�������ȨֵΪ1
			  {
			   FAST_WEIGHTED_VALUE = 1;
			   SLOW_WEIGHTED_VALUE = 1;                           
			  }
			}	  
	       i = 0;
		  }
	     unrotary_time = 0;  //������ת������ֹͣ��ת��ʱ��
		}
	   ENCODE_AB_IRQ = 0;    //�������¼�����ȳ��ֵ�������
       ROTARY_PHASE_TIME = 0;//������λ�������
       ROTARY_STOP_TIME = 0; //���������ʱ��
	   LCD_BLK_ON;           //��Һ��������
	   DKEYTIME=0;           //�ް������¼���������
	  }
    }
   if(ROTARY_STOP_TIME < 0xff) ROTARY_STOP_TIME += 1;//������������λ������ʱ
   if(unrotary_time < 400) unrotary_time += 1;       //��ת������ֹͣ��ת����ʱ���ʱ
   else                                              //���400msû����ת���������Զ��ָ���Ȩֵ1���������ٶȱ��滺����ָ��
    {
	 i = 0;
	 FAST_WEIGHTED_VALUE = 1;
	 SLOW_WEIGHTED_VALUE = 1;
	}
  }
 /*-------------------------------����ģ�鴮��ͨѶ��ֹʱ���ʱ------------------------------------*/
 if(BLUETOOTH_RXTIME_EN == YES)
  {
   if(BLUETOOTH_RXTIME < 5) BLUETOOTH_RXTIME += 1;//ͨѶ����9600ʱ��4���ֽڵ�ͨѶʱ��ԼΪ5ms
   else 
    {
     BLUETOOTH_RXTIME_END = YES;//��ֹʱ�䵽ʱ
     BLUETOOTH_RXTIME_EN  = NO; //ֹͣ��ֹʱ���ʱ
    }
  }
 /*----------------------------�����ʱ��4�жϱ�־-----------------------------------------------*/
 TIM_ClearFlag(TIM4, TIM_FLAG_Update);
}

/*******************************************************************************************************************************
һ�ٺ��뼶�����ʱ����ʱ�������
*******************************************************************************************************************************/
void Hundred_MSecond_Process(void)
{
 static unsigned char read_time;
 /*----------------------------------------��ȡRTCʱ��--------------------------------------------*/
 if(read_time < 1) read_time += 1;
 else
  {
   read_time = 0;
   RTC_ReadTime();//��ȡRTCʱ��
   /*---------------------------------����״̬������ˢ��--------------------------------------------*/
   if(SCRFL != 0) Top_Bar_Dupscr();
  }
 
 
 
 /*-------------------------------------ȫ�ֿ���˸���Ʊ�־----------------------------------------*/
 FAST_SHAN_SIG = ~FAST_SHAN_SIG; //ȫ�ֿ���˸���Ʊ�־
 /*-----------------------------------------------------------------------------------------------*/
 HUNDRED_MSECOND_SIG = NO;//����ٺ��뵽ʱ��־
}

/*******************************************************************************************************************************
�����Ӽ������ʱ����ʱ�������
*******************************************************************************************************************************/
void Half_Second_Process(void)
{
 /*----------------------------------������ʾ��˾LOGO��ʱ-----------------------------------------*/
 if(POWERON_TIME_COUNT < POWERON_TIME) POWERON_TIME_COUNT += 1;
 /*----------------------------------ȫ������˸���Ʊ�־-------------------------------------------*/
 SLOW_SHAN_SIG = ~SLOW_SHAN_SIG; //ȫ������˸���Ʊ�־
 /*----------------------------------������Ļ����ˢ��---------------------------------------------*/
 DUPDATE = YES;//ÿ������������Ļ����ˢ��
 /*-----------------------------------------------------------------------------------------------*/
 HALF_SECOND_SIG = NO;//��������ӵ�ʱ��־
}

/*******************************************************************************************************************************
������
*******************************************************************************************************************************/
int main(void)
{
 __disable_irq();//��ֹ�ж�
#if DEBUG == 1
  DEBUG_SETTING();
#endif

 RCC_Configuration();  //��ʼ��ϵͳʱ�Ӽ�����ʱ��
 GPIO_Configuration(); //��ʼ��IO��
 NVIC_Configuration(); //��ʼ���ж�ϵͳ
 LCD_Init();           //��ʼ��Һ����
 TIM2_Configuration(); //��ʼ����ʱ��2,10ms��ʱ��
 TIM3_Configuration(); //��ʼ����ʱ��3,1ms��ʱ��
 Mb85rcxx_ReadSheZhi();//��ȡ����
 BlueTooth_UART_Configuration(9600);//��ʼ��������ģ��ͨѶ��UART������ͨѶ����9600
 SPWMPOWER_UART_Configuration(9600);//��ʼ�����Ƶ����ԴͨѶ��UART������ͨѶ����9600
 Printer_Init();      //��ʼ����ӡ��
 //��ʼ��CH376
 if(CH376_Init_Host() != USB_INT_SUCCESS) Set_Bit(HARDWARE_EEOR_FLAG,USB_ERR);//CH376Ӳ������
 AD7606_Init();         //��ʼ��AD7606��SPI�ӿ�
 ADC_Configuration();   //��ʼ���ڲ�ADC�Լ��������(DMA1)
 SCRAPPLY = YES;       //����������ȫ��ˢ��
 LCD_Update();         //��������һ����Ļˢ��
 __enable_irq();        //���ж�
 RTC_Init();            //��ʼ��RTCʵʱʱ�ӣ��ڳ�ʼ��RTC֮ǰ�����ȳ�ʼ��TIM4�������ж�
 Bluetooth_Model_Init();//����ģ���ʼ������Ҫʹ�÷��ͽ����жϣ����Է��ڿ��ж����֮��
 SPWM_Power_Init();     //��ʼ����Ƶ����Դ����Ҫʹ�÷��ͽ����жϣ����Է��ڿ��ж����֮��
 if(HARDWARE_EEOR_FLAG) //Ӳ����ʼ��ʧ��
  {
   POPUP_SCRFL = 1;  //��ʾ��������1��Ӳ����ʼ������
   SCRAPPLY    = YES;//���󴰿�ˢ��
   LCD_Update();     //������ʾ����
  }
 IWDG_Configuration();  //��ʼ���ڲ��������Ź�
 while(1)
  {
   /*---------------ι�����ڲ��������Ź�---------------------------------------------------------*/
   IWDG_ReloadCounter();//��װ�� IWDG ������
   /*---------------��ز����������-------------------------------------------------------------*/
   if(ADC12_DONE == YES) ADC1_Analog_Data_Process();
   /*---------------ģ�������ݴ���---------------------------------------------------------------*/
   if(AD7606_DONE == YES) Analog_Data_Process();//�ⲿADC������ɣ�����ģ���������ݴ���
   /*---------------�ٺ��뼶��ʱ����-------------------------------------------------------------*/
   if(HUNDRED_MSECOND_SIG == YES) Hundred_MSecond_Process();
   /*---------------�����Ӽ���ʱ����-------------------------------------------------------------*/
   if(HALF_SECOND_SIG == YES) Half_Second_Process();
   /*---------------�а������£���������---------------------------------------------------------*/
   if(YESKEY != 0) Key_Pro();
   /*----------����Ļˢ��������Ļ����----------------------------------------------------------*/
   //�����ϵ���ʾ��˾LOGO�����ʱ�����Ӳ����ʼ���ɹ�������Խ��������Ļ
   if(POWERON_TIME_COUNT >= POWERON_TIME && SCRFL == 0 && HARDWARE_EEOR_FLAG == 0)
    {
     SCRFL    = 1;       //���������Ļ
     CDX1     = 1;       //��ʼ��һ���˵����
     SCRAPPLY = YES;     //����ȫ��ˢ��
     Top_Bar_Screen();   //��ʾ����״̬��
     Bottom_Bar_Screen();//��ʾ�ײ�״̬��
     Top_Bar_Dupscr();   //���¶���״̬������
     Bottom_Bar_Dupscr();//���µײ�״̬������
    }
   if(SCRAPPLY == YES || DUPDATE == YES) LCD_Update();
   LCD_Set_Bright(LCD_BRIGHTNESS);//����Һ��������
   /*--------------������ԴͨѶ�������ݴ������------------------------------------------------*/
   SPWM_Power_Modbus_Data_Receive();
   /*--------------������ģ��ͨѶ�������ݴ������------------------------------------------------*/
   if(BLUETOOTH_MODLE_INIT == YES) Bluetooth_Model_Data_Receive();//����ģ���ʼ���ɹ����Ž������ݽ��մ���
   /*-------------------�������̿��ƴ������-----------------------------------------------------*/
   if(TEST_PROCESS_EN == YES) Test_Process_Control();
   /*-------------------����У׼���̿��ƴ������-------------------------------------------------*/
   if(SCRFL == RANGE_CALIBRATION_SCREEN)//��ǰ��������У׼��Ļ����ִ����Ӧ���� 
    {
     if(CALI_PROCESS_FLAG != 2) Impedance_Angle_Difference_Calculate();//�����Դ�������������в�����ǲ���迹��
                                                                       //��Ϊ�ǲ��迹������������������ڳ���Ҫ��ȡFFT�����
                                                                       //��������ݿɶ�ȡ��־�����������г�ͻ��
                                                                       //���µ������ڳ���ִ���쳣����
     if(CALI_PROCESS_EN == YES) Range_Calibration_Current_Control();//���ʹ������У׼����ʼ��Ƶ����Դ��������
    }
   /*-------------------Ӳ������״̬������----------------------------------------------------*/
   Hardware_Work_State_Check();
   /*-------------------U������ʼ������--------------------------------------------------------*/
   if(U_DISK_CONNECT == YES && U_DISK_MOUNT == NO && U_DISK_MOUNT_TIME > 20)//U���Ѳ��룬���ǻ�û��ʼ���ɹ�������ʱ�Ѿ�����200ms����������³�ʼ��
    {
     if(CH376DiskMount() == USB_INT_SUCCESS)
      {
       U_DISK_MOUNT = YES;//U�̳�ʼ���ɹ�
      }
     else //U�̳�ʼ�����ɹ�������Ҫ��ʱ200ms�����³�ʼ��
      {
       U_DISK_MOUNT_TIME = 0;  //��ʼ����ʱ��ʱ��־��������ʱ
      }
    }
   else if(U_DISK_CONNECT == NO)//U���Ѱγ�
    {
     U_DISK_MOUNT = NO;//��ʼ��U�̳�ʼ����־
    }
   /*-------------------�������ݴ洢��U�̳���----------------------------------------------------*/ 
   if((POPUP_SCRFL == 4 && OPERATION_PROCESS == 3) ||
      (POPUP_SCRFL == 6 && OPERATION_PROCESS == 5))//��������洢����������Ļ���߲�����¼�洢����������Ļ,����Ҫ����������д��U��
    {
     U_Disk_Write_Test_Data_File();//����������洢������
     if(POPUP_SCRFL == 4)      OPERATION_PROCESS = 4;//�洢�ɹ�
     else if(POPUP_SCRFL == 6) OPERATION_PROCESS = 6;//�洢�ɹ�
     SCRAPPLY = YES;//����ȫ��ˢ��
    } 
   /*-------------------��ӡ�����������---------------------------------------------------------*/
   if(PRINTING_FLAG == YES) Print_Test_Data();//��ǰû�н��д�ӡ������Կ�ʼ�µĴ�ӡ����
   /*---------------�̵������Ƴ���---------------------------------------------------------------*/
   Relay_Control();
  }
}
