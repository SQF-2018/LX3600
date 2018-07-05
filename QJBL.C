#include "IOCONFIG.H"
#include "MACRO_DEFINITION.H"
#include "STRUCTURE_DEFINITION.H"
#include "math.h"

//没有赋初值的变量由于系统RAM上电初始化,所以初始值都为0
/*************************************************************************************************/
/*************************************************************************************************/
//存储器存储变量声明
/*************************************************************************************************/
unsigned char  DEVICE_NUMBER[4];  //装置编号
unsigned char  TEST_RECORD_NUMBER;//最新测量记录编号
unsigned char  TEST_RECORD_AMOUNT;//已经保存的测量记录的总个数

/*************************************************************************************************/
//常用通用程序部分声明
/*************************************************************************************************/
BCD_TIME NOW_TIME;                    //时间结构体
unsigned char  RELAY_DETECT_EN;       //继电器测试使能标志
unsigned char  RELAY_DETECT;          //继电器测试控制寄存器
unsigned char  RELAY_CTRL;            //继电器控制寄存器
unsigned char  HARDWARE_EEOR_FLAG;    //硬件错误标志
float          RTC_CORRECT_COE;       //RTC时钟校正系数
unsigned char  POWERON_TIME_COUNT;    //开机上电时间计时
unsigned char  U_DISK_CONNECT;        //U盘插入标志
unsigned char  U_DISK_MOUNT;          //U盘初始化标志
unsigned char  U_DISK_MOUNT_TIME;     //U盘初始化延时计时
unsigned char  PRINTING_FLAG;         //当前正在打印标志
unsigned char  OPERATION_PROCESS;     //操作过程标志
unsigned char  RECORD_STORAGED_INTER; //测量结果记录已保存至本机标志
unsigned char  BLUETOOTH_MODLE_INIT;  //蓝牙模块初始化标志
unsigned char  BLUETOOTH_CONNECT;     //蓝牙连接标志
unsigned short HARDWARE_WORK_STATE;   //硬件工作状态
unsigned char  WRITE_DEVICE_NUMBER_EN;//允许写装置编号标志,ON:允许;OFF:禁止
unsigned char  OPERATE_STATE;         //操作状态标志,主要用于系统参数设置等屏幕


/*******************************************************************************************************************************
ADC采样部分变量
*******************************************************************************************************************************/
unsigned char  AD7606_DONE;                      //外置AD7606一周波采样完成标志
unsigned short AD7606_POINT;                     //外置AD7606采样点数计数器


/*******************************************************************************************************************************
测试结果及测试参数设置变量
*******************************************************************************************************************************/
TEST_DATA_STRUCT TEST_DATA;//测试结果及测试参数设置结构体

/*******************************************************************************************************************************
校准系数结构体
*******************************************************************************************************************************/
COE_DATA_STRUCT COE_DATA;//校准系数结构体

/*******************************************************************************************************************************
液晶显示部分变量
*******************************************************************************************************************************/
unsigned short LCD_FORE_COLOR;//定义显示的前景色，便于统一管理
unsigned short LCD_BACK_COLOR;//定义显示的背景色，便于统一管理
unsigned char  LCD_BRIGHTNESS;//液晶屏背光亮度值
unsigned char  SCRFL;         //屏幕编号
unsigned char  POPUP_SCRFL;   //弹出屏幕编号
unsigned char  BURST_SCRFL;   //突发弹出屏幕编号
unsigned char  SCRAPPLY;      //允许全屏刷新标志
unsigned char  DUPDATE;       //允许数据刷新标志
unsigned char  CDX1;          //第一级菜单光标位置
unsigned char  CDX2;          //第二级菜单光标位置 
unsigned char  CDX3;          //第三级菜单光标位置
unsigned char  CDX4;          //第四级菜单光标位置
unsigned char  POPUP_CDX;     //弹出菜单光标位置
unsigned char  DISPLAY_PT_WIRING_DIAGRAM;//显示PT接线图使能控制标志
unsigned char  PARAMETER_DISPLAY_EN;     //测量过程详细参数显示控制标志

/*-----------------------------------------------------------------------------------------------*/
//按键部分处理变量
/*-----------------------------------------------------------------------------------------------*/
unsigned char  KEY1;            //第一次按键采样值
unsigned char  KEY2;            //第二次按键采样值
unsigned char  YESKEY;          //确定的按键键值
unsigned short KEYTIME;         //一个按键持续按下的时间
unsigned short DKEYTIME;        //无按键按下持续时间
//各个屏幕按键处理临时变量
unsigned char  KEY_CHAR_TMP[25];//定义屏幕按键处理用char型临时变量
unsigned short KEY_SHORT_TMP[5];//定义屏幕按键处理用short型临时变量

/*************************************************************************************************/
//测量过程控制部分变量声明
/*************************************************************************************************/
unsigned char  TEST_PROCESS_EN;  //使能禁止测量过程标志
unsigned char  TEST_PROCESS_FLAG;//测量过程控制标志
unsigned short TEST_PROCESS_TIME;//测量过程中每个阶段延时计时，单位：10ms

/*************************************************************************************************/
//蓝牙转串口通讯（UART4）部分变量
/*************************************************************************************************/
unsigned char BLUETOOTH_RXBUF[50]; //异步串行通讯接收缓冲区
unsigned char BLUETOOTH_TXBUF[70]; //异步串行通讯发送缓冲区
unsigned char BLUETOOTH_RXNO;      //通讯接收到的字节数目
unsigned char BLUETOOTH_TXNO;      //通讯发送数据指针
unsigned char BLUETOOTH_TXEND;     //通讯要发送的数据总个数
unsigned char BLUETOOTH_RXTIME;    //通讯接收到最后一个字节后的静止时间
unsigned char BLUETOOTH_RXTIME_EN; //允许通讯接收静止时间计时标志
unsigned char BLUETOOTH_RXTIME_END;//通讯接收静止时间到时标志

/*************************************************************************************************/
//定时用变量
/*************************************************************************************************/
unsigned char HALF_SECOND_SIG;    //半秒钟定时器到时标志
unsigned char HUNDRED_MSECOND_SIG;//一百毫秒定时器到时标志
unsigned char KAIJI_TIME;         //开机界面显示计时器
unsigned char SLOW_SHAN_SIG;      //慢闪烁标志，定时半秒钟
unsigned char FAST_SHAN_SIG;      //快闪烁标志，定时100ms

/*************************************************************************************************/
//软件延时函数
//50  延时4.52us
//100 延时8.68us
//200 延时17us
//500 延时42us
//1000延时83.8us
/*************************************************************************************************/
void Delay(unsigned int count)
{
 while(count) count--;
}
/*************************************************************************************************/
//毫秒软件延时函数
/*************************************************************************************************/
void Delay_ms(unsigned int ms)
{
 while(ms --) Delay(12000);
}
/*************************************************************************************************/
// Name: hex_to_bcd
// Description: 十六进制数转换成BDC码格式
// Calls: None
// Input: 需要转换的十六进制数
// Outputs: 转换后的BCD码
/*************************************************************************************************/
unsigned short Hex_To_Bcd(unsigned short data)
{
 unsigned short temp,temp0,temp10,temp100,temp1000;
 temp1000 = data / 1000;
 temp100  = (data % 1000) / 100;
 temp10   = ((data % 1000) % 100) / 10;
 temp0    = ((data % 1000) % 100) % 10;
 temp     =  (temp1000 << 12) | (temp100 << 8) | (temp10 << 4) | temp0;
 return(temp);
}
/*************************************************************************************************/
//Name: bcd_to_hex
//Description: BCD码格式转换成十六进制数
//Calls: None
//Input: 需要转换的BCD码
//Outputs: 转换后的十六进制数
/*************************************************************************************************/
unsigned short Bcd_To_Hex(unsigned short data)
{
 unsigned short temp,temp0,temp10,temp100,temp1000;
 temp1000 = (data & 0xf000) >> 12;
 temp100  = (data & 0x0f00) >> 8;
 temp10   = (data & 0x00f0)>>4;
 temp0    = data & 0x000f;
 temp     = temp1000*1000 + temp100*100 + temp10*10 + temp0;
 return(temp);
}

/*************************************************************************************************/
#define Set_Bit(val, bitn)    (val |=(1<<(bitn)))
#define Clr_Bit(val, bitn)    (val&=~(1<<(bitn)))
#define Get_Bit(val, bitn)    (val &(1<<(bitn)) )

/* CRC 高位字节值表 */ 
unsigned char const auchCRCHi[] = { 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 
} ; 
/* CRC低位字节值表*/ 
unsigned char const auchCRCLo[] = { 
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 
0x43, 0x83, 0x41, 0x81, 0x80, 0x40 
} ;
/*************************************************************************************************/
//计算CRC16程序
/*************************************************************************************************/
unsigned short int CRC16(unsigned char *puchMsg, unsigned short int usDataLen) 
{ 
 unsigned char uchCRCHi = 0xFF ; /* 高CRC字节初始化 */ 
 unsigned char uchCRCLo = 0xFF ; /* 低CRC 字节初始化 */ 
 unsigned int  uIndex ;          /* CRC循环中的索引 */ 
 while (usDataLen--)             /* 传输消息缓冲区 */ 
  { 
   uIndex = uchCRCHi ^ *puchMsg++ ; /* 计算CRC */ 
   uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ; 
   uchCRCLo = auchCRCLo[uIndex] ; 
  } 
 return (uchCRCHi << 8 | uchCRCLo); 
}

/*************************************************************************************************/
//报警蜂鸣器控制处理程序
/*************************************************************************************************/
void Alarm_BEEP(void)
{
 static unsigned short beep_time=0;
 switch(beep_time)
  {
   case  0:BELL_ON; //蜂鸣器响
           break;
   case  7:BELL_OFF;//蜂鸣器不响
           break;
   case 20:BELL_ON; //蜂鸣器响
           break;
   case 27:BELL_OFF;//蜂鸣器不响
           break;
   default:break;                                             
  }
 if(beep_time < 150) beep_time += 1;
 else 
  {
   beep_time = 0; 
  }
}

/*************************************************************************************************/
//继电器控制程序
/*************************************************************************************************/
void Relay_Control(void)
{
 if(RELAY_DETECT_EN == ON)//进行继电器检测
  {

  }
}

/*******************************************************************************************************************************
整形数据N次方计算函数(源自SGI STL算法库)
*******************************************************************************************************************************/
int Int_Power(int x, unsigned int n)
{
 unsigned int result;
 if(n == 0)
  {
   return 1;
  }
 else
  {
   while((n & 1) == 0)
    {
     n >>= 1;
     x *= x;
    }
  }
 result = x;
 n >>= 1;
 while(n != 0)
  {
   x *= x;
   if ((n & 1) != 0)
    result *= x;
    n >>= 1;
  }
 return result;
}

/*******************************************************************************************************************************
浮点数转换成字符型数组函数 
参数  data_f   :要转换的浮点数
      *data_i  :接收转换数据的字符型数组
      symbol_en:是否显示符号位,0:不显示,非0:显示符号位
      length   :要转换的长度,包括小数和符号位在内的长度
*******************************************************************************************************************************/
void Float_To_Char(float data_f,unsigned char *data_i,unsigned char symbol_en,unsigned char length)
{
 unsigned char px[10],dot,i,j,start_num;
 unsigned int  tmp;
 if(data_f >= 0) 
  {
   px[9] = '+';//正号   
  }
 else 
  {
   px[9]  = '-';//负号
   data_f = 0 - data_f;
  }
 //此步运算是为了保证7位有效数字全为整数
 if(data_f < 1)            
  {
   data_f *= 10000000;
   dot     = 7;//7位小数
  }
 else if(data_f < 10)      
  {
   data_f *= 1000000;
   dot     = 6;//6位小数
  }
 else if(data_f < 100)     
  {
   data_f *= 100000;
   dot     = 5;//5位小数
  }
 else if(data_f < 1000)    
  {
   data_f *= 10000;
   dot     = 4;//4位小数
  }
 else if(data_f < 10000)   
  {
   data_f *= 1000;
   dot     = 3;//3位小数
  }
 else if(data_f < 100000)  
  {
   data_f *= 100;
   dot     = 2;//2位小数
  }
 else if(data_f < 1000000)
  {
   data_f *= 10;
   dot     = 1;//1位小数
  }
 else dot = 0;//0位小数
 tmp = (unsigned int)data_f;
 //进行每一位数的数据提取
 if(dot == 0)//小数位为0
  {
   px[8]     = px[9];//将符号复制到此位,标志数据串到此结束
   px[7]     = px[9];//将符号复制到此位,标志数据串到此结束
   start_num = 7;    //从数组编号7位置开始显示
   for(i=6;i>0;i--)
    {
     px[i]  = tmp / Int_Power(10,i) + '0';//取值,并转换为ASCII编码
     tmp   %= Int_Power(10,i);
     if(i == 1) px[0] = tmp + '0';
    }
  }
 else //有小数位需要显示
  {
   j = 6;
   if(dot == 7) 
    {
     px[8]     = '0';//7位小数,则小数点前自动填0
     //length   += 1;   //长度自动加1,以显示小数点前多出来的数字0
     start_num = 9;   //从数组编号9位置开始显示
    } 
   else 
    {
     px[8]     = px[9];//将符号复制到此位,标志数据串到此结束
     start_num = 8;    //从数组编号8位置开始显示
    } 
   //length += 1; //长度自动加1,以显示小数点
   for(i=7;i>0;i--)
    {
     if(i == dot)//此位置是小数点的位置
      {
       px[i] = '.';//小数点,"."符号
      }
     else
      {
       px[i]  = tmp / Int_Power(10,j) + '0';//取值,并转换为ASCII编码
       tmp   %= Int_Power(10,j--);
      }
     if(i == 1) px[0] = tmp + '0';
    } 
  }
 //开始数据串的赋值 
 if(symbol_en != 0) 
  {
   //length += 1;//需要显示符号位,数据长度自动加1,用于显示符号位
  } 
 else
  {
   start_num -= 1;//去除符号位
  }
 for(i=0;i<length;i++)
  {
   *data_i++ = px[start_num-i];
  }  
}

/*******************************************************************************************************************************
浮点数转换成固定小数长度字符型数组函数 
参数  data_f   :要转换的浮点数
      *data_i  :接收转换数据的字符型数组
      symbol_en:是否显示符号位,0:不显示,非0:显示符号位
      length   :要转换的长度,包括小数和符号位在内的长度,最大长度10位(小数点1位,符号位1位,有效数字7位,小于0的数小数点前的0占一位)
      dot      :需要的小数长度
*******************************************************************************************************************************/
void Float_To_Fixed_Char(float data_f,unsigned char *data_i,unsigned char symbol_en,unsigned char length,unsigned char dot)
{
 unsigned char px[10],i,j;
 unsigned int  tmp;
 if(length > 10 || dot >= length) return;
 if(data_f >= 0) 
  {
   px[9] = ' ';//正号不显示   
  }
 else 
  {
   px[9]  = '-';//负号
   data_f = 0 - data_f;
  }
 data_f += powf(10,-dot-3);//此步运算是为了消除单精度浮点数的表示误差;例如单精度浮点数0.01,实际数值为0.009999997,需要显示2位小数,则在数值上加0.00001
 data_f *= Int_Power(10,dot);
 tmp = (unsigned int)data_f; 
 //进行每一位数的数据提取
 if(dot == 0)//小数位为0
  {
   px[8] = px[9];//将符号复制到此位,标志数据串到此结束
   for(i=7;i>0;i--)
    {
     px[i]  = tmp / Int_Power(10,i) + '0';//取值,并转换为ASCII编码
     tmp  %= Int_Power(10,i);
     if(i == 1) px[0] = tmp + '0';//取值,并转换为ASCII编码
    }
  }
 else //有小数位需要显示
  {
   //length += 1;//长度自动加1,用于显示小数点
   j = 7;
   for(i=8;i>0;i--)
    {
     if(i == dot)//此位置是小数点的位置
      {
       px[i] = '.';//小数点,"."符号
      }
     else
      {
       px[i]  = tmp / Int_Power(10,j) + '0';//取值,并转换为ASCII编码
       tmp  %= Int_Power(10,j--);
      }
     if(i == 1) px[0] = tmp + '0';//取值,并转换为ASCII编码
    } 
  }
 //数据串整理,去掉不需要显示的数字0
 if(dot == 0)//小数位为0
  {
   for(i=7;i>0;i--)//从第10位开始判断,最后一位不判断
    {
     if(px[i] == '0')//如果此位是0,则将符号复制到此位
      {
       px[i] = px[9];
      }
     else break;//结束循环,数据串整理完毕
    }
  }
 else //有小数位需要显示
  {
   for(i=8;i>2;i--)//从第11位开始判断,第3位以后不判断
    {
     if(px[i] == '0' && px[i-1] != '.')//如果此位是0且下一位不是小数点符号,则将符号复制到此位
      {
       px[i] = px[9];
      }
     else break;//结束循环,数据串整理完毕
    }
  }
 for(i=9;i>0;i--)//无用数据位清0
  {
   if(symbol_en != 0)//需要显示符号位
    {
     if(px[i-1] == '+' || px[i-1] == '-') 
      {
       px[i] = ' ';//当前数据位的下一位是符号位,则清除此位数据,数据串前端无需显示的数据显示"空"符号
      }
     else break;//结束循环
    }
   else //不需要显示符号位
    {
     if(px[i] == '+' || px[i] == '-') 
      {
       px[i] = ' ';//当前数据位是符号位,则清除此位数据,数据串前端无需显示的数据显示"空"符号
      }
     else break;//结束循环
    }          
  }  
 //开始数据串的赋值 
 for(i=0;i<length;i++)
  {
   *data_i++ = px[i];
  }  
}  

/*******************************************************************************************************************************
字符型数组转换成浮点数函数 
参数：data      要转换的字符串指针
      length    要转换的字符个数
例：data[6]={'1','2','3','.','4','5'},转换完成后=123.45
*******************************************************************************************************************************/
float Char_To_Float(unsigned char *data,unsigned char length)
{
 unsigned char i,dot=0;
 float result = 0;
 if(length > 8) return 0; //浮点数加上小数点最多8位(不处理正负号)
 for(i=0;i<length;i++)//扫描小数点位置并检查错误
  {
   if((data[i] >= '0' && data[i] <= '9') || data[i] == '.')
    {
     if(data[i] == '.' && dot == 0) //发现小数点,之前没有小数点
      {
       dot = i; //记录小数点位置
      }
     else if(data[i] == '.' && dot != 0) return 0; //发现了多个小数点 
    }
   else return 0; //数组中存在非法数据值,则直接返回
  }
 if(dot == 0) dot = length;//没有发现小数点,数据为整数 
 for(i=0;i<dot;i++)
  { 
   result += powf(10,(dot-i-1))*(data[i]-'0');//计算小数点前实际值
  }
 for(i=dot+1;i<length;i++)
  {
   result += powf(10,(dot-i))*(data[i]-'0');  //计算小数点后实际值
  }
 return result;
}
