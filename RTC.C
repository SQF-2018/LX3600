/*************************************************************************************************************
工程:   BQ32000RTC驱动程序(V1.1)
文件名: RTC.c
处理器: STM32F103系列CPU
编译器: RealView MDK
作者:   杨海涛
版权:   保定市力兴电子设备有限公司
日期：  2015.12.8
*************************************************************************************************************/
#include "stm32f10x.h"
#include "IOCONFIG.H"
#include "QJBL.H"


#define IO_TYPE 0   //IO口类型，0代表开漏输出，1代表推挽推出

#define BUSIN   0
#define BUSOUT  1

#define DELAYTIME 12 //延时约1.3us,时钟线频率在380KHZ左右

#define REG_READ 	0xd1
#define REG_WRITE 	0xd0
#define SFR 	    0x22 //Special function register
#define SF_KEY2 	0x21 //Special function key 2
#define SF_KEY1 	0x20 //Special function key 1
#define CFG2 		0x09 //Configuration 2
#define TCH2 		0x08 //Trickle charge enable
#define CAL_CFG1 	0x07 //Calibration and configuration
#define YEARS 		0x06 //Clock years
#define MONTH  		0x05 //Clock month
#define DATE 		0x04 //Clock date
#define DAY 	    0x03 //Clock day
#define CENT_HOURS 	0x02 //Clock hours, century, and CENT_EN bit
#define MINUTES 	0x01 //Clock minutes
#define SECONDS 	0x00 //Clock seconds and STOP bit

unsigned char RTC_IFACK; //record the SDA state to confirm if ACK has happened 
unsigned char ERR = 0; 
unsigned char RTC_DATA_ERR = 0;   //RTC数据错误
unsigned char RTC_OSC_FAIL = OFF; //RTC晶振失效标志

/************************************************************************************************************/
//函数名：void IIC_RTCDATA_DIR(unsigned char dir)
//目的：	SDATA输出方向
//输入：	dir	方向值
//		    1:输出
//		    0:输入
//DATA线使用PE1
/************************************************************************************************************/
void IIC_RTCDATA_DIR(unsigned char dir)
{
 unsigned int tmpreg;
 if(dir == BUSIN) 
  {
   tmpreg  = GPIOE->CRL;
   tmpreg &= 0xffffff0f;//清除PE1的IO口设置
   tmpreg |= 0x00000040;//设置为开漏输入
   GPIOE->CRL = tmpreg;
  }
 else
  {
   tmpreg  = GPIOE->CRL;
   tmpreg &= 0xffffff0f;//清除PE1的IO口设置
   tmpreg |= 0x00000070;//设置为开漏输出，最大输出速度50MHZ
   GPIOE->CRL = tmpreg;
  }
}

/************************************************************************************************************/
/*IIC总线输出一个开始总线信号*/
/************************************************************************************************************/
void IIC_RTC_Start(void) 
{
 RTC_SDA_1;
 IIC_RTCDATA_DIR(BUSOUT);//设置数据线为输出
 Delay(DELAYTIME);
 RTC_SCL_H;	  
 Delay(DELAYTIME);
 RTC_SDA_0;
 Delay(DELAYTIME);
 RTC_SCL_L;
}

/************************************************************************************************************/
/*IIC总线输出一个停止总线信号*/
/************************************************************************************************************/
void IIC_RTC_Stop(void)
{
 RTC_SDA_0;
 IIC_RTCDATA_DIR(BUSOUT);//设置数据线为输出
 Delay(DELAYTIME);
 RTC_SCL_H;
 Delay(DELAYTIME);
 RTC_SDA_1;
 Delay(DELAYTIME);
}

/************************************************************************************************************/
/*IIC总线输出一个ACK信号*/
/************************************************************************************************************/
void IIC_RTC_Ack(void)
{
 RTC_SDA_0;
 Delay(DELAYTIME);
 RTC_SCL_H;
 Delay(DELAYTIME);
 RTC_SCL_L;
 Delay(DELAYTIME);
}

/************************************************************************************************************/
/*IIC总线接收一个ACK信号或者输出一个NO_ACK信号*/
/************************************************************************************************************/
void IIC_RTC_Nack(unsigned char ack)
{
 IIC_RTCDATA_DIR(BUSIN);//设置数据线为输入
 Delay(DELAYTIME);
 RTC_SCL_H;
 Delay(DELAYTIME);
 if(RTC_SDA_STAT) RTC_IFACK = 1;
 else RTC_IFACK = 0; 
 RTC_SCL_L;
 if(ack == 1 && RTC_IFACK == 1) ERR += 1;
}

/************************************************************************************************************/
/*IIC总线输入一个字节，移位*/
/************************************************************************************************************/
unsigned char IIC_RTC_InByte(void) 
{
 unsigned char i,val=0;
 IIC_RTCDATA_DIR(BUSIN);//将数据线置为输入
 for(i=0x80;i>0;i/=2) //shift bit for masking
  { 
   Delay(DELAYTIME);
   RTC_SCL_H;
   Delay(DELAYTIME);
   if(RTC_SDA_STAT) val=(val | i); //read bit
   RTC_SCL_L;
  }    
 return(val);
}

/************************************************************************************************************/
/*IIC总线输出一个字节，移位*/
/************************************************************************************************************/
void IIC_RTC_OutByte(unsigned char data) 
{
 unsigned char i;
 IIC_RTCDATA_DIR(BUSOUT);//设置数据线为输出
 for(i=0x80;i>0;i/=2) //shift bit for masking
  { 
   if(i & data) RTC_SDA_1; //masking value with i , write to SENSI-BUS
   else RTC_SDA_0;
   Delay(DELAYTIME);
   RTC_SCL_H; //clk for SENSI-BUS
   Delay(DELAYTIME);
   RTC_SCL_L;
  }
 #if IO_TYPE == 0	//如果是开漏输出
  RTC_SDA_1;//释放数据线
 #elif IO_TYPE == 1	//如果是推挽输出，输出低电平，防止铁电输出低电平响应损坏芯片
  RTC_SDA_0;
 #endif
}

/************************************************************************************************************/
/*写寄存器*/
/************************************************************************************************************/
void RTC_WriteReg(unsigned char add,unsigned char wbyte)
{
 do
  {
   IIC_RTC_Start();
   IIC_RTC_OutByte(REG_WRITE);
   IIC_RTC_Nack(1);
   IIC_RTC_OutByte(add);
   IIC_RTC_Nack(1);
   IIC_RTC_OutByte(wbyte);
   IIC_RTC_Nack(1);
   IIC_RTC_Stop();
   if(ERR != 0)
    {
     ERR = 0;
     RTC_DATA_ERR += 1;
     if(RTC_DATA_ERR >= 5)
      {
	   RTC_DATA_ERR = 0;
	  }
    }
   else
    {
     RTC_DATA_ERR = 0;
    }
  }
 while(RTC_DATA_ERR);
}

/************************************************************************************************************/
/*读寄存器*/
/************************************************************************************************************/
unsigned char RTC_ReadReg (unsigned char add)
{
 unsigned char temp;
 do
  {
   IIC_RTC_Start();
   IIC_RTC_OutByte(REG_WRITE);
   IIC_RTC_Nack(1);
   IIC_RTC_OutByte(add);
   IIC_RTC_Nack(1);
   IIC_RTC_Start();
   IIC_RTC_OutByte(REG_READ);
   IIC_RTC_Nack(1);
   temp=IIC_RTC_InByte();
   IIC_RTC_Nack(0);
   IIC_RTC_Stop();
   if(ERR != 0)
    {
     ERR = 0;
     RTC_DATA_ERR += 1;
     if(RTC_DATA_ERR >= 5)
      {
	   RTC_DATA_ERR = 0;
	  }
    }
   else
    {
     RTC_DATA_ERR = 0;
    }
  }
 while(RTC_DATA_ERR);
 return(temp);
}

/************************************************************************************************************/
/*RTC初始化*/
/************************************************************************************************************/
void RTC_Init(void)
{
 unsigned char tmp;
 Delay_ms(1);  //A master device must wait at least 60 us after the RTC 
               //exits backup mode to generate a START condition.
 RTC_WriteReg(CFG2,0x0A);    //不充电
 RTC_WriteReg(TCH2,0x00);
 RTC_WriteReg(CAL_CFG1,0x80);//IRQ逻辑输出1，不进行频率测试输出
 tmp = RTC_ReadReg(SECONDS); //将当前秒钟数值读出
 if(tmp & 0x80)//如果STOP位为1(晶振停止)，则启动晶振
  {
   tmp &= 0x7F;                //置STOP位为0
   RTC_WriteReg(SECONDS,0x80); //强制振荡器起振，上电后先置STOP位为1，然后置STOP位为0
   RTC_WriteReg(SECONDS,tmp);  //置STOP位为0，并将上电时的秒钟数值写回
  }
 tmp = RTC_ReadReg(MINUTES);   //将当前分钟数值读出
 if(tmp & 0x80)//晶振失效标志OF被置位
  {
   tmp &= 0x7F;                //置OF(晶振失效)位为0，清除OF标志
   RTC_WriteReg(MINUTES,tmp);  //将上电时的分钟数值写回
  }
}

/************************************************************************************************************/
/*RTC读取时间值*/
/************************************************************************************************************/
void RTC_ReadTime(void)
{
 unsigned char tmp;
 NOW_TIME.year    = RTC_ReadReg(YEARS);
 NOW_TIME.month   = RTC_ReadReg(MONTH);
 NOW_TIME.date    = RTC_ReadReg(DATE);
 NOW_TIME.weekday = RTC_ReadReg(DAY);
 NOW_TIME.hour    = RTC_ReadReg(CENT_HOURS);
 tmp              = RTC_ReadReg(MINUTES);
 NOW_TIME.minute  = tmp & 0x7f;//屏蔽掉OF标志
 NOW_TIME.second  = RTC_ReadReg(SECONDS);
 //检测OF标志
 if(tmp >> 7) RTC_OSC_FAIL = ON;//RTC晶振失效
 else RTC_OSC_FAIL = OFF;
}

/************************************************************************************************************/
/*RTC设置时间值*/
/************************************************************************************************************/
void RTC_WriteTime(unsigned char year,unsigned char month,unsigned char date,unsigned char weekday,
                   unsigned char hour,unsigned char minute,unsigned char second)
{
 RTC_WriteReg(YEARS,year);
 RTC_WriteReg(MONTH,month);
 RTC_WriteReg(DATE,date);
 RTC_WriteReg(DAY,weekday);
 RTC_WriteReg(CENT_HOURS,hour);
 RTC_WriteReg(MINUTES,minute);
 RTC_WriteReg(SECONDS,second);
}

/************************************************************************************************************/
/*闰年计算*/
/************************************************************************************************************/
unsigned char Leap_Year(unsigned char year)
{
 unsigned short yeartmp;
 yeartmp  = 2000+Bcd_To_Hex(year);
 return ((yeartmp%4==0 && yeartmp%100!=0) || (yeartmp%400==0)); 
}

/************************************************************************************************************/
/*星期计算*/
/************************************************************************************************************/
unsigned char Week_Day(unsigned char year,unsigned char month,unsigned char date)
{
 unsigned short yeartmp;
 unsigned char monthtmp,datetmp,daytmp;
 yeartmp  = 2000+Bcd_To_Hex(year);
 monthtmp = Bcd_To_Hex(month);
 if(monthtmp==1||monthtmp==2){monthtmp+=12;yeartmp--;}
 datetmp  = Bcd_To_Hex(date);
 //基姆拉尔森计算公式,计算星期几
 daytmp = ((datetmp+2*monthtmp+3*(monthtmp+1)/5
          +yeartmp+yeartmp/4-yeartmp/100+yeartmp/400) % 7) + 1;
 return (daytmp); 
}
