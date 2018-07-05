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

#define DEBUG 1 //调试仿真标志,0正常运行,1在调试仿真状态
/*******************************************************************************************************************************
调试仿真设置
*******************************************************************************************************************************/
void DEBUG_SETTING(void)
{
 DBGMCU_Config(DBGMCU_TIM2_STOP,ENABLE);//TIM2为10ms定时器，用于按键采样等
 DBGMCU_Config(DBGMCU_TIM3_STOP,ENABLE);//TIM3为1ms定时器，用于通讯过程计时等
 DBGMCU_Config(DBGMCU_TIM4_STOP,ENABLE);//TIM4用于RTC的IRQ输入捕获，校准RTC
 DBGMCU_Config(DBGMCU_TIM8_STOP,ENABLE);//TIM8用于外部AD7606定时采样控制
 DBGMCU_Config(DBGMCU_IWDG_STOP,ENABLE);
}

/*******************************************************************************************************************************
IO口初始化
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
 GPIO_InitTypeDef GPIO_InitStructure;//定义IO口初始化结构体

 /*----------------------------------------释放JTAG口--------------------------------------------*/
 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//启用SW-DP，关闭JTAG-DP仿真口

 //设置输出口的初始化状态
 RS485TXD_EN_ON;     //允许RS485数据发送
 RUNLED_OFF;         //熄灭运行指示灯
 FM_SDA_1;           //释放铁电存储器的IIC总线
 FM_SCL_H;
 RTC_SDA_1;          //释放RTC的IIC总线
 RTC_SCL_H;
 COOL_FAN_OFF;       //风扇关闭
 JDQ_POWER_CTRL_ON;  //交流电源控制继电器吸合
 JDQ_MASTER_CTRL_OFF;//释放主控输出继电器
 JDQ1_OFF;           //释放恒流源输出控制继电器
 JDQ2_OFF;
 JDQ3_OFF;
 JDQ4_OFF;
 JDQ5_OFF;
 JDQ6_OFF;
 JDQ7_OFF;
 JDQ8_OFF;
 AD_JDQ1_OFF;        //释放电压输入控制继电器
 AD_JDQ2_OFF; 
 AD_JDQ3_OFF; 
 AD_JDQ4_OFF; 
 X9C103_INC_H;       //数字电位器X9C103保持当前状态不动作
 X9C103_UD_L;
 LCD_RST_L;          //液晶屏开机复位
 LCD_CS_H;           //液晶屏片选无效
 BT_MODE_L;          //蓝牙模块进入普通工作模式
 BELL_ON;            //蜂鸣器开机蜂鸣
 CH376_CS_H;         //CH376片选信号无效
 AD_CS_H;            //AD7606片选信号无效
 AD_RESET_L;         //AD芯片复位引脚低电平，为输出50ns高电平复位脉冲做准备

 /*-------------------------------------------设置PA口-----------------------------------------------*/
 //设置数字输入口
 GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3 | GPIO_Pin_6;//UART-RXD,LCD-SPIMISO                            
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
 GPIO_Init(GPIOA,&GPIO_InitStructure);
 GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;  //BT-CONNECT                              
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//下拉输入
 GPIO_Init(GPIOA,&GPIO_InitStructure);
 //设置空闲输出口输出0
 //PA0输出0(为了提高抗干扰性能，STM32手册中说明不用的IO口应设置为输出口，且输出0电平)
 GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);
 //通用IO推挽输出
 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 
                               | GPIO_Pin_11 | GPIO_Pin_12;
                                 //PA0,RS485TXD-EN,LCD-SPI/CS,JDQ5,JDQ4,JDQ3,JDQ2,JDQ1
 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;//推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//工作速度50MHZ
 GPIO_Init(GPIOA,&GPIO_InitStructure);
 //复用IO推挽复用输出
 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_7;//UART-TXD,LCD-SPISCK,LCD-SPIMOSI
 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; //推挽复用输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//工作速度50MHZ
 GPIO_Init(GPIOA,&GPIO_InitStructure);
 GPIO_PinLockConfig(GPIOA,GPIO_Pin_All);//锁定PA口设置

 /*-------------------------------------------设置PB口-----------------------------------------------*/
 //设置数字输入口
 GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_9 | GPIO_Pin_14;
                                //LCD-/BUSY,ENCODE-A,USB_SPIMISO,RTC-IRQ,AD-SPIMISO
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
 GPIO_Init(GPIOB,&GPIO_InitStructure);
 GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;   //PRINT-BUSY
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//下拉输入
 GPIO_Init(GPIOB,&GPIO_InitStructure);
 //设置空闲输出口输出0
 //PB8,PB10输出0
 GPIO_WriteBit(GPIOB,GPIO_Pin_8 | GPIO_Pin_10,Bit_RESET);//PB8,PB10输出0
 //通用IO推挽输出
 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_15;//PB8,PB10,AD-/CS,AD-RESET
 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;//推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//工作速度50MHZ
 GPIO_Init(GPIOB,&GPIO_InitStructure);
 //复用IO推挽复用输出
 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_11 | GPIO_Pin_13;
                                 //USB-SPISCK,USB-SPIMOSI,PRINT-TXD,AD-CONVST,AD-SPISCK
 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; //推挽复用输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//工作速度50MHZ
 GPIO_Init(GPIOB,&GPIO_InitStructure);
 GPIO_PinLockConfig(GPIOB,GPIO_Pin_All);//锁定PB口设置

 /*-------------------------------------------设置PC口-----------------------------------------------*/
 //设置数字输入口
 GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_11;//LCD-/INT,BT-MRST
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
 GPIO_Init(GPIOC,&GPIO_InitStructure);
 //设置空闲输出口输出0
 //PC2,PC3,PC13,PC14,PC15输出0
 GPIO_WriteBit(GPIOC,GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15,Bit_RESET);
 //通用IO推挽输出
 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_3  | GPIO_Pin_4  | GPIO_Pin_6 | GPIO_Pin_7 
                               | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
                               //PC2,PC3,LCD-/RST,X9C103-UD,JDQ8,JDQ7,JDQ6,BT-MODE,PC13,PC14,PC15
 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;//推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//工作速度50MHZ
 GPIO_Init(GPIOC,&GPIO_InitStructure);
 //复用IO推挽复用输出
 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;//BT-MTSR
 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; //推挽复用输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//工作速度50MHZ
 GPIO_Init(GPIOC,&GPIO_InitStructure);
 //通用IO开漏输出
 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1;//FM-SCL,FM-SDA
 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;       //开漏输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //工作速度50MHZ
 GPIO_Init(GPIOC,&GPIO_InitStructure);
 GPIO_PinLockConfig(GPIOC,~GPIO_Pin_1);//锁定除PC1以外的PC口设置

 /*-------------------------------------------设置PD口-----------------------------------------------*/
 //设置输入口
 GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4 | GPIO_Pin_7;//USB-/INT,USB-/ACT
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
 GPIO_Init(GPIOD,&GPIO_InitStructure);
 GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_8;//USB-BUSY,AD-BUSY
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//下拉输入
 GPIO_Init(GPIOD,&GPIO_InitStructure);
 //设置空闲输出口输出0
 //PD0,PD1,PD2,PD3输出0
 GPIO_WriteBit(GPIOD,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2| GPIO_Pin_3,Bit_RESET);
 //通用IO推挽输出
 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2| GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_9
                               | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
                               //PD0,PD1,PD2,PD3,USB-/CS,AD-JDQ1,AD-JDQ2,AD-JDQ3,AD-JDQ4,COOL-FAN,JDQ-MASTER-CTRL,X9C103-INC
 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;//推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//工作速度50MHZ
 GPIO_Init(GPIOD,&GPIO_InitStructure);
 GPIO_PinLockConfig(GPIOD,GPIO_Pin_All);//锁定PD口设置

 /*-------------------------------------------设置PE口-----------------------------------------------*/
 //设置数字输入口
 GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11
                              | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
							  //POWER-SWITCH,ENCODE-B,KEY-SET,KEY-ESC,KEY-DOWN,KEY-ENTER,KEY-LEFT,KEY-UP,KEY-RIGHT
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
 GPIO_Init(GPIOE,&GPIO_InitStructure);
 //设置空闲输出口输出0
 //PE6,PE15输出0
 GPIO_WriteBit(GPIOE,GPIO_Pin_6 | GPIO_Pin_15,Bit_RESET);
 //通用IO推挽输出
 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_4| GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_15;
                               //BELL,JDQ-POWER-CTRL,RUNLED,PE6,PE15
 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;//推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//工作速度50MHZ
 GPIO_Init(GPIOE,&GPIO_InitStructure);
 //通用IO开漏输出
 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1;//RTC-SCL,RTC-SDA
 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;       //开漏输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //工作速度50MHZ
 GPIO_Init(GPIOE,&GPIO_InitStructure);
 GPIO_PinLockConfig(GPIOE,~GPIO_Pin_1);//锁定除PE1以外的PE口设置
}

/*******************************************************************************************************************************
系统中断管理
*******************************************************************************************************************************/
void NVIC_Configuration(void)
{
 NVIC_InitTypeDef NVIC_InitStructure;//定义NVIC初始化数组
 
 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//设置抢占优先级中断1位，从优先级3位

#ifdef  VECT_TAB_RAM //RAM中运行程序 
 NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);  //设置中断向量起始地址为0x20000000 
#else //FLASH中运行程序 
 NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);//设置中断向量起始地址为0x08000000   
#endif

 /*---------------外部中断线8中断初始化--------------------------*/
 //PD8外部中断AD-BUSY
 NVIC_InitStructure.NVIC_IRQChannel                   = EXTI9_5_IRQn;//通道
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;         //抢占级
 NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;         //响应级
 NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;    //启动
 NVIC_Init(&NVIC_InitStructure);
 /*-------------------TIM3中断初始化-----------------------------*/
 //10ms定时器
 NVIC_InitStructure.NVIC_IRQChannel                   = TIM3_IRQn;//通道
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;        //抢占级
 NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;        //响应级
 NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;   //启动
 NVIC_Init(&NVIC_InitStructure);
 /*-------------------TIM4中断初始化-----------------------------*/
 //1ms定时器
 NVIC_InitStructure.NVIC_IRQChannel                   = TIM4_IRQn;//通道
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;        //抢占级
 NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;        //响应级
 NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;   //启动
 NVIC_Init(&NVIC_InitStructure);
 /*-------------------USART1中断初始化(打印机控制)---------------*/
 NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;//通道
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;          //抢占级
 NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;          //响应级
 NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;     //启动
 NVIC_Init(&NVIC_InitStructure);
 /*-------------------USART2中断初始化(外部通讯控制)-------------*/
 NVIC_InitStructure.NVIC_IRQChannel                   = USART2_IRQn;//通道
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;          //抢占级
 NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;          //响应级
 NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;     //启动
 NVIC_Init(&NVIC_InitStructure);
 /*-------------------UART4中断初始化(蓝牙控制)------------------*/
 NVIC_InitStructure.NVIC_IRQChannel                   = UART4_IRQn;//通道
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;         //抢占级
 NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;         //响应级
 NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;    //启动
 NVIC_Init(&NVIC_InitStructure); 
}

/*******************************************************************************************************************************
配置系统时钟,使能各外设时钟
*******************************************************************************************************************************/
void RCC_Configuration(void)
{
 SystemInit();//设置系统时钟，以及FLASH等待周期
 RCC_ClockSecuritySystemCmd(ENABLE);//使能时钟安全系统
 //启动所有IO口和AFIO时钟，启动USART1,SPI1时钟  
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC  | 
                        RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO   | 
                        RCC_APB2Periph_SPI1  | RCC_APB2Periph_USART1, ENABLE );
 //启动USART2,UART4时钟，TIM2、TIM3、TIM4时钟，SPI2、SPI3时钟
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_UART4 | RCC_APB1Periph_TIM2 | 
                        RCC_APB1Periph_TIM3   | RCC_APB1Periph_TIM4  | RCC_APB1Periph_SPI2 | 
                        RCC_APB1Periph_SPI3, ENABLE );

 //LSI的启动
 RCC_LSICmd(ENABLE);//打开LSI
 while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET);//等待直到LSI稳定
}

/*******************************************************************************************************************************
配置独立看门狗IWDG
*******************************************************************************************************************************/
void IWDG_Configuration(void)
{
 IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//使能对寄存器 IWDG_PR 和 IWDG_RLR 的写操作
 IWDG_SetPrescaler(IWDG_Prescaler_64);//8分频 一个周期1.6ms(40KHZ),2.133ms(40KHZ),1.067ms(60KHZ)
 IWDG_SetReload(4095);//最长12位，4096*1.6=6553.6ms,4096*2.133=8736.768ms,4096*1.067=4370.432ms
 IWDG_ReloadCounter();//重装载 IWDG 计数器
 IWDG_Enable();       //使能独立看门狗计数(the LSI oscillator will be enabled by hardware)
}

/********************************************************************************************************************************
设置定时器3，定时周期10ms
********************************************************************************************************************************/
void TIM3_Configuration(void)
{
 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//定义定时器数据结构体
 TIM_OCInitTypeDef        TIM_OCInitStructure;  //定义定时器比较输出数据结构体
 TIM_DeInit(TIM3);                              //复位TIM3定时器
 //设置定时器3，定时周期10ms
 TIM_TimeBaseStructure.TIM_Period        = 0x47;              //最大计数值71(0x47),0~71共计数72次      
 TIM_TimeBaseStructure.TIM_Prescaler     = 0x270f;            //预分频10000(0x270f)
 //TIM3在低速APB1总线，速度为36MHZ×2=72MHZ，预分频数值×最大计数数值=720000       
 TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;               // 时钟分割  
 TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;//计数方向向上计数
 TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
 //清除TIM3溢出中断标志
 TIM_ClearFlag(TIM3, TIM_FLAG_Update);
 //允许TIM3溢出中断
 TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);  
 //允许TIM3计数
 TIM_Cmd(TIM3, ENABLE);
}

/*******************************************************************************************************************************
设置定时器4，定时周期1ms
*******************************************************************************************************************************/
void TIM4_Configuration(void)
{
 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//定义定时器数据结构体
 TIM_DeInit(TIM4);                              //复位TIM4定时器
 //设置定时器4，定时周期1ms
 TIM_TimeBaseStructure.TIM_Period        = 0x47;              //最大计数值71(0x47),0~71共计数72次      
 TIM_TimeBaseStructure.TIM_Prescaler     = 0x3e7;             //预分频1000(0x3e7)
 //因为TIM4在低速APB1总线，速度为36MHZ×2=72MHZ，预分频数值×最大计数数值=72000       
 TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;               // 时钟分割  
 TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;//计数方向向上计数
 TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
 //清除TIM4溢出中断标志
 TIM_ClearFlag(TIM4, TIM_FLAG_Update);
 //允许TIM4溢出中断
 TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);  
 //允许TIM4计数
 TIM_Cmd(TIM4, ENABLE);
}

/*******************************************************************************************************************************
定时器3定时周期10ms中断服务程序
*******************************************************************************************************************************/
void TIM3_IRQHandler(void)
{
 static unsigned char hundred_msecond_count = 0;//一百毫秒定时器计数器
 static unsigned char half_second_count = 0;//半秒钟定时器计数器
 static unsigned char ch376_act_old,ch376_act_new,ch376_act_hold_time = 0;
 static unsigned char hc05_link_old,hc05_link_new,hc05_link_hold_time = 0;
 unsigned char tmp = 0;
 /*-----------------------------------按键采样---------------------------------------------------*/
 KEY1 = KEY2;
 if(KUP_STAT)    tmp |= 0x20;
 if(KDOWN_STAT)  tmp |= 0x10;
 if(KLEFT_STAT)  tmp |= 0x08;
 if(KRIGHT_STAT) tmp |= 0x04;
 if(KENTER_STAT) tmp |= 0x02;
 if(KESC_STAT)   tmp |= 0x01;
 KEY2 = tmp;
 if(HARDWARE_WORK_STATE)//硬件工作状态异常蜂鸣报警
  {
   Alarm_BEEP();//报警蜂鸣器控制
  }
 else BELL_OFF;//按键蜂鸣停止
 Key_In();
 /*----------------------------------U盘初始化延时计时-------------------------------------------*/
 if(U_DISK_MOUNT_TIME < 0xff) U_DISK_MOUNT_TIME += 1;
 /*-------------------------------检测CH376的ACT引脚状态（去抖动）-------------------------------*/
 ch376_act_old = ch376_act_new;
 ch376_act_new = CH376_ACT_STAT;   //取得当前ACT引脚状态
 if(ch376_act_old == ch376_act_new)//前后两次检测的ACT引脚状态相同，开始计时去抖动
  {
   if(ch376_act_hold_time < 10) ch376_act_hold_time += 1;
   else
    {
     if(ch376_act_new) U_DISK_CONNECT = NO;//ACT引脚高电平，无U盘插入
     else U_DISK_CONNECT = YES;            //ACT引脚低电平，有U盘插入
    }
  }
 else ch376_act_hold_time = 0;
 /*-----------------------检测蓝牙模块PIO9脚状态来判断蓝牙连接状态-------------------------------*/
 if(BLUETOOTH_MODLE_INIT == YES)//蓝牙模块初始化成功
  {
   hc05_link_old = hc05_link_new;
   hc05_link_new = BT_LINK_STAT;     //取得当前PIO9引脚状态
   if(hc05_link_old == hc05_link_new)//前后两次检测的PIO9引脚状态相同，开始计时去抖动
    {
     if(hc05_link_hold_time < 10) hc05_link_hold_time += 1;
     else
      {
       if(hc05_link_new) BLUETOOTH_CONNECT = YES;//PIO9引脚高电平，蓝牙已连接
       else BLUETOOTH_CONNECT = NO;              //PIO9引脚低电平，蓝牙未连接
      }
    }
   else hc05_link_hold_time = 0;
  }
 /*-----------------------------------100ms定时器计时--------------------------------------------*/
 if(hundred_msecond_count < 9) hundred_msecond_count += 1;
 else
  {
   hundred_msecond_count = 0;  //百毫秒计数器清零
   HUNDRED_MSECOND_SIG   = YES;//百毫秒定时器到时标志置位
  }
 /*----------------------------------半秒钟定时器计时--------------------------------------------*/
 if(half_second_count < 49) half_second_count += 1;
 else
  {
   half_second_count = 0;  //半秒钟计数器清零
   HALF_SECOND_SIG   = YES;//半秒钟定时器到时标志置位
  }
 /*----------------------------------测量过程延时计时--------------------------------------------*/
 if(TEST_PROCESS_TIME < 0xffff) TEST_PROCESS_TIME += 1;
 /*--------------------------------量程校准过程延时计时------------------------------------------*/
 if(CALI_PROCESS_TIME < 0xffff) CALI_PROCESS_TIME += 1;
 /*-------------------------------清除定时器3中断标志--------------------------------------------*/
 TIM_ClearFlag(TIM3, TIM_FLAG_Update);//清除UIF更新中断标志
}

/*******************************************************************************************************************************
定时器4定时周期1ms中断服务程序
*******************************************************************************************************************************/
void TIM4_IRQHandler(void)
{
 unsigned short rotary_time_avg;
 unsigned char new_a,new_b,j;
 static unsigned char old_ab,rotary_time[5],i = 0;
 static unsigned short unrotary_time  = 0;
 /*-------------------旋转编码器采样处理程序----------------------------------------------------------------*/
 //读取编码器A、B相当前状态
 if(ENCODE_A) new_a = 1;
 else new_a = 0;
 if(ENCODE_B) new_b = 1;
 else new_b = 0;
 //允许编码器解码相位差计时
 if(ROTARY_PHASE_TIME_EN == ON)
  {
   if(ROTARY_PHASE_TIME < 0xff) ROTARY_PHASE_TIME += 1;
   ROTARY_STOP_TIME = 0;  //清零较晚的跳变沿防抖计数器
  }
 //当旋转编码器A、B相电平相同时，表示旋转编码器旋转到位，这是可以判断旋转方向
 if(new_a == new_b)
  {
   ROTARY_PHASE_TIME_EN = OFF; //旋转到位后，禁止相位差计时
   if(ROTARY_STOP_TIME >= 1)   //较晚出现的跳变沿防抖处理1ms
    {
     if(ROTARY_PHASE_TIME >= 1)//相位差大于等于1ms为有效相位差
      {
	   //如果A、B当前电平状态与上次不同，可以进行新键值赋值；否则认为旋转编码器转动半格后退回，不执行新的键值
	   if(new_a != old_ab)
	    {
	     if(ENCODE_AB_IRQ == 'A') ROTARY_YESKEY = WHEEL_ADD;//如果A相先出现跳变沿，则为顺时针旋转加一操作
	     else ROTARY_YESKEY = WHEEL_SUB;                    //否则为逆时针旋转减一操作
		 old_ab = new_a;                                    //执行新的键值赋值后，重新保存A、B相电平状态
		 rotary_time[i] = ROTARY_PHASE_TIME;                //保存此次旋转编码器旋转速度时间
	     if(i<4) i += 1;
	     else
	      {
		   rotary_time_avg = 0;
		   for(j=0;j<5;j++)
		    {
		     rotary_time_avg += rotary_time[i];
		    }
		   rotary_time_avg /= 5;                            //求连续5次旋转编码器旋转速度平均值
		   if(unrotary_time < 400)                          //如果旋转编码器停转时间小于400ms，且连续5此动作速度为一定的值，则改变其加权值
		    {
			 if(rotary_time_avg <= 5) FAST_WEIGHTED_VALUE = 20;//旋转速度平均值每档在6ms以内，加权值为20
	         else if(rotary_time_avg <= 14)                 //旋转速度平均值每档在15ms以内，加权值为10 
			  {
			   FAST_WEIGHTED_VALUE = 10;
			   SLOW_WEIGHTED_VALUE = 10;
			  }
	         else                                           //旋转速度慢，加权值为1
			  {
			   FAST_WEIGHTED_VALUE = 1;
			   SLOW_WEIGHTED_VALUE = 1;                           
			  }
			}	  
	       i = 0;
		  }
	     unrotary_time = 0;  //清零旋转编码器停止旋转计时器
		}
	   ENCODE_AB_IRQ = 0;    //可以重新检测最先出现的跳变沿
       ROTARY_PHASE_TIME = 0;//清零相位差计数器
       ROTARY_STOP_TIME = 0; //清零防抖计时器
	   LCD_BLK_ON;           //打开液晶屏背光
	   DKEYTIME=0;           //无按键按下计数器清零
	  }
    }
   if(ROTARY_STOP_TIME < 0xff) ROTARY_STOP_TIME += 1;//后发生的跳变相位防抖计时
   if(unrotary_time < 400) unrotary_time += 1;       //旋转编码器停止旋转持续时间计时
   else                                              //如果400ms没有旋转动作，则自动恢复加权值1，并清零速度保存缓冲区指针
    {
	 i = 0;
	 FAST_WEIGHTED_VALUE = 1;
	 SLOW_WEIGHTED_VALUE = 1;
	}
  }
 /*-------------------------------蓝牙模块串口通讯静止时间计时------------------------------------*/
 if(BLUETOOTH_RXTIME_EN == YES)
  {
   if(BLUETOOTH_RXTIME < 5) BLUETOOTH_RXTIME += 1;//通讯速率9600时，4个字节的通讯时间约为5ms
   else 
    {
     BLUETOOTH_RXTIME_END = YES;//静止时间到时
     BLUETOOTH_RXTIME_EN  = NO; //停止静止时间计时
    }
  }
 /*----------------------------清除定时器4中断标志-----------------------------------------------*/
 TIM_ClearFlag(TIM4, TIM_FLAG_Update);
}

/*******************************************************************************************************************************
一百毫秒级软件定时器到时处理程序
*******************************************************************************************************************************/
void Hundred_MSecond_Process(void)
{
 static unsigned char read_time;
 /*----------------------------------------读取RTC时间--------------------------------------------*/
 if(read_time < 1) read_time += 1;
 else
  {
   read_time = 0;
   RTC_ReadTime();//读取RTC时间
   /*---------------------------------顶部状态栏数据刷新--------------------------------------------*/
   if(SCRFL != 0) Top_Bar_Dupscr();
  }
 
 
 
 /*-------------------------------------全局快闪烁控制标志----------------------------------------*/
 FAST_SHAN_SIG = ~FAST_SHAN_SIG; //全局快闪烁控制标志
 /*-----------------------------------------------------------------------------------------------*/
 HUNDRED_MSECOND_SIG = NO;//清除百毫秒到时标志
}

/*******************************************************************************************************************************
半秒钟级软件定时器到时处理程序
*******************************************************************************************************************************/
void Half_Second_Process(void)
{
 /*----------------------------------开机显示公司LOGO及时-----------------------------------------*/
 if(POWERON_TIME_COUNT < POWERON_TIME) POWERON_TIME_COUNT += 1;
 /*----------------------------------全局慢闪烁控制标志-------------------------------------------*/
 SLOW_SHAN_SIG = ~SLOW_SHAN_SIG; //全局慢闪烁控制标志
 /*----------------------------------请求屏幕数据刷新---------------------------------------------*/
 DUPDATE = YES;//每半秒钟请求屏幕数据刷新
 /*-----------------------------------------------------------------------------------------------*/
 HALF_SECOND_SIG = NO;//清除半秒钟到时标志
}

/*******************************************************************************************************************************
主程序
*******************************************************************************************************************************/
int main(void)
{
 __disable_irq();//禁止中断
#if DEBUG == 1
  DEBUG_SETTING();
#endif

 RCC_Configuration();  //初始化系统时钟及外设时钟
 GPIO_Configuration(); //初始化IO口
 NVIC_Configuration(); //初始化中断系统
 LCD_Init();           //初始化液晶屏
 TIM2_Configuration(); //初始化定时器2,10ms定时器
 TIM3_Configuration(); //初始化定时器3,1ms定时器
 Mb85rcxx_ReadSheZhi();//读取设置
 BlueTooth_UART_Configuration(9600);//初始化与蓝牙模块通讯的UART，设置通讯速率9600
 SPWMPOWER_UART_Configuration(9600);//初始化与变频逆变电源通讯的UART，设置通讯速率9600
 Printer_Init();      //初始化打印机
 //初始化CH376
 if(CH376_Init_Host() != USB_INT_SUCCESS) Set_Bit(HARDWARE_EEOR_FLAG,USB_ERR);//CH376硬件错误
 AD7606_Init();         //初始化AD7606及SPI接口
 ADC_Configuration();   //初始化内部ADC以及相关外设(DMA1)
 SCRAPPLY = YES;       //开机后请求全屏刷新
 LCD_Update();         //开机进行一次屏幕刷新
 __enable_irq();        //开中断
 RTC_Init();            //初始化RTC实时时钟，在初始化RTC之前必须先初始化TIM4，并开中断
 Bluetooth_Model_Init();//蓝牙模块初始化，需要使用发送接收中断，所以放在开中断语句之后
 SPWM_Power_Init();     //初始化变频逆变电源，需要使用发送接收中断，所以放在开中断语句之后
 if(HARDWARE_EEOR_FLAG) //硬件初始化失败
  {
   POPUP_SCRFL = 1;  //显示弹出界面1，硬件初始化错误
   SCRAPPLY    = YES;//请求窗口刷新
   LCD_Update();     //调用显示函数
  }
 IWDG_Configuration();  //初始化内部独立看门狗
 while(1)
  {
   /*---------------喂狗，内部独立看门狗---------------------------------------------------------*/
   IWDG_ReloadCounter();//重装载 IWDG 计数器
   /*---------------电池采样处理程序-------------------------------------------------------------*/
   if(ADC12_DONE == YES) ADC1_Analog_Data_Process();
   /*---------------模拟量数据处理---------------------------------------------------------------*/
   if(AD7606_DONE == YES) Analog_Data_Process();//外部ADC采样完成，进行模拟数据数据处理
   /*---------------百毫秒级定时处理-------------------------------------------------------------*/
   if(HUNDRED_MSECOND_SIG == YES) Hundred_MSecond_Process();
   /*---------------半秒钟级定时处理-------------------------------------------------------------*/
   if(HALF_SECOND_SIG == YES) Half_Second_Process();
   /*---------------有按键按下，按键处理---------------------------------------------------------*/
   if(YESKEY != 0) Key_Pro();
   /*----------有屏幕刷新请求，屏幕处理----------------------------------------------------------*/
   //开机上电显示公司LOGO界面计时完毕且硬件初始化成功，则可以进入待机屏幕
   if(POWERON_TIME_COUNT >= POWERON_TIME && SCRFL == 0 && HARDWARE_EEOR_FLAG == 0)
    {
     SCRFL    = 1;       //进入待机屏幕
     CDX1     = 1;       //初始化一级菜单光标
     SCRAPPLY = YES;     //请求全屏刷新
     Top_Bar_Screen();   //显示顶部状态栏
     Bottom_Bar_Screen();//显示底部状态栏
     Top_Bar_Dupscr();   //更新顶部状态栏数据
     Bottom_Bar_Dupscr();//更新底部状态栏数据
    }
   if(SCRAPPLY == YES || DUPDATE == YES) LCD_Update();
   LCD_Set_Bright(LCD_BRIGHTNESS);//设置液晶屏背光
   /*--------------与逆变电源通讯接收数据处理程序------------------------------------------------*/
   SPWM_Power_Modbus_Data_Receive();
   /*--------------与蓝牙模块通讯接收数据处理程序------------------------------------------------*/
   if(BLUETOOTH_MODLE_INIT == YES) Bluetooth_Model_Data_Receive();//蓝牙模块初始化成功，才进行数据接收处理
   /*-------------------测量过程控制处理程序-----------------------------------------------------*/
   if(TEST_PROCESS_EN == YES) Test_Process_Control();
   /*-------------------量程校准过程控制处理程序-------------------------------------------------*/
   if(SCRFL == RANGE_CALIBRATION_SCREEN)//当前处在量程校准屏幕，则执行相应程序 
    {
     if(CALI_PROCESS_FLAG != 2) Impedance_Angle_Difference_Calculate();//输出电源调整电流过程中不计算角差和阻抗，
                                                                       //因为角差阻抗计算程序和输出电流调节程序都要读取FFT结果，
                                                                       //并清除数据可读取标志，两个程序有冲突，
                                                                       //导致电流调节程序执行异常缓慢
     if(CALI_PROCESS_EN == YES) Range_Calibration_Current_Control();//如果使能量程校准，则开始变频逆变电源恒流控制
    }
   /*-------------------硬件工作状态监测程序----------------------------------------------------*/
   Hardware_Work_State_Check();
   /*-------------------U插入后初始化程序--------------------------------------------------------*/
   if(U_DISK_CONNECT == YES && U_DISK_MOUNT == NO && U_DISK_MOUNT_TIME > 20)//U盘已插入，但是还没初始化成功，且延时已经超过200ms，则可以重新初始化
    {
     if(CH376DiskMount() == USB_INT_SUCCESS)
      {
       U_DISK_MOUNT = YES;//U盘初始化成功
      }
     else //U盘初始化不成功，则需要延时200ms后重新初始化
      {
       U_DISK_MOUNT_TIME = 0;  //初始化延时计时标志，重新延时
      }
    }
   else if(U_DISK_CONNECT == NO)//U盘已拔出
    {
     U_DISK_MOUNT = NO;//初始化U盘初始化标志
    }
   /*-------------------测试数据存储到U盘程序----------------------------------------------------*/ 
   if((POPUP_SCRFL == 4 && OPERATION_PROCESS == 3) ||
      (POPUP_SCRFL == 6 && OPERATION_PROCESS == 5))//测量结果存储操作弹出屏幕或者测量记录存储操作弹出屏幕,且需要将测试数据写入U盘
    {
     U_Disk_Write_Test_Data_File();//将测量结果存储至优盘
     if(POPUP_SCRFL == 4)      OPERATION_PROCESS = 4;//存储成功
     else if(POPUP_SCRFL == 6) OPERATION_PROCESS = 6;//存储成功
     SCRAPPLY = YES;//请求全屏刷新
    } 
   /*-------------------打印测量结果程序---------------------------------------------------------*/
   if(PRINTING_FLAG == YES) Print_Test_Data();//当前没有进行打印，则可以开始新的打印过程
   /*---------------继电器控制程序---------------------------------------------------------------*/
   Relay_Control();
  }
}
