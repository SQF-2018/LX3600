/********************************************************************************************************************************
工程:   AD7606的驱动程序及采样处理
文件名: AD7606.C 
处理器: STM32F103系列CPU
编译器: Keil C Version 5.05
作者:   程元
版权:   保定市力兴电子设备有限公司
日期：  2017.08.08
********************************************************************************************************************************/
#include "stm32f10x.h"
#include "QJBL.H"
#include "IOCONFIG.H"

/********************************************************************************************************************************
与AD7606通讯使用的SPI口初始化程序
SPI2在APB1总线，最高时钟频率36MHz
********************************************************************************************************************************/
void AD7606_SPI_Configuration(void)
{
 SPI_InitTypeDef SPI_InitStructure;//定义SPI口数据结构体
 SPI_Cmd(SPI2, DISABLE);           //禁止SPI2
 SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;//设置为双线双向全双工模式
 SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;                //配置为主设备
 SPI_InitStructure.SPI_DataSize          = SPI_DataSize_16b;               //16位数据传输
 SPI_InitStructure.SPI_CPOL              = SPI_CPOL_High;                  //空闲时SCK保持高电平
 SPI_InitStructure.SPI_CPHA              = SPI_CPHA_2Edge;                 //数据从第一个时钟沿开始，此处为上升沿开始
 SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;                   //软件从设备管理
 SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;        //波特率= 36M/16 = 4.5MHz
 SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;		       //数据位高位在前
 SPI_InitStructure.SPI_CRCPolynomial     = 7;						       //对CRC复初值
 SPI_Init(SPI2, &SPI_InitStructure);
 SPI_I2S_ITConfig(SPI2, SPI_IT_CRCERR, DISABLE);//关闭CRC错误中断
 SPI_Cmd(SPI2, ENABLE);                         //使能SPI2
}

/********************************************************************************************************************************
外部中断初始化
AD7606-BUSY引脚
BUSY:PD8
********************************************************************************************************************************/
void AD7606_EXTI_Configuration(void)
{
 EXTI_InitTypeDef EXTI_InitStructure; 
 GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource8);//PD8为外部中断输入引脚 
 EXTI_InitStructure.EXTI_Line    = EXTI_Line8;          //开启外部中断线8
 EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt; //设置为中断模式 
 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//设置为下降沿中断
 EXTI_InitStructure.EXTI_LineCmd = ENABLE;              //允许外部中断
 EXTI_Init(&EXTI_InitStructure);                        //外部中断初始化
}

///********************************************************************************************************************************
//设置定时器8
//参数:base_freq 被采样的正弦信号基波频率,单位Hz
//参数:data_len  每个周波采样的长度,即采样点数
//由于外部ADC最大采样率与高采样倍率有关
//定时器8的CC1事件用于启动AD7606采样，TIM8_CH1(PC6)引脚的上升沿可以启动AD转换
//********************************************************************************************************************************/
//void TIM8_Configuration(float base_freq,unsigned int data_len)
//{
// TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//定义定时器数据结构体
// TIM_OCInitTypeDef        TIM_OCInitStructure;  //定义定时器比较输出数据结构体
// TIM_BDTRInitTypeDef      TIM_BDTRInitStructure;//定义定时器BDTR数据结构体
// float tim_prescal=0,tim_period,max_sample_rat; //定时器预分频系数、周期、AD允许的最大采样率

// switch(AD7606_OS_RAT)
//  {
//   case  0:max_sample_rat = 200000;//200KHz
//           break;
//   case  2:max_sample_rat = 100000;//100KHz
//           break;
//   case  4:max_sample_rat = 50000; //50KHz
//           break;
//   case  8:max_sample_rat = 25000; //25KHz
//           break;
//   case 16:max_sample_rat = 12500; //12.5KHz
//           break;
//   case 32:max_sample_rat = 6250;  //6.25KHz
//           break;
//   case 64:max_sample_rat = 3125;  //3.125KHz
//           break;
//   default:max_sample_rat = 200000;//200KHz
//           break;
//  }
// if(base_freq * data_len <= max_sample_rat)//采样率不得大于最大允许的采样率
//  {
//   do
//    {
//     tim_period = 72000000 / (1 + tim_prescal++) / (base_freq * data_len) + 0.5;//计算所需定时周期数值并四舍五入
//    }
//   while(tim_period > 65535 || tim_prescal > 65535);//判定计算出的数值是否越限

//   TIM_DeInit(TIM8);//复位TIM8定时器

//   //设置定时器8
//   TIM_TimeBaseStructure.TIM_Period        = tim_period  - 1;   //周期寄存器      
//   TIM_TimeBaseStructure.TIM_Prescaler     = tim_prescal - 1;   //预分频系数
//   //TIM8在高速APB2总线，速度为72MHZ       
//   TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;               // 时钟分割  
//   TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;//计数方向向上计数
//   TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
//   //配置TIM8通道1，CC1事件
//   TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1;         //TIM8脉冲宽度调制模式1;向上计数时，一旦TIMX_CNT<TIMX_CCRX时通道为有效电平，否则为无效电平； 
//   TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable;  //使能输出状态
//   TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//禁止互补输出状态
//   TIM_OCInitStructure.TIM_Pulse        = tim_period / 2;          //设置捕获/比较寄存器的值 
//   TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_Low;      //设置OCX输出极性
//   TIM_OCInitStructure.TIM_OCNPolarity  = TIM_OCPolarity_Low;      //设置OCNX输出极性         
//   TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Reset;   //设置输出空闲状态，当MOE=0时，如果实现了OCXN，则死区后OCX=0
//   TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;  //设置输出空闲状态，当MOE=0时，死区后OCXN=0      
//   TIM_OC1Init(TIM8, &TIM_OCInitStructure);                        //设置TIMX的OC1输出通道
//   
//   //设置刹车特性，死区时间，锁电平，OSSI，OSSR状态和AOE（自动输出使能）
//   TIM_BDTRInitStructure.TIM_OSSRState       = TIM_OSSRState_Enable;      
//   TIM_BDTRInitStructure.TIM_OSSIState       = TIM_OSSIState_Enable;      
//   TIM_BDTRInitStructure.TIM_LOCKLevel       = TIM_LOCKLevel_3;//使用3级锁定
//   //锁定TIMx_BDTR寄存器的DTG、BKE、BKP、AOE位，锁定TIMx_CR2寄存器的OISx、OISxN位
//   //锁定TIMx_CCER寄存器的CCxP、CCNxP位，锁定TIMx_BDTR寄存器的OSSR、OSSI位
//   //锁定TIMx_CCMRx寄存器的OCxM、OCxPE位      
//   TIM_BDTRInitStructure.TIM_DeadTime        = 0;                          //设置死区时间为0
//   TIM_BDTRInitStructure.TIM_Break           = TIM_Break_Disable;          //禁止刹车功能
//   TIM_BDTRInitStructure.TIM_BreakPolarity   = TIM_BreakPolarity_High;     //刹车信号极性为高电平有效  
//   TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;//禁止自动输出使能功能
//   TIM_BDTRConfig(TIM8, &TIM_BDTRInitStructure);

//   //允许TIM8计数
//   TIM_Cmd(TIM8, ENABLE);
//   //TIM_CtrlPWMOutputs(TIM8, DISABLE);//禁止PWM输出，清零MOE位
//   TIM_CtrlPWMOutputs(TIM8, ENABLE);//使能PWM输出，置位MOE位
//  }
//}

///********************************************************************************************************************************
//PC4(AD-BUSY)外部中断采样程序
//********************************************************************************************************************************/
//void EXTI4_IRQHandler(void)
//{
// if(AD7606_DONE == NO)
//  {
//   AD_CS_L;//使能AD7606片选信号
//   /*读取电压通道数据*/
//   SPI1->DR = 0x0000;//发出数据0x0000，目的是发出16个时钟信号   
//   while((SPI1->SR & SPI_I2S_FLAG_RXNE) == 0);//等待数据发送完成，并等待数据接收完成	
//    { 
//     AD7606_DATA_BUF[VOLTAGE_CHANNAL][AD7606_POINT] = SPI1->DR;//将读取到的数据存储到缓冲区内
//    }
//   /*读取电流通道数据*/ 
//   SPI1->DR = 0x0000;//发出数据0x0000，目的是发出16个时钟信号   
//   while((SPI1->SR & SPI_I2S_FLAG_RXNE) == 0);//等待数据发送完成，并等待数据接收完成	
//    { 
//     AD7606_DATA_BUF[CURRENT_CHANNAL][AD7606_POINT] = SPI1->DR;//将读取到的数据存储到缓冲区内
//    }
//   /*读取零序电压通道数据*/ 
//   SPI1->DR = 0x0000;//发出数据0x0000，目的是发出16个时钟信号   
//   while((SPI1->SR & SPI_I2S_FLAG_RXNE) == 0);//等待数据发送完成，并等待数据接收完成	
//    { 
//     AD7606_DATA_BUF[VOLTAGE_50HZ_CHANNAL][AD7606_POINT] = SPI1->DR;//将读取到的数据存储到缓冲区内
//    } 
//   AD_CS_H; //禁止AD7606片选信号
//   AD7606_POINT += 1;
//   if(AD7606_POINT >= FFT_POINT)
//    {
//     TIM_Cmd(TIM8, DISABLE);           //停止TIM8计数
//     TIM_CtrlPWMOutputs(TIM8, DISABLE);//禁止PWM输出，清零MOE位
//     AD7606_POINT = 0;                 //清零AD7606采样点数计数器
//     AD7606_DONE  = YES;               //AD7606一周波采样完成
//    }
//  }
// EXTI_ClearITPendingBit(EXTI_Line4);//清除挂起寄存器中中断标志
//}

///********************************************************************************************************************************
//AD7606过采样设置程序
//参数:rat 过采样倍率，取值0、2、4、8、16、32、64
//********************************************************************************************************************************/
//void AD7606_OverSample_Rat(unsigned char rat)
//{
////无过采样,SNR 5V 89dB,SNR 10V 90dB,BW 5V 15KHz,BW 10V 22KHz,CONVST Frequency 200K
////2倍过采样,SNR 5V 91.2dB,SNR 10V 92dB,BW 5V 15KHz,BW 10V 22KHz,CONVST Frequency 100K
////4倍过采样,SNR 5V 92.6dB,SNR 10V 93.6dB,BW 5V 13.7KHz,BW 10V 18.5KHz,CONVST Frequency 50K
////8倍过采样,SNR 5V 94.2dB,SNR 10V 95dB,BW 5V 10.3KHz,BW 10V 11.9KHz,CONVST Frequency 25K
////16倍过采样,SNR 5V 95.5dB,SNR 10V 96dB,BW 5V 6KHz,BW 10V 6KHz,CONVST Frequency 12.5K
////32倍过采样,SNR 5V 96.4dB,SNR 10V 96.7dB,BW 5V 3KHz,BW 10V 3KHz,CONVST Frequency 6.25K
////64倍过采样,SNR 5V 96.9dB,SNR 10V 97dB,BW 5V 1.5KHz,BW 10V 1.5KHz,CONVST Frequency 3.125K
////无过采样,SNR 5V 89dB,SNR 10V 90dB,BW 5V 15KHz,BW 10V 22KHz,CONVST Frequency 200K
////如果设置的过采样倍率超范围，则使用default设置，并设置AD7606_OS_RAT = 0以便程序得知AD7606被默认设置的过采样率
//}

///********************************************************************************************************************************
//AD7606待机模式设置程序
//********************************************************************************************************************************/
//void AD7606_STBY_Mode(unsigned char mode)
//{
// //正常工作模式，5V量程
// //AD_STBY_H;
// //AD_RANGE_5V_L;
// Delay_ms(50); //延时50ms，保证AD7606上电完全，手册要求30ms
// AD_RESET_H;   //复位芯片
// Delay_ms(1);  //延时1ms
// AD_RESET_L;   //复位完成
// Delay_ms(1);  //延时1ms
//}

///********************************************************************************************************************************
//电压通道增益切换控制程序
//参数: gear 增益档位，0、1、2
//********************************************************************************************************************************/
//void Voltage_Gain_Switch(unsigned char gear)
//{
// switch(gear)
//  {
//   case 0://增益档位0
//          JDQ_FY_OFF; //分压电阻网络切换到最大分压比
//          JDQ_VZY_OFF;//电压增益设置为1倍
//          VOLTAGE_GAIN_GEAR = 0;
//          break;
//   case 1://增益档位1
//          JDQ_FY_ON;  //分压电阻网络切换到小分压比
//          JDQ_VZY_OFF;//电压增益设置为1倍
//          VOLTAGE_GAIN_GEAR = 1;
//          break;
//   case 2://增益档位2
//          JDQ_FY_ON; //分压电阻网络切换到小分压比
//          JDQ_VZY_ON;//电压增益设置为10倍
//          VOLTAGE_GAIN_GEAR = 2;
//          break;
//  }
//}

///********************************************************************************************************************************
//电流通道增益切换控制程序
//参数: gear 增益档位，0、1
//********************************************************************************************************************************/
//void Current_Gain_Switch(unsigned char gear)
//{
// switch(gear)
//  {
//   case 0://增益档位0
//          JDQ_IZY_OFF;//电流增益设置为5倍
//          CURRENT_GAIN_GEAR = 0;
//          break;
//   case 1://增益档位1
//          JDQ_IZY_ON;//电流增益设置为41倍
//          CURRENT_GAIN_GEAR = 1;
//          break;
//  }
//}

///********************************************************************************************************************************
//AD模拟量增益切换控制程序
//参数: ctrl_en 切换控制允许标志，0:禁止切换，电压电流切换到最大输入量程;1:允许自动切换增益
//********************************************************************************************************************************/
//void AD_Analog_Gain_Switch (unsigned char ctrl_en)
//{
// unsigned short i;
// unsigned short u_over_upper_limit,u_over_lower_limit,i_over_upper_limit,i_over_lower_limit;
// u_over_upper_limit = 0;//电压通道采样值大于最高阀值次数
// u_over_lower_limit = 0;//电压通道采样值大于最低阀值次数
// i_over_upper_limit = 0;
// i_over_lower_limit = 0;
// /*----------------------------------------------允许自动切换量程---------------------------------------------*/
// if(ctrl_en)
//  {
//   for(i=0;i<FFT_POINT;i++)/*逐点检测电压、电流采样值是否超限*/
//    {
//     switch(VOLTAGE_GAIN_GEAR)/*检测电压通道采样值是否越限*/
//      {
//       case 0://分压比0.135643166579（62K+9.729634802135K）
//              //仪器输入6.3825～25.0224Vrms；（量程下限比6.8825Vrms低0.5Vrms，量程重合部分）
//              //分压输出0.8657～3.3941Vrms
//              //AD输入  ±1.2243（AD采样数值±8023）～±4.8Vpp（AD采样数值±31457）
//              /*电压AD采样值小于±8023，需要切换到增益更大的档位*/
//              if(AD7606_DATA_BUF[VOLTAGE_CHANNAL][i] > 8023 || AD7606_DATA_BUF[VOLTAGE_CHANNAL][i] < -8023) u_over_lower_limit++;
//              break;
//       case 1://分压比0.493148246266（10K+9.729634802135K），1倍放大
//              //仪器输入0.63825～6.8825Vrms
//              //分压输出0.31475～3.39409Vrms
//              //AD输入  ±0.4451（AD采样数值±2917）～±4.8Vpp（AD采样数值±31457）
//              /*电压AD采样值大于±31457，需要切换到增益更小的档位*/
//              if(AD7606_DATA_BUF[VOLTAGE_CHANNAL][i] > 31457 || AD7606_DATA_BUF[VOLTAGE_CHANNAL][i] < -31457) u_over_upper_limit++;
//              /*电压AD采样值小于±2917，需要切换到增益更大的档位*/
//              else if(AD7606_DATA_BUF[VOLTAGE_CHANNAL][i] > 2917 || AD7606_DATA_BUF[VOLTAGE_CHANNAL][i] < -2917) u_over_lower_limit++;
//              break;
//       case 2://分压比0.493148246266（10K+9.729634802135K），10倍放大
//              //仪器输入 0～0.68825Vrms
//              //分压输出 0～0.33941Vrms
//              //AD输入   0～±4.8Vpp（AD采样数值±31457）
//              /*电压AD采样值大于±31457，需要切换到增益更小的档位*/
//              if(AD7606_DATA_BUF[VOLTAGE_CHANNAL][i] > 31457 || AD7606_DATA_BUF[VOLTAGE_CHANNAL][i] < -31457) u_over_upper_limit++;
//              break;
//      }
//     switch(CURRENT_GAIN_GEAR)/*检测电流通道采样值是否越限*/
//      {
//       case 0://0.25Ω取样电阻，5倍放大
//              //仪器输入     281.1317mArms～2.71528Arms
//              //取样电阻输出 70.28293 ～ 678.82mVrms
//              //AD输入       ±496.97536mVrms（AD采样数值±3257）～ ±4.8Vpp（AD采样数值±31457）
//              /*AD采样值小于±3257，需要切换到增益更大的档位*/
//              if(AD7606_DATA_BUF[CURRENT_CHANNAL][i] > 3257 || AD7606_DATA_BUF[CURRENT_CHANNAL][i] < -3257) i_over_lower_limit++;
//              break;
//       case 1://0.25Ω取样电阻，41倍放大
//              //仪器输入     0 ～ 331.1317mArms
//              //取样电阻输出 0 ～ 82.78292mVrms
//              //AD输入       0 ～ ±4.8Vpp（AD采样数值±31457）
//              /*AD采样值大于±31457，需要切换到增益更小的档位*/
//              if(AD7606_DATA_BUF[CURRENT_CHANNAL][i] > 31457 || AD7606_DATA_BUF[CURRENT_CHANNAL][i] < -31457) i_over_upper_limit++;
//              break;
//      }
//    }      
//   /*----------------------------------电压量程自动切换------------------------------------*/   
//   if(u_over_upper_limit > 0)/*采样值至少有一个点超过了量程最大值，需要切换到增益更小的档位*/
//    {
//     if(VOLTAGE_GAIN_GEAR == 1)
//      {
//       Voltage_Gain_Switch(0);  //电压通道增益切换到档位0 
//       JDQ_FY_ACTION_FLAG = YES;//电阻分压网络进行了切换动作，需要延时采样
//       AD_GAIN_SWITCHING  = YES;//增益有变化 
//       Delay_ms(5);             //延时5ms，等待继电器吸合释放完成
//      }
//     else if(VOLTAGE_GAIN_GEAR == 2)
//      {
//       Voltage_Gain_Switch(1); //电压通道增益切换到档位1
//       AD_GAIN_SWITCHING = YES;//增益有变化 
//       Delay_ms(5);            //延时5ms，等待继电器吸合释放完成
//      } 
//    }
//   else if(u_over_lower_limit == 0)/*采样值所有点都没有超过量程最小值，需要切换到增益更大的档位*/
//    {
//     if(VOLTAGE_GAIN_GEAR == 0)
//      {
//       Voltage_Gain_Switch(1);  //电压通道增益切换到档位1
//       JDQ_FY_ACTION_FLAG = YES;//电阻分压网络进行了切换动作，需要延时采样
//       AD_GAIN_SWITCHING  = YES;//增益有变化
//       Delay_ms(5);             //延时5ms，等待继电器吸合释放完成
//      }
//     else if(VOLTAGE_GAIN_GEAR == 1)
//      {
//       Voltage_Gain_Switch(2); //电压通道增益切换到档位2
//       AD_GAIN_SWITCHING = YES;//增益有变化
//       Delay_ms(5);            //延时5ms，等待继电器吸合释放完成
//      }     
//    }
//   /*----------------------------------电流量程自动切换------------------------------------*/   
//   if(i_over_upper_limit > 0)/*采样值至少有一个点超过了量程最大值，需要切换到增益更小的档位*/
//    {
//     if(CURRENT_GAIN_GEAR == 1)
//      {
//       Current_Gain_Switch(0); //电流通道增益切换到档位0
//       AD_GAIN_SWITCHING = YES;//增益有变化
//       Delay_ms(5);            //延时5ms，等待继电器吸合释放完成
//      }
//    }
//   else if(i_over_lower_limit == 0)/*采样值所有点都没有超过量程最小值，需要切换到增益更大的档位*/
//    {
//     if(CURRENT_GAIN_GEAR == 0)
//      {
//       Current_Gain_Switch(1); //电流通道增益切换到档位1
//       AD_GAIN_SWITCHING = YES;//增益有变化
//       Delay_ms(5);            //延时5ms，等待继电器吸合释放完成
//      }
//    }   
//  }
// else /*-------------------------------------禁止自动切换量程-------------------------------------*/
//  {
//   if(RELAY_DETECT_EN == OFF)//没有进行继电器检测，可以对继电器进行控制
//    {
//     Voltage_Gain_Switch(0);//电压通道增益切换到档位0
//     Current_Gain_Switch(0);//电流通道增益切换到档位0
//    }
//  }
//}

/********************************************************************************************************************************
AD7606初始化程序
ADI官网最新文档说明AD7606上电初始化需要先关断AD7606再开启，同时给复位信号，这样可以保证AD正常工作
********************************************************************************************************************************/
void AD7606_Init(void)
{
// AD_Analog_Gain_Switch(0);            //初始化电压通道增益控制
// AD7606_OS_RAT = 8;                   //设置过采样倍率为8倍，12HZ时以2HZ为基频(4096HZ),180HZ时以10HZ为基频(20480HZ)
// AD7606_SPI_Configuration();          //配置STM32与AD7606的SPI接口
// AD7606_EXTI_Configuration();         //配置AD-BUSY引脚外部中断
// //AD7606_OverSample_Rat(AD7606_OS_RAT);//设置过采样倍率
// //AD7606_STBY_Mode('G');               //完全关断AD7606
// AD7606_STBY_Mode('A');               //启动AD7606
// TIM8_Configuration(FFT_BASE_FREQENCY,FFT_POINT);//初始化TIM8,设置基波频率FFT_BASE_FREQENCY,采样AD_SAMPLE_POINT点,采样开始
}

