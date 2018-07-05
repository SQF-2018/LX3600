#ifndef __MACRO_DEFINITION_H
#define __MACRO_DEFINITION_H

/*******************************************************************************************************************************
主控板硬件、软件版本
*******************************************************************************************************************************/
#define HARDWARE_VERSION 100 //装置硬件版本
#define SOFTWARE_VERSION 100 //装置软件版本

/******************************************************************************************************************************/
#define ON  0xff
#define OFF 0x00

#define YES 0xff
#define NO  0x00

/*******************************************************************************************************************************
编译控制宏定义
*******************************************************************************************************************************/
#define LIXING  0
#define FUAN    1
#define AGENT   2
#define FACTORY LIXING //厂家定义,LIXING:力兴公司;FUAN:伏安;AGENT:代理商

//供电方式及功能
#define MODEL_A 0 //无C1PT功能
#define MODEL_B 1 //只有1PT、C1PT功能
#define MODEL_C 2 //具备所有功能

#define MODEL MODEL_C //型号定义,用来定义仪器的功能

#if FACTORY == AGENT //代理商型号关闭蓝牙上传功能
 #define BLUETOOTH OFF //蓝牙上传功能控制,ON开启,OFF关闭
#else
 #define BLUETOOTH ON  //蓝牙上传功能控制,ON开启,OFF关闭
#endif

/*******************************************************************************************************************************
按键键值宏定义
*******************************************************************************************************************************/
#define KEY_UP       0x1f
#define KEY_DOWN     0x2f
#define KEY_LEFT     0x37
#define KEY_RIGHT    0x3b
#define KEY_ENTER    0x3d
#define KEY_ESC      0x3e
#define KEY_SUPER    0x06 //左键、上键、下键、取消键组合按键，用于进入或退出超级调试模式
#define NO_KEY_PRESS 0x3f

/*******************************************************************************************************************************
液晶显示颜色及字体宏定义
*******************************************************************************************************************************/
//               R5G6B5            R8   G8   B8    R3G3B2
#define BLACK    0x0000 //黑色    0x00,0x00,0x00    0x00
#define NAVY     0x000F //深蓝色  0x00,0x00,0x7F    0x01
#define INDIGO   0x0017 //靛蓝色  0x00,0x00,0x17    0x02
#define GBLUE    0x02B4 //灰蓝色  0x00,0x2A,0xA0    0x0A   
#define DGREEN   0x03E0 //深绿色  0x00,0x7F,0x00    0x0C
#define DCYAN    0x03EF //深青色  0x00,0x7F,0x7F    0x0D       
#define MAROON   0x7800 //深红色  0x7F,0x00,0x00    0x60     
#define PURPLE   0x780F //紫色    0x7F,0x00,0x7F    0x61      
#define OLIVE    0x7BE0 //橄榄绿  0x7F,0x7F,0x00    0x6C     
#define LGRAY    0xC618 //灰白色  0xC0,0xC0,0xC0    0xDB      
#define DGRAY    0x7BEF //深灰色  0x7F,0x7F,0x7F    0x6D      
#define BLUE     0x001F //蓝色    0x00,0x00,0xFF    0x03        
#define GREEN    0x07E0 //绿色    0x00,0xFF,0x00    0x1C
#define MOSS     0x07F0 //苔绿色  0x00,0xFC,0x10    0x1E     
#define CYAN     0x07FF //青色    0x00,0xFF,0xFF    0x1F       
#define RED      0xF800 //红色    0xFF,0x00,0x00    0xE0
#define PINK     0xFB10 //粉红色  0xF8,0x60,0x80    0xEE    
#define MAGENTA  0xF81F //品红    0xFF,0x00,0xFF    0xE3      
#define YELLOW   0xFFE0 //黄色    0xFF,0xFF,0x00    0xFC
#define BEIGE    0xFFF0 //米黄色  0xF8,0xFC,0x80    0xFE     
#define WHITE    0xFFFF //白色    0xFF,0xFF,0xFF    0xFF

#define HIGH_BRIGHTNESS 255 //液晶屏高亮度值
#define LOW_BRIGHTNESS  10  //液晶屏低亮度值

#define X_PIXEL_MAX   639 //水平最大像素坐标
#define Y_PIXEL_MAX   479 //垂直最大像素坐标

#define FONT_NORMAL   0  //外部GT30L32S4W字库芯片中标准ASCII字体
#define FONT_AIRAL    1  //外部GT30L32S4W字库芯片中Airal ASCII字体
#define FONT_ROMAN    2  //外部GT30L32S4W字库芯片中Times New Roman ASCII字体
#define FONT_SIZE_X1  0  //字体不放大
#define FONT_SIZE_X2  1  //字体放大2倍
#define FONT_SIZE_X3  2  //字体放大3倍
#define FONT_SIZE_X4  3  //字体放大4倍

#define FONT_8  8  // 6* 8点阵字体，对应的ASCII字体大小为 5*8
#define FONT_12 12 // 6*12点阵字体，对应的ASCII字体大小为 6*12
#define FONT_16 16 //16*16点阵字体，对应的ASCII字体大小为 8*16
#define FONT_24 24 //24*24点阵字体，对应的ASCII字体大小为12*24
#define FONT_32 32 //32*32点阵字体，对应的ASCII字体大小为16*32
#define FONT_48 48 //24*48点阵字体，对应的ASCII字体大小为24*48
#define FONT_64 64 //64*64点阵字体，对应的ASCII字体大小为32*64

#define LAYER_1 0  //液晶屏显示图层1
#define LAYER_2 1  //液晶屏显示图层2

#define DIS_X_MAX    175 //最大X轴像素坐标
#define DIS_Y_MAX    130 //最大Y轴像素坐标
#define DIS_BYTE_MAX 22  //最大X轴字节数量
#define THIN_LINE    0   //绘制细线
#define THICK_LINE   1   //绘制粗线

#define TESTING_SCREEN     3 //实时测试屏幕编号宏定义
#define TEST_RECORD_SCREEN 4 //测试记录查询屏幕编号宏定义

/*******************************************************************************************************************************
AD采样宏定义
*******************************************************************************************************************************/
#define VOLTAGE_CHANNAL      0 //电压通道
#define CURRENT_CHANNAL      1 //电流通道
#define VOLTAGE_50HZ_CHANNAL 2 //50HZ电压通道

/*******************************************************************************************************************************
硬件错误标志宏定义
*******************************************************************************************************************************/
#define RTC_ERR       0
#define FM_MEM_ERR    1
#define USB_ERR       2
#define SPWMPOWER_ERR 3
#define BLUETOOTH_ERR 4

/*******************************************************************************************************************************
继电器控制寄存器宏定义
*******************************************************************************************************************************/
#define JDQ_QH  0 //低频、高频信号切换继电器
#define JDQ_FY  1 //分压电阻网络切换控制继电器
#define JDQ_VZY 2 //电压通道增益控制继电器
#define JDQ_IZY 3 //电流通道增益控制继电器
#define JDQ_OUT 4 //输出控制继电器

/*******************************************************************************************************************************
硬件工作状态宏定义
*******************************************************************************************************************************/
#define OV_50HZ_BIT   0 //50HZ零序电压过压
#define SPWM_OSC_BIT  1 //变频电源输出短路
#define SPWM_OOC_BIT  2 //变频电源输出开路
#define FUSE_OOC_BIT  3 //保险管熔断开路
#define SPWM_FAIL_BIT 4 //变频电源通讯失败
#define BAT_LOW_BIT   5 //电池电量低
#define LONG_IDLE_BIT 6 //测试仪长时间空闲

/*******************************************************************************************************************************
变频逆变电源通讯错误代码宏定义
*******************************************************************************************************************************/
#define FUNCTION_ERR   0 //非法功能
#define ADDRESS_ERR    1 //非法地址
#define VALUE_ERR      2 //非法数值
#define BUSY_ERR       3 //从属设备忙
#define CRC_ERR        4 //CRC校验错误
#define DATA_MATCH_ERR 5 //发送数据与接收数据不匹配
#define UNKNOWN_ERR    6 //未定义的异常码
#define DATA_FRAME_ERR 7 //数据帧错误
#define INTERRUPT_ERR  8 //通讯中断
#define RESPONS_ERR    9 //通讯响应超时

/********************************************************************************************************************************
变频电源控制宏定义
********************************************************************************************************************************/
#define POWER_OUT_LF      12  //变频电源输出低频正弦波；单位：HZ
#define POWER_OUT_HF      180 //变频电源输出高频正弦波；单位：HZ
#define BASE_FREQENCY_LF  2   //采样低频信号时的基波频率
#define BASE_FREQENCY_HF  10  //采样高频信号时的基波频率
#define MODULATION_MAX    940 //保证波形不失真的允许使用的最大调制度
/********************************************************************************************************************************
变频逆变电源控制指令宏替换
********************************************************************************************************************************/
#define SPWMPOWER_SOFT_STOP      0 //软停机
#define SPWMPOWER_SOFT_START     1 //软启动
#define SPWMPOWER_EMERGENCY_STOP 2 //紧急停机
#define SPWMPOWER_FREE_STOP      4 //自由停机
#define SPWMPOWER_RESET     0x9712 //变频逆变电源复位控制代码
/********************************************************************************************************************************
变频逆变电源通讯数据表宏替换
********************************************************************************************************************************/
#define SPWMPOWER_CTRL_FLAG  SPWMPOWER_DATA[0] //输出控制寄存器
#define SPWMPOWER_TURN_ON_OFF_BIT  0//BIT0:输出启动、停止控制位（0:停止;1:启动）
#define SPWMPOWER_SOFT_CONTROL_BIT 1//BIT1:软起、软停过程状态位（0:软起、软停过程未结束;1:软起、软停过程结束
#define SPWMPOWER_MDLT_RAT   SPWMPOWER_DATA[1] //变频逆变电源调制度
#define SPWMPOWER_MDLT_FREQ  SPWMPOWER_DATA[2] //变频逆变电源调制波频率
#define SPWMPOWER_RESET_CTRL SPWMPOWER_DATA[3] //变频逆变电源复位控制寄存器
#define SPWMPOWER_SV         SPWMPOWER_DATA[4] //变频逆变电源软件版本号
#define SPWMPOWER_HV         SPWMPOWER_DATA[5] //变频逆变电源硬件版本号
/********************************************************************************************************************************
变频逆变电源通讯地址宏定义
********************************************************************************************************************************/
#define READ_STA_ADDR   0x0000 //可读单元起始地址
#define READ_END_ADDR   0x0005 //可读单元结束地址
#define WRITE_STA_ADDR  0x0000 //可写单元起始地址
#define WRITE_END_ADDR  0x0003 //可写单元结束地址
#define MAX_DATA_NUMBER 6      //数据表格中最大数据量
/********************************************************************************************************************************
变频逆变电源通讯命令宏定义
********************************************************************************************************************************/
#define MULTI_READ   0x03 //多字节读操作
#define SINGLE_WRITE 0x06 //单字节写操作
#define MULTI_WRITE  0x10 //多字节写操作

/*******************************************************************************************************************************
测量参数设置阀值宏定义
*******************************************************************************************************************************/
#define RATED_VOLATE_MAX     999 //允许的最高额定高压
//#define BUS_VOL_RATIO_MAX    1.2 //允许的母线电压相对额定电压倍率的最大值
//#define BUS_VOL_RATIO_MIN    0.8 //允许的母线电压相对额定电压倍率的最小值
#define BUS_VOLTAGE_MAX      999 //允许的母线电压最大值
#define BUS_VOLTAGE_MIN      1   //允许的母线电压最小值
//#define CAPACITANCE_MAX      1000//能够测量的电容量最大值,单位:uF
#define PT_CONNECT_MODE_MAX  7   //PT连接方式最多种类
#define PT_3PT               0   //3PT连接方式
#define PT_3PT1              1   //3PT1连接方式
#define PT_3PT2              2   //3PT2连接方式
#define PT_4PT               3   //4PT连接方式
#define PT_4PT1              4   //4PT1连接方式
#define PT_4PT2              5   //4PT2连接方式
#define PT_1PT               6   //1PT连接方式
#define PT_C1PT              7   //C1PT连接方式
#define TRANS_RAT_3PT        17.320508075689 //电压互感器变比，此值扩大了1000倍，此值乘以UL(单位:KV)即为电压互感器实际变比
#define TRANS_RAT_3PT1       5.773502691896
#define TRANS_RAT_3PT2       10
#define TRANS_RAT_4PT        10
#define TRANS_RAT_4PT1       6.339745962156
#define TRANS_RAT_4PT2       17.320508075689
#define TRANS_RAT_1PT        10
#define COE_VRMS_0_THEORY    7.95439496947e-4 //62K 9.7296K合成电阻分压， 1倍放大，AD量程±5V，理论电压校正系数(校正结果为有效值)
#define COE_VRMS_1_THEORY    2.18790055536e-4 //10K 9.7296K合成电阻分压， 1倍放大，AD量程±5V，理论电压校正系数(校正结果为有效值)
#define COE_VRMS_2_THEORY    2.18790055536e-5 //10K 9.7296K合成电阻分压，10倍放大，AD量程±5V，理论电压校正系数(校正结果为有效值)
#define COE_IRMS_0_THEORY    8.63167457503e-5 //0.25欧电阻， 5倍放大，AD量程±5V，理论电流校正系数(校正结果为有效值)
#define COE_IRMS_1_THEORY    1.05264324086e-5 //0.25欧电阻，41倍放大，AD量程±5V，理论电流校正系数(校正结果为有效值)
#define COE_VRMS_50HZ_THEORY 3.57185620834e-3 //零序电压理论系数，200K 6.8K电阻分压，1倍放大，AD量程±5V(校正结果为有效值)
#define COE_V_BAT_THEORY     5.28157552083e-3 //电池电压理论系数， 10K 1.8K电阻分压，1倍放大，AD量程0~3.3V

/*******************************************************************************************************************************
测量过程控制阀值宏定义
*******************************************************************************************************************************/
//#define DIODE_VOLTAGE_DROP      0.51 //变频电源输入二极管压降，单位：V
#define BOOST_POWER_VOLTAGE     30   //升压电源输出电压值（稳压输出）
#define INIT_POWER_OUT_VOLTAGE  0.2  //测试开始时输出电压值，单位：V
#define VOLTAGE_50HZ_THRESHOLD  10   //50HZ电压信号越限阀值，单位：V
#define RATED_OUT_CURRENT       1    //变频电源额定输出电流有效值，即测试最大电流，单位：A
#define RATED_OUT_POWER         20   //变频电源额定输出功率，单位：W
//#define INTER_IMPEDANCE_MIN     1    //变频电源输出内阻最小值
#define LOAD_IMPEDANCE_MIN      0.1  //允许外部负载阻抗最小值
#define VOLTAGE_OUT_MIN         0.1  //判断保险管熔断允许的最小电压值，单位：V
#define CURRENT_OUT_MIN         0.01 //判断保险管熔断允许的最小电流值，单位：A
//输出低频时，开路阻抗在64K左右；输出高频时，开路阻抗在11K左右
//输出直接接1uF电容时，低频阻抗15K左右；高频阻抗1K左右
#define OOC_LF_IMPEDANCE        15000  //输出低频信号时，测试回路开路阻抗阀值     
#define OOC_HF_IMPEDANCE        8000   //输出高频信号时，测试回路开路阻抗阀值，这里取值较大是考虑到接入高压PT后此阻抗较大，尤其66KV PT
// 2HZ采样时，采样点之间的时间间隔为0.244140625ms
//10HZ采样时，采样点之间的时间间隔为0.048828125ms
#define AWD_THRESHOLD_CROSS_TIME_LIMIT  2 //模拟看门狗超阀值计时时长，单位：ms，实际计时时长为(X+1)ms
#define AWD_THRESHOLD_CROSS_COUNT_LIMIT 3 //模拟看门狗有效计数个数，实际计数个数为(X+1)
#define CURRENT_CTRL_TIME_MAX   160  //电流调节过程允许的最长调节时间，单位：100ms
#define ADC12_SAMPLE_POINT      10   //内部ADC采样点数

/*******************************************************************************************************************************
普通数据宏定义
*******************************************************************************************************************************/
#define POWERON_TIME             6     //开始显示公司LOGO时间，单位：0.5S
#define SINGLE_BAT_VOLTAGE_MIN   3.45  //单节电池允许的最低电压，此处设置应与电池电量程序中对应数值相同
#define BAT_SERIES_QUANTITY      4     //供电电池串联数量
#define BAT_VOLTAGE_MIN	         SINGLE_BAT_VOLTAGE_MIN * BAT_SERIES_QUANTITY//允许工作的最低电池电压，单位：V
#define LOW_BAT_TIME             60    //电池电量低延时报警时间，单位：0.5S
#define IDLE_TIME_MAX            30000 //允许的最长空闲时间，单位：10ms
#define SYSTEM_SETTING_SCREEN    6     //系统参数设置屏幕编号宏替换
#define RANGE_CALIBRATION_SCREEN 10    //量程校准屏幕编号宏替换
#define U03_CALIBRATION_SCREEN   8     //3U0校准屏幕编号宏替换       

/*******************************************************************************************************************************
铁电存储器存储变量地址定义
*******************************************************************************************************************************/
//存储器空标志
#define ee_data_sig1            0x0000 //存储器空标志1
#define ee_data_sig2            0x1fff //存储器空标志2，8K铁电存储器最后一个地址为0x1fff
//装置编号
#define ee_device_number0       0x0001 //功率控制板编号第一位
#define ee_device_number1       0x0002 //功率控制板编号第二位
#define ee_device_number2       0x0003 //功率控制板编号第三位
#define ee_device_number3       0x0004 //功率控制板编号第四位
//测试参数保存
#define ee_test_number0         0x0010 //试验编号第一位
#define ee_test_number1         0x0011 //试验编号第二位
#define ee_test_number2         0x0012 //试验编号第三位
#define ee_test_number3         0x0013 //试验编号第四位
#define ee_test_number4         0x0014 //试验编号第五位
#define ee_test_number5         0x0015 //试验编号第六位
#define ee_device_id0           0x0016 //设备ID第一位
#define ee_device_id1           0x0017 //设备ID第二位
#define ee_device_id2           0x0018 //设备ID第三位
#define ee_device_id3           0x0019 //设备ID第四位
#define ee_device_id4           0x001a //设备ID第五位
#define ee_device_id5           0x001b //设备ID第六位
#define ee_rated_voltage_l      0x001c //额定高压低字节
#define ee_rated_voltage_h      0x001d //额定高压高字节
#define ee_bus_voltage_l        0x001e //母线电压低字节
#define ee_bus_voltage_h        0x001f //母线电压高字节
#define ee_pt_connect_mode      0x0020 //PT连接方式
#define ee_phase_a_cap          0x0021 //A相补偿电容器组电容量
#define ee_phase_b_cap          0x0025 //B相补偿电容器组电容量
#define ee_phase_c_cap          0x0029 //C相补偿电容器组电容量
//模拟量校正系数
#define ee_coe_z_lf_v0i0        0x0030 //低频阻抗校正系数，电压档位0，电流档位0，浮点数，占用4个字节
#define ee_coe_z_lf_v1i0        0x0034 //低频阻抗校正系数，电压档位1，电流档位0，浮点数，占用4个字节
#define ee_coe_z_lf_v2i0        0x0038 //低频阻抗校正系数，电压档位2，电流档位0，浮点数，占用4个字节
#define ee_coe_z_lf_v0i1        0x003c //低频阻抗校正系数，电压档位0，电流档位1，浮点数，占用4个字节
#define ee_coe_z_lf_v1i1        0x0040 //低频阻抗校正系数，电压档位1，电流档位1，浮点数，占用4个字节
#define ee_coe_z_lf_v2i1        0x0044 //低频阻抗校正系数，电压档位2，电流档位1，浮点数，占用4个字节
#define ee_coe_pha_dif_lf_v0i0  0x0048 //低频电压、电流相角差校正系数，电压档位0，电流档位0，浮点数，占用4个字节
#define ee_coe_pha_dif_lf_v1i0  0x004c //低频电压、电流相角差校正系数，电压档位1，电流档位0，浮点数，占用4个字节
#define ee_coe_pha_dif_lf_v2i0  0x0050 //低频电压、电流相角差校正系数，电压档位2，电流档位0，浮点数，占用4个字节
#define ee_coe_pha_dif_lf_v0i1  0x0054 //低频电压、电流相角差校正系数，电压档位0，电流档位1，浮点数，占用4个字节
#define ee_coe_pha_dif_lf_v1i1  0x0058 //低频电压、电流相角差校正系数，电压档位1，电流档位1，浮点数，占用4个字节
#define ee_coe_pha_dif_lf_v2i1  0x005c //低频电压、电流相角差校正系数，电压档位2，电流档位1，浮点数，占用4个字节
#define ee_coe_v_lf_v0          0x0060 //低频电压档位0校正系数，浮点数，占用4个字节
#define ee_coe_v_lf_v1          0x0064 //低频电压档位1校正系数，浮点数，占用4个字节
#define ee_coe_v_lf_v2          0x0068 //低频电压档位2校正系数，浮点数，占用4个字节
#define ee_coe_i_lf_i0          0x006c //低频电流档位0校正系数，浮点数，占用4个字节
#define ee_coe_i_lf_i1          0x0070 //低频电流档位1校正系数，浮点数，占用4个字节
#define ee_coe_z_hf_v0i0        0x0074 //高频阻抗校正系数，电压档位0，电流档位0，浮点数，占用4个字节
#define ee_coe_z_hf_v1i0        0x0078 //高频阻抗校正系数，电压档位1，电流档位0，浮点数，占用4个字节
#define ee_coe_z_hf_v2i0        0x007c //高频阻抗校正系数，电压档位2，电流档位0，浮点数，占用4个字节
#define ee_coe_z_hf_v0i1        0x0080 //高频阻抗校正系数，电压档位0，电流档位1，浮点数，占用4个字节
#define ee_coe_z_hf_v1i1        0x0084 //高频阻抗校正系数，电压档位1，电流档位1，浮点数，占用4个字节
#define ee_coe_z_hf_v2i1        0x0088 //高频阻抗校正系数，电压档位2，电流档位1，浮点数，占用4个字节
#define ee_coe_pha_dif_hf_v0i0  0x008c //高频电压、电流相角差校正系数，电压档位0，电流档位0，浮点数，占用4个字节
#define ee_coe_pha_dif_hf_v1i0  0x0090 //高频电压、电流相角差校正系数，电压档位1，电流档位0，浮点数，占用4个字节
#define ee_coe_pha_dif_hf_v2i0  0x0094 //高频电压、电流相角差校正系数，电压档位2，电流档位0，浮点数，占用4个字节
#define ee_coe_pha_dif_hf_v0i1  0x0098 //高频电压、电流相角差校正系数，电压档位0，电流档位1，浮点数，占用4个字节
#define ee_coe_pha_dif_hf_v1i1  0x009c //高频电压、电流相角差校正系数，电压档位1，电流档位1，浮点数，占用4个字节
#define ee_coe_pha_dif_hf_v2i1  0x00a0 //高频电压、电流相角差校正系数，电压档位2，电流档位1，浮点数，占用4个字节
#define ee_coe_v_hf_v0          0x00a4 //高频电压档位0校正系数，浮点数，占用4个字节
#define ee_coe_v_hf_v1          0x00a8 //高频电压档位1校正系数，浮点数，占用4个字节
#define ee_coe_v_hf_v2          0x00ac //高频电压档位2校正系数，浮点数，占用4个字节
#define ee_coe_i_hf_i0          0x00b0 //高频电流档位0校正系数，浮点数，占用4个字节
#define ee_coe_i_hf_i1          0x00b4 //高频电流档位1校正系数，浮点数，占用4个字节
#define ee_coe_vrms_50hz        0x00b8 //50HZ电压幅值信号校正系数，浮点数，占用4个字节
#define ee_coe_v_bat            0x00bc //电池电压校正系数，浮点数，占用4个字节
//测量记录
//每条测量记录数据量：
//试验编号      6字节
//设备ID        6字节
//额定高压      2字节
//母线电压      2字节
//PT连接方式    1字节
//PT变比        4字节浮点
//A相电容器容量 4字节浮点
//B相电容器容量 4字节浮点
//C相电容器容量 4字节浮点
//零序电压      4字节浮点
//电容容量      4字节浮点
//电容电流      4字节浮点
//测量日期      6字节
//总共         49字节
#define ee_test_record_number    0x0100 //最新测量记录编号保存地址，1字节
#define ee_test_record_amount    0x0101 //已经保存的测量记录的总个数保存地址，1字节
#define ee_test_record_data      0x0102 //测量记录储存起始地址

#define TEST_RECORD_AMOUNT_MAX   150   //允许的最大测量记录保存数量
#define TEST_RECORD_SIZE         51    //每条测量记录的字节数量

#endif
