#ifndef STRUCTURE_DEFNITION_H
#define STRUCTURE_DEFNITION_H

/*******************************************************************************************************************************
测试结果及测试参数设置变量
*******************************************************************************************************************************/
typedef struct //测试数据结构变量
{
 unsigned char  TEST_NUMBER[6]; //试验编号
 unsigned char  DEVICE_ID[6];   //设备ID
 unsigned short RATED_VOLTAGE;  //额定高压
 unsigned short BUS_VOLTAGE;    //母线实际电压值
 unsigned char  PT_CONNECT_MODE;//PT连接方式
 float          PT_TRANS_RAT;   //PT变比值
 float          PHASE_A_CAP;    //A相补偿电容器组电容
 float          PHASE_B_CAP;    //B相补偿电容器组电容量
 float          PHASE_C_CAP;    //C相补偿电容器组电容量
 float          Z_LF[10];       //低频信号回路阻抗
 float          D_LF[10];       //低频信号电压、电流相角差
 float          R_LF[10];       //低频测试频率下系统的纯电阻
 float          XC_LF[10];      //低频测试频率下系统的容抗
 float          XL_LF[10];      //低频测试频率下系统的感抗
 float          Z_HF[10];       //高频信号回路阻抗
 float          D_HF[10];       //高频信号电压、电流相角差
 float          R_HF[10];       //高频测试频率下系统的纯电阻
 float          XC_HF[10];      //高频测试频率下系统的容抗
 float          XL_HF[10];      //高频测试频率下系统的感抗
 float          VRMS_50HZ;      //测量结束后PT零序电压
 float          CAP_MEDIAN[10]; //测量计算的电容量中间值
 float          CAPACITANCE;    //测量计算的最终电容量
 float          CAP_CURRENT;    //测量出来的电容电流
 float          INTER_IMPEDANCE;//变频电源输出内阻
 float          LOAD_IMPEDANCE; //被测负载阻抗
 unsigned char  year;           //测量时间年份
 unsigned char  month;          //测量时间月份
 unsigned char  date;           //测量时间日期
 unsigned char  hour;           //测量时间小时
 unsigned char  minute;         //测量时间分钟
 unsigned char  second;         //测量时间秒钟
}TEST_DATA_STRUCT;

/*******************************************************************************************************************************
校准系数结构体
*******************************************************************************************************************************/
typedef struct //校准系数结构体
{
 float COE_Z_LF_V0I0;      //低频阻抗校正系数，电压档位0，电流档位0
 float COE_Z_LF_V1I0;      //低频阻抗校正系数，电压档位1，电流档位0
 float COE_Z_LF_V2I0;      //低频阻抗校正系数，电压档位2，电流档位0
 float COE_Z_LF_V0I1;      //低频阻抗校正系数，电压档位0，电流档位1
 float COE_Z_LF_V1I1;      //低频阻抗校正系数，电压档位1，电流档位1
 float COE_Z_LF_V2I1;      //低频阻抗校正系数，电压档位2，电流档位1
 float COE_PHA_DIF_LF_V0I0;//低频电压、电流相角差校正系数，电压档位0，电流档位0
 float COE_PHA_DIF_LF_V1I0;//低频电压、电流相角差校正系数，电压档位1，电流档位0
 float COE_PHA_DIF_LF_V2I0;//低频电压、电流相角差校正系数，电压档位2，电流档位0
 float COE_PHA_DIF_LF_V0I1;//低频电压、电流相角差校正系数，电压档位0，电流档位1
 float COE_PHA_DIF_LF_V1I1;//低频电压、电流相角差校正系数，电压档位1，电流档位1
 float COE_PHA_DIF_LF_V2I1;//低频电压、电流相角差校正系数，电压档位2，电流档位1
 float COE_V_LF_V0;        //低频电压档位0校正系数
 float COE_V_LF_V1;        //低频电压档位1校正系数
 float COE_V_LF_V2;        //低频电压档位2校正系数 
 float COE_I_LF_I0;        //低频电流档位0校正系数
 float COE_I_LF_I1;        //低频电流档位1校正系数 
 float COE_Z_HF_V0I0;      //高频阻抗校正系数，电压档位0，电流档位0
 float COE_Z_HF_V1I0;      //高频阻抗校正系数，电压档位1，电流档位0
 float COE_Z_HF_V2I0;      //高频阻抗校正系数，电压档位2，电流档位0
 float COE_Z_HF_V0I1;      //高频阻抗校正系数，电压档位0，电流档位1
 float COE_Z_HF_V1I1;      //高频阻抗校正系数，电压档位1，电流档位1
 float COE_Z_HF_V2I1;      //高频阻抗校正系数，电压档位2，电流档位1
 float COE_PHA_DIF_HF_V0I0;//高频电压、电流相角差校正系数，电压档位0，电流档位0
 float COE_PHA_DIF_HF_V1I0;//高频电压、电流相角差校正系数，电压档位1，电流档位0
 float COE_PHA_DIF_HF_V2I0;//高频电压、电流相角差校正系数，电压档位2，电流档位0
 float COE_PHA_DIF_HF_V0I1;//高频电压、电流相角差校正系数，电压档位0，电流档位1
 float COE_PHA_DIF_HF_V1I1;//高频电压、电流相角差校正系数，电压档位1，电流档位1
 float COE_PHA_DIF_HF_V2I1;//高频电压、电流相角差校正系数，电压档位2，电流档位1
 float COE_V_HF_V0;        //高频电压档位0校正系数
 float COE_V_HF_V1;        //高频电压档位1校正系数
 float COE_V_HF_V2;        //高频电压档位2校正系数 
 float COE_I_HF_I0;        //高频电流档位0校正系数
 float COE_I_HF_I1;        //高频电流档位1校正系数
 float COE_VRMS_50HZ;      //50HZ电压信号有效值校正系数
 float COE_V_BAT;          //电池电压校正系数 
}COE_DATA_STRUCT;

/*******************************************************************************************************************************
模拟量计算结果结构变量
*******************************************************************************************************************************/
typedef struct //模拟量计算结果结构变量
{
 float VRMS_LF;        //低频电压信号有效值实际值
 float VRMS_LF_ORIG;   //低频电压信号有效值原始值
 float VPHA_LF;        //低频电压信号初相角
 float IRMS_LF;        //低频电流信号有效值实际值
 float IRMS_LF_ORIG;   //低频电流信号有效值原始值
 float IPHA_LF;        //低频电流信号初相角
 float PHA_DIF_LF;     //低频信号电压、电流相角差实际值
 float PHA_DIF_LF_ORIG;//低频信号电压、电流相角差原始值
 float VRMS_HF;        //高频电压信号有效值实际值
 float VRMS_HF_ORIG;   //高频电压信号有效值原始值
 float VPHA_HF;        //高频电压信号初相角
 float IRMS_HF;        //高频电流信号有效值实际值
 float IRMS_HF_ORIG;   //高频电流信号有效值原始值
 float IPHA_HF;        //高频电流信号初相角
 float PHA_DIF_HF;     //高频信号电压、电流相角差实际值
 float PHA_DIF_HF_ORIG;//高频信号电压、电流相角差原始值
 float VRMS_50HZ;      //50HZ电压信号有效值实际值
 //float VPHA_50HZ;      //50HZ电压信号初相角
 float V_BAT;          //电池电压实际值
}ANALOG_VALUE_STRUCT;

/*******************************************************************************************************************************
时间结构变量结构变量
*******************************************************************************************************************************/
typedef struct //时间结构变量
{
 unsigned char  year;
 unsigned char  month;
 unsigned char  date;
 unsigned char  weekday;
 unsigned char  hour;
 unsigned char  minute;
 unsigned char  second;
}BCD_TIME;

#endif
