/***********************************************************************************
工程:   RA8875液晶屏驱动IC驱动程序(V1.0)
        液晶屏使用5.6寸640*480点阵TFT液晶屏,液晶屏信号为群创AT056TN53-V.1
        外挂字库芯片GT30L32S4W
文件名: LCDDRIVER.c
处理器: STM32F1系列CPU
接口:   SPI口
编译器: RealView MDK
作者:   程元
版权:   保定市力兴电子设备有限公司
日期：  2017.07.14
RGB565:R R R R R G G G G G G B B B B B
RGB332:R R R G G G B B
此版程序只提供8位颜色模式
***********************************************************************************/
#include "stm32f10x.h"
#include "IOCONFIG.H"
#include "QJBL.H"
#include "ZIKU.H"

unsigned short NUM_OF_CHAR3264;//32*64点阵字符库字符个数
unsigned short NUM_OF_HZDOT48; //48*48点阵汉字库汉字个数

#define DATA_WRITE  0x00
#define DATA_READ   0x40
#define REG_WRITE   0x80
#define STATUS_READ 0xC0

/*******************************************************************************************************************************
液晶屏驱动控制宏定义
8位颜色模式下,如果使用双图层模式,意义不大,因为如果两个图层是OR的关系,显示颜色会变色,影响显示效果,所以这里仍使用单图层模式
*******************************************************************************************************************************/
#define COLOR_DEPTH   8 //定义液晶屏色深,640*480分辨率下,8位彩色模式支持2个图层;16位彩色模式只支持1个图层
#define GRAPHIC_MODE  0  //图形模式
#define TEXT_MODE     1  //文本模式
#define INT_CGROM     0  //RA8875内部字库
#define EXT_CGROM     1  //RA8875外部字库
#define CGRAM_DISABLE 0  //禁用RA8875内部CGRAM
#define CGRAM_ENABLE  1  //启用RA8875内部CGRAM

#define SPI_LOW_SPEED  0x00
#define SPI_HIGH_SPEED 0xff

/*******************************************************************************************************************************
设置SPI1,用于与液晶屏通讯
RA8875 SPI写入最大速度为System clock/3
RA8875 SPI读取最大速度为System clock/6
当RA8875上电PLL未启动之前System clock与RA8875外部晶振频率相同,此时SPI允许的速度较低
SPI1在APB2总线，最高时钟频率72MHz
********************************************************************************************************************************/
void SPI1_Configuration(unsigned char speed)
{
 SPI_InitTypeDef SPI_InitStructure;                                         //定义SPI口数据结构体
 SPI_Cmd(SPI1, DISABLE);                                                    //禁止SPI1
 SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex; //设置为全双工双向通讯模式
 SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;                 //配置为主设备
 SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;                 //8位数据传输
 SPI_InitStructure.SPI_CPOL              = SPI_CPOL_High;                   //空闲时SCK保持高电平
 SPI_InitStructure.SPI_CPHA              = SPI_CPHA_2Edge;                  //数据从第二个时钟沿开始
 SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;                    //软件从设备管理
 if(speed == SPI_LOW_SPEED)
 SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;        //波特率= 72M/64 = 1.125M(<20M/6)
 else
 SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;         //波特率= 72M/8 = 9M(<65M/6)
 SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;		        //数据位高位在前
 SPI_InitStructure.SPI_CRCPolynomial     = 7;						        //对CRC复初值
 SPI_Init(SPI1, &SPI_InitStructure);
 SPI_I2S_ITConfig(SPI1, SPI_IT_CRCERR, DISABLE);//关闭CRC错误中断
 SPI_Cmd(SPI1, ENABLE);                         //使能SPI1
}

/*******************************************************************************************************************************
液晶驱动部分变量定义
*******************************************************************************************************************************/
unsigned char  FONT_X_ZOOM;   //文字水平放大倍数
unsigned char  FONT_Y_ZOOM;   //文字垂直放大倍数

/*******************************************************************************************************************************
读取状态寄存器函数
*******************************************************************************************************************************/
unsigned char LCD_Read_Status(void)
{
 unsigned char value;
 LCD_CS_L;                       //使能RA8875 SPI片选
 SPI2->DR = STATUS_READ;         //发送数据,读取状态信息
 while((SPI2->SR & 0x0002) == 0);//等待发送缓冲区空(TXE非0)
 while((SPI2->SR & 0x0001) == 0);//等待数据接收完成(RXNE非0)
 value = SPI2->DR;               //读取接收到的数据 
 SPI2->DR = 0x00;                //发送8个脉冲以接收数据
 while((SPI2->SR & 0x0002) == 0);//等待发送缓冲区空(TXE非0)
 while((SPI2->SR & 0x0001) == 0);//等待数据接收完成(RXNE非0)
 value = SPI2->DR;               //读取接收到的数据
 LCD_CS_H;                       //禁止RA8875 SPI片选
 return value;
}

/*******************************************************************************************************************************
内存读写判忙函数
*******************************************************************************************************************************/
void LCD_Check_Mem_Busy(void)
{
 while((LCD_Read_Status() & 0x80) == 0x80);
}

/*******************************************************************************************************************************
BTE判忙函数
*******************************************************************************************************************************/
void LCD_Check_BTE_Busy(void)
{
 while((LCD_Read_Status() & 0x40) == 0x40);
}

/*******************************************************************************************************************************
命令写入函数
*******************************************************************************************************************************/
void LCD_Write_Command(unsigned char command)
{
 LCD_CS_L;                       //使能RA8875 SPI片选
 SPI2->DR = REG_WRITE;           //发送数据,写入寄存器
 while((SPI2->SR & 0x0002) == 0);//等待发送缓冲区空(TXE非0)
 while((SPI2->SR & 0x0001) == 0);//等待数据接收完成(RXNE非0)
 SPI2->DR;                       //读取接收到的数据
 SPI2->DR = command;             //发送命令
 while((SPI2->SR & 0x0002) == 0);//等待发送缓冲区空(TXE非0)
 while((SPI2->SR & 0x0001) == 0);//等待数据接收完成(RXNE非0)
 SPI2->DR;                       //读取接收到的数据
 LCD_CS_H;                       //禁止RA8875 SPI片选
}

/*******************************************************************************************************************************
数据写入函数
*******************************************************************************************************************************/
void LCD_Write_Data(unsigned char data)
{
 LCD_CS_L;                       //使能RA8875 SPI片选
 SPI2->DR = DATA_WRITE;          //发送数据,写入数据
 while((SPI2->SR & 0x0002) == 0);//等待发送缓冲区空(TXE非0)
 while((SPI2->SR & 0x0001) == 0);//等待数据接收完成(RXNE非0)
 SPI2->DR;                       //读取接收到的数据
 SPI2->DR = data;                //发送要写入的数据
 while((SPI2->SR & 0x0002) == 0);//等待发送缓冲区空(TXE非0)
 while((SPI2->SR & 0x0001) == 0);//等待数据接收完成(RXNE非0)
 SPI2->DR;                       //读取接收到的数据
 LCD_CS_H;                       //禁止RA8875 SPI片选
}

/*******************************************************************************************************************************
读取数据函数
*******************************************************************************************************************************/
unsigned char LCD_Read_Data(void)
{
 unsigned char value;
 LCD_CS_L;                       //使能RA8875 SPI片选
 SPI2->DR = DATA_READ;           //发送数据,读取数据
 while((SPI2->SR & 0x0002) == 0);//等待发送缓冲区空(TXE非0)
 while((SPI2->SR & 0x0001) == 0);//等待数据接收完成(RXNE非0)
 value = SPI2->DR;               //读取接收到的数据
 SPI2->DR = 0x00;                //发送8个时钟脉冲以接收数据
 while((SPI2->SR & 0x0002) == 0);//等待发送缓冲区空(TXE非0)
 while((SPI2->SR & 0x0001) == 0);//等待数据接收完成(RXNE非0)
 value = SPI2->DR;               //读取接收到的数据
 LCD_CS_H;                       //禁止RA8875 SPI片选
 return value;
}

/*******************************************************************************************************************************
寄存器写入函数
*******************************************************************************************************************************/
void LCD_Write_Register(unsigned char add,unsigned char data)
{
 LCD_CS_L;                       //使能RA8875 SPI片选
 SPI2->DR = REG_WRITE;           //发送数据,写入寄存器
 while((SPI2->SR & 0x0002) == 0);//等待发送缓冲区空(TXE非0)
 while((SPI2->SR & 0x0001) == 0);//等待数据接收完成(RXNE非0)
 SPI2->DR;                       //读取接收到的数据
 SPI2->DR = add;                 //发送命令
 while((SPI2->SR & 0x0002) == 0);//等待发送缓冲区空(TXE非0)
 while((SPI2->SR & 0x0001) == 0);//等待数据接收完成(RXNE非0)
 SPI2->DR;                       //读取接收到的数据
 SPI2->DR = DATA_WRITE;          //发送数据,写入数据
 while((SPI2->SR & 0x0002) == 0);//等待发送缓冲区空(TXE非0)
 while((SPI2->SR & 0x0001) == 0);//等待数据接收完成(RXNE非0)
 SPI2->DR;                       //读取接收到的数据
 SPI2->DR = data;                //发送要写入的数据
 while((SPI2->SR & 0x0002) == 0);//等待发送缓冲区空(TXE非0)
 while((SPI2->SR & 0x0001) == 0);//等待数据接收完成(RXNE非0)
 SPI2->DR;                       //读取接收到的数据
 LCD_CS_H;                       //禁止RA8875 SPI片选
}

/*******************************************************************************************************************************
寄存器读取函数
*******************************************************************************************************************************/
unsigned char LCD_Read_Register(unsigned char add)
{
 unsigned char value;
 LCD_CS_L;                       //使能RA8875 SPI片选
 SPI2->DR = REG_WRITE;           //发送数据,写入寄存器
 while((SPI2->SR & 0x0002) == 0);//等待发送缓冲区空(TXE非0)
 while((SPI2->SR & 0x0001) == 0);//等待数据接收完成(RXNE非0)
 SPI2->DR;                       //读取接收到的数据
 SPI2->DR = add;                 //发送命令
 while((SPI2->SR & 0x0002) == 0);//等待发送缓冲区空(TXE非0)
 while((SPI2->SR & 0x0001) == 0);//等待数据接收完成(RXNE非0)
 SPI2->DR;                       //读取接收到的数据
 SPI2->DR = DATA_READ;           //发送数据,读取数据
 while((SPI2->SR & 0x0002) == 0);//等待发送缓冲区空(TXE非0)
 while((SPI2->SR & 0x0001) == 0);//等待数据接收完成(RXNE非0)
 value = SPI2->DR;               //读取接收到的数据
 SPI2->DR = 0x00;                //发送8个时钟脉冲以接收数据
 while((SPI2->SR & 0x0002) == 0);//等待发送缓冲区空(TXE非0)
 while((SPI2->SR & 0x0001) == 0);//等待数据接收完成(RXNE非0)
 value = SPI2->DR;               //读取接收到的数据
 LCD_CS_H;                       //禁止RA8875 SPI片选
 return value;
}

/*******************************************************************************************************************************
液晶屏背光亮度控制子函数
输入参数 level:亮度等级,0~255
*******************************************************************************************************************************/
void LCD_Set_Bright(unsigned char level)
{
 static unsigned char old_level = 0;
 if(level != old_level)
  {
   old_level = level;
   LCD_Write_Register(0x8B,level);//8Bh P1DCR,PWM1占空比设置寄存器；0x00~0xff(1~256),用于调整液晶屏背光亮度
  } 
}

/*******************************************************************************************************************************
液晶屏显示模式选择子函数
输入参数 mode:显示模式,GRAPHIC_MODE图形模式,TEXT_MODE文字模式
*******************************************************************************************************************************/
void LCD_Mode_Select(unsigned char mode)
{	
 if(mode == GRAPHIC_MODE) LCD_Write_Register(0x40,0x00);//40h MWCR0,内存写控制寄存器；进入图形模式
 else LCD_Write_Register(0x40,0x80);                    //40h MWCR0,内存写控制寄存器；进入文字模式
 //Bit[6]   = 0(文字/内存写入光标不显示)
 //Bit[5]   = 0(文字/内存写入光标不闪烁)
 //Bit[3:2] = 00b(绘图模式时内存写入方向:左→右,然后上→下)
 //Bit[1]   = 0(当内存写入时光标位置自动加一)
 //Bit[0]   = 0(当内存读取时光标位置自动加一)
}

/*******************************************************************************************************************************
液晶屏操作图层选择子函数
输入参数 layer:图层,0:图层1,1:图层2
*******************************************************************************************************************************/
void LCD_Layer_Select(unsigned char layer)
{
 unsigned char tmp;
 if(COLOR_DEPTH == 8) //8位彩色模式,才具备2个图层
  {
   tmp = LCD_Read_Register(0x41); //读取41h MWCR1
   if(layer == 0) tmp &= 0xf0;    //选择图层1
   else tmp = (tmp & 0xf0) | 0x01;//选择图层2
   LCD_Write_Register(0x41,tmp);  //写入41h MWCR1
  }
 else //16位彩色模式,只能1个图层
  {
   tmp = LCD_Read_Register(0x41);//读取41h MWCR1
   tmp &= 0xf0;                  //选择图层1
   LCD_Write_Register(0x41,tmp); //写入41h MWCR1
  } 
}

/*******************************************************************************************************************************
设置液晶屏工作窗口并将光标设置到工作窗口内
*******************************************************************************************************************************/
void LCD_Active_Window(unsigned short x_start,unsigned short y_start,unsigned short x_end,unsigned short y_end)
{
 /*设置工作窗口x方向起始点和结束点*/
 //设置x起始点
 LCD_Write_Register(0x30,x_start);     //30h HSAW0
 LCD_Write_Register(0x31,x_start >> 8);//31h HSAW1	   
 //设置x结束点
 LCD_Write_Register(0x34,x_end);       //34h HEAW0
 LCD_Write_Register(0x35,x_end >> 8);  //35h HEAW1	   
 /*设置工作窗口y方向起始点和结束点*/
 //设置y起始点
 LCD_Write_Register(0x32,y_start);     //32h VSAW0
 LCD_Write_Register(0x33,y_start >> 8);//33h VSAW1	   
 //设置y结束点
 LCD_Write_Register(0x36,y_end);       //36h VEAW0
 LCD_Write_Register(0x37,y_end >> 8);  //37h VEAW1
}

/*******************************************************************************************************************************
16位RGB565转8位RGB332函数
*******************************************************************************************************************************/
unsigned char RGB565_RGB332(unsigned short color)
{
 return (((color & 0xe000) >> 8) | ((color & 0x0700) >> 6) | ((color & 0x0018) >> 3));
}

/*******************************************************************************************************************************
8位RGB332转16位RGB565函数
*******************************************************************************************************************************/
unsigned short RGB332_RGB565(unsigned short color)
{
 return (((color & 0xe0) << 8) | ((color & 0x1c) << 6) | ((color & 0x03) << 3));
}

/*******************************************************************************************************************************
设置液晶屏前景色
*******************************************************************************************************************************/
void LCD_Set_Foreground_Color(unsigned short color)
{
 LCD_FORE_COLOR = color;//记录当前设置的前景色
#if COLOR_DEPTH == 8    /*8位彩色模式*/
 LCD_Write_Register(0x63,(color & 0xf800) >> 13);//60h 前景色红色部分
 LCD_Write_Register(0x64,(color & 0x07e0) >> 8); //61h 前景色绿色部分
 LCD_Write_Register(0x65,(color & 0x001f) >> 3); //62h 前景色蓝色部分
#elif COLOR_DEPTH == 16 /*16位彩色模式*/
 LCD_Write_Register(0x63,(color & 0xf800) >> 11);//60h 前景色红色部分
 LCD_Write_Register(0x64,(color & 0x07e0) >> 5); //61h 前景色绿色部分
 LCD_Write_Register(0x65,(color & 0x001f));      //62h 前景色蓝色部分
#endif
}

/*******************************************************************************************************************************
设置液晶屏背景色
*******************************************************************************************************************************/
void LCD_Set_Background_Color(unsigned short color)
{
 //LCD_BACK_COLOR = color;//记录当前设置的背景色
#if COLOR_DEPTH == 8    /*8位彩色模式*/
 LCD_Write_Register(0x60,(color & 0xf800) >> 13);//60h 背景色红色部分
 LCD_Write_Register(0x61,(color & 0x07e0) >> 8); //61h 背景色绿色部分
 LCD_Write_Register(0x62,(color & 0x001f) >> 3); //62h 背景色蓝色部分
#elif COLOR_DEPTH == 16 /*16位彩色模式*/
 LCD_Write_Register(0x60,(color & 0xf800) >> 11);//60h 背景色红色部分
 LCD_Write_Register(0x61,(color & 0x07e0) >> 5); //61h 背景色绿色部分
 LCD_Write_Register(0x62,(color & 0x001f));      //62h 背景色蓝色部分
#endif
}

/*******************************************************************************************************************************
设置图形模式写显存的光标位置子函数
输入参数 x:横坐标，以像素点为单位
         y:纵坐标，以像素行为单位
*******************************************************************************************************************************/
void LCD_Set_Graphic_Cursor(unsigned short x, unsigned short y)
{
 /*设置内存写光标的坐标，注意0x80-83是图形光标的显示位置坐标*/
 LCD_Write_Register(0x46,x);
 LCD_Write_Register(0x47,x >> 8);
 LCD_Write_Register(0x48,y);
 LCD_Write_Register(0x49,y >> 8);
}

/*******************************************************************************************************************************
设置图形模式读显存的光标位置子函数(很多其他的控制器写光标和读光标是相同的寄存器，但是RA8875是不同的)
输入参数 x:横坐标，以像素点为单位
         y:纵坐标，以像素行为单位
*******************************************************************************************************************************/
void LCD_Set_Read_Cursor(unsigned short x, unsigned short y)
{
 /*设置内存读光标的坐标*/
 LCD_Write_Register(0x4A,x);
 LCD_Write_Register(0x4B,x >> 8);
 LCD_Write_Register(0x4C,y);
 LCD_Write_Register(0x4D,y >> 8);
}

/*******************************************************************************************************************************
设置文字模式的光标位置子函数
输入参数 x:横坐标，以像素点为单位
         y:纵坐标，以像素行为单位
*******************************************************************************************************************************/
void LCD_Set_Text_Cursor(unsigned short x, unsigned short y)
{
 /*设置文字写入时的光标的坐标*/
 LCD_Write_Register(0x2A,x);
 LCD_Write_Register(0x2B,x >> 8);
 LCD_Write_Register(0x2C,y);
 LCD_Write_Register(0x2D,y >> 8);
}

/*******************************************************************************************************************************
读取指定位置像素点颜色子函数
输入参数 x:横坐标，以像素点为单位
         y:纵坐标，以像素行为单位
在读取内存操作之前执行一次空读操作
见RA8875手册REG[02h] Memory Read/Write Command (MRWC)寄存器说明,
在发送0x02命令之后第一次读到的数据为无用(官方回应这时读光标并不会自动加一)数据需要忽略掉
*******************************************************************************************************************************/
unsigned short LCD_Get_Pixel_Color(unsigned short x,unsigned short y)
{ 
 unsigned short tmp;
 LCD_Set_Read_Cursor(x,y);/*设置图形读取光标位置*/
 LCD_Write_Command(0x02); /*02h MRWC,内存读写命令,进入数据读写模式*/
 tmp = LCD_Read_Data();   /*Dummy*/
 //此延时时间是使用STM32F407工作在168MHz频率,并使用FSMC总线驱动,需要10个__NOP()
 //此延时时间是使用STM32F103工作在72MHz频率,并使用9MHz SPI总线驱动或者使用IO口驱动,需要0个__NOP()
 //__NOP();
 //__NOP();
 //__NOP();
 //__NOP();
 //__NOP();
 //__NOP();
 //__NOP();
 //__NOP();
 //__NOP();
 //__NOP();
 tmp = LCD_Read_Data();//得到最终真实数据
 return tmp;
}

/*******************************************************************************************************************************
画点子函数
输入参数 x    :横坐标，以像素点为单位
         y    :纵坐标，以像素行为单位
         color:点的颜色
*******************************************************************************************************************************/
void LCD_Draw_Dot(unsigned short x,unsigned short y,unsigned char color)
{
 LCD_Set_Graphic_Cursor(x,y);//设置图形写入光标位置
 LCD_Write_Command(0x02);    //设置RA8875进入内存(DDRAM或CGRAM)读取/写入模式
 LCD_Write_Data(color);      //写入点的颜色
 LCD_Check_Mem_Busy();       //等待写入完成
}

/*******************************************************************************************************************************
画线子函数(使用RA8875 2D加速功能绘制)
输入参数 x_start,x_end:起止横坐标，以像素点为单位
         y_start,y_end:起止纵坐标，以像素行为单位
         color        : 线的颜色
*******************************************************************************************************************************/
void LCD_Draw_Line(unsigned short x_start,unsigned short y_start,unsigned short x_end,unsigned short y_end,unsigned short color)
{
 LCD_Write_Register(0x91,x_start);     //91h DLHSR0
 LCD_Write_Register(0x92,x_start >> 8);//92h DLHSR1,设置直线水平起始坐标
 LCD_Write_Register(0x93,y_start);     //93h DLVSR0
 LCD_Write_Register(0x94,y_start >> 8);//94h DLVSR1,设置直线垂直起始坐标
 LCD_Write_Register(0x95,x_end);       //95h DLHER0
 LCD_Write_Register(0x96,x_end >> 8);  //96h DLHER1,设置直线水平结束坐标
 LCD_Write_Register(0x97,y_end);       //97h DLVER0
 LCD_Write_Register(0x98,y_end >> 8);  //98h DLVER1,设置直线垂直结束坐标
 LCD_Set_Foreground_Color(color);//设置前景色,即直线颜色
 LCD_Write_Register(0x90,0x80);  //启动画直线
 LCD_Check_Mem_Busy();           //等待绘制完成
}

/*******************************************************************************************************************************
画虚线子函数(软件绘制)
输入参数 x_start,x_end:起止横坐标，以像素点为单位
         y_start,y_end:起止纵坐标，以像素行为单位
         color        : 线的颜色
*******************************************************************************************************************************/
void LCD_Draw_Dotted_Line(unsigned short x_start,unsigned short y_start,unsigned short x_end,unsigned short y_end,unsigned short color)
{
 signed short dx = x_end - x_start;
 signed short dy = y_end - y_start;
 signed short ux = ((dx > 0) << 1) - 1;//x的增量方向，取或-1
 signed short uy = ((dy > 0) << 1) - 1;//y的增量方向，取或-1
 signed short x = x_start, y = y_start, eps;//eps为累加误差
 unsigned char dotted_en = 0;
#if COLOR_DEPTH == 8 //8位色深模式,需要彩色转换
 color = RGB565_RGB332(color);
#endif
 eps = 0;
 if(dx < 0) dx = -dx; 
 if(dy < 0) dy = -dy; 
 if(dx > dy) 
  {
   for(x = x_start; x != x_end+ux; x += ux)
    {
     if(dotted_en == 0) LCD_Draw_Dot(x,y,color);
     dotted_en = ~dotted_en; 
     eps += dy;
     if((eps << 1) >= dx)
      {
       y += uy; eps -= dx;
      }
    }
   }
  else
   {
    for(y = y_start; y != y_end+uy; y += uy)
     {
      if(dotted_en == 0) LCD_Draw_Dot(x,y,color);
      dotted_en = ~dotted_en;
      eps += dx;
      if((eps << 1) >= dy)
       {
        x += ux; eps -= dy;
       }
     }
   }
}

/*******************************************************************************************************************************
绘制三角形子函数(使用RA8875绘制矩形2D加速功能实现)
输入参数 x_point0:顶点0横坐标，以像素点为单位
         y_point0:顶点0纵坐标，以像素点为单位
         x_point1:顶点1横坐标，以像素点为单位
         y_point1:顶点1纵坐标，以像素点为单位
         x_point2:顶点2横坐标，以像素点为单位
         y_point2:顶点2纵坐标，以像素点为单位
         color   :线的颜色
         fill_en :0:不填充;非零:填充
*******************************************************************************************************************************/
void LCD_Draw_Triangle(unsigned short x_point0,unsigned short y_point0,unsigned short x_point1,unsigned short y_point1,
                       unsigned short x_point2,unsigned short y_point2,unsigned short color,signed char fill_en)
{
 LCD_Write_Register(0x91,x_point0);     //91h DLHSR0
 LCD_Write_Register(0x92,x_point0 >> 8);//92h DLHSR1,设置三角形顶点0水平坐标
 LCD_Write_Register(0x93,y_point0);     //93h DLVSR0
 LCD_Write_Register(0x94,y_point0 >> 8);//94h DLVSR1,设置三角形顶点0垂直坐标
 LCD_Write_Register(0x95,x_point1);     //95h DLHER0
 LCD_Write_Register(0x96,x_point1 >> 8);//96h DLHER1,设置三角形顶点1水平坐标
 LCD_Write_Register(0x97,y_point1);     //97h DLVER0
 LCD_Write_Register(0x98,y_point1 >> 8);//98h DLVER1,设置三角形顶点1垂直坐标
 LCD_Write_Register(0xA9,x_point2);     //A9h DTPH0
 LCD_Write_Register(0xAA,x_point2 >> 8);//AAh DTPH1 ,设置三角形顶点2水平坐标
 LCD_Write_Register(0xAB,y_point2);     //ABh DTPV0
 LCD_Write_Register(0xAC,y_point2 >> 8);//ACh DTPV1 ,设置三角形顶点2垂直坐标
 LCD_Set_Foreground_Color(color);               //设置前景色,即圆形颜色
 if(fill_en == 0) LCD_Write_Register(0x90,0x81);//启动绘制三角形
 else LCD_Write_Register(0x90,0xA1);            //启动绘制颜色填满三角形
 LCD_Check_Mem_Busy();                          //等待绘制完成  
}

/*******************************************************************************************************************************
绘制矩形线框子函数(使用RA8875绘制矩形2D加速功能实现)
输入参数 x_start,x_end:起止横坐标，以像素点为单位
         y_start,y_end:起止纵坐标，以像素行为单位
         color        : 线的颜色
         fill_en      :0:不填充;非零:填充
*******************************************************************************************************************************/
void LCD_Draw_Rectangle(unsigned short x_start,unsigned short y_start,unsigned short x_end,unsigned short y_end,unsigned short color,signed char fill_en)
{
 LCD_Write_Register(0x91,x_start);     //91h DLHSR0
 LCD_Write_Register(0x92,x_start >> 8);//92h DLHSR1,设置矩形顶点水平起始坐标
 LCD_Write_Register(0x93,y_start);     //93h DLVSR0
 LCD_Write_Register(0x94,y_start >> 8);//94h DLVSR1,设置矩形顶点垂直起始坐标
 LCD_Write_Register(0x95,x_end);       //95h DLHER0
 LCD_Write_Register(0x96,x_end >> 8);  //96h DLHER1,设置矩形顶点水平结束坐标
 LCD_Write_Register(0x97,y_end);       //97h DLVER0
 LCD_Write_Register(0x98,y_end >> 8);  //98h DLVER1,设置矩形顶点垂直结束坐标
 LCD_Set_Foreground_Color(color);               //设置前景色,即矩形颜色
 if(fill_en == 0) LCD_Write_Register(0x90,0x90);//启动绘制矩形
 else LCD_Write_Register(0x90,0xB0);            //启动绘制颜色填满矩形
 LCD_Check_Mem_Busy();                          //等待绘制完成  
}

/*******************************************************************************************************************************
绘制圆角矩形子函数(使用RA8875绘制矩形2D加速功能实现)
输入参数 x_strat:起点横坐标，以像素点为单位
         y_strat:起点0纵坐标，以像素点为单位
         x_end  :终点横坐标，以像素点为单位
         y_end  :终点纵坐标，以像素点为单位
         radius :圆角半径，以像素点为单位
         color  :线的颜色
         fill_en:0:不填充;非零:填充
*******************************************************************************************************************************/
void LCD_Draw_Round_Rectangle(unsigned short x_strat,unsigned short y_strat,unsigned short x_end,unsigned short y_end,
                              unsigned short radius,unsigned short color,signed char fill_en)
{
 LCD_Write_Register(0x91,x_strat);     //91h DLHSR0
 LCD_Write_Register(0x92,x_strat >> 8);//92h DLHSR1,设置矩形顶点水平坐标
 LCD_Write_Register(0x93,y_strat);     //93h DLVSR0
 LCD_Write_Register(0x94,y_strat >> 8);//94h DLVSR1,设置矩形顶点垂直坐标
 LCD_Write_Register(0x95,x_end);       //95h DLHER0
 LCD_Write_Register(0x96,x_end >> 8);  //96h DLHER1,设置矩形顶点水平坐标
 LCD_Write_Register(0x97,y_end);       //97h DLVER0
 LCD_Write_Register(0x98,y_end >> 8);  //98h DLVER1,设置矩形顶点垂直坐标
 LCD_Write_Register(0xA1,radius);      //A1h ELL_A0
 LCD_Write_Register(0xA2,radius >> 8); //A2h ELL_A1,设置圆角(X轴)半径
 LCD_Write_Register(0xA3,radius);      //A3h ELL_B0
 LCD_Write_Register(0xA4,radius >> 8); //A4h ELL_B1,设置圆角(Y轴)半径
 LCD_Set_Foreground_Color(color);               //设置前景色,即圆形颜色
 if(fill_en == 0) LCD_Write_Register(0xA0,0xA0);//启动绘制圆角矩形
 else LCD_Write_Register(0xA0,0xE0);            //启动绘制颜色填满圆角矩形
 LCD_Check_Mem_Busy();                          //等待绘制完成  
}

/*******************************************************************************************************************************
绘制圆形子函数(使用RA8875绘制矩形2D加速功能实现)
输入参数 x_center:圆心横坐标，以像素点为单位
         y_center:圆心纵坐标，以像素行为单位
         radius  :半径，以像素行为单位
         color   :线的颜色
         fill_en :0:不填充;非零:填充
*******************************************************************************************************************************/
void LCD_Draw_Circle(unsigned short x_center,unsigned short y_center,unsigned char radius,unsigned short color,signed char fill_en)
{
 LCD_Write_Register(0x99,x_center);     //99h DCHR0
 LCD_Write_Register(0x9A,x_center >> 8);//9Ah DCHR1,设置圆心水平坐标
 LCD_Write_Register(0x9B,y_center);     //9Bh DCVR0
 LCD_Write_Register(0x9C,y_center >> 8);//9Ch DCVR1,设置圆心垂直坐标
 LCD_Write_Register(0x9D,radius);       //9Dh DCRR,设置圆半径
 LCD_Set_Foreground_Color(color);               //设置前景色,即圆形颜色
 if(fill_en == 0) LCD_Write_Register(0x90,0x40);//启动绘制圆形
 else LCD_Write_Register(0x90,0x60);            //启动绘制颜色填满圆形
 LCD_Check_Mem_Busy();                          //等待绘制完成  
}

/*******************************************************************************************************************************
绘制椭圆子函数(使用RA8875绘制矩形2D加速功能实现)
输入参数 x_center:椭圆圆心横坐标，以像素点为单位
         y_center:椭圆圆心纵坐标，以像素行为单位
         x_length:x轴长度，以像素行为单位
         y_length:y轴长度，以像素行为单位
         color   :线的颜色
         fill_en :0:不填充;非零:填充
*******************************************************************************************************************************/
void LCD_Draw_Ellipse(unsigned short x_center,unsigned short y_center,unsigned short x_length,unsigned short y_length,unsigned short color,signed char fill_en)
{
 LCD_Write_Register(0xA5,x_center);     //A5h DEHR0
 LCD_Write_Register(0xA6,x_center >> 8);//A6h DEHR1,设置椭圆圆心水平坐标
 LCD_Write_Register(0xA7,y_center);     //A7h DEVR0
 LCD_Write_Register(0xA8,y_center >> 8);//A8h DEVR1,设置椭圆圆心垂直坐标 
 LCD_Write_Register(0xA1,x_length);     //A1h ELL_A0
 LCD_Write_Register(0xA2,x_length >> 8);//A2h ELL_A1,设置椭圆长轴(X轴)长度
 LCD_Write_Register(0xA3,y_length);     //A3h ELL_B0
 LCD_Write_Register(0xA4,y_length >> 8);//A4h ELL_B1,设置椭圆短轴(Y轴)长度
 LCD_Set_Foreground_Color(color);               //设置前景色,即圆形颜色
 if(fill_en == 0) LCD_Write_Register(0xA0,0x80);//启动绘制椭圆形
 else LCD_Write_Register(0xA0,0xC0);            //启动绘制颜色填满椭圆形
 LCD_Check_Mem_Busy();                          //等待绘制完成  
}

/*******************************************************************************************************************************
绘制椭圆曲线子函数(使用RA8875绘制矩形2D加速功能实现)
输入参数 x_center:椭圆圆心横坐标，以像素点为单位
         y_center:椭圆圆心纵坐标，以像素行为单位
         x_length:x轴长度，以像素行为单位
         y_length:y轴长度，以像素行为单位
         part    :绘制的椭圆曲线部位，0:左下角曲线，1:左上角曲线，2:右上角曲线，3:右下角曲线
         color   :线的颜色
         fill_en :0:不填充;非零:填充
*******************************************************************************************************************************/
void LCD_Draw_Ellipse_Curve(unsigned short x_center,unsigned short y_center,unsigned short x_length,unsigned short y_length,
                            unsigned char part,unsigned short color,signed char fill_en)
{
 LCD_Write_Register(0xA5,x_center);     //A5h DEHR0
 LCD_Write_Register(0xA6,x_center >> 8);//A6h DEHR1,设置椭圆圆心水平坐标
 LCD_Write_Register(0xA7,y_center);     //A7h DEVR0
 LCD_Write_Register(0xA8,y_center >> 8);//A8h DEVR1,设置椭圆圆心垂直坐标 
 LCD_Write_Register(0xA1,x_length);     //A1h ELL_A0
 LCD_Write_Register(0xA2,x_length >> 8);//A2h ELL_A1,设置椭圆长轴(X轴)长度
 LCD_Write_Register(0xA3,y_length);     //A3h ELL_B0
 LCD_Write_Register(0xA4,y_length >> 8);//A4h ELL_B1,设置椭圆短轴(Y轴)长度
 LCD_Set_Foreground_Color(color);                      //设置前景色,即圆形颜色
 if(fill_en == 0) LCD_Write_Register(0xA0,0x90 | part);//启动绘制椭圆形
 else LCD_Write_Register(0xA0,0xD0 | part);            //启动绘制颜色填满椭圆形
 LCD_Check_Mem_Busy();                                 //等待绘制完成  
}

/*******************************************************************************************************************************
整屏填充子函数(借助RA8875以背景色清除数据特性实现)
输入参数 color:颜色
*******************************************************************************************************************************/
void LCD_FullScreen_Fill(unsigned short color)
{
 LCD_Set_Background_Color(color);//设置背景色
 LCD_Write_Register(0x8e,0x80);  //8eh MCLR,开始清除全屏数据
 LCD_Check_Mem_Busy();           //等待清除完成
}

/*******************************************************************************************************************************
区域按点填充子函数(使用RA8875绘制实心矩形2D加速功能实现)
输入参数 x_start,x_end:起止横坐标，以像素点为单位
         y_start,y_end:起止纵坐标，以像素行为单位
         color        : 线的颜色
*******************************************************************************************************************************/
void LCD_Area_Fill(unsigned short x_start,unsigned short y_start,unsigned short x_end,unsigned short y_end,unsigned short color)
{
 LCD_Write_Register(0x91,x_start);     //91h DLHSR0
 LCD_Write_Register(0x92,x_start >> 8);//92h DLHSR1,设置矩形顶点水平起始坐标
 LCD_Write_Register(0x93,y_start);     //93h DLVSR0
 LCD_Write_Register(0x94,y_start >> 8);//94h DLVSR1,设置矩形顶点垂直起始坐标
 LCD_Write_Register(0x95,x_end);       //95h DLHER0
 LCD_Write_Register(0x96,x_end >> 8);  //96h DLHER1,设置矩形顶点水平结束坐标
 LCD_Write_Register(0x97,y_end);       //97h DLVER0
 LCD_Write_Register(0x98,y_end >> 8);  //98h DLVER1,设置矩形顶点垂直结束坐标
 LCD_Set_Foreground_Color(color);//设置前景色,即矩形颜色
 LCD_Write_Register(0x90,0xB0);  //启动绘制颜色填满矩形
 LCD_Check_Mem_Busy();           //等待绘制完成  
}

/*******************************************************************************************************************************
绘制动态矩形窗口子函数,分为有阴影和无阴影两种状态
输入参数 x_start,x_end:起止横坐标，以像素点为单位
         y_start,y_end:起止纵坐标，以像素行为单位
         radius       :圆角半径，以像素为单位；0:无圆角
         line_color   :矩形框颜色
         status       :0:无阴影状态;非零:有阴影状态
*******************************************************************************************************************************/
void LCD_Draw_Dynamic_Window(unsigned short x_start,unsigned short y_start,unsigned short x_end,  unsigned short y_end,
                             unsigned short radius,unsigned short line_color,signed char status)
{
 unsigned short l_color,s_color;
 if(status)//有阴影 
  {
   l_color = LCD_BACK_COLOR;
   s_color = line_color;
  } 
 else//无阴影,以背景色擦除阴影线条
  {
   l_color = line_color;
   s_color = LCD_BACK_COLOR;
  } 
 if(radius == 0)//绘制矩形动态窗口
  {
   /*--------------------绘制主矩形框-------------------------*/
   LCD_Draw_Rectangle(  x_start,  y_start,  x_end,  y_end,line_color,NO);//绘制主矩形框
   LCD_Draw_Rectangle(x_start+1,y_start+1,x_end-1,y_end-1,line_color,NO);//绘制主矩形框,加粗效果
   LCD_Draw_Rectangle(x_start+2,y_start+2,x_end-2,y_end-2,   l_color,NO);//绘制主矩形框,按下效果
   /*----------------------绘制阴影---------------------------*/
   /*矩形框下边阴影*/
   LCD_Draw_Line(x_start+1,y_end+1,x_end+1,y_end+1,s_color);//矩形框下面第1条横线
   LCD_Draw_Line(x_start+2,y_end+2,x_end+2,y_end+2,s_color);//矩形框下面第2条横线
   LCD_Draw_Line(x_start+3,y_end+3,x_end+3,y_end+3,s_color);//矩形框下面第3条横线
   LCD_Draw_Line(x_start+4,y_end+4,x_end+4,y_end+4,s_color);//矩形框下面第3条横线
   /*矩形框右边阴影*/
   LCD_Draw_Line(x_end+1,y_start+1,x_end+1,y_end+1,s_color);//矩形框右边第1条竖线
   LCD_Draw_Line(x_end+2,y_start+2,x_end+2,y_end+2,s_color);//矩形框右边第2条竖线
   LCD_Draw_Line(x_end+3,y_start+3,x_end+3,y_end+3,s_color);//矩形框右边第3条竖线
   LCD_Draw_Line(x_end+4,y_start+4,x_end+4,y_end+4,s_color);//矩形框右边第4条竖线
  }
 else //绘制圆角矩形动态窗口
  {
   /*--------------------绘制主圆角矩形框-------------------------*/
   LCD_Draw_Round_Rectangle(  x_start,  y_start,  x_end,  y_end,  radius,line_color,NO);//绘制主圆角矩形框
   LCD_Draw_Round_Rectangle(x_start+1,y_start+1,x_end-1,y_end-1,radius-1,line_color,NO);//绘制主圆角矩形框,加粗效果
   LCD_Draw_Round_Rectangle(x_start+2,y_start+2,x_end-2,y_end-2,radius-2,   l_color,NO);//绘制主圆角矩形框,按下效果
   /*----------------------绘制阴影---------------------------*/
   /*圆角矩形框下边阴影*/
   LCD_Draw_Line(x_start+radius+0,y_end+1,x_end-radius,y_end+1,s_color);//圆角矩形框下面第1条横线
   LCD_Draw_Line(x_start+radius+1,y_end+2,x_end-radius,y_end+2,s_color);//圆角矩形框下面第2条横线
   LCD_Draw_Line(x_start+radius+2,y_end+3,x_end-radius,y_end+3,s_color);//圆角矩形框下面第3条横线
   LCD_Draw_Line(x_start+radius+3,y_end+4,x_end-radius,y_end+4,s_color);//圆角矩形框下面第3条横线
   /*圆角矩形框右边阴影*/
   LCD_Draw_Line(x_end+1,y_start+radius+0,x_end+1,y_end-radius,s_color);//圆角矩形框右边第1条竖线
   LCD_Draw_Line(x_end+2,y_start+radius+1,x_end+2,y_end-radius,s_color);//圆角矩形框右边第2条竖线
   LCD_Draw_Line(x_end+3,y_start+radius+2,x_end+3,y_end-radius,s_color);//圆角矩形框右边第3条竖线
   LCD_Draw_Line(x_end+4,y_start+radius+3,x_end+4,y_end-radius,s_color);//圆角矩形框右边第4条竖线
   /*圆角矩形框右下角阴影*/
   LCD_Draw_Ellipse_Curve(x_end-radius,y_end-radius,radius+1,radius+1,3,s_color,NO);//圆角矩形框右下角第1条圆弧
   LCD_Draw_Ellipse_Curve(x_end-radius,y_end-radius,radius+2,radius+2,3,s_color,NO);//圆角矩形框右下角第2条圆弧
   LCD_Draw_Ellipse_Curve(x_end-radius,y_end-radius,radius+3,radius+3,3,s_color,NO);//圆角矩形框右下角第3条圆弧
   LCD_Draw_Ellipse_Curve(x_end-radius,y_end-radius,radius+4,radius+4,3,s_color,NO);//圆角矩形框右下角第4条圆弧
  }   
}

/*******************************************************************************************************************************
绘制矩形窗口子函数
输入参数 x_start,x_end:起止横坐标，以像素点为单位
         y_start,y_end:起止纵坐标，以像素行为单位
         radius       :圆角半径，以像素为单位；0:无圆角
         line_color   :矩形框颜色
         back_color   :背景颜色
*******************************************************************************************************************************/
void LCD_Draw_Rectangle_Window(unsigned short x_start,unsigned short y_start,unsigned short x_end,  unsigned short y_end,
                               unsigned short radius,unsigned short line_color,unsigned short back_color)
{
 if(radius == 0)//绘制矩形动态窗口
  {
   /*--------------------绘制主矩形框-------------------------*/
   LCD_Draw_Rectangle(x_start  ,y_start  ,x_end  ,y_end  ,line_color,NO); //绘制主矩形框
   LCD_Draw_Rectangle(x_start+1,y_start+1,x_end-1,y_end-1,line_color,NO); //绘制主矩形框,加粗效果
   LCD_Draw_Rectangle(x_start+2,y_start+2,x_end-2,y_end-2,back_color,YES);//绘制主矩形框,填充背景色
  }
 else //绘制圆角矩形动态窗口
  {
   /*--------------------绘制主圆角矩形框-------------------------*/
   LCD_Draw_Round_Rectangle(x_start  ,y_start  ,x_end  ,y_end  ,radius  ,line_color,NO);//绘制主圆角矩形框
   LCD_Draw_Round_Rectangle(x_start+1,y_start+1,x_end-1,y_end-1,radius-1,line_color,NO);//绘制主圆角矩形框,加粗效果
   LCD_Draw_Round_Rectangle(x_start+2,y_start+2,x_end-2,y_end-2,radius-2,back_color,YES);//绘制主圆角矩形框,填充背景色
  }   
}

/*******************************************************************************************************************************
显示ASCII字符子函数(软件绘制，具备文字放大功能，放大倍数与RA8875设置倍数相同)(使用BTE颜色扩充功能)
输入参数 code   :ASCII字库编号
         x      :横坐标,以字节为单位
         y      :纵坐标,以像素为单位
         font   :字体大小,FONT_8 FONT_12 FONT16 FONT_24 FONT_32
         f_color:文字颜色
         b_color:文字背景色
         reverse:反显标志,0 反显 其它值正常显示
*******************************************************************************************************************************/
void LCD_Put_Char(unsigned short code,unsigned short x,unsigned short y,unsigned char font,
                  unsigned short f_color,unsigned short b_color,signed char reverse)
{
 unsigned char  i,j,x_zoom,y_zoom,x_dot,y_dot,x_dot_sum,y_dot_sum,x_byte,layer,data[12],now_bit,msb_bit;
 unsigned short color_tmp;
 unsigned int   dot_data;
 if(reverse == 0)/*需要反显*/
  {
   color_tmp = f_color;//交换颜色
   f_color   = b_color;
   b_color   = color_tmp;
  }
 switch(font)/*计算一个字符实际点阵大小*/
  {
   case  FONT_8://6*8点阵ASCII字符
                x_dot      = 6;
                y_dot      = 8;
                x_dot_sum  = 6 * (FONT_X_ZOOM + 1);
                y_dot_sum  = 8 * (FONT_Y_ZOOM + 1);
                msb_bit    = 7;//字模最高位
                break;
   case FONT_12://6*12点阵ASCII字符
                x_dot      =  6;
                y_dot      = 12;
                x_dot_sum  =  6 * (FONT_X_ZOOM + 1);
                y_dot_sum  = 12 * (FONT_Y_ZOOM + 1);
                msb_bit    = 7;//字模最高位
                break;
   case FONT_16://8*16点阵ASCII字符
   default:
                x_dot      =  8;
                y_dot      = 16;
                x_dot_sum  =  8 * (FONT_X_ZOOM + 1);
                y_dot_sum  = 16 * (FONT_Y_ZOOM + 1);
                msb_bit    = 7;//字模最高位
                break;
   case FONT_24://12*24点阵ASCII字符
                x_dot      = 12;
                y_dot      = 24;
                x_dot_sum  = 12 * (FONT_X_ZOOM + 1);
                y_dot_sum  = 24 * (FONT_Y_ZOOM + 1);
                msb_bit    = 15;//字模最高位
                break;
   case FONT_32://16*32点阵ASCII字符
                x_dot      = 16;
                y_dot      = 32;
                x_dot_sum  = 16 * (FONT_X_ZOOM + 1);
                y_dot_sum  = 32 * (FONT_Y_ZOOM + 1);
                msb_bit    = 15;//字模最高位
                break;
   case FONT_48://24*48点阵ASCII字符
                x_dot      = 24;
                y_dot      = 48;
                x_dot_sum  = 24 * (FONT_X_ZOOM + 1);
                y_dot_sum  = 48 * (FONT_Y_ZOOM + 1);
                msb_bit    = 23;//字模最高位
                break;
  }
 x_byte = x_dot_sum / 8;
 if(x_dot_sum % 8) x_byte += 1;//计算每行占用字节数量,当不是整字节个数时,多加一个整字节,24*48点阵字符放大4倍后最大占用12字节 
  
 layer = LCD_Read_Register(0x41) & 0x01;/*读取41h MWCR1,读取读写图层信息*/
 if(layer == 0) y &= 0x7fff;//设置BTE目标写入位置时,写入图层1
 else           y |= 0x8000;//设置BTE目标写入位置时,写入图层2 
 LCD_Write_Register(0x58,x);       /*BTE目标写入位置*/
 LCD_Write_Register(0x59,x >> 8);
 LCD_Write_Register(0x5A,y);
 LCD_Write_Register(0x5B,y >> 8); 
 LCD_Write_Register(0x5C,x_dot_sum);    /*BTE区块宽度*/
 LCD_Write_Register(0x5D,x_dot_sum >> 8); 
 LCD_Write_Register(0x5E,y_dot_sum);    /*BTE区块高度*/
 LCD_Write_Register(0x5F,y_dot_sum >> 8); 
 LCD_Set_Background_Color(b_color);/*设置背景色和前景色*/
 LCD_Set_Foreground_Color(f_color); 
 LCD_Write_Register(0x51,0x78);    /*写入BTE运算码，颜色扩充功能*/ 
 LCD_Write_Register(0x50,0x80);    /*开始BTE功能*/
 LCD_Write_Command(0x02);          /*02h MRWC,内存读写命令,进入数据读写模式*/
 
 for(i=0;i<y_dot;i++)/*按照字库原始行数进行行扫描*/
  {
   /*首先预读取字模点阵第一行数据,以便后续运算*/
   if(font == FONT_8)       dot_data = DOT_ARRAY_68[code][i]; //读取字模到内存,加速操作速度
   else if(font == FONT_12) dot_data = DOT_ARRAY_612[code][i];//读取字模到内存,加速操作速度
   else if(font == FONT_16) dot_data = DOT_ARRAY_816[code][i];//读取字模到内存,加速操作速度
   else if(font == FONT_24)
    {
     if(FONT_X_ZOOM || FONT_Y_ZOOM) dot_data = (DOT_ARRAY_1224[code][i*2] << 8) | DOT_ARRAY_1224[code][i*2+1];//读取字模到内存,加速操作速度
     else dot_data = DOT_ARRAY_1224[code][i*2] | (DOT_ARRAY_1224[code][i*2+1] << 8);//不放大时,需要先写的数据放在低位,便于加速写入
    } 
   else if(font == FONT_32)
    {
     if(FONT_X_ZOOM || FONT_Y_ZOOM) dot_data = (DOT_ARRAY_1632[code][i*2] << 8) | DOT_ARRAY_1632[code][i*2+1];//读取字模到内存,加速操作速度
     else dot_data = DOT_ARRAY_1632[code][i*2] | (DOT_ARRAY_1632[code][i*2+1] << 8);//不放大时,需要先写的数据放在低位,便于加速写入
    }
   else if(font == FONT_48)
    {
     if(FONT_X_ZOOM || FONT_Y_ZOOM) dot_data = (DOT_ARRAY_2448[code][i*3] << 16) | (DOT_ARRAY_2448[code][i*3+1] << 8) | DOT_ARRAY_2448[code][i*3+2];//读取字模到内存,加速操作速度
     else dot_data = DOT_ARRAY_2448[code][i*3] | (DOT_ARRAY_2448[code][i*3+1] << 8) | (DOT_ARRAY_2448[code][i*3+2] << 16);//不放大时,需要先写的数据放在低位,便于加速写入
    }    
   else dot_data = DOT_ARRAY_816[code][i];//读取字模到内存,加速操作速度   
   /*根据FONT_X_ZOOM数值对每行数据进行处理*/
   if(FONT_X_ZOOM || FONT_Y_ZOOM)/*字符需要放大*/
    {
     now_bit = 0;                      //初始化当前处理的位标号
     for(j=0;j<x_byte;j++) data[j] = 0;//初始化数据缓冲区
     for(j=0;j<x_dot;j++)
      {
       if(Get_Bit(dot_data,msb_bit-j))//根据字膜判断，此处需要显示点
        {
         for(x_zoom=0;x_zoom<(FONT_X_ZOOM+1);x_zoom++)/*文字水平放大*/
          {
           Set_Bit(data[now_bit / 8],7 - now_bit % 8);
           now_bit += 1;
          }
        }
       else
        {
         now_bit += (FONT_X_ZOOM + 1);
        }
      }
     for(y_zoom=0;y_zoom<(FONT_Y_ZOOM+1);y_zoom++)/*文字垂直放大*/
      {
       for(j=0;j<x_byte;j++)/*colum扫描*/
        {
         LCD_Write_Data(data[j]);
         while(LCD_BUSY_STAT == 0);
        }       
      }
    }
   else /*字符不需要放大*/
    {
     data[0] = dot_data;
     data[1] = dot_data >> 8;
     data[2] = dot_data >> 16;//直接进行数据幅值,从而加快运算速度
     for(j=0;j<x_byte;j++)
      {       
       LCD_Write_Data(data[j]);
       while(LCD_BUSY_STAT == 0);
      }
    }   
  }  
 LCD_Write_Register(0x50,0x00);//关闭BTE功能
}

/*******************************************************************************************************************************
内置外置字库选择子函数
输入参数 cgrom_mode:字库模式；
         cgram_en  :使用内部字库模式时，是否选择内部自建字库
*******************************************************************************************************************************/
void LCD_CGROM_Select(unsigned char cgrom_mode,unsigned char cgram_en)
{
 unsigned char tmp;
 if(cgrom_mode == INT_CGROM) /*选择内部字库*/
  {
   if(cgram_en == CGRAM_DISABLE) //使用内部CGROM
    {
     LCD_Write_Register(0x21,0x00);//21h FNCR0,字体控制寄存器0；Bit[7]   = 0(CGROM)
                                                              //Bit[5]   = 0(选择内部CGROM)
                                                              //Bit[1:0] = 00b(选择ISO/IEC8859-1标准8*16点阵文字)
     LCD_Write_Register(0x2F,0x00);//2Fh,串行外部字库设置寄存器；当选择内部CGROM时，0x2F寄存器必须设置成0x00
    }
   else //使用内部CGRAM自建字库
    {
     LCD_Write_Register(0x21,0xA0);//21h FNCR0,字体控制寄存器0；Bit[7]   = 1(选择内部CGRAM自建字库)
                                                              //Bit[5]   = 1(选择内部CGRAM自建字库时，此位必须为1)
                                                              //Bit[1:0] = 00b(选择ISO/IEC8859-1标准8*16点阵文字)
    }
   /*使用内部CGRAM或CGROM时，文字大小必须设置为16*16 8*16*/ 
   tmp = LCD_Read_Register(0x2E);//读取2Eh 字体写入类型设置寄存器；
   tmp &= 0x3f;                  //设置文字大小为16*16 8*16
   LCD_Write_Register(0x2E,tmp); //2Eh 字体写入类型设置寄存器；Bit[7:6] = 00b(设置文字大小为16*16 8*16) 
  }
 else /*选择外部字库*/
  {
   /*设置外部字库芯片接口*/
   //这里外部字库接口的SPI时钟速率使用芯片上电默认值,因字库芯片SPI时钟速率为120MHz,远大于RA8875系统频率
   LCD_Write_Register(0x06,0x00);//06h SFCLR,串行FLASH/ROM时钟设置寄存器；Bit[1:0] = 00b(SFCL频率=系统频率)
   LCD_Write_Register(0x05,0x28);//05h SROC ,串行FLASH/ROM配置寄存器；Bit[7]   = 0(选择Serial Flash/ROM 0接口,硬件上此接口接字库芯片)
                                                                    //Bit[6]   = 0(24位寻址)
                                                                    //Bit[5]   = 1(SPI模式3,CLK上升沿采样数据)
                                                                    //Bit[4:3] = 01b(5BUS模式,一字节孔周期,快速读取模式)
                                                                    //Bit[2]   = 0(字形模式)
                                                                    //Bit[1:0] = 1(Serial Flash /ROM I/F Data Latch模式为单一模式)
   tmp = LCD_Read_Register(0x2F);//读取2Fh,串行字库设置寄存器；
   tmp = (tmp & 0x03) | 0x80;    //Bit[7:5] = 100b(使用GT30L32S4W字库芯片)
                                 //Bit[4:2] = 000b(GB2312汉子字符集)
                                 //Bit[1:0]ASCII字符字体   
   LCD_Write_Register(0x2F,tmp); //2Fh,串行字库设置寄存器；
   LCD_Write_Register(0x21,0x20);//21h FNCR0,字体控制寄存器0；Bit[7]   = 0(CGROM)
                                                            //Bit[5]   = 1(选择外部CGROM)
                                                            //Bit[1:0] = 00b(选择ISO/IEC8859-1标准8*16点阵文字)
  } 
}

/*******************************************************************************************************************************
设置外置字库ASCII字符字体子函数
输入参数 font:字体；
*******************************************************************************************************************************/
void LCD_Set_EXT_CGROM_Font_Type(unsigned char font)
{
 unsigned char tmp;
 tmp = LCD_Read_Register(0x2F);//读取0x2F串行外部字库设置寄存器
 switch(font)
  {
   case FONT_NORMAL://标准ASCII字体
                    tmp &= 0xfc;
                    break;
   case  FONT_AIRAL://Airal ASCII字体
                    tmp = (tmp & 0xfc) | 0x01;
                    break;
   case  FONT_ROMAN://Times New Roman ASCII字体
                    tmp = (tmp & 0xfc) | 0x02;
                    break;
   default://标准ASCII字体
           tmp &= 0xfc;
           break;
  }
 LCD_Write_Register(0x2F,tmp);//2Fh 串行外部字库设置寄存器;设置ASCII字体 
}

/*******************************************************************************************************************************
设置外置字库字体大小子函数
输入参数 size:字体大小；
*******************************************************************************************************************************/
void LCD_Set_EXT_CGROM_Font_Size(unsigned char size)
{
 unsigned char tmp;
 tmp = LCD_Read_Register(0x2E);//读取0x2E字体写类型设置寄存器
 switch(size)
  {
   case FONT_16://16*16点阵字体
   default:
                tmp &= 0x3f;
                break;
   case FONT_24://24*24点阵字体
                tmp = (tmp & 0x3f) | 0x40;
                break;
   case FONT_32://32*32点阵字体
                tmp = (tmp & 0x3f) | 0x80;
                break;
  }
 LCD_Write_Register(0x2E,tmp);//2Eh 字体写类型设置寄存器;设置字体大小
}

/*******************************************************************************************************************************
设置文字间距子函数
输入参数 x_space:文字水平间距
         y_space:文字垂直间距
*******************************************************************************************************************************/
void LCD_Set_Text_Space(unsigned char x_space,unsigned char y_space)
{
 unsigned char tmp;
 if(x_space > 63) x_space = 63;   //水平间距最大为63个像素
 if(y_space > 31) y_space = 31;   //垂直间距最大为31个像素
 tmp = LCD_Read_Register(0x2E);   //读取0x2E字体写类型设置寄存器
 tmp = (tmp & 0xC0) | x_space;    //设置水平间距
 LCD_Write_Register(0x2E,tmp);    //2Eh 字体写类型设置寄存器；文字水平间距
 LCD_Write_Register(0x29,y_space);//29h FLDR,文字行间距寄存器；文字垂直间距
}

/*******************************************************************************************************************************
设置文字横向、纵向放大子函数
输入参数 x_zoom:水平放大倍数；0：不放大，1：放大2倍，2：放大3倍；3：放大4倍
         y_zoom:垂直放大倍数；0：不放大，1：放大2倍，2：放大3倍；3：放大4倍
*******************************************************************************************************************************/
void LCD_Set_Text_Zoom(unsigned char x_zoom,unsigned char y_zoom)
{
 unsigned char tmp;
 if(x_zoom > 3) x_zoom = 3;           //水平最大放大4倍
 if(y_zoom > 3) y_zoom = 3;           //垂直最大放大4倍
 FONT_X_ZOOM = x_zoom;                //记录当前文字水平放大倍数
 FONT_Y_ZOOM = y_zoom;                //记录当前文字垂直放大倍数
 x_zoom <<= 2;                        //调整x_zoom值便于操作
 tmp = LCD_Read_Register(0x22);       //读取22h FNCR1字体控制寄存器
 tmp = (tmp & 0x40) | x_zoom | y_zoom;//设置文字水平、垂直放大倍数；Bit[7] = 0(关闭文字对齐功能)
                                                                  //Bit[4] = 0(文字不旋转)
 LCD_Write_Register(0x22,tmp);        //22h FNCR1字体控制寄存器；设置文字水平、垂直放大倍数
}

/*******************************************************************************************************************************
汉字写入子程序(汉子取自外部字库芯片)
输入参数 x        :横坐标，以像素点为单位
         y        :纵坐标，以像素行为单位
         code     :区位码
         font     :字体；支持FONT16、FONT24、FONT32
         f_color  :文字颜色
         b_color  :背景颜色
         reverse  :反显标志；0:反显；1:正常显示
*******************************************************************************************************************************/
void LCD_Display_HZ(unsigned short x,unsigned short y,unsigned short code,unsigned char font,
                    unsigned short f_color,unsigned short b_color,signed char reverse)
{
 unsigned char tmp,i,data[2];
 unsigned short color_tmp;
 data[0] = code >> 8;
 data[1] = code;
 if(reverse == 0)/*需要反显*/
  {
   color_tmp = f_color;//交换颜色
   f_color   = b_color;
   b_color   = color_tmp;
  }
 //else b_color = LCD_Get_Pixel_Color(x,y);/*取得写入位置的背景颜色*/
 color_tmp = LCD_BACK_COLOR;//暂存背景色
 LCD_Mode_Select(TEXT_MODE);       /*进入文字模式*/
 LCD_Set_Foreground_Color(f_color);/*设置文字颜色，即前景色*/
 LCD_Set_Background_Color(b_color);/*设置文字背景颜色，即背景色*/
 LCD_Set_Text_Cursor(x,y);         /*设置文字光标位置*/
 LCD_CGROM_Select(EXT_CGROM,CGRAM_DISABLE);/*使用外部CGROM字库，禁用内部CGRAM*/
 LCD_Set_EXT_CGROM_Font_Size(font);/*设置外部CGROM字库字体大小*/
 tmp = LCD_Read_Register(0x22);    /*22h FNCR1,读取字体控制寄存器,为反显做准备*/
 tmp &= 0xbf;                 //22h FNCR1,Bit[6] = 0(文字具备背景色模式)
 LCD_Write_Register(0x22,tmp);//设置22h FNCR1字体控制寄存器
 LCD_Write_Command(0x02);/*02h MRWC,内存读写命令,进入数据读写模式*/
 for(i=0;i<2;i++)
  {
   LCD_Write_Data(data[i]);//写入字符区位码      
  }
 LCD_Check_Mem_Busy();//等待操作完成 
 //while((LCD_Read_Status() & 0x80) == 0x80);//等待操作完成,这是不使用LCD_Check_Busy()是因为测试发现操作外部字库时,WAIT信号有时不正常(时间非常短),导致RA8875一直忙 
 LCD_Set_Background_Color(color_tmp);/*恢复原有背景色*/ 
 LCD_Mode_Select(GRAPHIC_MODE);      /*恢复到图形模式*/
}

/*******************************************************************************************************************************
32×64点阵字符字库显示子函数(使用BTE颜色扩充功能)
输入参数 code   :字符索引，ASCII码即为ASCII码实际值
         x      :横坐标,以像素为单位
         y      :纵坐标,以像素为单位
         f_color:文字颜色
         b_color:背景颜色
         reverse:反显标志,0 反显 其它值正常显示
*******************************************************************************************************************************/
void LCD_Put_Dot3264(unsigned char code,unsigned short x,unsigned short y,unsigned short f_color,unsigned short b_color,signed char reverse)
{
 unsigned short i,j,layer,color_tmp,position;
 for(position=0;position<NUM_OF_CHAR3264;position++)/*在字符字库中寻找要显示的字符字库的位置*/
  {
   if(DOT_ARRAY_3264[position][0] == code)
    {
     break;
    }
  } 
 if(reverse == 0)/*需要反显*/
  {
   color_tmp = f_color;  //交换颜色
   f_color   = b_color;
   b_color   = color_tmp;
  }
 layer = LCD_Read_Register(0x41) & 0x01;/*读取41h MWCR1,读取读写图层信息*/
 if(layer == 0) y &= 0x7fff;//设置BTE目标写入位置时,写入图层1
 else           y |= 0x8000;//设置BTE目标写入位置时,写入图层2  
 LCD_Write_Register(0x58,x);       /*BTE目标写入位置*/
 LCD_Write_Register(0x59,x >> 8);
 LCD_Write_Register(0x5A,y);
 LCD_Write_Register(0x5B,y >> 8); 
 LCD_Write_Register(0x5C,0x20);    /*BTE区块宽度32*/
 LCD_Write_Register(0x5D,0x00); 
 LCD_Write_Register(0x5E,0x40);    /*BTE区块高度64*/
 LCD_Write_Register(0x5F,0x00); 
 LCD_Set_Background_Color(b_color);/*设置背景色和前景色*/
 LCD_Set_Foreground_Color(f_color); 
 LCD_Write_Register(0x51,0x78);    /*写入BTE运算码，颜色扩充功能*/ 
 LCD_Write_Register(0x50,0x80);    /*开始BTE功能*/
 LCD_Write_Command(0x02);          /*02h MRWC,内存读写命令,进入数据读写模式*/ 
 for(i=0;i<64;i++)//共64行
  {
   for(j=0;j<4;j++)//共4 Colum
    {
     if(position == NUM_OF_CHAR3264)/*未找到要显示的字符，显示空白*/
      {
       LCD_Write_Data(0x00);
      }
     else
      {     
       LCD_Write_Data(DOT_ARRAY_3264[position][i*4+1+j]);
      } 
     while(LCD_BUSY_STAT == 0);     
    }
  }
 LCD_Write_Register(0x50,0x00);//关闭BTE功能
}

/*******************************************************************************************************************************
24×24点阵字库显示子函数(使用BTE颜色扩充功能)
输入参数 code   :图标字库编号
         x      :横坐标,以像素为单位
         y      :纵坐标,以像素为单位
         f_color:文字颜色
         b_color:背景颜色
         reverse:反显标志,0 反显 其它值正常显示
*******************************************************************************************************************************/
void LCD_Put_Dot2424(unsigned short code,unsigned short x,unsigned short y,unsigned short f_color,unsigned short b_color,signed char reverse)
{
 unsigned short i,j,layer,color_tmp; 
 if(reverse == 0)/*需要反显*/
  {
   color_tmp = f_color;  //交换颜色
   f_color   = b_color;
   b_color   = color_tmp;
  }
 layer = LCD_Read_Register(0x41) & 0x01;/*读取41h MWCR1,读取读写图层信息*/
 if(layer == 0) y &= 0x7fff;//设置BTE目标写入位置时,写入图层1
 else           y |= 0x8000;//设置BTE目标写入位置时,写入图层2  
 LCD_Write_Register(0x58,x);       /*BTE目标写入位置*/
 LCD_Write_Register(0x59,x >> 8);
 LCD_Write_Register(0x5A,y);
 LCD_Write_Register(0x5B,y >> 8); 
 LCD_Write_Register(0x5C,0x18);    /*BTE区块宽度24*/
 LCD_Write_Register(0x5D,0x00); 
 LCD_Write_Register(0x5E,0x18);    /*BTE区块高度24*/
 LCD_Write_Register(0x5F,0x00); 
 LCD_Set_Background_Color(b_color);/*设置背景色和前景色*/
 LCD_Set_Foreground_Color(f_color); 
 LCD_Write_Register(0x51,0x78);    /*写入BTE运算码，颜色扩充功能*/ 
 LCD_Write_Register(0x50,0x80);    /*开始BTE功能*/
 LCD_Write_Command(0x02);          /*02h MRWC,内存读写命令,进入数据读写模式*/ 
 for(i=0;i<24;i++)//共24行
  {
   for(j=0;j<3;j++)//共3 Colum
    {
     LCD_Write_Data(DOT_ARRAY_2424[code][i*3+2+j]);
     while(LCD_BUSY_STAT == 0);     
    }
  }
 LCD_Write_Register(0x50,0x00);//关闭BTE功能
}

/*******************************************************************************************************************************
32×32点阵字库显示子函数(使用BTE颜色扩充功能)
输入参数 code   :图标字库编号
         x      :横坐标,以像素为单位
         y      :纵坐标,以像素为单位
         f_color:文字颜色
         b_color:背景颜色
         reverse:反显标志,0 反显 其它值正常显示
*******************************************************************************************************************************/
void LCD_Put_Dot3232(unsigned short code,unsigned short x,unsigned short y,unsigned short f_color,unsigned short b_color,signed char reverse)
{
 unsigned short i,j,layer,color_tmp; 
 if(reverse == 0)/*需要反显*/
  {
   color_tmp = f_color;  //交换颜色
   f_color   = b_color;
   b_color   = color_tmp;
  }
 layer = LCD_Read_Register(0x41) & 0x01;/*读取41h MWCR1,读取读写图层信息*/
 if(layer == 0) y &= 0x7fff;//设置BTE目标写入位置时,写入图层1
 else           y |= 0x8000;//设置BTE目标写入位置时,写入图层2  
 LCD_Write_Register(0x58,x);       /*BTE目标写入位置*/
 LCD_Write_Register(0x59,x >> 8);
 LCD_Write_Register(0x5A,y);
 LCD_Write_Register(0x5B,y >> 8); 
 LCD_Write_Register(0x5C,0x20);    /*BTE区块宽度32*/
 LCD_Write_Register(0x5D,0x00); 
 LCD_Write_Register(0x5E,0x20);    /*BTE区块高度32*/
 LCD_Write_Register(0x5F,0x00); 
 LCD_Set_Background_Color(b_color);/*设置背景色和前景色*/
 LCD_Set_Foreground_Color(f_color); 
 LCD_Write_Register(0x51,0x78);    /*写入BTE运算码，颜色扩充功能*/ 
 LCD_Write_Register(0x50,0x80);    /*开始BTE功能*/
 LCD_Write_Command(0x02);          /*02h MRWC,内存读写命令,进入数据读写模式*/ 
 for(i=0;i<32;i++)//共32行
  {
   for(j=0;j<4;j++)//共4 Colum
    {
     LCD_Write_Data(DOT_ARRAY_3232[code][i*4+2+j]);
     while(LCD_BUSY_STAT == 0);     
    }
  }
 LCD_Write_Register(0x50,0x00);//关闭BTE功能
}

/*******************************************************************************************************************************
48×48点阵字库显示子函数(使用BTE颜色扩充功能)
输入参数 code   :图标字库编号
         x      :横坐标,以像素为单位
         y      :纵坐标,以像素为单位
         f_color:文字颜色
         b_color:背景颜色
         reverse:反显标志,0 反显 其它值正常显示
*******************************************************************************************************************************/
void LCD_Put_Dot4848(unsigned short code,unsigned short x,unsigned short y,unsigned short f_color,unsigned short b_color,signed char reverse)
{
 unsigned short i,j,layer,color_tmp; 
 if(reverse == 0)/*需要反显*/
  {
   color_tmp = f_color;  //交换颜色
   f_color   = b_color;
   b_color   = color_tmp;
  }
 layer = LCD_Read_Register(0x41) & 0x01;/*读取41h MWCR1,读取读写图层信息*/
 if(layer == 0) y &= 0x7fff;//设置BTE目标写入位置时,写入图层1
 else           y |= 0x8000;//设置BTE目标写入位置时,写入图层2  
 LCD_Write_Register(0x58,x);       /*BTE目标写入位置*/
 LCD_Write_Register(0x59,x >> 8);
 LCD_Write_Register(0x5A,y);
 LCD_Write_Register(0x5B,y >> 8); 
 LCD_Write_Register(0x5C,0x30);    /*BTE区块宽度48*/
 LCD_Write_Register(0x5D,0x00); 
 LCD_Write_Register(0x5E,0x30);    /*BTE区块高度48*/
 LCD_Write_Register(0x5F,0x00); 
 LCD_Set_Background_Color(b_color);/*设置背景色和前景色*/
 LCD_Set_Foreground_Color(f_color); 
 LCD_Write_Register(0x51,0x78);    /*写入BTE运算码，颜色扩充功能*/ 
 LCD_Write_Register(0x50,0x80);    /*开始BTE功能*/
 LCD_Write_Command(0x02);          /*02h MRWC,内存读写命令,进入数据读写模式*/ 
 for(i=0;i<48;i++)//共32行
  {
   for(j=0;j<6;j++)//共6 Colum
    {
     LCD_Write_Data(DOT_ARRAY_4848[code][i*6+2+j]);
     while(LCD_BUSY_STAT == 0);     
    }
  }
 LCD_Write_Register(0x50,0x00);//关闭BTE功能
}

/*******************************************************************************************************************************
汉字和ASCII字符混合写入子程序(ASCII字符和汉子全部取自外部字库芯片)
输入参数 x        :横坐标，以像素点为单位
         y        :纵坐标，以像素行为单位
         pstr     :字符串
         font     :字体；支持FONT16、FONT24、FONT32
         f_color  :文字颜色
         b_color  :背景颜色
         reverse  :反显标志；0:反显；1:正常显示
*******************************************************************************************************************************/
void LCD_Display_String(unsigned short x,unsigned short y,const char *pstr,unsigned char font,
                        unsigned short f_color,unsigned short b_color,signed char reverse)
{
 unsigned char tmp;
 unsigned short color_tmp;
 if(font >= FONT_48) font = FONT_32;//外部字库芯片最大字体支持32*32点阵字体
 if(reverse == 0)/*需要反显*/
  {
   color_tmp = f_color;//交换颜色
   f_color   = b_color;
   b_color   = color_tmp;
  }
 //else b_color = LCD_Get_Pixel_Color(x,y);/*取得写入位置的背景颜色*/
 color_tmp = LCD_BACK_COLOR;//暂存背景色
 LCD_Mode_Select(TEXT_MODE);       /*进入文字模式*/
 LCD_Set_Foreground_Color(f_color);/*设置文字颜色，即前景色*/
 LCD_Set_Background_Color(b_color);/*设置文字背景颜色，即背景色*/
 LCD_Set_Text_Cursor(x,y);         /*设置文字光标位置*/
 LCD_CGROM_Select(EXT_CGROM,CGRAM_DISABLE);/*使用外部CGROM字库，禁用内部CGRAM*/
 LCD_Set_EXT_CGROM_Font_Size(font);/*设置外部CGROM字库字体大小*/
 tmp = LCD_Read_Register(0x22);    /*22h FNCR1,读取字体控制寄存器,为反显做准备*/
 tmp &= 0xbf;                 //22h FNCR1,Bit[6] = 0(文字具备背景色模式)
 LCD_Write_Register(0x22,tmp);//设置22h FNCR1字体控制寄存器
 LCD_Write_Command(0x02);/*02h MRWC,内存读写命令,进入数据读写模式*/
 while (*pstr != 0)/*开始循环处理字符*/
  {
   LCD_Write_Data(*pstr++);//写入字符区位码，同时指针加一
   LCD_Check_Mem_Busy();//等待操作完成
   //while((LCD_Read_Status() & 0x80) == 0x80);//等待操作完成,这是不使用LCD_Check_Busy()是因为测试发现操作外部字库时,WAIT信号有时不正常(时间非常短),导致RA8875一直忙       
  }
 LCD_Set_Background_Color(color_tmp);/*恢复原有背景色*/ 
 LCD_Mode_Select(GRAPHIC_MODE);      /*恢复到图形模式*/
}

/*******************************************************************************************************************************
汉字和ASCII字符混合写入子程序(ASCII字符取自程序软件，汉子取自外部字库芯片，FONT48字体取自软件字库)
输入参数 x        :横坐标，以像素点为单位
         y        :纵坐标，以像素行为单位
         pstr     :字符串
         font     :字体；支持FONT16、FONT24、FONT32、FONT48
         f_color  :文字颜色
         b_color  :背景颜色
         reverse  :反显标志；0:反显；1:正常显示
*******************************************************************************************************************************/
void LCD_Put_String(unsigned short x,unsigned short y,const char *pstr,unsigned char font,
                    unsigned short f_color,unsigned short b_color,signed char reverse)
{
 unsigned short numofdot,x_offset,code,code1,i;
 switch(font)
  {
   case  FONT_8://6*8点阵ASCII字符
                x_offset = 6*(FONT_X_ZOOM+1);
                break;
   case FONT_12://6*12点阵ASCII字符
                x_offset = 6*(FONT_X_ZOOM+1);
                break;
   case FONT_16://8*16点阵ASCII字符
   default:
                x_offset = 8*(FONT_X_ZOOM+1);
                break;
   case FONT_24://12*24点阵ASCII字符
                x_offset = 12*(FONT_X_ZOOM+1);
                break;
   case FONT_32://16*32点阵ASCII字符
                x_offset = 16*(FONT_X_ZOOM+1);
                break;
   case FONT_48://24*48点阵ASCII字符
                x_offset = 24*(FONT_X_ZOOM+1);
                break;
  }
 while(*pstr > 0)/*判断字符串结尾标志0x00*/
  {
   code = *pstr++; //取字符代码
   if(code >= 0x20 && code <= 0x7f)/*判断为ASCII字符*/
    {
     numofdot = code - 0x20;//计算字符字模起始地址
     LCD_Put_Char(numofdot,x,y,font,f_color,b_color,reverse);//显示ASCII字符
     x += x_offset;//列地址自动加一个ASCII字符位置
    }
   else if(code >= 0xa1 && code <= 0xf7)/*判断为国标一二级字库汉字*/
    {
     if(font >= FONT_48)/*大于48*48点阵的汉字只能使用软件字库*/
      {
       code1 = *pstr ++;//取机器码低字节，同时字符串指针再加一，完成对此汉字机器码的访问
       for(i=0;i<NUM_OF_HZDOT48;i++)
        {
         //在字库中找到此汉字的点阵所以机器码，开始显示
         if((DOT_ARRAY_4848[i][0] == code) && (DOT_ARRAY_4848[i][1] == code1))
          {
           LCD_Put_Dot4848(i,x,y,f_color,b_color,reverse);
           x += 48;//列号自动加一个汉字字符位置
           break;
          }
        }
      }
     else
      {     
       code = (code << 8) | *pstr++;//取机器码低字节，同时字符串指针再加一，完成对此汉字机器的访问
       LCD_Display_HZ(x,y,code,font,f_color,b_color,reverse);
	   x += x_offset * 2;//列号自动加一个汉字字符位置
      } 
    }
  }
}

/*******************************************************************************************************************************
大字体汉字和ASCII字符混合写入子程序(ASCII字符和汉子取自程序软件字库，ASCII字符和汉子最小点阵从32*64和64*64起步)
此函数不支持放大功能
输入参数 x        :横坐标，以像素点为单位
         y        :纵坐标，以像素行为单位
         pstr     :字符串
         font     :字体；支持FONT_64
         f_color  :文字颜色
         b_color  :背景颜色
         reverse  :反显标志；0:反显；1:正常显示
*******************************************************************************************************************************/
void LCD_Put_Big_String(unsigned short x,unsigned short y,const char *pstr,unsigned char font,
                        unsigned short f_color,unsigned short b_color,signed char reverse)
{
 unsigned short x_offset,code;
 while(*pstr > 0)/*判断字符串结尾标志0x00*/
  {
   code = *pstr++; //取字符代码
   if(code >= 0x20 && code <= 0x7f)/*判断为ASCII字符*/
    {
     switch(font)
      {
       case FONT_64://32*64点阵ASCII字符
       default     :
                    x_offset = 32;
                    LCD_Put_Dot3264(code,x,y,f_color,b_color,reverse);//显示ASCII字符
                    break;
      }
     x += x_offset;//列地址自动加一个ASCII字符位置
    }
   else if(code >= 0xa1 && code <= 0xf7)/*判断为国标一二级字库汉字*/
    {
      
    }
  }
}

/*******************************************************************************************************************************
显示有符号INT型变量子函数
输入参数 data     :要显示的数据
         symbol_en:是否显示符号位,0:不显示,非0:显示符号位
         x        :横坐标,以字节为单位
         y        :纵坐标,以像素为单位
         length   :要显示的位数,最多显示10位
         dot      :要显示的小数位数,最多显示9位小数
         font     :字体；支持FONT16、FONT24、FONT32、FONT48
         f_color  :文字颜色
         b_color  :背景颜色
         reverse  :反显标志；0:反显；1:正常显示
*******************************************************************************************************************************/
void LCD_IntNum(signed int data,unsigned short x,unsigned short y,unsigned char symbol_en,unsigned char length,unsigned char dot,
                unsigned char font,unsigned short f_color,unsigned short b_color,signed char reverse)
{
 unsigned char px[12],i,j,x_offset;
 //进行每一位数的数据提取 
 if(data >= 0)
  {
   px[11] = 0x0b;//正数,"+"号
  }
 else
  {
   data   = 0 - data;
   px[11] = 0x0d;//负数,"-"号
  }
 if(dot == 0)//小数位为0
  {
   px[10] = px[11];//将符号复制到此位,标志数据串到此结束
   for(i=9;i>0;i--)
    {
     px[i]  = data / Int_Power(10,i) + 0x10;//取值,并转换为字模编码
     data  %= Int_Power(10,i);
     if(i == 1) px[0] = data + 0x10;
    }
  }
 else //有小数位需要显示
  {
   length += 1;//长度自动加1,用于显示小数点
   j = 9;
   for(i=10;i>0;i--)
    {
     if(i == dot)//此位置是小数点的位置
      {
       px[i] = 0x0e;//小数点,"."符号
      }
     else
      {
       px[i]  = data / Int_Power(10,j) + 0x10;//取值,并转换为字模编码;
       data  %= Int_Power(10,j--);
      }
     if(i == 1) px[0] = data + 0x10;
    } 
  }
 //数据串整理,去掉不需要显示的数字0
 if(dot == 0)//小数位为0
  {
   for(i=9;i>0;i--)//从第10位开始判断,最后一位不判断
    {
     if(px[i] == 0x10)//如果此位是0,则将符号复制到此位
      {
       px[i] = px[11];
      }
     else break;//结束循环,数据串整理完毕
    }
  }
 else //有小数位需要显示
  {
   for(i=10;i>2;i--)//从第11位开始判断,第3位以后不判断
    {
     if(px[i] == 0x10 && px[i-1] != 0x0e)//如果此位是0且下一位不是小数点符号,则将符号复制到此位
      {
       px[i] = px[11];
      }
     else break;//结束循环,数据串整理完毕
    }
  }
 for(i=11;i>0;i--)//无用数据位清0
  {
   if(symbol_en != 0)//需要显示符号位
    {
     if(px[i-1] == 0x0b || px[i-1] == 0x0d) 
      {
       px[i] = 0;//当前数据位的下一位是符号位,则清除此位数据,数据串前端无需显示的数据显示"空"符号
      }
     else break;//结束循环
    }
   else //不需要显示符号位
    {
     if(px[i] == 0x0b || px[i] == 0x0d) 
      {
       px[i] = 0;//当前数据位是符号位,则清除此位数据,数据串前端无需显示的数据显示"空"符号
      }
     else break;//结束循环
    }          
  }   
 //开始数据串的显示
 if(symbol_en != 0) length += 1;//需要显示符号位,数据长度自动加1,用于显示符号位
 for(i=0;i<length;i++)
  {
   if(px[i] == 0x0b) px[i] = 0;//"+"号不用显示出来
   switch(font)
    {
     case  FONT_8://6*8点阵字符
                  x_offset = 6 * (FONT_X_ZOOM + 1);
                  break;
     case FONT_12://6*12点阵字符
                  x_offset = 6 * (FONT_X_ZOOM + 1);
                  break;
     case FONT_16://8*16点阵字符
     default:
                  x_offset = 8 * (FONT_X_ZOOM + 1);
                  break;
     case FONT_24://12*24点阵字符
                  x_offset = 12 * (FONT_X_ZOOM + 1);
                  break;
     case FONT_32://16*32点阵字符
                  x_offset = 16 * (FONT_X_ZOOM + 1);
                  break;
     case FONT_48://24*48点阵字符
                  x_offset = 24 * (FONT_X_ZOOM + 1);
                  break;
     case FONT_64://32*64点阵字符
                  x_offset = 32;//32*64点阵字符不支持放大功能
                  break;
    }
   if(font < FONT_64)/*小于32*64点阵的字符显示函数不同*/
    {   
     LCD_Put_Char(px[length-i-1],x+i*x_offset,y,font,f_color,b_color,reverse);//显示数值
    }
   else
    {
     LCD_Put_Dot3264(px[length-i-1]+0x20,x+i*x_offset,y,f_color,b_color,reverse);//显示数值
    }   
  }
}

/*******************************************************************************************************************************
十六进制数据显示子函数
输入参数 data   :要显示的数据
         x      :横坐标,以字节为单位
         y      :纵坐标,以像素为单位
         length :要显示的位数
         font   :字体；支持FONT16、FONT24、FONT32、FONT48
         f_color:文字颜色
         b_color:背景颜色
         reverse:反显标志；0:反显；1:正常显示
*******************************************************************************************************************************/
void LCD_IntHex(unsigned int data,unsigned short x,unsigned short y,unsigned char length,
                unsigned char font,unsigned short f_color,unsigned short b_color,signed char reverse) 
{
 unsigned char px[8],i,x_offset;         
 for(i=0;i<8;i++)
  {
   px[i] = (data & (0x0000000f << (i * 4))) >> (i * 4);
   if(px[i] <= 9) px[i] |= 0x10;
   else px[i] += 0x17;
  }
 for(i=0;i<length;i++)
  {
   switch(font)
    {
     case  FONT_8://6*8点阵字符
                  x_offset = 6 * (FONT_X_ZOOM + 1);
                  break;
     case FONT_12://6*12点阵字符
                  x_offset = 6 * (FONT_X_ZOOM + 1);
                  break;
     case FONT_16://8*16点阵字符
     default:
                  x_offset = 8 * (FONT_X_ZOOM + 1);
                  break;
     case FONT_24://12*24点阵字符
                  x_offset = 12 * (FONT_X_ZOOM + 1);
                  break;
     case FONT_32://16*32点阵字符
                  x_offset = 16 * (FONT_X_ZOOM + 1);
                  break;
     case FONT_48://24*48点阵字符
                  x_offset = 24 * (FONT_X_ZOOM + 1);
                  break;
     case FONT_64://32*64点阵字符
                  x_offset = 32;//32*64点阵字符不支持放大功能
                  break;
    }
   if(font < FONT_64)/*小于32*64点阵的字符显示函数不同*/
    { 
     LCD_Put_Char(px[length-i-1],x+i*x_offset,y,font,f_color,b_color,reverse);//显示数值
    }
   else
    {
     LCD_Put_Dot3264(px[length-i-1]+0x20,x+i*x_offset,y,f_color,b_color,reverse);//显示数值
    }   
  } 
}

/********************************************************************************************************************************
FLOAT型数据显示子函数
输入参数 data     :要显示的数据
         x        :横坐标,以像素为单位
         y        :纵坐标,以像素为单位
         symbol_en:是否显示符号位,0:不显示,非0:显示符号位
         length   :要显示的有效数字位数,最多显示7位(float型变量有效位数最多为7位)
         font     :字体大小
         f_color:文字颜色
         b_color:背景颜色
         reverse  :反显标志
float的指数位有8位，而double的指数位有11位，分布如下：
  float：
  1bit（符号位） 8bits（指数位） 23bits（尾数位）
  double：
  1bit（符号位） 11bits（指数位） 52bits（尾数位）
float的指数范围为-127~+128，而double的指数范围为-1023~+1024
  float： 2^23 = 8388608，一共七位，这意味着最多能有7位有效数字，但绝对能保证的为6位，也即float的精度为6~7位有效数字；
  double：2^52 = 4503599627370496，一共16位，同理，double的精度为15~16位
********************************************************************************************************************************/
void LCD_FloatNum(float data,unsigned short x,unsigned short y,unsigned char symbol_en,unsigned char length,
                  unsigned char font,unsigned short f_color,unsigned short b_color,signed char reverse)
{
 unsigned char px[10],dot,i,j,start_num,x_offset;
 unsigned int  tmp;
 if(data >= 0) 
  {
   px[9] = 0;//正号不显示   
  }
 else 
  {
   px[9] = 0x0d;//显示负号
   data = 0 - data;
  }
 //此步运算是为了保证7位有效数字全为整数
 if(data < 1)            
  {
   data *= 10000000;
   dot   = 7;//7位小数
  }
 else if(data < 10)      
  {
   data *= 1000000;
   dot   = 6;//6位小数
  }
 else if(data < 100)     
  {
   data *= 100000;
   dot   = 5;//5位小数
  }
 else if(data < 1000)    
  {
   data *= 10000;
   dot   = 4;//4位小数
  }
 else if(data < 10000)   
  {
   data *= 1000;
   dot   = 3;//3位小数
  }
 else if(data < 100000)  
  {
   data *= 100;
   dot   = 2;//2位小数
  }
 else if(data < 1000000)
  {
   data *= 10;
   dot   = 1;//1位小数
  }
 else dot = 0;//0位小数
 tmp = (unsigned int)data;
 //进行每一位数的数据提取
 if(dot == 0)//小数位为0
  {
   px[8]     = px[9];//将符号复制到此位,标志数据串到此结束
   px[7]     = px[9];//将符号复制到此位,标志数据串到此结束
   start_num = 7;    //从数组编号7位置开始显示
   for(i=6;i>0;i--)
    {
     px[i]  = tmp / Int_Power(10,i) + 0x10;//取值,并转换为字模编码
     tmp   %= Int_Power(10,i);
     if(i == 1) px[0] = tmp + 0x10;
    }
  }
 else //有小数位需要显示
  {
   j = 6;
   if(dot == 7) 
    {
     px[8]     = 0x10;//7位小数,则小数点前自动填0
     //length   += 1;   //长度自动加1,以显示小数点前多出来的数字0
     start_num = 9;   //从数组编号9位置开始显示
    } 
   else 
    {
     px[8]     = px[9];//将符号复制到此位,标志数据串到此结束
     start_num = 8;    //从数组编号8位置开始显示
    } 
   length += 1; //长度自动加1,以显示小数点
   for(i=7;i>0;i--)
    {
     if(i == dot)//此位置是小数点的位置
      {
       px[i] = 0x0e;//小数点,"."符号
      }
     else
      {
       px[i]  = tmp / Int_Power(10,j) + 0x10;//取值,并转换为字模编码;
       tmp   %= Int_Power(10,j--);
      }
     if(i == 1) px[0] = tmp + 0x10;
    } 
  }
 //开始数据串的显示 
 if(symbol_en != 0) 
  {
   length += 1;//需要显示符号位,数据长度自动加1,用于显示符号位
  } 
 else
  {
   start_num -= 1;//去除符号位
  }
 if(px[start_num-length+1] == 0x0e)//要显示的最后一个字符是".",则start_num向后移动一位,从而保证显示的最后一个字符不是"."
  {
   start_num -= 1;
  } 
 for(i=0;i<length;i++)
  {
   switch(font)
    {
     case  FONT_8://6*8点阵字符
                  x_offset = 6 * (FONT_X_ZOOM + 1);
                  break;
     case FONT_12://6*12点阵字符
                  x_offset = 6 * (FONT_X_ZOOM + 1);
                  break;
     case FONT_16://8*16点阵字符
     default:
                  x_offset = 8 * (FONT_X_ZOOM + 1);
                  break;
     case FONT_24://12*24点阵字符
                  x_offset = 12 * (FONT_X_ZOOM + 1);
                  break;
     case FONT_32://16*32点阵字符
                  x_offset = 16 * (FONT_X_ZOOM + 1);
                  break;
     case FONT_48://24*48点阵字符
                  x_offset = 24 * (FONT_X_ZOOM + 1);
                  break;
     case FONT_64://32*64点阵字符
                  x_offset = 32;//32*64点阵字符不支持放大功能
                  break;
    }
   if(font < FONT_64)/*小于32*64点阵的字符显示函数不同*/
    { 
     LCD_Put_Char(px[start_num-i],x+i*x_offset,y,font,f_color,b_color,reverse);//显示数值
    }
   else
    {
     LCD_Put_Dot3264(px[start_num-i]+0x20,x+i*x_offset,y,f_color,b_color,reverse);//显示数值
    }   
  }    
}

/*******************************************************************************************************************************
400*275点阵公司图标显示函数(使用BTE颜色扩充功能)
参数：x         :横坐标,以字节为单位
      y         :纵坐标,以像素为单位
      logo_color:图标颜色
      font_color:文字颜色
*******************************************************************************************************************************/
void LCD_Show_Logo(unsigned short x,unsigned short y,unsigned short logo_color,unsigned short font_color)
{
 unsigned short i,j,b_color,logo_line,layer;
 b_color = LCD_Get_Pixel_Color(x,y);/*取得写入位置的背景颜色*/
#if COLOR_DEPTH == 8 //8位色深模式,需要彩色转换
 b_color = RGB332_RGB565(b_color);
#endif 
 
#if FACTORY == LIXING
 logo_line = 122;//力兴LOGO文字在122行以后
#elif FACTORY == FUAN
 logo_line = 188;//伏安LOGO文字在188行之后
#endif

 layer = LCD_Read_Register(0x41) & 0x01;/*读取41h MWCR1,读取读写图层信息*/
 if(layer == 0) y &= 0x7fff;//设置BTE目标写入位置时,写入图层1
 else           y |= 0x8000;//设置BTE目标写入位置时,写入图层2  
 LCD_Write_Register(0x58,x);             /*BTE目标写入位置*/
 LCD_Write_Register(0x59,x >> 8);
 LCD_Write_Register(0x5A,y);
 LCD_Write_Register(0x5B,y >> 8); 
 LCD_Write_Register(0x5C,0x90);          /*BTE区块宽度400*/
 LCD_Write_Register(0x5D,0x01); 
 LCD_Write_Register(0x5E,logo_line);     /*BTE区块高度先设置为图标高度,显示完图标后再显示文字*/
 LCD_Write_Register(0x5F,logo_line >> 8); 
 LCD_Set_Background_Color(b_color);      /*设置背景色和前景色*/
 LCD_Set_Foreground_Color(logo_color); 
 LCD_Write_Register(0x51,0x78);          /*写入BTE运算码，颜色扩充功能*/ 
 LCD_Write_Register(0x50,0x80);          /*开始BTE功能*/
 LCD_Write_Command(0x02);                /*02h MRWC,内存读写命令,进入数据读写模式*/ 
 for(i=0;i<logo_line;i++)//先显示LOGO图标
  {
   for(j=0;j<50;j++)//共50 Colum
    {
     LCD_Write_Data(COMPANY_LOGO[i*50+j]);
     while(LCD_BUSY_STAT == 0);     
    }
  }
 LCD_Write_Register(0x50,0x00);//关闭BTE功能
 
 LCD_Write_Register(0x58,x);                     /*BTE目标写入位置*/
 LCD_Write_Register(0x59,x >> 8);
 LCD_Write_Register(0x5A,y+logo_line);
 LCD_Write_Register(0x5B,(y+logo_line) >> 8); 
 LCD_Write_Register(0x5C,0x90);                  /*BTE区块宽度400*/
 LCD_Write_Register(0x5D,0x01); 
 LCD_Write_Register(0x5E,275 - logo_line);       /*BTE区块高度设置为LOGO中文字高度*/
 LCD_Write_Register(0x5F,(275 - logo_line) >> 8); 
 LCD_Set_Background_Color(b_color);              /*设置背景色和前景色*/
 LCD_Set_Foreground_Color(font_color); 
 LCD_Write_Register(0x51,0x78);                  /*写入BTE运算码，颜色扩充功能*/ 
 LCD_Write_Register(0x50,0x80);                  /*开始BTE功能*/
 LCD_Write_Command(0x02);                        /*02h MRWC,内存读写命令,进入数据读写模式*/  
 for(i=logo_line;i<275;i++)//显示LOGO中文字部分
  {
   for(j=0;j<50;j++)//共50 Colum
    {
     LCD_Write_Data(COMPANY_LOGO[i*50+j]);
     while(LCD_BUSY_STAT == 0);     
    }
  }  
 LCD_Write_Register(0x50,0x00);//关闭BTE功能
}

/*******************************************************************************************************************************
352*274点阵PT接线原理图显示函数(使用BTE颜色扩充功能)
参数：code       :PT接线图索引
      x          :横坐标,以字节为单位
      y          :纵坐标,以像素为单位
      image_color:图标颜色
*******************************************************************************************************************************/
void LCD_Show_Wiring_Diagram(unsigned char code,unsigned short x,unsigned short y,unsigned short image_color)
{
  unsigned short i,j,b_color,layer;
 b_color = LCD_Get_Pixel_Color(x,y);/*取得写入位置的背景颜色*/
#if COLOR_DEPTH == 8 //8位色深模式,需要彩色转换
 b_color = RGB332_RGB565(b_color);
#endif
 layer = LCD_Read_Register(0x41) & 0x01;/*读取41h MWCR1,读取读写图层信息*/
 if(layer == 0) y &= 0x7fff;//设置BTE目标写入位置时,写入图层1
 else           y |= 0x8000;//设置BTE目标写入位置时,写入图层2  
 LCD_Write_Register(0x58,x);           /*BTE目标写入位置*/
 LCD_Write_Register(0x59,x >> 8);
 LCD_Write_Register(0x5A,y);
 LCD_Write_Register(0x5B,y >> 8); 
 LCD_Write_Register(0x5C,0x60);        /*BTE区块宽度352*/
 LCD_Write_Register(0x5D,0x01); 
 LCD_Write_Register(0x5E,0x12);        /*BTE区块高度274*/
 LCD_Write_Register(0x5F,0x01); 
 LCD_Set_Background_Color(b_color);    /*设置背景色和前景色*/
 LCD_Set_Foreground_Color(image_color); 
 LCD_Write_Register(0x51,0x78);        /*写入BTE运算码，颜色扩充功能*/ 
 LCD_Write_Register(0x50,0x80);        /*开始BTE功能*/
 LCD_Write_Command(0x02);              /*02h MRWC,内存读写命令,进入数据读写模式*/ 
 for(i=0;i<274;i++)//行扫描
  {
   for(j=0;j<44;j++)//列扫描，共44 Colum
    {
     LCD_Write_Data(IMAGE_PT[code][i*44+j]);
     while(LCD_BUSY_STAT == 0);     
    }
  }
 LCD_Write_Register(0x50,0x00);//关闭BTE功能
}

/*******************************************************************************************************************************
48×24点阵字库显示子函数(使用BTE颜色扩充功能)
输入参数 code   :图标字库编号
         x      :横坐标,以像素为单位
         y      :纵坐标,以像素为单位
         f_color:文字颜色
         b_color:背景颜色
         reverse:反显标志,0 反显 其它值正常显示
*******************************************************************************************************************************/
void LCD_Put_Dot4824(unsigned short code,unsigned short x,unsigned short y,unsigned short f_color,unsigned short b_color,signed char reverse)
{
 unsigned short i,j,layer,color_tmp; 
 if(reverse == 0)/*需要反显*/
  {
   color_tmp = f_color;  //交换颜色
   f_color   = b_color;
   b_color   = color_tmp;
  }
 layer = LCD_Read_Register(0x41) & 0x01;/*读取41h MWCR1,读取读写图层信息*/
 if(layer == 0) y &= 0x7fff;//设置BTE目标写入位置时,写入图层1
 else           y |= 0x8000;//设置BTE目标写入位置时,写入图层2 
 LCD_Write_Register(0x58,x);       /*BTE目标写入位置*/
 LCD_Write_Register(0x59,x >> 8);
 LCD_Write_Register(0x5A,y);
 LCD_Write_Register(0x5B,y >> 8); 
 LCD_Write_Register(0x5C,0x30);    /*BTE区块宽度48*/
 LCD_Write_Register(0x5D,0x00); 
 LCD_Write_Register(0x5E,0x18);    /*BTE区块高度24*/
 LCD_Write_Register(0x5F,0x00); 
 LCD_Set_Background_Color(b_color);/*设置背景色和前景色*/
 LCD_Set_Foreground_Color(f_color); 
 LCD_Write_Register(0x51,0x78);    /*写入BTE运算码，颜色扩充功能*/ 
 LCD_Write_Register(0x50,0x80);    /*开始BTE功能*/
 LCD_Write_Command(0x02);          /*02h MRWC,内存读写命令,进入数据读写模式*/ 
 for(i=0;i<24;i++)//共24行
  {
   for(j=0;j<6;j++)//共6 Colum
    {
     LCD_Write_Data(DOT_ARRAY_4824[code][i*6+j]);
     while(LCD_BUSY_STAT == 0);     
    }
  }
 LCD_Write_Register(0x50,0x00);//关闭BTE功能
}

/*******************************************************************************************************************************
48*24点阵电池电量显示函数
输入参数 voltage:要显示的电池电压,单位:伏
         x      :横坐标,以像素为单位
         y      :纵坐标,以像素为单位
         f_color:前景颜色
         b_color:背景颜色
         reverse:反显标志,0 反显(前景色、背景色互换),其它值正常显示
*******************************************************************************************************************************/
void LCD_Battery_Level_4824(float voltage,unsigned short x,unsigned short y,
                            unsigned short f_color,unsigned short b_color,signed char reverse)
{
 unsigned char level = 0;//电池电量等级,共33个等级
 unsigned char i,j;
 unsigned short tmp;
 //参考电池资料容量与电压曲线0.5C
 if(voltage <= 3.45f * BAT_SERIES_QUANTITY)      level =  0;//电量小于等于10%    单节3.45V
 else if(voltage <= 3.54f * BAT_SERIES_QUANTITY) level =  1;//电量小于等于12.8%  单节3.54V
 else if(voltage <= 3.58f * BAT_SERIES_QUANTITY) level =  2;//电量小于等于15.6%  单节3.58V
 //3.58V~4.00V之间电压与容量为线性区对应容量范围15.6%~91%
 else if(voltage < 4.026f * BAT_SERIES_QUANTITY) 
  {
   level = (voltage - 3.58f * BAT_SERIES_QUANTITY) / (0.0159285f * BAT_SERIES_QUANTITY) + 3;
  }
 else if(voltage <= 4.08f * BAT_SERIES_QUANTITY) level = 31;//电量小于等于95.5%  单节4.08V
 else level = 32;//电量小于等于100%
 //判断是否需要反显
 if(reverse == 0)//反显
  {
   tmp     = f_color;//反显，交换前景色和背景色
   f_color = b_color;
   b_color = tmp;
  }
 if(level == 0 && SLOW_SHAN_SIG == 0) f_color = b_color;//电池电量小于等于10%，电池图标开始闪烁
 //开始绘制电池外框
 LCD_Draw_Rectangle(x+5,  y,x+47,y+23,f_color,NO);
 LCD_Draw_Rectangle(x+6,y+1,x+46,y+22,f_color,NO);
 LCD_Draw_Rectangle(x+7,y+2,x+45,y+21,f_color,NO);
 LCD_Draw_Line(x+1,y+6,x+1,y+18,f_color);
 LCD_Draw_Line(x+2,y+6,x+2,y+18,f_color);
 LCD_Draw_Line(x+3,y+6,x+3,y+18,f_color);
 LCD_Draw_Line(x+4,y+6,x+4,y+18,f_color);
 //开始绘制电池电量
 j = 0;
 for(i=0;i<32;i++)
  {
   if((i + 1) <= level) LCD_Draw_Line(x+43-j,y+4,x+43-j,y+19,f_color);//有电量部分
   else LCD_Draw_Line(x+43-j,y+4,x+43-j,y+19,b_color);//无电量部分
   j++;
   if(i == 7 || i == 15 || i == 23 || i == 31) 
    {
     LCD_Draw_Line(x+43-j,y+4,x+43-j,y+19,b_color);//绘制四档电量之间的间隔竖线
     j++;
    }
  }
}

/*******************************************************************************************************************************
进度条显示函数
总长度408点，内部进度条长度400点，总高48点，内部进度条高度40点
输出参数 Percentage  进度条显示百分比
         x           进度条显示起始x坐标
         y           进度条显示起始y坐标
         data_color  进度条颜色
         line_color  进度条外框颜色
         back_color  进度条背景色
*******************************************************************************************************************************/
void LCD_Progress_Bar(float Percentage,unsigned short x,unsigned short y,
                      unsigned short data_color,unsigned short line_color,unsigned short back_color)
{
 unsigned short i,data_line_number;
 if(Percentage > 1) Percentage = 1;
 LCD_Draw_Rectangle(x  ,y  ,x+407,y+47,line_color,NO);//绘制外层矩形框，不填充
 LCD_Draw_Rectangle(x+1,y+1,x+406,y+46,line_color,NO);//绘制内层矩形框，不填充
 LCD_Draw_Rectangle(x+2,y+2,x+405,y+45,line_color,NO);//绘制外层矩形框，不填充
 LCD_Draw_Rectangle(x+3,y+3,x+404,y+44,line_color,NO);//绘制内层矩形框，不填充
 data_line_number = 400 * Percentage;//计算需要绘制的数据内容长度
 for(i=0;i<400;i++)
  {
   if(i < data_line_number) LCD_Draw_Line(x+4+i,y+4,x+4+i,y+43,data_color);
   else LCD_Draw_Line(x+4+i,y+4,x+4+i,y+43,back_color);
  }
}

/*******************************************************************************************************************************
液晶屏GPOX引脚控制子函数
输入参数 status:0,低电平；非0,高电平
*******************************************************************************************************************************/
void LCD_GPOX_Set(unsigned char status)
{
 if(status) LCD_Write_Register(0xC7,0x01);//GPOX引脚输出高电平
 else LCD_Write_Register(0xC7,0x00);      //GPOX引脚输出低电平
}

/*******************************************************************************************************************************
液晶屏GPIX引脚状态读取子函数
返回参数:0x00,低电平;0x01,高电平
*******************************************************************************************************************************/
unsigned char LCD_GPIX_Get(void)
{
 return LCD_Read_Register(0xC7);//读取GPIX引脚状态
}

/*******************************************************************************************************************************
液晶屏PWM2输出控制子函数
输入参数 level:等级,0~255
*******************************************************************************************************************************/
void LCD_Set_PWM2(unsigned char level)
{
 if(level)
  {
   /*PWM2输出参数设置用于控制LED2亮度*/
   LCD_Write_Register(0x8C,0x8A);//8Ch P2CR, PWM2控制寄存器；Bit[7]   = 1(开启PWM2) 
                                                           //Bit[6]   = 0(PWM1停止时输出低电平) 
                                                           //Bit[4]   = 0(PWM模式) 
                                                           //Bit[3:0] = 1010b(SYS_CLK / 1024) = 60MHz / 1024 / 256 = 228.882Hz
   LCD_Write_Register(0x8D,level);//8Dh P2DCR,PWM2占空比设置寄存器；0x00~0xff(1~256),用于调整LED2亮度
  }
 else
  {
   LCD_Write_Register(0x8C,0x0A);//8Ch P2CR, PWM2控制寄存器；Bit[7]   = 0(关闭PWM2) 
                                                           //Bit[6]   = 0(PWM1停止时输出低电平) 
                                                           //Bit[4]   = 0(PWM模式) 
                                                           //Bit[3:0] = 1010b(SYS_CLK / 1024) = 60MHz / 1024 / 256 = 228.882Hz
  } 
}

/*******************************************************************************************************************************
液晶屏初始化子函数
液晶屏使用5.6寸640*480点阵TFT液晶屏,液晶屏信号为群创AT056TN53-V.1
*******************************************************************************************************************************/
void LCD_Init(void)
{
 SPI1_Configuration(SPI_LOW_SPEED);//配置STM32与RA8875的SPI接口,低速模式
 LCD_RST_L; Delay_ms(10);          //延时10ms，等待电源稳定,并产生复位脉冲
 LCD_RST_H; Delay_ms(10);          //延时10ms，等待内部复位序列执行完成
 /*-------------------------------------------------------------------------------------*/
 /*设置RA8875 PLL锁相环*/
 LCD_Write_Register(0x88,0x0c);//88h PLLC1,PLL控制寄存器1;PLLDIVM[7] = 0,PLLDIVN[4:0] = 12
 Delay_ms(1);                  //等待频率稳定
 LCD_Write_Register(0x89,0x02);//89h PLLC2,PLL控制寄存器2;PLLDIVK[2:0] = 2,除以4
 Delay_ms(1);                  //等待频率稳定 
 /*
 RA8875的内部系统频率(SYS_CLK)是结合振荡电路及PLL电路所产生,频率计算公式如下:
 SYS_CLK = FIN * (PLLDIVN[4:0] +1) / ((PLLDIVM+1) * (2^PLLDIVK [2:0]))
         = 20M * (12 + 1) / ((0 + 1) * (2 ^ 2))
	     = 65MHz
 RA8875内部系统频率设置为60MHz时，Stm32使用9MHz的SPI速率时，数据出错，这里超频5MHz可以保证9MHz的SPI口操作正确，且超频限度很低
 */
 SPI1_Configuration(SPI_HIGH_SPEED);//PLL工作后,SPI进入高速模式
 /*-------------------------------------------------------------------------------------*/
 /*设置色彩深度及MCU接口*/
#if COLOR_DEPTH == 8 //8位彩色模式
 LCD_Write_Register(0x10,0x00);//10h SYSR,系统配置寄存器；bit[4:3]=00b 256 color,bit[2:1]=00b 8位MCU接口
#elif COLOR_DEPTH == 16 //16位彩色模式
 LCD_Write_Register(0x10,0x0A);//10h SYSR,系统配置寄存器；bit[4:3]=11b 65K color,bit[2:1]=11b 16位MCU接口
#endif
 Delay(40);                    //等待1us
 /*设置像素时钟*/
 LCD_Write_Register(0x04,0x01);//04h PSCR,像素时钟设计寄存器；Bit[7] = 0(PCLK上升沿有效)
                                                            //Bit[1:0] = 1,PCLK = system clock/2 = 60/2 = 30 MHz
 Delay_ms(1);
 /*水平显示参数设置*/
 LCD_Write_Register(0x14,0x4F);//14h HDWR  ,水平显示宽度寄存器；Bit[6:0] = 79,水平像素宽度(pixels) = (HDWR + 1)*8 = (79+1)*8 = 640
 LCD_Write_Register(0x15,0x05);//15h HNDFTR,水平非显示周期微调选项寄存器；Bit[7] = 0(DE信号高电平有效),Bit[3:0] = 5(液晶面板水平非显示微调宽度为10个像素)
 LCD_Write_Register(0x16,0x0f);//16h HNDR  ,水平非显示周期宽度设置寄存器；Bit[4:0] = 15,水平非显示周期(pixels) = (HNDR + 1)*8 = 128 
 LCD_Write_Register(0x17,0x01);//17h HSTR  ,HSYNC起始位置寄存器；Bit[4:0] = 1,HSYNC起始位置(PCLK) = (HSTR + 1)*8 = 16
 LCD_Write_Register(0x18,0x00);//18h HPWR  ,HSYNC极性宽度寄存器；Bit[7] = 0(HSYNC低电平有效),Bit[4:0] = 0(HSYNC水平同步信号脉冲宽度)HSYNC Pulse width(PCLK) = (HPWR + 1)*8 = 8
 /*垂直显示参数设置*/
 LCD_Write_Register(0x19,0xdf);//19h VDHR0,垂直显示高度寄存器0； 
 LCD_Write_Register(0x1A,0x01);//1Ah VDHR1,垂直显示高度寄存器1；垂直像素 = VDHR + 1 = 479 + 1 = 480
 LCD_Write_Register(0x1B,0x0A);//1Bh VNDR0,垂直非显示周期寄存器0； 
 LCD_Write_Register(0x1C,0x00);//1Ch VNDR1,垂直非显示周期寄存器1；垂直非显示区域 = (VNDR + 1) = 10 + 1 = 11
 LCD_Write_Register(0x1D,0x0E);//1Dh VSTR0,VSYNC起始位置寄存器0，
 LCD_Write_Register(0x1E,0x00);//1Eh VSTR1,VSYNC起始位置寄存器1，VSYNC起始位置(PCLK) = (VSTR + 1) = 14 + 1 = 15
 LCD_Write_Register(0x1F,0x01);//1Fh VPWR ,VSYNC极性宽度寄存器；Bit[7] = 0(VSYNC低电平有效),Bit[6:0] = 1(VSYNC脉冲宽度(PCLK) = (VPWR + 1) = 2)
  /*设置液晶屏显示模式*/
#if COLOR_DEPTH == 8 //8位彩色模式
 LCD_Write_Register(0x20,0x80);//20h DPCR,显示设置寄存器DPCR；Bit[7] = 1(双图层模式)
                                                            //Bit[3] = 0(水平扫描方向由SEG0 到 SEG(n-1))
                                                            //Bit[2] = 0(垂直扫描方向由COM0 到 COM(n-1))
 LCD_Write_Register(0x52,0x00);//52h LTPR0,图层透明度设置寄存器；Bit[7:6] = 00b(图层0与图层1同时卷动)
                                                               //Bit[5]   = 0(用BGTR设定浮动窗口不通透显示)
                                                               //Bit[2:0] = 000b(只显示图层1)
#elif COLOR_DEPTH == 16 //16位彩色模式
 LCD_Write_Register(0x20,0x00);//20h DPCR,显示设置寄存器DPCR；Bit[7] = 0(单图层模式)
                                                            //Bit[3] = 0(水平扫描方向由SEG0 到 SEG(n-1))
                                                            //Bit[2] = 0(垂直扫描方向由COM0 到 COM(n-1))
#endif
 LCD_Layer_Select(LAYER_1);                     //设置工作图层
 LCD_Active_Window(0,0,X_PIXEL_MAX,Y_PIXEL_MAX);//设置工作窗口为全屏
 LCD_Mode_Select(GRAPHIC_MODE);                 //进入图形模式
 /*PWM1输出参数设置用于控制液晶屏背光*/
 LCD_Write_Register(0x8A,0x8A);  //8Ah P1CR, PWM1控制寄存器；Bit[7]   = 1(开启PWM1) 
                                                           //Bit[6]   = 0(PWM1停止时输出低电平) 
                                                           //Bit[4]   = 0(PWM模式) 
                                                           //Bit[3:0] = 1010b(SYS_CLK / 1024) = 60MHz / 1024 / 256 = 228.882Hz
 LCD_BRIGHTNESS = HIGH_BRIGHTNESS;
 LCD_Set_Bright(LCD_BRIGHTNESS);//设置液晶屏背光亮度为最高亮度
 
 /*开显示*/
 LCD_FullScreen_Fill(BLACK);//图层1填充黑色，同时改变背景色设置
 LCD_Layer_Select(LAYER_2); //设置工作图层为图层2
 LCD_FullScreen_Fill(BLACK);//图层2填充黑色，同时改变背景色设置
 LCD_Layer_Select(LAYER_1); //设置工作图层为图层1
 LCD_Write_Register(0x01,0x80);//01h PWRR,Bit[7] = 1(开显示)
 
 NUM_OF_CHAR3264 = Character3264_Number();//计算32*64点阵字符库中的字符个数
 NUM_OF_HZDOT48  = HZDot48_Number();      //计算48点阵汉字库中的汉字个数
}

