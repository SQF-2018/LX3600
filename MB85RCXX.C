/***********************************************************************************
工程:   MB85RCxx系列IIC铁电存储器驱动程序(V1.0)
文件名: MB85RCxx.c
处理器: STM32F1.3系列CPU
编译器: Keil C Version 4.03
作者:   程元
版权:   保定市力兴电子设备有限公司
日期：  2014.10.29
***********************************************************************************/
#include "stm32f10x.h"
#include "IOCONFIG.H"
#include "QJBL.H"
#include "FUNCTION.H"

#define IO_TYPE 0   //IO口类型，0代表开漏输出，1代表推挽推出

#define BUSIN   0
#define BUSOUT  1

#define FM_DELAYTIME 12 //延时1.29us,高电平1.29us,时钟线频率在387KHZ左右

#define DATA_READ     0xa1
#define DATA_WRITE    0xa0

unsigned char IFACK;//record the SDA state to confirm if ACK has happened
unsigned char IIC_ERR = 0; 
unsigned char DATA_ERR = 0;

/*******************************************************************************************************************************
改变SDA引脚数据方向函数
参数:dir 方向值；0:输入；1:输出
DATA线使用PC1
*******************************************************************************************************************************/
void IIC_DATA_DIR(unsigned char dir)
{
 unsigned int tmpreg;
 if(dir == BUSIN) 
  {
   tmpreg = GPIOC->CRL;
   tmpreg &= 0xffffff0f;//清除PC1的IO口设置
   tmpreg |= 0x00000040;//设置为开漏输入
   GPIOC->CRL = tmpreg;
   GPIOC->ODR |= 0x00000080;//内部弱上拉效果
  }
 else
  {
   tmpreg = GPIOC->CRL;
   tmpreg &= 0xffffff0f;//清除PC1的IO口设置
   tmpreg |= 0x00000070;//设置为开漏输出，最大输出速度50MHZ
   GPIOC->CRL = tmpreg;
  }
}

/*************************************************************************************************/
/*IIC总线输出一个开始总线信号*/
/*************************************************************************************************/
void IIC_Start(void) 
{
 FM_SDA_1;
 IIC_DATA_DIR(BUSOUT);//设置数据线为输出
 Delay(FM_DELAYTIME);
 FM_SCL_H;
 Delay(FM_DELAYTIME);
 FM_SDA_0;
 Delay(FM_DELAYTIME);
 FM_SCL_L;
}

/*************************************************************************************************/
/*IIC总线输出一个停止总线信号*/
/*************************************************************************************************/
void IIC_Stop(void)
{
 FM_SDA_0;
 IIC_DATA_DIR(BUSOUT);//设置数据线为输出
 Delay(FM_DELAYTIME);
 FM_SCL_H;
 Delay(FM_DELAYTIME);
 FM_SDA_1;
 Delay(FM_DELAYTIME);
}

/*************************************************************************************************/
/*IIC总线输出一个ACK信号*/
/*************************************************************************************************/
void IIC_Ack(void)
{
 FM_SDA_0;
 Delay(FM_DELAYTIME);
 FM_SCL_H;
 Delay(FM_DELAYTIME);
 FM_SCL_L;
 Delay(FM_DELAYTIME);
}

/*************************************************************************************************/
/*IIC总线接收一个ACK信号或者输出一个NO_ACK信号*/
/*************************************************************************************************/
void IIC_Nack(unsigned char ack)
{
 IIC_DATA_DIR(BUSIN);//设置数据线为输入
 Delay(FM_DELAYTIME);
 FM_SCL_H;
 Delay(FM_DELAYTIME);
 if(FM_SDA_STAT) IFACK = 1;
 else IFACK = 0; 
 FM_SCL_L;
 if(ack == 1 && IFACK == 1) IIC_ERR += 1;
}

/*************************************************************************************************/
/*IIC总线输入一个字节，移位*/
/*************************************************************************************************/
unsigned char IIC_InByte(void) 
{
 unsigned char i,val=0;
 IIC_DATA_DIR(BUSIN);//将数据线置为输入
 for(i=0x80;i>0;i/=2) //shift bit for masking
  { 
   Delay(FM_DELAYTIME);
   FM_SCL_H;
   Delay(FM_DELAYTIME);
   if(FM_SDA_STAT) val=(val | i); //read bit
   FM_SCL_L;
  }    
 return(val);
}

/*************************************************************************************************/
/*IIC总线输出一个字节，移位*/
/*************************************************************************************************/
void IIC_OutByte(unsigned char data) 
{
 unsigned char i;
 IIC_DATA_DIR(BUSOUT);//设置数据线为输出
 for(i=0x80;i>0;i/=2) //shift bit for masking
  { 
   if(i & data) FM_SDA_1; //masking value with i , write to SENSI-BUS
   else FM_SDA_0;
   Delay(FM_DELAYTIME);
   FM_SCL_H; //clk for SENSI-BUS
   Delay(FM_DELAYTIME);
   FM_SCL_L;
  }
 #if IO_TYPE == 0    //如果是开漏输出
  FM_SDA_1;//释放数据线
 #elif IO_TYPE == 1    //如果是推挽输出，输出低电平，防止铁电输出低电平响应损坏芯片
  FM_SDA_0;
 #endif
}

/*************************************************************************************************/
/*写一字节数据*/
/*************************************************************************************************/
void Mb85rcxx_WriteByte(unsigned short add,unsigned char wbyte)
{
 do
  {
   IIC_Start();
   IIC_OutByte(DATA_WRITE);
   IIC_Nack(1);
   IIC_OutByte(add>>8);
   IIC_Nack(1);
   IIC_OutByte(add);
   IIC_Nack(1);
   IIC_OutByte(wbyte);
   IIC_Nack(1);
   IIC_Stop();
   if(IIC_ERR != 0)
    {
     IIC_ERR = 0;
     DATA_ERR += 1;
     if(DATA_ERR >= 5)//如果出错，则循环操作5遍
      {
       DATA_ERR = 0;
      }
    }
   else
    {
     DATA_ERR = 0;
    }
  }
 while(DATA_ERR);
}

/*************************************************************************************************/
/*读一字节数据*/
/*************************************************************************************************/
unsigned char Mb85rcxx_ReadByte (unsigned short add)
{
 unsigned char temp;
 do
  {
   IIC_Start();
   IIC_OutByte(DATA_WRITE);
   IIC_Nack(1);
   IIC_OutByte(add>>8);
   IIC_Nack(1);
   IIC_OutByte(add);
   IIC_Nack(1);
   IIC_Start();
   IIC_OutByte(DATA_READ);
   IIC_Nack(1);
   temp=IIC_InByte();
   IIC_Nack(0);
   IIC_Stop();
   if(IIC_ERR != 0)
    {
     IIC_ERR = 0;
     DATA_ERR += 1;
     if(DATA_ERR >= 5)
      {
       DATA_ERR = 0;
      }
    }
   else
    {
     DATA_ERR = 0;
    }
  }
 while(DATA_ERR);
 return(temp);
}

/*************************************************************************************************/
/*将FLOAT型变量存入到铁闪中去
  data:要存入的浮点数 add:要存入的地址，一个浮点数四个字节(一个字节8位)*/
/*************************************************************************************************/
void Mb85rcxx_Write_Float(unsigned short add,float data) 
{ 
 unsigned char i;
 union float_to_char //使用联合定义方式取得浮点数的4字节数据,data和px[4]共用一块4字节内存空间
  {
   float         data_f;
   unsigned char px[4];
  }tmp;
 tmp.data_f = data;
 for(i=0;i<4;i++)
  { 
   Mb85rcxx_WriteByte(add+i,tmp.px[i]); 
  } 
}

/*************************************************************************************************/
/*将存储的FLOAT型变量从铁闪中读出
  add:要读取的地址*/
/*************************************************************************************************/
float Mb85rcxx_Read_Float(unsigned short add)       
{ 
 unsigned char i;
 union float_to_char //使用联合定义方式取得浮点数的4字节数据,data和px[4]共用一块4字节内存空间
  {
   float         data_f;
   unsigned char px[4];
  }tmp;
 for(i=0;i<4;i++)
  { 
   tmp.px[i] = Mb85rcxx_ReadByte(add+i); 
  } 
 return(tmp.data_f); 
}

/*************************************************************************************************/
//写入测量记录
/*************************************************************************************************/
void Mb85rcxx_Write_Test_Record(void)
{

}

/*************************************************************************************************/
//读取测量记录
//参数:    now_record_number 要读取的测量记录编号，以最新保存的测量记录编号为基础
/*************************************************************************************************/
void Mb85rcxx_Read_Test_Record(unsigned char now_record_number)
{
 
}

/*************************************************************************************************/
/*读取试验设置的参数值*/
/*************************************************************************************************/
void Mb85rcxx_Read_Test_Set(void)
{

}

/*************************************************************************************************/
/*写入试验设置的参数值*/
/*************************************************************************************************/
void Mb85rcxx_Write_Test_Set(void)
{

}

/*************************************************************************************************/
/*写入试验设置的参数默认值*/
/*************************************************************************************************/
void Mb85rcxx_Write_Test_Set_Default(void)
{
 
}

/*************************************************************************************************/
/*读取模拟量校正系数*/
/*************************************************************************************************/
void Mb85rcxx_Read_Correction_Coefficient(void)
{
 
}

/*************************************************************************************************/
/*写入模拟量校正系数默认值*/
/*************************************************************************************************/
void Mb85rcxx_Write_Correction_Coefficient_Default(unsigned short control_bit)
{
 
}

/*************************************************************************************************/
/*开机读取设置的参数值*/
/*************************************************************************************************/
void Mb85rcxx_ReadSheZhi(void)
{
 unsigned short tmp1,tmp2;
 unsigned char i;
 /*----------------------------读取存储器空标志--------------------------------------------------*/
 tmp1 = Mb85rcxx_ReadByte(ee_data_sig1);//读取存储器空标志1
 tmp2 = Mb85rcxx_ReadByte(ee_data_sig2);//读取存储器空标志2
 /*--------------------------如果存储器为空，则写入默认值----------------------------------------*/
 //设置参数存储区空，需要重新写入默认数据
 if(tmp1 != 0x1a || tmp2 != 0xa1)//存储器为空片
  {
   Mb85rcxx_WriteByte(ee_data_sig1,0x1a);//写入存储器空标志1
   Mb85rcxx_WriteByte(ee_data_sig2,0xa1);//写入存储器空标志2
   tmp1 = Mb85rcxx_ReadByte(ee_data_sig1);//重新读取存储器空标志1
   tmp2 = Mb85rcxx_ReadByte(ee_data_sig2);//重新读取存储器空标志2
   if(tmp1 != 0x1a || tmp2 != 0xa1)//读出的数据仍然不对
    {
     Set_Bit(HARDWARE_EEOR_FLAG,FM_MEM_ERR);//铁电存储器硬件错误
     return;
    }
   for(i=0;i<4;i++)
    {
     Mb85rcxx_WriteByte(ee_device_number0 + i,0);//默认装置编号0000
    }
   Mb85rcxx_Write_Test_Set_Default();                     //写入试验参数设置默认值
   Mb85rcxx_Write_Correction_Coefficient_Default(ALL_BIT);//写入所有校准值默认数据
   Mb85rcxx_WriteByte(ee_test_record_number,0);           //默认最新测量记录编号为0
   Mb85rcxx_WriteByte(ee_test_record_amount,0);           //默认已保存的测量记录数量为0
  }
 /*-------------------------------读取保存数据---------------------------------------------------*/
 //读取装置编号
 for(i=0;i<4;i++)
  {
   DEVICE_NUMBER[i] = Mb85rcxx_ReadByte(ee_device_number0 + i);
  }
 //读取最后一次测试参数设置
 Mb85rcxx_Read_Test_Set();
 /*-----------------------------读取模拟量校准系数------------------------------------------------*/
 Mb85rcxx_Read_Correction_Coefficient();//读取模拟量校准系数
 /*------------------------读取保存的测量记录编号及数量-------------------------------------------*/
 TEST_RECORD_NUMBER = Mb85rcxx_ReadByte(ee_test_record_number);//读取最新测量记录编号
 TEST_RECORD_AMOUNT = Mb85rcxx_ReadByte(ee_test_record_amount);//读取已保存的测量记录数量
}
