#ifndef __QJBL_H
#define __QJBL_H

#include "MACRO_DEFINITION.H"
#include "STRUCTURE_DEFINITION.H"

/*************************************************************************************************/
//存储器存储变量声明
/*************************************************************************************************/
extern unsigned char  DEVICE_NUMBER[4];  //装置编号
extern unsigned char  TEST_RECORD_NUMBER;//最新测量记录编号
extern unsigned char  TEST_RECORD_AMOUNT;//已经保存的测量记录的总个数

/*************************************************************************************************/
//常用通用程序部分声明
/*************************************************************************************************/
extern BCD_TIME NOW_TIME;                    //时间结果体
extern unsigned char  RELAY_DETECT_EN;       //继电器测试使能标志
extern unsigned char  RELAY_DETECT;          //继电器测试控制寄存器
extern unsigned char  RELAY_CTRL;            //继电器控制寄存器
extern unsigned char  HARDWARE_EEOR_FLAG;    //硬件错误标志
extern float          RTC_CORRECT_COE;       //RTC时钟校正系数
extern unsigned char  POWERON_TIME_COUNT;    //开机上电时间计时
extern unsigned char  U_DISK_CONNECT;        //U盘插入标志
extern unsigned char  U_DISK_MOUNT;          //U盘初始化标志
extern unsigned char  U_DISK_MOUNT_TIME;     //U盘初始化延时计时
extern unsigned char  PRINTING_FLAG;         //当前正在打印标志
extern unsigned char  OPERATION_PROCESS;     //操作过程标志
extern unsigned char  RECORD_STORAGED_INTER; //测量结果记录已保存至本机标志
extern unsigned char  BLUETOOTH_MODLE_INIT;  //蓝牙模块初始化标志
extern unsigned char  BLUETOOTH_CONNECT;     //蓝牙连接标志
extern unsigned short HARDWARE_WORK_STATE;   //硬件工作状态
extern unsigned char  WRITE_DEVICE_NUMBER_EN;//允许写装置编号标志,ON:允许;OFF:禁止
extern unsigned char  OPERATE_STATE;         //操作状态标志,主要用于系统参数设置等屏幕

/*******************************************************************************************************************************
ADC采样部分变量
*******************************************************************************************************************************/
extern unsigned char  AD7606_DONE;                      //外置AD7606一周波采样完成标志
extern unsigned short AD7606_POINT;                     //外置AD7606采样点数计数器

/*******************************************************************************************************************************
测试结果及测试参数设置变量
*******************************************************************************************************************************/
extern TEST_DATA_STRUCT TEST_DATA;//测试结果及测试参数设置结构体

/*******************************************************************************************************************************
校准系数结构体
*******************************************************************************************************************************/
extern COE_DATA_STRUCT COE_DATA;//校准系数结构体

/*******************************************************************************************************************************
液晶显示部分变量
*******************************************************************************************************************************/
extern unsigned short LCD_FORE_COLOR;//定义显示的前景色，便于统一管理
extern unsigned short LCD_BACK_COLOR;//定义显示的背景色，便于统一管理
extern unsigned char  LCD_BRIGHTNESS;//液晶屏背光亮度值
extern unsigned char  SCRFL;         //屏幕编号
extern unsigned char  POPUP_SCRFL;   //弹出屏幕编号
extern unsigned char  BURST_SCRFL;   //突发弹出屏幕编号
extern unsigned char  SCRAPPLY;      //允许全屏刷新标志
extern unsigned char  DUPDATE;       //允许数据刷新标志
extern unsigned char  CDX1;          //第一级菜单光标位置
extern unsigned char  CDX2;          //第二级菜单光标位置 
extern unsigned char  CDX3;          //第三级菜单光标位置
extern unsigned char  CDX4;          //第四级菜单光标位置
extern unsigned char  POPUP_CDX;     //弹出菜单光标位置
extern unsigned char  DISPLAY_PT_WIRING_DIAGRAM;//显示PT接线图使能控制标志
extern unsigned char  PARAMETER_DISPLAY_EN;     //测量过程详细参数显示控制标志

/*-----------------------------------------------------------------------------------------------*/
//按键部分处理变量
/*-----------------------------------------------------------------------------------------------*/
extern unsigned char  KEY1;            //第一次按键采样值
extern unsigned char  KEY2;            //第二次按键采样值
extern unsigned char  YESKEY;          //确定的按键键值
extern unsigned short KEYTIME;         //一个按键持续按下的时间
extern unsigned short DKEYTIME;        //无按键按下持续时间
//各个屏幕按键处理临时变量
extern unsigned char  KEY_CHAR_TMP[25];//定义屏幕按键处理用char型临时变量
extern unsigned short KEY_SHORT_TMP[5];//定义屏幕按键处理用short型临时变量
    
#define YEAR_KEY               KEY_CHAR_TMP[0]//实时时钟设置屏幕按键临时变量
#define MONTH_KEY              KEY_CHAR_TMP[1]
#define DATE_KEY               KEY_CHAR_TMP[2]
#define WEEKDAY_KEY            KEY_CHAR_TMP[3]
#define HOUR_KEY               KEY_CHAR_TMP[4]
#define MINUTE_KEY             KEY_CHAR_TMP[5]
#define SECOND_KEY             KEY_CHAR_TMP[6]
#define TEST_RECORD_NUMBER_KEY KEY_CHAR_TMP[0]//读取测量记录编号按键临时变量
#define MIMA0                  KEY_CHAR_TMP[0]//密码输入宏替换
#define MIMA1                  KEY_CHAR_TMP[1]
#define MIMA2                  KEY_CHAR_TMP[2]
#define MIMA3                  KEY_CHAR_TMP[3]
#define MIMA4                  KEY_CHAR_TMP[4]
#define MIMA5                  KEY_CHAR_TMP[5]


/*************************************************************************************************/
//测量过程控制部分变量声明
/*************************************************************************************************/
extern unsigned char  TEST_PROCESS_EN;  //使能禁止测量过程标志
extern unsigned char  TEST_PROCESS_FLAG;//测量过程控制标志
extern unsigned short TEST_PROCESS_TIME;//测量过程中每个阶段延时计时，单位：10ms

/*************************************************************************************************/
//蓝牙转串口通讯（UART4）部分变量
/*************************************************************************************************/
extern unsigned char BLUETOOTH_RXBUF[50]; //异步串行通讯接收缓冲区
extern unsigned char BLUETOOTH_TXBUF[70]; //异步串行通讯发送缓冲区
extern unsigned char BLUETOOTH_RXNO;      //通讯接收到的字节数目
extern unsigned char BLUETOOTH_TXNO;      //通讯发送数据指针
extern unsigned char BLUETOOTH_TXEND;     //通讯要发送的数据总个数
extern unsigned char BLUETOOTH_RXTIME;    //通讯接收到最后一个字节后的静止时间
extern unsigned char BLUETOOTH_RXTIME_EN; //允许通讯接收静止时间计时标志
extern unsigned char BLUETOOTH_RXTIME_END;//通讯接收静止时间到时标志

/*************************************************************************************************/
//定时用变量
/*************************************************************************************************/
extern unsigned char HALF_SECOND_SIG;    //半秒钟定时器到时标志
extern unsigned char HUNDRED_MSECOND_SIG;//一百毫秒定时器到时标志
extern unsigned char KAIJI_TIME;         //开机界面显示计时器
extern unsigned char SLOW_SHAN_SIG;      //慢闪烁标志，定时半秒钟
extern unsigned char FAST_SHAN_SIG;      //快闪烁标志，定时100ms

/*************************************************************************************************/
//软件延时函数
//50  延时4.52us
//100 延时8.68us
//200 延时17us
//500 延时42us
//1000延时83.8us
/*************************************************************************************************/
extern void Delay(unsigned int count);
/*************************************************************************************************/
//毫秒软件延时函数
/*************************************************************************************************/
extern void Delay_ms(unsigned int ms);
/*************************************************************************************************/
// Name: hex_to_bcd
// Description: 十六进制数转换成BDC码格式
// Calls: None
// Input: 需要转换的十六进制数
// Outputs: 转换后的BCD码
/*************************************************************************************************/
extern unsigned short Hex_To_Bcd(unsigned short data);
/*************************************************************************************************/
//Name: bcd_to_hex
//Description: BCD码格式转换成十六进制数
//Calls: None
//Input: 需要转换的BCD码
//Outputs: 转换后的十六进制数
/*************************************************************************************************/
extern unsigned short Bcd_To_Hex(unsigned short data);

/*************************************************************************************************/
#define Set_Bit(val, bitn)    (val |=(1<<(bitn)))
#define Clr_Bit(val, bitn)    (val&=~(1<<(bitn)))
#define Get_Bit(val, bitn)    (val &(1<<(bitn)) )

/*************************************************************************************************/
//计算CRC16程序
/*************************************************************************************************/
extern unsigned short int CRC16(unsigned char *puchMsg, unsigned short int usDataLen);

/*************************************************************************************************/
//报警蜂鸣器控制处理程序
/*************************************************************************************************/
extern void Alarm_BEEP(void);

/*************************************************************************************************/
//继电器控制程序
/*************************************************************************************************/
extern void Relay_Control(void);

/*******************************************************************************************************************************
整形数据N次方计算函数(源自SGI STL算法库)
*******************************************************************************************************************************/
extern int Int_Power(int x, unsigned int n);

/*******************************************************************************************************************************
浮点数转换成字符型数组函数 
参数  data_f   :要转换的浮点数
      *data_i  :接收转换数据的字符型数组
      symbol_en:是否显示符号位,0:不显示,非0:显示符号位
      length   :要转换的长度,包括小数和符号位在内的长度
*******************************************************************************************************************************/
extern void Float_To_Char(float data_f,unsigned char *data_i,unsigned char symbol_en,unsigned char length);

/*******************************************************************************************************************************
浮点数转换成固定小数长度字符型数组函数 
参数  data_f   :要转换的浮点数
      *data_i  :接收转换数据的字符型数组
      symbol_en:是否显示符号位,0:不显示,非0:显示符号位
      length   :要转换的长度,包括小数和符号位在内的长度,最大长度10位(小数点1位,符号位1位,有效数字7位,小于0的数小数点前的0占一位)
      dot      :需要的小数长度
*******************************************************************************************************************************/
extern void Float_To_Fixed_Char(float data_f,unsigned char *data_i,unsigned char symbol_en,unsigned char length,unsigned char dot);  

/*******************************************************************************************************************************
字符型数组转换成浮点数函数 
参数：data      要转换的字符串指针
      length    要转换的字符个数
例：data[6]={'1','2','3','.','4','5'},转换完成后=123.45
*******************************************************************************************************************************/
extern float Char_To_Float(unsigned char *data,unsigned char length);

#endif
