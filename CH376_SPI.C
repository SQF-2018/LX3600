/*************************************************************************************************/
//CH376芯片 硬件标准SPI串行连接的硬件抽象层 V1.0 
//提供I/O接口子程序
//修改：程元
//修改日期：2014.10.29
/*************************************************************************************************/

#include "stm32f10x.h"
#include "IOCONFIG.H"
#include "QJBL.H"
#include "FILE_SYS.H"

/*************************************************************************************************/
//用户定义，引脚及功能控制的宏
/*************************************************************************************************/
#define CH376_INT_WIRE //定义CH376_INT_WIRE宏，确认使用CH376的INT引脚

/*******************************************************************************************************************************
设置SPI3，用于与CH376通讯
SPI3在APB1总线，最高时钟频率36MHz
*******************************************************************************************************************************/
void SPI3_Configuration(void)
{
 SPI_InitTypeDef SPI_InitStructure;//定义SPI口数据结构体
 SPI_Cmd(SPI3, DISABLE);           //禁止SPI3
 SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;//设置为双线双向全双工模式
 SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;                //配置为主设备
 SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;                //8位数据传输
 SPI_InitStructure.SPI_CPOL              = SPI_CPOL_High;                  //空闲时SCK保持高电平
 SPI_InitStructure.SPI_CPHA              = SPI_CPHA_2Edge;                 //数据从第二个时钟沿开始
 SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;                   //软件从设备管理
 SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;        //波特率=36M/4=9M
 SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;               //数据位高位在前
 SPI_InitStructure.SPI_CRCPolynomial     = 7;                              //对CRC赋初值
 SPI_Init(SPI3, &SPI_InitStructure);
 SPI_I2S_ITConfig(SPI3, SPI_IT_CRCERR, DISABLE);//关闭CRC错误中断
 SPI_Cmd(SPI3, ENABLE);                         //使能SPI3
}

/*************************************************************************************************/
//硬件SPI输出且输入8个位数据
/*************************************************************************************************/
unsigned char Spi376Exchange(unsigned char d)  
{  /* 为了提高速度,可以将该子程序做成宏以减少子程序调用层次 */
 unsigned char i;
 SPI3->DR = d;
 while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_RXNE) == 0);//等待数据接收完成
 i = SPI3->DR;//读取收到的数据
 return i&0xff;
}

/*************************************************************************************************/
//向CH376写命令
/*************************************************************************************************/
void xWriteCH376Cmd(unsigned char mCmd)  
{
 CH376_CS_H;
 CH376_CS_L;            /* SPI片选有效 */
 Spi376Exchange(mCmd);  /* 发出命令码 */
 while(CH376_BUSY_STAT);
}

/*************************************************************************************************/
//向CH376写数据
/*************************************************************************************************/
void xWriteCH376Data(unsigned char mData)  
{
 Spi376Exchange(mData);
 while(CH376_BUSY_STAT);
 //Delay(50);//确保写周期大于0.6us
}

/*************************************************************************************************/
//从CH376读数据
/*************************************************************************************************/
unsigned char xReadCH376Data(void) 
{
 //Delay(50);//确保读周期大于0.6us
 while(CH376_BUSY_STAT);
 return(Spi376Exchange(0xFF));
}

/*************************************************************************************************/
//查询CH376中断(INT#低电平)
/*************************************************************************************************/
unsigned char Query376Interrupt(void)
{
#ifdef CH376_INT_WIRE
 return(CH376_INT_STAT ? FALSE : TRUE );   //如果连接了CH376的中断引脚则直接查询中断引脚
#else
 return(CH376_SPIMISO_STAT ? FALSE : TRUE);//如果未连接CH376的中断引脚则查询兼做中断输出的SDO引脚状态
#endif
}

/*************************************************************************************************/
//上电初始化CH376
//初始化成主机方式
/*************************************************************************************************/
unsigned char CH376_Init_Host(void)  
{
 unsigned char res,init_err=0;
 SPI3_Configuration();             //初始化与CH376通讯的SPI3口
 Delay_ms(100);                    //上电延时100ms，等待CH376复位完毕
 xWriteCH376Cmd(CMD00_RESET_ALL);  //上电后执行一次硬件复位指令，如果不复位，在某些情况下，当单片机复位后，做上电通讯测试会出错
 xEndCH376Cmd();                   //置CH376片选高电平
 Delay_ms(100);                    //上电延时100ms
 xWriteCH376Cmd(CMD11_CHECK_EXIST);//测试单片机与CH376之间的通讯接口
 xWriteCH376Data(0x65);            //发送数据0x65，应返回0x9a
 res = xReadCH376Data();           //读返回数据
 xEndCH376Cmd();                   //置CH376片选高电平
 if(res != 0x9A) init_err = ERR_USB_UNKNOWN;//通讯接口不正常,可能原因有:接口连接异常,其它设备影响(片选不唯一),串口波特率,一直在复位,晶振不工作 
 else
  {
   xWriteCH376Cmd(CMD11_SET_USB_MODE);//设备USB工作模式
   xWriteCH376Data(0x06);             //设置USB工作模式06H，已启用的USB主机方式，自动产生SOF包
   Delay(200);                        //延时17us等待设置完成(文档中要求10us)
   res = xReadCH376Data();            //读返回状态数据
   xEndCH376Cmd();                    //置CH376片选高电平
#ifndef CH376_INT_WIRE
 #ifdef    CH376_SPI_SDO
   xWriteCH376Cmd(CMD20_SET_SDO_INT); //设置SPI的SDO引脚的中断方式 */
   xWriteCH376Data(0x16);
   xWriteCH376Data(0x90);             //SDO引脚在SCS片选无效时兼做中断请求输出 */
   xEndCH376Cmd();
 #endif
#endif
   if(res == CMD_RET_SUCCESS) 
    {
     init_err = USB_INT_SUCCESS;//USB初始化正确
     //检测上电前是否已经有U盘插入，如果没有U盘则使CH376进入睡眠模式
     if(CH376DiskConnect() != USB_INT_SUCCESS)
      {
       xWriteCH376Cmd(CMD00_ENTER_SLEEP);//CH376进入睡眠模式
       xEndCH376Cmd();                   //置CH376片选高电平
      }
    }
   else init_err = ERR_USB_UNKNOWN;//设置模式错误
  }
 return(init_err);
}
