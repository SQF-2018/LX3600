#include "IOCONFIG.H"
#include "QJBL.H"
#include "MB85RCXX.H"
#include "LCDDRIVER.H"
#include "RTC.H"
#include "SPWM_POWER.H"
#include "PRINTER.H"
#include "U_DISK.H"
#include "BLUETOOTH.H"
#include "LCD.H"
#include "FUNCTION.H"
#include "AD7606.H"

#define ADD 0x00
#define SUB 0xff

/*******************************************************************************************************************************
调整浮点数转换成字符数组后的数组相应位数值函数 
参数  data  :要操作的字符数组
      length:要操作的字符数组长度
      digit :要调整数组的位标志,下标从1开始计算
      action:调整动作标志;ADD(0):加;SUB(非零):减
*******************************************************************************************************************************/
void Float_Char_Adjust(unsigned char *data,unsigned char length,unsigned char digit,unsigned char action)
{
 unsigned char i,dot=0;
 if(length > 8 || digit > 8 || digit > length) return;//浮点数加上小数点最多8位(不处理正负号),且操作的位不能大于数组长度
 for(i=0;i<length;i++)
  {
   if((data[i] >= '0' && data[i] <= '9') || data[i] == '.')
    {
     if(data[i] == '.' && dot == 0) //发现小数点,之前没有小数点
      {
       dot = i; //记录小数点位置
      }
     else if(data[i] == '.' && dot != 0) return; //发现了多个小数点 
    }
   else return; //数组中存在非法数据值,则直接返回 
  }
 if(action == ADD) //加
  {
   //无小数点,且不是最低位和最高位(最低位和最高位不能设置成小数点);或者已经存在小数点,且当前操作的就是小数点位
   if((dot == 0 && digit != 1 && digit != length) || (dot != 0 && digit - 1 == dot)) 
    {
     if(data[digit-1] == '.') data[digit-1] = '0';
     else if(data[digit-1] < '9') data[digit-1] += 1;
     else if(data[digit-1] == '9') data[digit-1] = '.';
    }
   else
    {     
     if(data[digit-1] < '9') data[digit-1] += 1;
     else data[digit-1] = '0';
    } 
  }
 else //减
  {
   //无小数点,且不是最低位和最高位(最低位和最高位不能设置成小数点);或者已经存在小数点,且当前操作的就是小数点位
   if((dot == 0 && digit != 1 && digit != length) || (dot != 0 && digit - 1 == dot)) 
    {
     if(data[digit-1] == '.') data[digit-1] = '9';
     else if(data[digit-1] > '0') data[digit-1] -= 1;
     else if(data[digit-1] == '0') data[digit-1] = '.';
    }
   else
    {     
     if(data[digit-1] > '0') data[digit-1] -= 1;
     else data[digit-1] = '9';
    }
  } 
}

/*************************************************************************************************/
//启动测量过程处理程序
/*************************************************************************************************/
void Start_Test_Process(void)
{
 
}

/*************************************************************************************************/
//按键采样处理程序
/*************************************************************************************************/
void Key_In(void)
{
 if(KEY2 == NO_KEY_PRESS)//无按键按下
  {
   KEYTIME=0;
   //延时5分钟关闭液晶屏背光
   if(DKEYTIME == IDLE_TIME_MAX)
    {
     LCD_BRIGHTNESS = LOW_BRIGHTNESS;//设置液晶屏背光亮度为最低亮度
    }
   if(DKEYTIME < IDLE_TIME_MAX)
    {
     DKEYTIME+=1;
    }
  }   
 else
  {
   if (KEYTIME < 0xffff) KEYTIME+=1;//按键按下计数器如未到达指定值则加一
   if(KEY2 == KEY1)
    {
     if((KEYTIME == 4) || (KEYTIME >= 100 && (KEYTIME % 5 == 0))) 
      {
       if(DKEYTIME >= IDLE_TIME_MAX) LCD_BRIGHTNESS = HIGH_BRIGHTNESS;//设置液晶屏背光亮度为最高亮度
       DKEYTIME = 0;   //无按键按下计数器清零
       YESKEY   = KEY2;//取按键实际值
       if(HARDWARE_WORK_STATE == 0) BELL_ON;//开启按键蜂鸣器
      }
    }  
   else
    {
     KEYTIME = 0;
    } 
  }
}
/************************************************************************************************************/
//各个屏幕的按键处理程序
/************************************************************************************************************/
void Key_Pro(void)
{
 unsigned char i;
 float tmp_f,coe_tmp;
 if(YESKEY == KEY_SUPER)/*请求进入或退出超级调试模式*/
  {
   if(PARAMETER_DISPLAY_EN == NO) PARAMETER_DISPLAY_EN = YES;
   else 
    {
     PARAMETER_DISPLAY_EN = NO;
     if(POPUP_SCRFL == 2 && TEST_PROCESS_FLAG == 9) POPUP_SCRFL = 0;//关闭超级调试模式时，如果当前是测量过程显示弹出窗口且测量过程完毕，则关闭弹出显示窗口
    } 
   SCRAPPLY = YES;//请求窗口刷新
  }
 /*---------------------------------------各屏幕按键操作------------------------------------------------------*/
 if(BURST_SCRFL)//突发弹出屏幕编号不为0，有弹出屏幕显示请求
  {
   switch(BURST_SCRFL)
    {
     /*----------------------------------------------------------------------------------------------------*/
     //硬件工作状态异常报警弹出屏幕 BURST_SCRFL = 1
     /*----------------------------------------------------------------------------------------------------*/
     case  1:
             if(Get_Bit(HARDWARE_WORK_STATE,BAT_LOW_BIT)) LOW_BAT_ALARM_TIME = 0;//如果发生了电池电量低状态，则清零延时报警计数器，重新计数
             HARDWARE_WORK_STATE = 0;  //清除所有报警标志
             BURST_SCRFL         = 0;  //取消突发弹出窗口
             SCRAPPLY            = YES;//请求窗口刷新
             break;
    }
  }
 else if(POPUP_SCRFL)//弹出屏幕编号不为0，有弹出屏幕显示请求
  {
   switch(POPUP_SCRFL)
    {
     /*----------------------------------------------------------------------------------------------------*/
     //硬件初始化出错弹出屏幕       POPUP_SCRFL = 1
     /*----------------------------------------------------------------------------------------------------*/
     case  1:if(YESKEY == KEY_ENTER || YESKEY == KEY_ESC)
              {
               POPUP_SCRFL = 0;  //关闭弹出窗口
               SCRAPPLY    = YES;//请求窗口刷新
              }
             break;
     /*----------------------------------------------------------------------------------------------------*/
     //测量过程弹出屏幕 POPUP_SCRFL = 2
     /*----------------------------------------------------------------------------------------------------*/
     case  2:
             if(YESKEY == KEY_UP || YESKEY == KEY_DOWN || YESKEY == KEY_LEFT || YESKEY == KEY_RIGHT)
              {
               if(POPUP_CDX == 1) POPUP_CDX = 2;
               else POPUP_CDX = 1;
               SCRAPPLY = YES;//请求窗口刷新
              }
             else if(YESKEY == KEY_ENTER)
              {
               if(PARAMETER_DISPLAY_EN == YES)//显示详细参数时，按确认键重新开始测量过程
                {
                 if(TEST_PROCESS_EN == NO)//测量过程处在结束状态
                  {
                   Start_Test_Process();//启动测量过程
                  }
                }
              }
             else if(YESKEY == KEY_ESC)
              {
               SPWMPOWER_CTRL_FLAG   = SPWMPOWER_SOFT_STOP;//软停机
               SP_MODBUS_COMMAND     = SINGLE_WRITE;       //单节写操作
               SP_MODBUS_START_ADD   = WRITE_STA_ADDR;     //数据表起始地址
               SP_MODBUS_DATA_NUMBER = 1;                  //写入1个输出控制寄存器
               SPWM_Power_Command_Send();                  //发送指令，控制变频逆变电源软启动
               JDQ_QH_LF;                                  //信号切换继电器切换到低频通道
               JDQ_OUT_OFF;                                //释放输出继电器
               AD_Analog_Gain_Switch(NO);                  //禁止自动量程切换，切换到电压、电流增益最小档位
               TEST_PROCESS_FLAG = 0;                      //初始化测量过程控制标志
               TEST_PROCESS_EN   = NO;                     //测量过程结束
               POPUP_SCRFL       = 0;                      //取消弹出窗口
               SCRAPPLY          = YES;                    //请求窗口刷新
              }
             break;
     /*----------------------------------------------------------------------------------------------------*/
     //测量结果打印操作弹出屏幕       POPUP_SCRFL = 3
     /*----------------------------------------------------------------------------------------------------*/
     case  3:
             if(YESKEY == KEY_ENTER || YESKEY == KEY_ESC)/*---------------确认键、取消键---------------*/
              {
               if(OPERATION_PROCESS == 0 || (OPERATION_PROCESS == 1 && PRINTING_FLAG == NO))//打印机未插入,或者打印数据完成
                {
                 POPUP_SCRFL = 0;  //关闭弹出窗口
                 SCRAPPLY    = YES;//请求全屏刷新
                }                
              }
             break;
     /*----------------------------------------------------------------------------------------------------*/
     //测量结果蓝牙上传操作弹出屏幕 POPUP_SCRFL = 5
     //校准电阻阻值错误弹出屏幕     POPUP_SCRFL = 9
     //额定阻抗值输入错误弹出屏幕   POPUP_SCRFL = 10
     //零序回路呈感性提示弹出屏幕   POPUP_SCRFL = 12
     /*----------------------------------------------------------------------------------------------------*/
     case  5:             
     case  9:
     case 10:
     case 12:
             if(YESKEY == KEY_ENTER || YESKEY == KEY_ESC)
              {
               POPUP_SCRFL = 0;  //取消弹出窗口
               SCRAPPLY    = YES;//请求窗口刷新
              }
             break;
     /*----------------------------------------------------------------------------------------------------*/
     //测量结果存储操作弹出屏幕 POPUP_SCRFL = 4
     /*----------------------------------------------------------------------------------------------------*/
     case  4:
             if(YESKEY == KEY_UP)/*---------------上键---------------*/
              {
               if(SCRFL == TESTING_SCREEN)//测量结果屏幕
                {
                 if(POPUP_CDX > 1 && OPERATION_PROCESS == 0)//只有在存储操作开始前可以调节光标,其它OPERATION_PROCESS值无光标操作过程 
                  {
                   POPUP_CDX -= 1;
                  }
                }  
              }
             else if(YESKEY == KEY_DOWN)/*---------------下键---------------*/
              {
               if(SCRFL == TESTING_SCREEN)//测量结果屏幕
                {
                 if(POPUP_CDX < 2 && OPERATION_PROCESS == 0)//只有在存储操作开始前可以调节光标,其它OPERATION_PROCESS值无光标操作过程 
                  {
                   POPUP_CDX += 1;
                  }
                }  
              }
             else if(YESKEY == KEY_ENTER)/*---------------确认键---------------*/
              {
               if(POPUP_CDX == 1)//存储至本机
                {
                 if(OPERATION_PROCESS == 0)//存储动作未开始
                  {
                   if(RECORD_STORAGED_INTER == NO)//本次测量结果还未被保存至本机存储器
                    {
                     Mb85rcxx_Write_Test_Record();//将测量结果存储至本机铁电存储器
                     OPERATION_PROCESS     = 4;   //存储成功
                     RECORD_STORAGED_INTER = YES; //本次测量结果已保存
                    }
                   else OPERATION_PROCESS = 5;//提示本次测量结果已被保存过
                  }
                 else if(OPERATION_PROCESS == 1 || OPERATION_PROCESS == 2)
                  {
                   OPERATION_PROCESS = 0;//初始化存储操作过程标志
                  }
                 else if(OPERATION_PROCESS == 4 || OPERATION_PROCESS == 5)//存储成功或已保存无需重复保存提示,关闭弹出窗口
                  {
                   POPUP_SCRFL = 0; //关闭弹出窗口
                  }
                }
               else if(POPUP_CDX == 2)//存储至优盘
                {
                 if(OPERATION_PROCESS == 0)//存储动作未开始
                  {
                   if(U_DISK_CONNECT == NO) OPERATION_PROCESS = 1;//未插入优盘
                   else if(U_DISK_MOUNT == NO) OPERATION_PROCESS = 2;//优盘初始化错误
                   else //优盘正常，可以存储
                    {
                     OPERATION_PROCESS = 3;//测试数据存储中 请稍后。。。
                    }
                  }
                 else if(OPERATION_PROCESS == 1 || OPERATION_PROCESS == 2)
                  {
                   OPERATION_PROCESS = 0;//初始化存操作过程标志
                  }
                 else if(OPERATION_PROCESS == 4)//存储成功
                  {
                   POPUP_SCRFL = 0; //关闭弹出窗口
                  }
                }
               SCRAPPLY = YES;//请求全屏刷新
              }
             else if(YESKEY == KEY_ESC)/*---------------取消键---------------*/
              {
               if(OPERATION_PROCESS == 1 || OPERATION_PROCESS == 2 || OPERATION_PROCESS == 5)
                {
                 OPERATION_PROCESS = 0;//初始化操作过程标志
                }
               else
                {
                 POPUP_SCRFL = 0; //关闭弹出窗口
                }
               SCRAPPLY = YES;//请求全屏刷新
              }
             break;
     /*----------------------------------------------------------------------------------------------------*/
     //测量记录操作弹出屏幕 POPUP_SCRFL = 6
     //OPERATION_PROCESS = 0:操作方法选择
     //OPERATION_PROCESS = 1:未插入打印机
     //OPERATION_PROCESS = 2:打印数据
     //OPERATION_PROCESS = 3:未插入优盘
     //OPERATION_PROCESS = 4:优盘初始化失败
     //OPERATION_PROCESS = 5:数据存储中
     //OPERATION_PROCESS = 6:数据存储完成
     //OPERATION_PROCESS = 7:蓝牙未连接
     //OPERATION_PROCESS = 8:蓝牙上传完毕
     /*----------------------------------------------------------------------------------------------------*/
     case  6:
             if(YESKEY == KEY_UP)
              {
               if(POPUP_CDX > 1 && OPERATION_PROCESS == 0)//只有在存储操作开始前可以调节光标 
                {
                 POPUP_CDX -= 1;
                }
              }
             else if(YESKEY == KEY_DOWN)
              {
#if BLUETOOTH == ON //开启了蓝牙上传功能
               if(POPUP_CDX < 3 && OPERATION_PROCESS == 0)//只有在存储操作开始前可以调节光标 
                {
                 POPUP_CDX += 1;
                }
#elif BLUETOOTH == OFF //关闭了蓝牙上传功能
               if(POPUP_CDX < 2 && OPERATION_PROCESS == 0)//只有在存储操作开始前可以调节光标 
                {
                 POPUP_CDX += 1;
                }
#endif
              }
             else if(YESKEY == KEY_ENTER)
              {
               if(POPUP_CDX == 1)/*记录打印*/
                {
                 if(OPERATION_PROCESS == 0)//打印动作未开始
                  {
                   if(PRINTING_FLAG == NO)//未启动打印过程
                    {
                     if(PRINTER_CONNECT == NO) OPERATION_PROCESS = 1;//打印机未插入
                     else 
                      {
                       OPERATION_PROCESS = 2; //打印机已插入,打印过程开始
                       PRINTING_FLAG     = YES;//打印开始
                      }
                    }
                  }
                 else if(OPERATION_PROCESS == 1 || OPERATION_PROCESS == 2)//打印机未插入或者打印完成
                  {
                   OPERATION_PROCESS = 0;//返回操作选择
                  }
                }
               else if(POPUP_CDX == 2)/*转存优盘*/
                {
                 if(OPERATION_PROCESS == 0)//存储动作未开始
                  {
                   if(U_DISK_CONNECT == NO) OPERATION_PROCESS = 3;//未插入优盘
                   else if(U_DISK_MOUNT == NO) OPERATION_PROCESS = 4;//优盘初始化错误
                   else //优盘正常，可以存储
                    {
                     OPERATION_PROCESS = 5;//测试数据存储中 请稍后。。。
                    }
                  }
                 else if(OPERATION_PROCESS == 3 || OPERATION_PROCESS == 4)
                  {
                   OPERATION_PROCESS = 0;//初始化存操作过程标志
                  }
                 else if(OPERATION_PROCESS == 6)//存储成功
                  {
                   OPERATION_PROCESS = 0;//返回操作选择
                  }
                }
               else if(POPUP_CDX == 3)//蓝牙上传
                {
                 if(OPERATION_PROCESS == 0)//存储动作未开始
                  {
                   if(BLUETOOTH_CONNECT == NO) OPERATION_PROCESS = 7;//蓝牙未连接
                   else //蓝牙已连接
                    {
                     Bluetooth_Send_Test_Record();//蓝牙上传测量记录
                     OPERATION_PROCESS = 8;       //蓝牙上传完毕
                    }
                  }
                 else if(OPERATION_PROCESS == 7 || OPERATION_PROCESS == 8)//蓝牙未连接、蓝牙上传完毕
                  {
                   OPERATION_PROCESS = 0;//初始化操作过程标志
                  }
                }
               SCRAPPLY = YES;//请求全屏刷新
              }
             else if(YESKEY == KEY_ESC)
              {
               if(OPERATION_PROCESS == 1 || OPERATION_PROCESS == 2 || OPERATION_PROCESS == 3 || 
                  OPERATION_PROCESS == 4 || OPERATION_PROCESS == 6 || OPERATION_PROCESS == 7 || OPERATION_PROCESS == 8)
                {
                 OPERATION_PROCESS = 0;//初始化操作过程标志
                }
               else
                {
                 POPUP_SCRFL = 0; //关闭弹出窗口
                }
               SCRAPPLY = YES;//请求全屏刷新
              }
             break;
     /*----------------------------------------------------------------------------------------------------*/
     //密码输入操作弹出屏幕 POPUP_SCRFL = 7
     /*----------------------------------------------------------------------------------------------------*/
     case  7:
             if(YESKEY == KEY_UP)
              {
               if(KEY_CHAR_TMP[POPUP_CDX-1] < 9) KEY_CHAR_TMP[POPUP_CDX-1] += 1;
               else KEY_CHAR_TMP[POPUP_CDX-1] = 0;
              }
             else if(YESKEY == KEY_DOWN)
              {
               if(KEY_CHAR_TMP[POPUP_CDX-1] > 0) KEY_CHAR_TMP[POPUP_CDX-1] -= 1;
               else KEY_CHAR_TMP[POPUP_CDX-1] = 9;
              }
             else if(YESKEY == KEY_LEFT)
              {
               if(POPUP_CDX > 1) POPUP_CDX -= 1;
              }
             else if(YESKEY == KEY_RIGHT)
              {
               if(POPUP_CDX < 6) POPUP_CDX += 1;
              }
             else if(YESKEY == KEY_ENTER)
              {
               if(CDX1 == 4)//要进入厂家参数设置屏幕
                {//进入厂家参数设置屏幕密码为590712
                 //修改装置编号的超级密码为325688
                 if((MIMA0 == 5 && MIMA1 == 9 && MIMA2 == 0 && MIMA3 == 7 && MIMA4 == 1 && MIMA5 == 2) ||
                    (MIMA0 == 3 && MIMA1 == 2 && MIMA2 == 5 && MIMA3 == 6 && MIMA4 == 8 && MIMA5 == 8))
                  {
                   if(MIMA0 == 3 && MIMA1 == 2 && MIMA2 == 5 && MIMA3 == 6 && MIMA4 == 8 && MIMA5 == 8)
                    {
                     WRITE_DEVICE_NUMBER_EN = YES;//输入超级密码可以修改装置编号
                    }
                   else
                    {
                     if(DEVICE_NUMBER[0] == 0 && DEVICE_NUMBER[1] == 0 && DEVICE_NUMBER[2] == 0 && DEVICE_NUMBER[3] == 0)
                      {
                       WRITE_DEVICE_NUMBER_EN = YES;//输入普通密码时，如果装置编号全为0，则可修改装置编号
                      }
                     else
                      {
                       WRITE_DEVICE_NUMBER_EN = NO;//输入普通密码时，装置编号不全为0，不可修改装置编号
                      }
                    }
                   POPUP_SCRFL = 0;  //关闭弹出窗口显示
                   SCRFL       = 6;  //进入厂家参数设置屏幕
                   SCRAPPLY    = YES;//请求全屏刷新
                   CDX2        = 1;  //初始化二级菜单光标
                   Bottom_Bar_Screen();//刷新一次底部状态栏，目的是为了代理商情况下显示编号
                  }
                }
              }
             else if(YESKEY == KEY_ESC)
              {
               POPUP_SCRFL = 0;  //关闭弹出窗口
               SCRAPPLY    = YES;//请求全屏刷新
              }
             break;
     /*----------------------------------------------------------------------------------------------------*/
     //操作提示弹出屏幕  POPUP_SCRFL = 8
     /*----------------------------------------------------------------------------------------------------*/
     case  8:
             if(SCRFL == 14) /*设置仪器出厂编号屏幕*/
                {
                 POPUP_SCRFL = 0;  //关闭弹出窗口
                 SCRFL       = 6;  //返回系统参数设置屏幕
                 SCRAPPLY    = YES;//请求全屏刷新
                }
               else if(SCRFL == 12) /*清除存储测试记录屏幕*/
                {
                 if(YESKEY == KEY_ENTER && OPERATE_STATE == 0)
                  {
                   TEST_RECORD_NUMBER = 0;
                   TEST_RECORD_AMOUNT = 0;
                   Mb85rcxx_WriteByte(ee_test_record_number,0);//最新测量记录编号为0
                   Mb85rcxx_WriteByte(ee_test_record_amount,0);//已保存的测量记录数量为0
                   OPERATE_STATE = 0xff;//操作完成
                  }
                 else
                  {
                   POPUP_SCRFL = 0;  //关闭弹出显示屏幕
                   SCRAPPLY    = YES;//请求全屏刷新
                  }                 
                }               
               else
                {
                 POPUP_SCRFL = 0;  //关闭弹出显示屏幕
                 SCRAPPLY    = YES;//请求全屏刷新
                }
             break;
     /*----------------------------------------------------------------------------------------------------*/
     //设置电容器组容量弹出屏幕 POPUP_SCRFL = 11
     /*----------------------------------------------------------------------------------------------------*/
     case 11:
             if(YESKEY == KEY_UP)
              {
               if(KEY_CHAR_TMP[POPUP_CDX+2] < 9) KEY_CHAR_TMP[POPUP_CDX+2] += 1;
               else KEY_CHAR_TMP[POPUP_CDX+2] = 0;
              }
             else if(YESKEY == KEY_DOWN)
              {
               if(KEY_CHAR_TMP[POPUP_CDX+2] > 0) KEY_CHAR_TMP[POPUP_CDX+2] -= 1;
               else KEY_CHAR_TMP[POPUP_CDX+2] = 9;
              }
             else if(YESKEY == KEY_LEFT)
              {
               if(POPUP_CDX > 1) POPUP_CDX -= 1;
               else POPUP_CDX = 19;
              }
             else if(YESKEY == KEY_RIGHT)
              {
               if(POPUP_CDX < 19) POPUP_CDX += 1;
               else POPUP_CDX = 1;
              }
             else if(YESKEY == KEY_ENTER)
              {
               if(POPUP_CDX <= 6) POPUP_CDX = 7;
               else if(POPUP_CDX >= 7 && POPUP_CDX <= 12) POPUP_CDX = 13;
               else if(POPUP_CDX >= 13  && POPUP_CDX <= 18) POPUP_CDX = 19;
               else if(POPUP_CDX == 19)
                {
                 tmp_f  = (float)(PHASE_A_CAP_KEY5*10 + PHASE_A_CAP_KEY6) / 100;
                 tmp_f += PHASE_A_CAP_KEY1*1000 + PHASE_A_CAP_KEY2*100 + PHASE_A_CAP_KEY3*10 + PHASE_A_CAP_KEY4;
                 TEST_DATA.PHASE_A_CAP = tmp_f;//计算A相电容器组电容值
                 tmp_f  = (float)(PHASE_B_CAP_KEY5*10 + PHASE_B_CAP_KEY6) / 100;
                 tmp_f += PHASE_B_CAP_KEY1*1000 + PHASE_B_CAP_KEY2*100 + PHASE_B_CAP_KEY3*10 + PHASE_B_CAP_KEY4;
                 TEST_DATA.PHASE_B_CAP = tmp_f;//计算B相电容器组电容值
                 tmp_f  = (float)(PHASE_C_CAP_KEY5*10 + PHASE_C_CAP_KEY6) / 100;
                 tmp_f += PHASE_C_CAP_KEY1*1000 + PHASE_C_CAP_KEY2*100 + PHASE_C_CAP_KEY3*10 + PHASE_C_CAP_KEY4;
                 TEST_DATA.PHASE_C_CAP = tmp_f;//计算C相电容器组电容值
                 Start_Test_Process();//启动测量过程
                }
              }
             else if(YESKEY == KEY_ESC)
              {
               POPUP_SCRFL = 0;  //关闭弹出屏幕
               SCRAPPLY    = YES;//请求全屏刷新
              }
             break;
     default:break;
    }
  }
 else
  {
   switch(SCRFL)
    {
     /*----------------------------------------------------------------------------------------------------*/
     //开机显示屏幕 SCRFL = 0
     /*----------------------------------------------------------------------------------------------------*/
     case  0:
             break;
     /*----------------------------------------------------------------------------------------------------*/
     //待机屏幕 SCRFL = 1
     /*----------------------------------------------------------------------------------------------------*/
     case  1:
             if(YESKEY == KEY_UP)
              {
               if(CDX1 > 1) CDX1 -= 1;
               else CDX1 = 4;
              }
             else if(YESKEY == KEY_DOWN)
              {
               if(CDX1 < 4) CDX1 += 1;
               else CDX1 = 1;
              }
             else if(YESKEY == KEY_LEFT)
              {
              }
             else if(YESKEY == KEY_RIGHT)
              {
              }
             else if(YESKEY == KEY_ENTER)
              {
               if(CDX1 == 1)/*进入电容电流测量参数设置屏幕*/
                {
                 Mb85rcxx_Read_Test_Set();//读取最后一次保存的试验参数设置值
                 RATED_VOLTAGE_KEY1 = TEST_DATA.RATED_VOLTAGE/100;
                 RATED_VOLTAGE_KEY2 = TEST_DATA.RATED_VOLTAGE%100/10; 
                 RATED_VOLTAGE_KEY3 = TEST_DATA.RATED_VOLTAGE%100%10;
                 Phase_Cap_To_Char();//将电容器组电容量转换为字符便于显示和操作
                 SCRFL = 2;          //进入电容电流测量参数设置屏幕
                }
               else if(CDX1 == 2)/*进入测量记录查询屏幕*/
                {
                 TEST_RECORD_NUMBER_KEY = 1;
                 Mb85rcxx_Read_Test_Record(TEST_RECORD_NUMBER_KEY);//读取测量记录
                 Phase_Cap_To_Char();//将电容器组电容量转换为字符便于显示和操作
                 SCRFL = 4;//进入测量记录查询屏幕
                }
               else if(CDX1 == 3)/*进入实时时钟设置屏幕*/
                {
                 YEAR_KEY    = NOW_TIME.year;//取得当前时间
                 MONTH_KEY   = NOW_TIME.month; 
                 DATE_KEY    = NOW_TIME.date;  
                 WEEKDAY_KEY = NOW_TIME.weekday;   
                 HOUR_KEY    = NOW_TIME.hour;  
                 MINUTE_KEY  = NOW_TIME.minute;
                 SECOND_KEY  = NOW_TIME.second;
                 SCRFL       = 5;//进入实时时钟设置屏幕
                }
               else if(CDX1 == 4)/*进入系统参数设置屏幕*/
                {
                 POPUP_SCRFL = 7;//显示弹出界面7，密码输入弹出框
                 POPUP_CDX   = 1;//初始化弹出屏幕光标索引
                 for(i=0;i<6;i++) KEY_CHAR_TMP[i] = 0;//初始化输入密码
                }
               CDX2     = 1;  //初始化二级菜单光标
               SCRAPPLY = YES;//请求全屏刷新
              }
             else if(YESKEY == KEY_ESC)
              {
              }
             break;
     /*----------------------------------------------------------------------------------------------------*/
     //电容电流测量参数设置屏幕 SCRFL = 2
     /*----------------------------------------------------------------------------------------------------*/
     case  2:
             if(YESKEY == KEY_UP)/*-------------------------------上键-------------------------------------*/
              {
               if(DISPLAY_PT_WIRING_DIAGRAM == NO)//不显示PT接线图的情况下，可改变光标位置
                {
                 if(CDX2 == 1) CDX2 = 24;      //试验编号跳至开始测量
                 else if(CDX2 == 8) CDX2 = 1;  //设备ID跳至试验编号
                 else if(CDX2 == 15) CDX2 = 8; //额定高压跳至设备ID
                 else if(CDX2 == 19) CDX2 = 15;//母线电压跳至额定高压
                 else if(CDX2 == 21) CDX2 = 19;//PT方式跳至母线电压
                 else if(CDX2 == 24) CDX2 = 21;//开始测量跳至PT方式
                 if(CDX2 >= 2 && CDX2 <= 7)//设置试验编号
                  {
                   if(TEST_DATA.TEST_NUMBER[CDX2-2] < 9) TEST_DATA.TEST_NUMBER[CDX2-2]++;
                   else TEST_DATA.TEST_NUMBER[CDX2-2] = 0;
                  }
                 else if(CDX2 >= 9 && CDX2 <= 14)//设置设备ID
                  {
                   if(TEST_DATA.DEVICE_ID[CDX2-9] < 9) TEST_DATA.DEVICE_ID[CDX2-9]++;
                   else TEST_DATA.DEVICE_ID[CDX2-9] = 0;
                  }
                 else if(CDX2 >= 16 && CDX2 <= 18)//设置额定高压
                  {
                   if(CDX2 == 16)
                    {
                     if(RATED_VOLTAGE_KEY1 < 9) RATED_VOLTAGE_KEY1 += 1;
                     else RATED_VOLTAGE_KEY1 = 0;
                    }
                   else if(CDX2 == 17)
                    {
                     if(RATED_VOLTAGE_KEY2 < 9) RATED_VOLTAGE_KEY2 += 1;
                     else RATED_VOLTAGE_KEY2 = 0;
                    }
                   else if(CDX2 == 18)
                    {
                     if(RATED_VOLTAGE_KEY3 < 9) RATED_VOLTAGE_KEY3 += 1;
                     else RATED_VOLTAGE_KEY3 = 0;
                    }
                   TEST_DATA.RATED_VOLTAGE = RATED_VOLTAGE_KEY1 * 100 + RATED_VOLTAGE_KEY2 * 10 + RATED_VOLTAGE_KEY3;
                   if(TEST_DATA.RATED_VOLTAGE == 0)
                    {
                     TEST_DATA.RATED_VOLTAGE = 1;
                     RATED_VOLTAGE_KEY3      = 1;
                    }
                   TEST_DATA.BUS_VOLTAGE = TEST_DATA.RATED_VOLTAGE;//修改PT额定高压后,母线电压随之改变
                   PT_Trans_Rat_Calculation();                     //计算PT实际变比
                  }
                 else if(CDX2 == 20)//设置母线电压
                  {
                   if(TEST_DATA.BUS_VOLTAGE < BUS_VOLTAGE_MAX) TEST_DATA.BUS_VOLTAGE += 1;//母线电压调节最大值限制
                   else TEST_DATA.BUS_VOLTAGE = BUS_VOLTAGE_MIN;
                  }                 
                 else if(CDX2 == 22)//修改PT连接方式
                  {
#if MODEL == MODEL_A //禁用C1PT连接方式
                   if(TEST_DATA.PT_CONNECT_MODE < PT_CONNECT_MODE_MAX-1) TEST_DATA.PT_CONNECT_MODE ++;
                   else TEST_DATA.PT_CONNECT_MODE = 0;
#elif MODEL == MODEL_B //可以选择1PT、C1PT连接方式
                   if(TEST_DATA.PT_CONNECT_MODE == PT_1PT) TEST_DATA.PT_CONNECT_MODE = PT_C1PT;
                   else TEST_DATA.PT_CONNECT_MODE = PT_1PT;
#elif MODEL == MODEL_C //可以选择所有连接方式
                   if(TEST_DATA.PT_CONNECT_MODE < PT_CONNECT_MODE_MAX) TEST_DATA.PT_CONNECT_MODE ++;
                   else TEST_DATA.PT_CONNECT_MODE = 0;
#endif
                   PT_Trans_Rat_Calculation();//计算PT实际变比
                  }
                }
               else if(CDX2 == 23)//光标在接线图位置 
                {
                 DISPLAY_PT_WIRING_DIAGRAM = NO; //关闭显示PT接线图
                 SCRAPPLY                  = YES;//请求一次全屏刷新
                }
              }
             else if(YESKEY == KEY_DOWN)/*-------------------------------下键-------------------------------------*/
              {
               if(DISPLAY_PT_WIRING_DIAGRAM == NO)//不显示PT接线图的情况下，可改变光标位置
                {
                 if(CDX2 == 1) CDX2 = 8;       //试验编号跳至设备ID
                 else if(CDX2 == 8) CDX2 = 15; //设备ID跳至额定高压
                 else if(CDX2 == 15) CDX2 = 19;//额定高压跳至母线电压
                 else if(CDX2 == 19) CDX2 = 21;//母线电压跳至PT方式
                 else if(CDX2 == 21) CDX2 = 24;//PT方式跳至开始测量
                 else if(CDX2 == 24) CDX2 = 1; //开始测量跳至试验编号
                 if(CDX2 >= 2 && CDX2 <= 7)//设置试验编号
                  {
                   if(TEST_DATA.TEST_NUMBER[CDX2-2] > 0) TEST_DATA.TEST_NUMBER[CDX2-2]--;
                   else TEST_DATA.TEST_NUMBER[CDX2-2] = 9;
                  }
                 else if(CDX2 >= 9 && CDX2 <= 14)//设置设备ID
                  {
                   if(TEST_DATA.DEVICE_ID[CDX2-9] > 0) TEST_DATA.DEVICE_ID[CDX2-9]--;
                   else TEST_DATA.DEVICE_ID[CDX2-9] = 9;
                  }
                 else if(CDX2 >= 16 && CDX2 <= 18)//设置额定高压
                  {
                   if(CDX2 == 16)
                    {
                     if(RATED_VOLTAGE_KEY1 > 0) RATED_VOLTAGE_KEY1 -= 1;
                     else RATED_VOLTAGE_KEY1 = 9;
                    }
                   else if(CDX2 == 17)
                    {
                     if(RATED_VOLTAGE_KEY2 > 0) RATED_VOLTAGE_KEY2 -= 1;
                     else RATED_VOLTAGE_KEY2 = 9;
                    }
                   else if(CDX2 == 18)
                    {
                     if(RATED_VOLTAGE_KEY3 > 0) RATED_VOLTAGE_KEY3 -= 1;
                     else RATED_VOLTAGE_KEY3 = 9;
                    }
                   TEST_DATA.RATED_VOLTAGE = RATED_VOLTAGE_KEY1 * 100 +    RATED_VOLTAGE_KEY2 * 10 + RATED_VOLTAGE_KEY3;
                   if(TEST_DATA.RATED_VOLTAGE == 0)
                    {
                     TEST_DATA.RATED_VOLTAGE = 1;
                     RATED_VOLTAGE_KEY3      = 1;
                    }
                   TEST_DATA.BUS_VOLTAGE = TEST_DATA.RATED_VOLTAGE;//修改PT额定高压后,母线电压随之改变
                   PT_Trans_Rat_Calculation();                     //计算PT实际变比                   
                  }
                 else if(CDX2 == 20)//设置母线电压
                  {
                   if(TEST_DATA.BUS_VOLTAGE > BUS_VOLTAGE_MIN) TEST_DATA.BUS_VOLTAGE -= 1;//母线电压调节最小值限制
                   else TEST_DATA.BUS_VOLTAGE = BUS_VOLTAGE_MAX;
                  } 
                 else if(CDX2 == 22)//修改PT连接方式
                  {
#if MODEL == MODEL_A //禁用C1PT连接方式
                   if(TEST_DATA.PT_CONNECT_MODE > 0) TEST_DATA.PT_CONNECT_MODE --;
                   else TEST_DATA.PT_CONNECT_MODE = PT_CONNECT_MODE_MAX-1;
#elif MODEL == MODEL_B //可以选择1PT、C1PT连接方式
                   if(TEST_DATA.PT_CONNECT_MODE == PT_1PT) TEST_DATA.PT_CONNECT_MODE = PT_C1PT;
                   else TEST_DATA.PT_CONNECT_MODE = PT_1PT;
#elif MODEL == MODEL_C //可以选择所有连接方式
                   if(TEST_DATA.PT_CONNECT_MODE > 0) TEST_DATA.PT_CONNECT_MODE --;
                   else TEST_DATA.PT_CONNECT_MODE = PT_CONNECT_MODE_MAX;
#endif
                   PT_Trans_Rat_Calculation();//计算PT实际变比
                  }
                }
               else if(CDX2 == 23)//光标在接线图位置 
                {
                 DISPLAY_PT_WIRING_DIAGRAM = NO; //关闭显示PT接线图
                 SCRAPPLY                  = YES;//请求一次全屏刷新
                }
              }
             else if(YESKEY == KEY_LEFT)/*-------------------------------左键-------------------------------------*/
              {
               if(DISPLAY_PT_WIRING_DIAGRAM == NO)//不显示PT接线图的情况下，可改变光标位置
                {
                 if(CDX2 == 2) CDX2 = 1;       //试验编号数值跳至试验编号
                 else if(CDX2 == 9) CDX2 = 8;  //设备ID数值跳至设备ID
                 else if(CDX2 == 16) CDX2 = 15;//额定高压数值跳至额定高压
                 else if(CDX2 == 20) CDX2 = 19;//母线电压数值跳至母线电压
                 else if(CDX2 == 22) CDX2 = 21;//PT方式数值跳至PT方式
                 else if(CDX2 == 23) CDX2 = 22;//接线图跳至PT方式数值
                 else if(CDX2 > 16 && CDX2 <= 18) CDX2 -= 1;//额定高压光标控制
                 else if(CDX2 >  9 && CDX2 <= 14) CDX2 -= 1;//设备ID光标控制
                 else if(CDX2 >  2 && CDX2 <=  7) CDX2 -= 1;//试验编号光标控制
                }
               else if(CDX2 == 23)//光标在接线图位置 
                {
                 DISPLAY_PT_WIRING_DIAGRAM = NO; //关闭显示PT接线图
                 SCRAPPLY                  = YES;//请求一次全屏刷新
                }
              }
             else if(YESKEY == KEY_RIGHT)/*-------------------------------右键-------------------------------------*/
              {
               if(DISPLAY_PT_WIRING_DIAGRAM == NO)//不显示PT接线图的情况下，可改变光标位置
                {
                 if(CDX2 == 1) CDX2 = 2;       //试验编号跳至试验编号数值
                 else if(CDX2 == 8) CDX2 = 9;  //设备ID跳至设备ID数值
                 else if(CDX2 == 15) CDX2 = 16;//额定高压跳至额定高压数值
                 else if(CDX2 == 19) CDX2 = 20;//母线电压跳至母线电压数值
                 else if(CDX2 == 21) CDX2 = 22;//PT方式跳至PT方式数值
                 else if(CDX2 == 22) CDX2 = 23;//PT方式数值跳至接线图
                 else if(CDX2 >=  2 && CDX2 <  7) CDX2 += 1;//试验编号光标控制
                 else if(CDX2 >=  9 && CDX2 < 14) CDX2 += 1;//设备ID光标控制
                 else if(CDX2 >= 16 && CDX2 < 18) CDX2 += 1;//额定高压光标控制                 
                }
               else if(CDX2 == 23)//光标在接线图位置 
                {
                 DISPLAY_PT_WIRING_DIAGRAM = NO; //关闭显示PT接线图
                 SCRAPPLY                  = YES;//请求一次全屏刷新
                }
              }
             else if(YESKEY == KEY_ENTER)/*-----------------------------确定键-------------------------------------*/
              {
               if(CDX2 == 1 || CDX2 == 8 || CDX2 == 15 || CDX2 == 19 || CDX2 == 21) CDX2 += 1;
               else if(CDX2 >= 2 && CDX2 <= 7)   CDX2 = 1; //设置试验编号
               else if(CDX2 >= 9 && CDX2 <= 14)  CDX2 = 8; //设置设备ID
               else if(CDX2 >= 16 && CDX2 <= 18) CDX2 = 15;//设置额定高压
               else if(CDX2 == 20)               CDX2 = 19;//设置额定高压
               else if(CDX2 == 22)               CDX2 = 21;//修改PT连接方式
               else if(CDX2 == 23)//光标在接线图位置
                {
                 if(DISPLAY_PT_WIRING_DIAGRAM == NO) DISPLAY_PT_WIRING_DIAGRAM = YES;//显示PT接线图
                 else
                  {
                   DISPLAY_PT_WIRING_DIAGRAM = NO; //关闭显示PT接线图
                   SCRAPPLY                  = YES;//请求一次全屏刷新
                  }
                }
               else if(CDX2 == 24)//开始测量
                {
                 if(TEST_DATA.PT_CONNECT_MODE == PT_C1PT)//C1PT连接方式，需要设置电容器组容量
                  {
                   POPUP_SCRFL = 11;//进入设置电容器组容量弹出屏幕
                   POPUP_CDX   = 1;
                   SCRAPPLY    = YES;//请求窗口刷新
                  }
                 else
                  {                 
                   Start_Test_Process();//启动测量过程
                  }
                }
              }
             else if(YESKEY == KEY_ESC)/*-----------------------------取消键-------------------------------------*/
              {
               if(CDX2 == 23 && DISPLAY_PT_WIRING_DIAGRAM == YES)//光标在接线图位置，且正在显示接线图则关闭显示
                {
                 DISPLAY_PT_WIRING_DIAGRAM = NO; //关闭显示PT接线图
                 SCRAPPLY                  = YES;//请求一次全屏刷新
                }
               else
                {
                 DISPLAY_PT_WIRING_DIAGRAM = NO;//关闭显示PT接线图
                 SCRFL    =  1; //返回待机屏幕
                 SCRAPPLY = YES;//请求全屏刷新
                }
              }
             break;
     /*----------------------------------------------------------------------------------------------------*/
     //电容电流测量结果显示屏幕 SCRFL = 3
     /*----------------------------------------------------------------------------------------------------*/
     case  3:
             if(YESKEY == KEY_LEFT)
              {
#if BLUETOOTH == ON //开启了蓝牙上传功能
               if(CDX3 > 1) CDX3 -= 1;
               else CDX3 = 4;
#elif BLUETOOTH == OFF //关闭了蓝牙上传功能
               if(CDX3 > 1) CDX3 -= 1;
               else CDX3 = 3;
#endif
              }
             else if(YESKEY == KEY_RIGHT)
              {
#if BLUETOOTH == ON //开启了蓝牙上传功能
               if(CDX3 < 4) CDX3 += 1;
               else CDX3 = 1;
#elif BLUETOOTH == OFF //关闭了蓝牙上传功能
               if(CDX3 < 3) CDX3 += 1;
               else CDX3 = 1;
#endif
              }
             else if(YESKEY == KEY_ENTER)
              {
               if(CDX3 == 1)//重新测量
                {
                 Start_Test_Process();//启动测量过程
                }
               else if(CDX3 == 2)/*打印数据*/
                {
                 if(PRINTING_FLAG == NO)//未启动打印过程 
                  {
                   if(PRINTER_CONNECT == NO) OPERATION_PROCESS = 0;//打印机未插入,初始化操作过程标志为0
                   else 
                    {
                     OPERATION_PROCESS = 1; //打印机已插入,打印过程开始
                     PRINTING_FLAG     = YES;//打印开始
                    } 
                   POPUP_SCRFL = 3;   //显示弹出界面3，打印操作显示
                   SCRAPPLY    = YES; //请求全屏刷新
                  }
                }
               else if(CDX3 == 3)/*存储数据*/
                {
                 OPERATION_PROCESS = 0;//初始化操作过程标志
                 POPUP_SCRFL       = 4;//显示弹出界面4，测量结果存储操作显示
                }
               else if(CDX3 == 4)//上传
                {
                 if(BLUETOOTH_CONNECT == YES)//蓝牙已连接
                  {
                   Bluetooth_Send_Test_Record();//蓝牙上传测量记录
                   OPERATION_PROCESS = 1;       //蓝牙上传完毕
                  }
                 else //蓝牙未连接
                  {
                   OPERATION_PROCESS = 0;//蓝牙未连接
                  }
                 POPUP_SCRFL = 5;//显示弹出界面5，测量结果蓝牙上传操作显示
                }
               POPUP_CDX = 1;  //初始化弹出界面光标
               SCRAPPLY  = YES;//请求窗口刷新
              }
             else if(YESKEY == KEY_ESC)
              {
               SCRFL    =  2; //返回电容电流测量参数设置屏幕
               SCRAPPLY = YES;//请求全屏刷新
              }
             break;
     /*----------------------------------------------------------------------------------------------------*/
     //测量记录查询屏幕 SCRFL = 4
     /*----------------------------------------------------------------------------------------------------*/
     case  4:
             if(YESKEY == KEY_LEFT)
              {
               if(TEST_RECORD_NUMBER_KEY > 1) TEST_RECORD_NUMBER_KEY -= 1;
               //else TEST_RECORD_NUMBER_KEY = TEST_RECORD_AMOUNT;
               Mb85rcxx_Read_Test_Record(TEST_RECORD_NUMBER_KEY);//读取事件记录信息
               Phase_Cap_To_Char();//将电容器组电容量转换为字符便于显示和操作
              }
             else if(YESKEY == KEY_RIGHT)
              {
               if(TEST_RECORD_NUMBER_KEY < TEST_RECORD_AMOUNT) TEST_RECORD_NUMBER_KEY += 1;
               //else TEST_RECORD_NUMBER_KEY = 1;
               Mb85rcxx_Read_Test_Record(TEST_RECORD_NUMBER_KEY);//读取事件记录信息
               Phase_Cap_To_Char();//将电容器组电容量转换为字符便于显示和操作
              }
             else if(YESKEY == KEY_ENTER)
              {
               if(TEST_RECORD_AMOUNT > 0)//有测试记录时，才能进行操作
                {
                 OPERATION_PROCESS = 0;  //初始化操作过程标志
                 PRINTING_FLAG     = NO; //初始化打印过程标志
                 POPUP_SCRFL       = 6;  //显示弹出界面6，测量记录操作显示
                 POPUP_CDX         = 1;  //初始化弹出界面光标
                 SCRAPPLY          = YES;//请求窗口刷新
                }
              }
             else if(YESKEY == KEY_ESC)
              {
               SCRFL    =  1; //返回待机屏幕
               SCRAPPLY = YES;//请求全屏刷新
              }
             break;
     /*----------------------------------------------------------------------------------------------------*/
     //实时时钟设置屏幕 SCRFL = 5
     /*----------------------------------------------------------------------------------------------------*/
     case  5:
             if(YESKEY == KEY_UP)
              {
               if(CDX2 == 1)//设置年份
                {
                 YEAR_KEY = Bcd_To_Hex(YEAR_KEY);
                 if(YEAR_KEY < 99) YEAR_KEY += 1;
                 else YEAR_KEY = 0;
                 YEAR_KEY    = Hex_To_Bcd(YEAR_KEY);
                 WEEKDAY_KEY = Week_Day(YEAR_KEY,MONTH_KEY,DATE_KEY);
                }
               else if(CDX2 == 2)//设置月份
                {
                 MONTH_KEY = Bcd_To_Hex(MONTH_KEY);
                 if(MONTH_KEY < 12) MONTH_KEY += 1;
                 else MONTH_KEY = 1;
                 MONTH_KEY  = Hex_To_Bcd(MONTH_KEY);
                 if(MONTH_KEY == 0x01 || MONTH_KEY == 0x03 || MONTH_KEY == 0x05 || MONTH_KEY == 0x07 ||
                    MONTH_KEY == 0x08 || MONTH_KEY == 0x10 || MONTH_KEY == 0x12)
                  {
                   if(DATE_KEY == 0x30) DATE_KEY = 0x31;
                  }
                 else if(MONTH_KEY == 0x04 || MONTH_KEY == 0x06 || MONTH_KEY == 0x09 || MONTH_KEY == 0x11)
                  {
                   if(DATE_KEY == 0x31) DATE_KEY = 0x30;
                  }
                 else if(MONTH_KEY == 0x02)//二月
                   {
                    if(Leap_Year(YEAR_KEY))//闰年,二月份有29号
                     {
                      if(DATE_KEY == 0x31 || DATE_KEY == 0x30) DATE_KEY = 0x29;
                     }
                    else//不是闰年,则二月份只有28号
                     {
                      if(DATE_KEY == 0x31 || DATE_KEY == 0x30 || DATE_KEY == 0x29) DATE_KEY = 0x28;
                     }
                   }
                 WEEKDAY_KEY = Week_Day(YEAR_KEY,MONTH_KEY,DATE_KEY);
                }
                else if(CDX2 == 3)//设置日
                {
                 DATE_KEY = Bcd_To_Hex(DATE_KEY);
                 if(MONTH_KEY == 0x01 || MONTH_KEY == 0x03 || MONTH_KEY == 0x05 || MONTH_KEY == 0x07 || 
                    MONTH_KEY == 0x08 || MONTH_KEY == 0x10 || MONTH_KEY == 0x12)
                  {
                   i = 31;
                  }
                 else if(MONTH_KEY == 0x04 || MONTH_KEY == 0x06 || MONTH_KEY == 0x09 || MONTH_KEY == 0x11)
                  {
                   i = 30;
                  }
                 else if(MONTH_KEY == 2)
                  {
                   if(Leap_Year(YEAR_KEY))//闰年,二月份有29号
                    {
                     i = 29;
                    }
                   else//不是闰年,则二月份只有28号
                    {
                     i = 28;
                    }
                  }
                 if(DATE_KEY < i) DATE_KEY += 1;
                 else  DATE_KEY = 1;
                 DATE_KEY    = Hex_To_Bcd(DATE_KEY);
                 WEEKDAY_KEY = Week_Day(YEAR_KEY,MONTH_KEY,DATE_KEY);
                }
               else if(CDX2 == 4)//设置小时
                {
                 HOUR_KEY = Bcd_To_Hex(HOUR_KEY);
                 if(HOUR_KEY < 23) HOUR_KEY += 1;
                 else HOUR_KEY = 0;
                 HOUR_KEY = Hex_To_Bcd(HOUR_KEY);
                }
               else if(CDX2 == 5)//设置分钟
                {
                 MINUTE_KEY = Bcd_To_Hex(MINUTE_KEY);
                 if(MINUTE_KEY < 59) MINUTE_KEY += 1;
                 else MINUTE_KEY = 0;
                 MINUTE_KEY = Hex_To_Bcd(MINUTE_KEY);
                }
               else if(CDX2 == 6)//设置秒钟
                {
                 SECOND_KEY = Bcd_To_Hex(SECOND_KEY);
                 if(SECOND_KEY < 59) SECOND_KEY += 1;
                 else SECOND_KEY = 0;
                 SECOND_KEY = Hex_To_Bcd(SECOND_KEY);
                }
              }
             else if(YESKEY == KEY_DOWN)
              {
               if(CDX2 == 1)//设置年份
                {
                 YEAR_KEY = Bcd_To_Hex(YEAR_KEY);
                 if (YEAR_KEY > 0) YEAR_KEY -= 1;
                 else YEAR_KEY = 99;
                 YEAR_KEY    = Hex_To_Bcd(YEAR_KEY);
                 WEEKDAY_KEY = Week_Day(YEAR_KEY,MONTH_KEY,DATE_KEY);
                }
               else if(CDX2 == 2)//设置月份
                {
                 MONTH_KEY = Bcd_To_Hex(MONTH_KEY);
                 if(MONTH_KEY > 1) MONTH_KEY -= 1;
                 else MONTH_KEY = 12;
                 MONTH_KEY  = Hex_To_Bcd(MONTH_KEY);
                 if(MONTH_KEY == 0x01 || MONTH_KEY == 0x03 || MONTH_KEY == 0x05 || MONTH_KEY == 0x07 || 
                    MONTH_KEY == 0x08 || MONTH_KEY == 0x10 || MONTH_KEY == 0x12)
                  {
                   if(DATE_KEY == 0x30) DATE_KEY = 0x31;
                  }
                 else if(MONTH_KEY == 0x04 || MONTH_KEY == 0x06 || MONTH_KEY == 0x09 || MONTH_KEY == 0x11)
                  {
                   if(DATE_KEY == 0x31) DATE_KEY = 0x30;
                  }
                 else if(MONTH_KEY == 0x02)
                  {
                   if(Leap_Year(YEAR_KEY))//闰年,二月份有29号
                    {
                     if(DATE_KEY == 0x31 || DATE_KEY == 0x30) DATE_KEY = 0x29;
                    }
                   else//不是闰年,则二月份只有28号
                    {
                     if(DATE_KEY == 0x31 || DATE_KEY == 0x30 || DATE_KEY == 0x29) DATE_KEY = 0x28;
                    }
                  }
                 WEEKDAY_KEY = Week_Day(YEAR_KEY,MONTH_KEY,DATE_KEY);                 
                }
               else if(CDX2 == 3)//设置日
                {
                 DATE_KEY = Bcd_To_Hex(DATE_KEY);
                 if(DATE_KEY > 1) DATE_KEY -= 1;
                 else 
                  {
                   if(MONTH_KEY == 0x01 || MONTH_KEY == 0x03 || MONTH_KEY == 0x05 || MONTH_KEY == 0x07 || 
                      MONTH_KEY == 0x08 || MONTH_KEY == 0x10 || MONTH_KEY == 0x12)
                    {
                     DATE_KEY = 31;
                    }
                   else if(MONTH_KEY == 0x04 || MONTH_KEY == 0x06 || MONTH_KEY == 0x09 || MONTH_KEY == 0x11)
                    {
                     DATE_KEY = 30;
                    }
                   else if(MONTH_KEY == 0x2)
                    {
                     if(Leap_Year(YEAR_KEY)) DATE_KEY = 29;
                     else DATE_KEY = 28;
                    }
                  }
                 DATE_KEY    = Hex_To_Bcd(DATE_KEY);
                 WEEKDAY_KEY = Week_Day(YEAR_KEY,MONTH_KEY,DATE_KEY);                 
                }
               else if(CDX2 == 4)//设置小时
                {
                 HOUR_KEY = Bcd_To_Hex(HOUR_KEY);
                 if (HOUR_KEY > 0) HOUR_KEY -= 1;
                 else HOUR_KEY = 23;
                 HOUR_KEY = Hex_To_Bcd(HOUR_KEY);
                }
               else if(CDX2 == 5)//设置分钟
                {
                 MINUTE_KEY = Bcd_To_Hex(MINUTE_KEY);
                 if(MINUTE_KEY > 0) MINUTE_KEY -= 1;
                 else MINUTE_KEY = 59;
                 MINUTE_KEY = Hex_To_Bcd(MINUTE_KEY);      
                }
               else if(CDX2 == 6)//设置秒钟
                {
                 SECOND_KEY = Bcd_To_Hex(SECOND_KEY);
                 if(SECOND_KEY > 0) SECOND_KEY -= 1;
                 else SECOND_KEY = 59;
                 SECOND_KEY = Hex_To_Bcd(SECOND_KEY);
                }
              }
             else if(YESKEY == KEY_LEFT)
              {
               if(CDX2 > 1) CDX2 -= 1;
               else CDX2 = 6;
              }
             else if(YESKEY == KEY_RIGHT)
              {
               if(CDX2 < 6) CDX2 += 1;
               else CDX2 = 1;
              }
             else if(YESKEY == KEY_ENTER)
              {
               RTC_WriteTime(YEAR_KEY,MONTH_KEY,DATE_KEY,WEEKDAY_KEY,HOUR_KEY,MINUTE_KEY,SECOND_KEY);//写入设置时间
               SCRFL    =  1; //返回待机屏幕
               SCRAPPLY = YES;//请求全屏刷新
              }
             else if(YESKEY == KEY_ESC)
              {
               SCRFL    =  1; //返回待机屏幕
               SCRAPPLY = YES;//请求全屏刷新
              }
             break;
     /*----------------------------------------------------------------------------------------------------*/
     //系统参数设置显示屏幕 SCRFL = 6
     /*----------------------------------------------------------------------------------------------------*/
     case  6:
             if(YESKEY == KEY_UP)
              {
               if(CDX2 > 1) CDX2 -= 1;
               else CDX2 = 8;
              }
             else if(YESKEY == KEY_DOWN)
              {
               if(CDX2 < 8) CDX2 += 1;
               else CDX2 = 1;
              }
             else if(YESKEY == KEY_LEFT)
              {
               if(CDX2 == 5) CDX2 = 1;
               else if(CDX2 == 6) CDX2 = 2;
               else if(CDX2 == 7) CDX2 = 3;
               else if(CDX2 == 8) CDX2 = 4;
              }
             else if(YESKEY == KEY_RIGHT)
              {
               if(CDX2 == 1) CDX2 = 5;
               else if(CDX2 == 2) CDX2 = 6;
               else if(CDX2 == 3) CDX2 = 7;
               else if(CDX2 == 4) CDX2 = 8;
              }
             else if(YESKEY == KEY_ENTER)
              {
               if(CDX2 == 1)/*进入仪器电池电压校准屏幕*/
                {
                 Float_To_Char(16.8,&CAL_VBAT_KEY,NO,6);//初始化电池电压设置为16.8V,带小数点转换6位
                 SCRFL = 7;//进入电池校准屏幕
                }
               else if(CDX2 == 2)/*进入零序3U0电压校准屏幕*/
                {
                 Float_To_Char(50,&CAL_VRMS_50HZ_KEY,NO,6);//默认使用50V进行校准,带小数点转换6位
                 SCRFL = 8;//进入3U0 校准屏幕
                }
               else if(CDX2 == 3)/*进入变频逆变电源调试屏幕*/
                {
                 POWER_OUT_FREQENCY_KEY = POWER_OUT_LF;//默认输出低频
                 POWER_OUT_MDLT_RAT_KEY = 500;         //默认调制度50.0%
                 SCRFL = 9;//进入逆变调试屏幕
                }
               else if(CDX2 == 4)/*进入量程档位参数校准屏幕*/
                {
                 GAIN_GEAR_KEY = 0;                //默认增益档位为0,低频通道档位一
                 Fixed_Range_Switch(GAIN_GEAR_KEY);//切换档位，并初始化ADC
                 Float_To_Char(EXAMPLE_IMPEDANCE,&EXAMPLE_IMPEDANCE_KEY,NO,6);//将标准阻抗转成字符,带小数点转换6位
                 Float_To_Char(EXAMPLE_VOLTAGE,&EXAMPLE_VOLTAGE_KEY,NO,6);    //将标准电压转成字符,带小数点转换6位
                 SCRFL = 10;//进入量程校准屏幕
                }
               else if(CDX2 == 5)/*进入测试继电器动作屏幕*/
                {
                 RELAY_DETECT_EN = YES;//进入测试继电器动作屏幕
                 SCRFL           = 11; //进入检继电器屏幕
                }
               else if(CDX2 == 6)/*进入清除存储测量记录屏幕*/
                {
                 SCRFL = 12;//进入清除记录屏幕
                }
               else if(CDX2 == 7)/*进入恢复出厂参数设置屏幕*/
                {
                 RECOVERY_KEY = 0; //默认不选择任何选项
                 SCRFL        = 13;//进入恢复出厂屏幕
                }
               else if(CDX2 == 8)/*进入仪器出厂编号设置屏幕*/
                {
                 if(WRITE_DEVICE_NUMBER_EN == YES) 
                  {
                   for(i=0;i<4;i++)
                    {
                     KEY_CHAR_TMP[i] = DEVICE_NUMBER[i];
                    }
                   SCRFL = 14;//进入编号设置屏幕
                  }
                }
               CDX3     = 1; //初始化三级屏幕光标
               SCRAPPLY = YES;//请求全屏刷新
              }
             else if(YESKEY == KEY_ESC)
              {
               SCRFL    = 1; //返回待机屏幕
               SCRAPPLY = YES;//请求全屏刷新
              }
             break;
     /*----------------------------------------------------------------------------------------------------*/
     //仪器电池电压校准屏幕 SCRFL = 7
     /*----------------------------------------------------------------------------------------------------*/
     case  7:
             if(YESKEY == KEY_UP)/*---------------上键---------------*/
              {
               if(MODIFY_STATUS_KEY == NO)/*调整光标状态*/
                {
                 if(CDX3 < 2) CDX3 = 2;
                 else CDX3 = 1;
                }
               else /*修改设置值状态*/
                {
                 if(CDX3 < 2) //修改设置电池电压值
                  {
                   Float_Char_Adjust(&CAL_VBAT_KEY,6,CDX4,ADD);//数据串加操作
                  }
                }
              }
             else if(YESKEY == KEY_DOWN)/*---------------下键---------------*/
              {
               if(MODIFY_STATUS_KEY == NO)/*调整光标状态*/
                {
                 if(CDX3 < 2) CDX3 = 2;
                 else CDX3 = 1;
                }
               else /*修改设置值状态*/
                {
                 if(CDX3 < 2) //修改设置电池电压值
                  {
                   Float_Char_Adjust(&CAL_VBAT_KEY,6,CDX4,SUB);//数据串减操作
                  }
                }               
              }
             else if(YESKEY == KEY_LEFT)/*---------------左键---------------*/
              {
               if(MODIFY_STATUS_KEY == YES)/*修改设置值状态*/
                {
                 if(CDX3 == 1) //修改设置电池电压值
                  {
                   if(CDX4 == 4) CDX4 = 2;//不允许将光标移动到小数点位置
                   else if(CDX4 > 1) CDX4 -= 1;
                   else CDX4 = 6;
                  } 
                }               
              }
             else if(YESKEY == KEY_RIGHT)/*---------------右键---------------*/
              {
               if(MODIFY_STATUS_KEY == YES)/*修改设置值状态*/
                {
                 if(CDX3 == 1) //修改设置电池电压值
                  {
                   if(CDX4 == 2) CDX4 = 4;//不允许将光标移动到小数点位置
                   else if(CDX4 < 6) CDX4 += 1;
                   else CDX4 = 1;
                  } 
                } 
              }              
             else if(YESKEY == KEY_ENTER)/*---------------确认键---------------*/
              {
               if(MODIFY_STATUS_KEY == NO)/*调整光标状态*/
                {
                 if(CDX3 < 2) //修改设置电池电压值
                  {
                   CDX4 = 1;
                   MODIFY_STATUS_KEY = YES;//进入修改参数状态
                  }
                }
               else /*修改设置值状态*/
                {
                 if(CDX3 < 2) //修改设置电池电压值
                  {
                   MODIFY_STATUS_KEY = NO;//进入光标调整状态
                  }
                }
               if(CDX3 == 2)//校准按钮
                {
                 OPERATE_STATE = 0;//初始化操作状态标志
                 tmp_f   = Char_To_Float(&CAL_VBAT_KEY,6); //将字符数组转换为浮点数
                 coe_tmp = tmp_f / ANALOG_DATA.V_BAT * COE_DATA.COE_V_BAT;//计算校准系数
                 if(coe_tmp  < 0.8f || coe_tmp > 1.2f) OPERATE_STATE = 0xff; //校准系数应该在0.8~1.2的范围内
                 else
                  {
                   COE_DATA.COE_V_BAT = coe_tmp;
                   Mb85rcxx_Write_Float(ee_coe_v_bat,COE_DATA.COE_V_BAT);//保存校准数据
                  }
                 POPUP_SCRFL = 8; //显示弹出界面8，校准完毕屏幕
                 SCRAPPLY    = YES;//请求全屏刷新 
                } 
              }
             else if(YESKEY == KEY_ESC)/*---------------取消键---------------*/
              {
               if(CDX3 < 2 && MODIFY_STATUS_KEY == YES)//设置电池电压值
                {
                 MODIFY_STATUS_KEY = NO;
                }
               else
                {
                 SCRFL    = 6;  //返回厂家参数设置屏幕
                 SCRAPPLY = YES;//请求全屏刷新
                } 
              }
             break;
     /*----------------------------------------------------------------------------------------------------*/
     //零序3U0 电压校准屏幕 SCRFL = 8
     /*----------------------------------------------------------------------------------------------------*/
     case  8:
             if(YESKEY == KEY_UP)/*---------------上键---------------*/
              {
               if(MODIFY_STATUS_KEY == NO)/*调整光标状态*/
                {
                 if(CDX3 < 2) CDX3 = 2;
                 else CDX3 = 1;
                }
               else /*修改设置值状态*/
                {
                 if(CDX3 < 2) //修改设置电池电压值
                  {
                   Float_Char_Adjust(&CAL_VRMS_50HZ_KEY,6,CDX4,ADD);//数据串加操作
                  }
                }
              }
             else if(YESKEY == KEY_DOWN)/*---------------下键---------------*/
              {
               if(MODIFY_STATUS_KEY == NO)/*调整光标状态*/
                {
                 if(CDX3 < 2) CDX3 = 2;
                 else CDX3 = 1;
                }
               else /*修改设置值状态*/
                {
                 if(CDX3 < 2) //修改设置电池电压值
                  {
                   Float_Char_Adjust(&CAL_VRMS_50HZ_KEY,6,CDX4,SUB);//数据串减操作
                  }
                }               
              }
             else if(YESKEY == KEY_LEFT)/*---------------左键---------------*/
              {
               if(MODIFY_STATUS_KEY == YES)/*修改设置值状态*/
                {
                 if(CDX3 == 1) //修改设置电池电压值
                  {
                   if(CDX4 == 4) CDX4 = 2;//不允许将光标移动到小数点位置
                   else if(CDX4 > 1) CDX4 -= 1;
                   else CDX4 = 6;
                  } 
                }               
              }
             else if(YESKEY == KEY_RIGHT)/*---------------右键---------------*/
              {
               if(MODIFY_STATUS_KEY == YES)/*修改设置值状态*/
                {
                 if(CDX3 == 1) //修改设置电池电压值
                  {
                   if(CDX4 == 2) CDX4 = 4;//不允许将光标移动到小数点位置
                   else if(CDX4 < 6) CDX4 += 1;
                   else CDX4 = 1;
                  } 
                } 
              }              
             else if(YESKEY == KEY_ENTER)/*---------------确认键---------------*/
              {
               if(MODIFY_STATUS_KEY == NO)/*调整光标状态*/
                {
                 if(CDX3 < 2) //修改设置电池电压值
                  {
                   CDX4 = 1;
                   MODIFY_STATUS_KEY = YES;//进入修改参数状态
                  }
                }
               else /*修改设置值状态*/
                {
                 if(CDX3 < 2) //修改设置电池电压值
                  {
                   MODIFY_STATUS_KEY = NO;//进入光标调整状态
                  }
                }
               if(CDX3 == 2)//校准按钮
                {
                 OPERATE_STATE = 0;//初始化操作状态标志
                 tmp_f   = Char_To_Float(&CAL_VRMS_50HZ_KEY,6); //将字符数组转换为浮点数
                 coe_tmp = tmp_f / ANALOG_DATA.VRMS_50HZ * COE_DATA.COE_VRMS_50HZ;//计算校准系数
                 if(coe_tmp  < 0.8f || coe_tmp > 1.2f) OPERATE_STATE = 0xff; //校准系数应该在0.8~1.2的范围内
                 else
                  {
                   COE_DATA.COE_VRMS_50HZ = coe_tmp;
                   Mb85rcxx_Write_Float(ee_coe_vrms_50hz,COE_DATA.COE_VRMS_50HZ);//保存校准数据
                  }
                 POPUP_SCRFL = 8; //显示弹出界面8，校准完毕屏幕
                 SCRAPPLY    = YES;//请求全屏刷新 
                } 
              }
             else if(YESKEY == KEY_ESC)/*---------------取消键---------------*/
              {
               if(CDX3 < 2 && MODIFY_STATUS_KEY == YES)//设置电池电压值
                {
                 MODIFY_STATUS_KEY = NO;
                }
               else
                {
                 SCRFL    = 6;  //返回厂家参数设置屏幕
                 SCRAPPLY = YES;//请求全屏刷新
                } 
              }
             break;
     /*----------------------------------------------------------------------------------------------------*/
     //变频逆变电源调试屏幕 SCRFL = 9
     /*----------------------------------------------------------------------------------------------------*/
     case  9:
             if(YESKEY == KEY_UP)/*---------------上键---------------*/
              {
               if(MODIFY_STATUS_KEY == NO)/*调整光标状态*/
                {
                 if(Get_Bit(SPWMPOWER_CTRL_FLAG,SPWMPOWER_TURN_ON_OFF_BIT) == 0)//逆变源停止时，可以调节光标位置
                  {
                   if(CDX3 > 1) CDX3 -= 1;
                   else CDX3 = 3;
                  } 
                }
               else /*修改设置值状态*/
                {
                 if(CDX3 == 1)//修改输出频率
                  {
                   if(POWER_OUT_FREQENCY_KEY == POWER_OUT_LF) POWER_OUT_FREQENCY_KEY = POWER_OUT_HF;
                   else POWER_OUT_FREQENCY_KEY = POWER_OUT_LF;
                  }
                 else if(CDX3 == 2)//修改调制度值
                  {
                   if(POWER_OUT_MDLT_RAT_KEY < 1000) POWER_OUT_MDLT_RAT_KEY += 10;
                   else POWER_OUT_MDLT_RAT_KEY = 10; 
                  }
                }
              }
             else if(YESKEY == KEY_DOWN)/*---------------下键---------------*/
              {
               if(MODIFY_STATUS_KEY == NO)/*调整光标状态*/
                {
                 if(Get_Bit(SPWMPOWER_CTRL_FLAG,SPWMPOWER_TURN_ON_OFF_BIT) == 0)//逆变源停止时，可以调节光标位置
                  {
                   if(CDX3 < 3) CDX3 += 1;
                   else CDX3 = 1;
                  } 
                }
               else /*修改设置值状态*/
                {
                 if(CDX3 == 1)//修改输出频率
                  {
                   if(POWER_OUT_FREQENCY_KEY == POWER_OUT_LF) POWER_OUT_FREQENCY_KEY = POWER_OUT_HF;
                   else POWER_OUT_FREQENCY_KEY = POWER_OUT_LF;
                  }
                 else if(CDX3 == 2)//修改调制度值
                  {
                   if(POWER_OUT_MDLT_RAT_KEY > 10) POWER_OUT_MDLT_RAT_KEY -= 10;
                   else POWER_OUT_MDLT_RAT_KEY = 1000; 
                  }
                }
              }
             else if(YESKEY == KEY_LEFT)/*---------------左键---------------*/
              {
              }
             else if(YESKEY == KEY_RIGHT)/*---------------右键---------------*/
              {
              }
             else if(YESKEY == KEY_ENTER)/*---------------确认键---------------*/
              {
               if(MODIFY_STATUS_KEY == NO)/*调整光标状态*/
                {
                 if(CDX3 < 3) //修改输出频率及调制度
                  {
                   CDX4 = 1;
                   MODIFY_STATUS_KEY = YES;//进入修改参数状态
                  }
                }
               else /*修改设置值状态*/
                {
                 if(CDX3 < 3) //修改输出频率及调制度
                  {
                   MODIFY_STATUS_KEY = NO;//进入光标调整状态
                  }
                }
               if(CDX3 == 3)/*启动、停止逆变源*/
                {
                 if(Get_Bit(SPWMPOWER_CTRL_FLAG,SPWMPOWER_TURN_ON_OFF_BIT))//逆变源已启动
                  {
                   SPWMPOWER_CTRL_FLAG = SPWMPOWER_SOFT_STOP;//软停机
                   JDQ_OUT_OFF;//释放输出继电器
                  }
                 else
                  {
                   SPWMPOWER_CTRL_FLAG = SPWMPOWER_SOFT_START;//软启动
                   JDQ_OUT_ON;//吸合输出继电器
                  }
                 SPWMPOWER_MDLT_RAT    = POWER_OUT_MDLT_RAT_KEY;      //输出调制度
                 SPWMPOWER_MDLT_FREQ   = POWER_OUT_FREQENCY_KEY * 100;//输出所需频率
                 SP_MODBUS_COMMAND     = MULTI_WRITE;                 //多字节写操作
                 SP_MODBUS_START_ADD   = WRITE_STA_ADDR;              //数据表起始地址
                 SP_MODBUS_DATA_NUMBER = 3;                           //写入三个输出控制寄存器
                 SPWM_Power_Command_Send();                           //发送指令，控制变频逆变电源
                }
              }
             else if(YESKEY == KEY_ESC)/*---------------取消键---------------*/
              {
               if(CDX3 < 3 && MODIFY_STATUS_KEY == YES)/*修改输出频率及调制度*/
                {
                 MODIFY_STATUS_KEY = NO;//返回移动光标状态
                }
               else
                {
                 /*逆变电源启动后，将不能退出此屏，需要停机后才能退出*/
                 if(Get_Bit(SPWMPOWER_CTRL_FLAG,SPWMPOWER_TURN_ON_OFF_BIT) == 0)
                  {
                   SCRFL    = 6; //返回厂家参数设置屏幕
                   SCRAPPLY = YES;//请求全屏刷新
                  }
                }
              }
             break;
     /*----------------------------------------------------------------------------------------------------*/
     //量程校准屏幕 SCRFL = 10
     /*----------------------------------------------------------------------------------------------------*/
     case 10:
             if(YESKEY == KEY_UP)/*-----------------------------上键---------------------------------*/
              {
               if(MODIFY_STATUS_KEY == NO)/*调整光标状态*/
                {
                 if(Get_Bit(SPWMPOWER_CTRL_FLAG,SPWMPOWER_TURN_ON_OFF_BIT))/*变频逆变电源已启动输出*/
                  {
                   if(CDX3 > 2)//启用输出时不能更换档位、不能设置额定阻抗
                    {
                     if(CDX3 > 3)//光标在“设置电压”之后
                      {
                       if(GAIN_GEAR_KEY < 6)//档位4之前的档位需要设置电压
                        {
                         CDX3 -= 1;
                        }
                       else
                        {
                         if(CDX3 == 4 && CALI_PROCESS_FLAG >= 4) CDX3 = 5;//当无需设置电压值且光标在启动输出时，光标移动至开始校准
                         else if(CDX3 == 5) CDX3 = 4;
                        }                       
                      }
                     else 
                      {
                       if(CALI_PROCESS_FLAG >= 4) CDX3 = 5;//校准过程调流完毕，光标移动至"开始校准"按钮
                       else CDX3 = 4;//校准过程正在调整电流中，光标移动至"启动输出"按钮
                      } 
                    }
                  }
                 else /*变频逆变电源停机状态*/
                  {
                   if(CDX3 == 4) CDX3 = 2;//停止输出时不能不能设置电压值
                   else if(CDX3 > 1) CDX3 -= 1;
                   else CDX3 = 4;
                  }                  
                }
               else /*修改设置值状态*/
                {
                 if(CDX3 == 2)/*修改额定阻抗*/
                  {
                   Float_Char_Adjust(&EXAMPLE_IMPEDANCE_KEY,6,CDX4,ADD);//数据串加操作
                  }
                 else if(CDX3 == 3)/*修改设置电压*/
                  {
                   Float_Char_Adjust(&EXAMPLE_VOLTAGE_KEY,6,CDX4,ADD);//数据串加操作
                  }                 
                }
              }
             else if(YESKEY == KEY_DOWN)/*-------------------------下键------------------------------*/
              {
               if(MODIFY_STATUS_KEY == NO)/*调整光标状态*/
                {
                 if(Get_Bit(SPWMPOWER_CTRL_FLAG,SPWMPOWER_TURN_ON_OFF_BIT))/*变频逆变电源已启动输出*/
                  {
                   if(CDX3 > 2)//启用输出时不能更换档位、不能设置额定阻抗
                    {
                     if(CALI_PROCESS_FLAG >= 4)//校准过程调流完毕，光标可以移动至"开始校准"按钮
                      {
                       if(CDX3 < 5) CDX3 += 1;
                       else
                        {
                         if(GAIN_GEAR_KEY < 6)//档位4之前的档位需要设置电压
                          {
                           CDX3 = 3;
                          }
                         else
                          {
                           CDX3 = 4;
                          }                         
                        }
                      }
                     else //校准过程调流未完成，光标不可以移动至"开始校准"按钮
                      {
                       if(CDX3 < 4) CDX3 += 1;
                       else
                        {
                         if(GAIN_GEAR_KEY < 6)//档位4之前的档位需要设置电压
                          {
                           CDX3 = 3;
                          }
                         else
                          {
                           CDX3 = 4;
                          }                         
                        }
                      } 
                    }
                  }
                 else /*变频逆变电源停机状态*/
                  {
                   if(CDX3 == 2) CDX3 = 4;//停止输出时不能不能设置电压值
                   else if(CDX3 < 4) CDX3 += 1;
                   else CDX3 = 1;
                  }
                }
               else /*修改设置值状态*/
                {
                 if(CDX3 == 2)/*修改额定阻抗*/
                  {
                   Float_Char_Adjust(&EXAMPLE_IMPEDANCE_KEY,6,CDX4,SUB);//数据串减操作
                  }
                 else if(CDX3 == 3)/*修改设置电压*/
                  {
                   Float_Char_Adjust(&EXAMPLE_VOLTAGE_KEY,6,CDX4,SUB);//数据串减操作
                  }
                }                
              }
             else if(YESKEY == KEY_LEFT)/*--------------------------左键--------------------------------*/
              {
               if(MODIFY_STATUS_KEY == NO)/*调整光标状态*/
                {

                }
               else /*修改设置值状态*/
                {
                 if(CDX3 == 1)/*调整档位*/
                  {
                   if(GAIN_GEAR_KEY > 0)
                    {
                     GAIN_GEAR_KEY -= 1;
                     Fixed_Range_Switch(GAIN_GEAR_KEY);//切换档位，并初始化ADC
                     CALIBRATION_DATA_NO = 0;          //初始化校准数据编号，重新采样并计算
                     Float_To_Char(EXAMPLE_IMPEDANCE,&EXAMPLE_IMPEDANCE_KEY,NO,6);//将标准阻抗转成字符,带小数点转换6位
                     Float_To_Char(EXAMPLE_VOLTAGE,&EXAMPLE_VOLTAGE_KEY,NO,6);    //将标准电压转成字符,带小数点转换6位
                    } 
                  }
                 else if(CDX3 == 2)/*修改额定阻抗*/
                  {
                   if(CDX4 > 1) CDX4 -= 1;
                   else CDX4 = 6;
                  }
                 else if(CDX3 == 3)/*修改设置电压*/
                  {
                   if(CDX4 > 1) CDX4 -= 1;
                   else CDX4 = 6;
                  }                 
                }     
              }
             else if(YESKEY == KEY_RIGHT)/*-------------------------右键-------------------------------*/
              {
               if(MODIFY_STATUS_KEY == NO)/*调整光标状态*/
                {

                }
               else /*修改设置值状态*/
                {
                 if(CDX3 == 1)/*调整档位*/
                  {
                   if(GAIN_GEAR_KEY < 11)
                    {
                     GAIN_GEAR_KEY += 1;
                     Fixed_Range_Switch(GAIN_GEAR_KEY);//切换档位，并初始化ADC
                     CALIBRATION_DATA_NO = 0;          //初始化校准数据编号，重新采样并计算
                     Float_To_Char(EXAMPLE_IMPEDANCE,&EXAMPLE_IMPEDANCE_KEY,NO,6);//将标准阻抗转成字符,带小数点转换6位
                     Float_To_Char(EXAMPLE_VOLTAGE,&EXAMPLE_VOLTAGE_KEY,NO,6);    //将标准电压转成字符,带小数点转换6位
                    } 
                  }
                 else if(CDX3 == 2)/*修改额定阻抗*/
                  {
                   if(CDX4 < 6) CDX4 += 1;
                   else CDX4 = 1;
                  }
                 else if(CDX3 == 3)/*修改设置电压*/
                  {
                   if(CDX4 < 6) CDX4 += 1;
                   else CDX4 = 1;
                  } 
                }
              }
             else if(YESKEY == KEY_ENTER)/*------------------------确认键-------------------------------*/
              {
               if(MODIFY_STATUS_KEY == NO)/*调整光标状态*/
                {
                 if(CDX3 < 4) //修改档位、额定电阻、设置电压值
                  {
                   CDX4 = 1;
                   MODIFY_STATUS_KEY = YES;//进入修改参数状态
                  }
                 else if(CDX3 == 4)/*光标在启动、停止输出按钮*/
                  {
                   if(Get_Bit(SPWMPOWER_CTRL_FLAG,SPWMPOWER_TURN_ON_OFF_BIT))/*逆变源已启动*/
                    {
                     SPWMPOWER_CTRL_FLAG   = SPWMPOWER_SOFT_STOP;//软停机
                     SP_MODBUS_COMMAND     = SINGLE_WRITE;       //单节写操作
                     SP_MODBUS_START_ADD   = WRITE_STA_ADDR;     //数据表起始地址
                     SP_MODBUS_DATA_NUMBER = 1;                  //写入1个输出控制寄存器
                     SPWM_Power_Command_Send();                  //发送指令，控制变频逆变电源软停机
                     JDQ_OUT_OFF;           //释放输出继电器
                     CALI_PROCESS_EN   = NO;//停止量程校准过程输出电流控制
                     CALI_PROCESS_FLAG = 0; //初始化控制标志
                    }
                   else /*变频逆变电源未启动*/
                    {
                     CALI_PROCESS_EN   = YES;//启动量程校准过程输出电流控制
                     CALI_PROCESS_FLAG = 0;  //初始化控制标志
                    }
                  }
                 else if(CDX3 == 5)/*光标在开始校准按钮*/
                  {
                   Range_Calibration_Coe_Calculate(GAIN_GEAR_KEY);//计算相应档位校准系数并保存
                   OPERATE_STATE = 0;  //校准成功
                   POPUP_SCRFL   = 8;  //显示弹出界面8
                   SCRAPPLY      = YES;//请求全屏刷新
                  }
                }
               else /*修改设置值状态*/
                {
                 if(CDX3 < 4) //修改档位、额定电阻、设置电压值
                  {
                   MODIFY_STATUS_KEY = NO;//退出调整光标状态
                   if(CDX3 == 2)/*修改额定阻抗*/
                    {
                     EXAMPLE_IMPEDANCE = Char_To_Float(&EXAMPLE_IMPEDANCE_KEY,6);//将设置的额定阻抗值转换为浮点数                     
                     if(EXAMPLE_IMPEDANCE >= EXAMPLE_IMPEDANCE_MIN && EXAMPLE_IMPEDANCE <= EXAMPLE_IMPEDANCE_MAX)/*额定阻抗值在规定范围内*/
                      {
                       CDX3 = 4;//额定阻抗值设置正确后，光标自动移动至启动输出按钮
                      }
                     else /*设置的额定阻抗值超范围*/
                      {
                       OPERATE_STATE = 0;  //标示额定阻抗设置超范围
                       POPUP_SCRFL   = 10; //显示弹出界面10，额定阻抗值输入错误屏幕
                       SCRAPPLY      = YES;//请求全屏刷新
                      }
                    }
                   else if(CDX3 == 3)/*修改设置电压*/
                    {
                     EXAMPLE_VOLTAGE = Char_To_Float(&EXAMPLE_VOLTAGE_KEY,6);//将设置的设置电压值转换为浮点数
                     if(EXAMPLE_VOLTAGE >= EXAMPLE_VOLTAGE_MIN && EXAMPLE_VOLTAGE <= EXAMPLE_VOLTAGE_MAX)/*设置电压值在规定范围内*/
                      {
                       CDX3 = 5;//电压值设置正确后，光标自动移动至开始校准按钮
                      }
                     else /*设置的设置电压超范围*/
                      {
                       OPERATE_STATE = 1;  //标示设置电压设置超范围
                       POPUP_SCRFL   = 10; //显示弹出界面10，设置电压输入错误屏幕
                       SCRAPPLY      = YES;//请求全屏刷新
                      }
                    }
                  }
                }               
              }
             /*------------------------------------------取消键---------------------------------------*/ 
             else if(YESKEY == KEY_ESC && Get_Bit(SPWMPOWER_CTRL_FLAG,SPWMPOWER_TURN_ON_OFF_BIT) == 0)/*变频逆变电源启动时不可退出此屏幕*/
              {
               if(MODIFY_STATUS_KEY == NO)/*调整光标状态*/
                {
                 Fixed_Range_Switch(1);//初始化电压、电流增益档位及ADC，使用高频档位，从而提高50HZ信号采样速度
                 JDQ_OUT_OFF;          //释放输出继电器
                 SCRFL    = 6;         //返回厂家参数设置屏幕
                 SCRAPPLY = YES;       //请求全屏刷新
                }
               else /*修改设置值状态*/
                {
                 MODIFY_STATUS_KEY = NO;//返回移动光标状态
                }
              }
             break;
     /*----------------------------------------------------------------------------------------------------*/
     //检继电器屏幕 SCRFL = 11
     /*----------------------------------------------------------------------------------------------------*/
     case 11:
             if(YESKEY == KEY_RIGHT || YESKEY == KEY_LEFT)
              {
               if(CDX3 == 1)
                {
                 if(Get_Bit(RELAY_CTRL,JDQ_OUT)) Clr_Bit(RELAY_CTRL,JDQ_OUT);
                 else Set_Bit(RELAY_CTRL,JDQ_OUT);
                }
               else if(CDX3 == 2)
                {
                 if(Get_Bit(RELAY_CTRL,JDQ_FY)) Clr_Bit(RELAY_CTRL,JDQ_FY);
                 else Set_Bit(RELAY_CTRL,JDQ_FY);
                }
               else if(CDX3 == 3)
                {
                 if(Get_Bit(RELAY_CTRL,JDQ_QH)) Clr_Bit(RELAY_CTRL,JDQ_QH);
                 else Set_Bit(RELAY_CTRL,JDQ_QH);
                }
               else if(CDX3 == 4)
                {
                 if(Get_Bit(RELAY_CTRL,JDQ_VZY)) Clr_Bit(RELAY_CTRL,JDQ_VZY);
                 else Set_Bit(RELAY_CTRL,JDQ_VZY);
                }
               else if(CDX3 == 5)
                {
                 if(Get_Bit(RELAY_CTRL,JDQ_IZY)) Clr_Bit(RELAY_CTRL,JDQ_IZY);
                 else Set_Bit(RELAY_CTRL,JDQ_IZY);
                }
              }
             else if(YESKEY == KEY_UP)
              {
               if(CDX3 > 1) CDX3 -= 1;
               else CDX3 = 5;
              }
             else if(YESKEY == KEY_DOWN)
              {
               if(CDX3 < 5) CDX3 += 1;
               else CDX3 = 1;
              }
             else if(YESKEY == KEY_ESC || YESKEY == KEY_ENTER)
              {
               JDQ_OUT_OFF;
               JDQ_FY_OFF;
               JDQ_QH_LF;
               JDQ_VZY_OFF;
               JDQ_IZY_OFF;
               RELAY_DETECT_EN = NO; //关闭继电器检测
               RELAY_CTRL      = 0;  //清零继电器控制寄存器
               SCRFL           = 6;  //返回厂家参数设置屏幕
               SCRAPPLY        = YES;//请求全屏刷新
              }
             break;
     /*----------------------------------------------------------------------------------------------------*/
     //清除存储测量记录屏幕 SCRFL = 12
     /*----------------------------------------------------------------------------------------------------*/
     case 12:
             if(YESKEY == KEY_ENTER)/*---------------------确认键---------------------*/
              {
               OPERATE_STATE = 0;
               POPUP_SCRFL   = 8; //显示弹出界面8，操作提示屏幕
               SCRAPPLY      = YES;//请求全屏刷新
              }
             else if(YESKEY == KEY_ESC)/*---------------------取消键---------------------*/
              {
               SCRFL    = 6; //返回厂家参数设置屏幕
               SCRAPPLY = YES;//请求全屏刷新
              }
             break;
     /*----------------------------------------------------------------------------------------------------*/
     //恢复出厂参数设置屏幕 SCRFL = 13
     /*----------------------------------------------------------------------------------------------------*/
     case 13:
             if(YESKEY == KEY_UP)/*---------------------上键---------------------*/
              {
               if(CDX3 > 1) CDX3 -= 1;
               else CDX3 = 5;
              }
             else if(YESKEY == KEY_DOWN)/*---------------------下键---------------------*/
              {
               if(CDX3 < 5) CDX3 += 1;
               else CDX3 = 1;
              }
             else if(YESKEY == KEY_LEFT)/*---------------------左键---------------------*/
              {

              }
             else if(YESKEY == KEY_RIGHT)/*---------------------右键---------------------*/
              {

              }
             else if(YESKEY == KEY_ENTER)/*---------------------确认键---------------------*/
              {
               if(CDX3 < 5)/*选择要恢复的选项*/
                {
                 if(Get_Bit(RECOVERY_KEY,CDX3-1)) Clr_Bit(RECOVERY_KEY,CDX3-1);
                 else Set_Bit(RECOVERY_KEY,CDX3-1);
                }
               else if(CDX3 == 5)/*光标在恢复出厂参数按钮*/
                {
                 if(Get_Bit(RECOVERY_KEY,TESTING_SET_BIT)) Mb85rcxx_Write_Test_Set_Default();//测量参数设置恢复到出厂默认值
                 Mb85rcxx_Write_Correction_Coefficient_Default(RECOVERY_KEY);//按照选择恢复相应参数的出厂默认值
                 Mb85rcxx_Read_Correction_Coefficient();//读取模拟量校准系数
                 POPUP_SCRFL = 8;  //弹出操作提示屏幕
                 SCRAPPLY    = YES;//请求全屏刷新
               }
              }
             else if(YESKEY == KEY_ESC)/*---------------------取消键---------------------*/
              {
               SCRFL    = 6; //返回厂家参数设置屏幕
               SCRAPPLY = YES;//请求全屏刷新
              }
             break;
     /*----------------------------------------------------------------------------------------------------*/
     //仪器出厂编号设置屏幕 SCRFL = 14
     /*----------------------------------------------------------------------------------------------------*/
     case 14:
             if(YESKEY == KEY_UP)/*---------------上键---------------*/
              {
               if(KEY_CHAR_TMP[CDX3-1] < 9) KEY_CHAR_TMP[CDX3-1] += 1;
               else KEY_CHAR_TMP[CDX3-1] = 0;
              }
             else if(YESKEY == KEY_DOWN)/*---------------下键---------------*/
              {
               if(KEY_CHAR_TMP[CDX3-1] > 0) KEY_CHAR_TMP[CDX3-1] -= 1;
               else KEY_CHAR_TMP[CDX3-1] = 9;
              }
             else if(YESKEY == KEY_LEFT)/*---------------左键---------------*/
              {
               if(CDX3 > 1) CDX3 -= 1;
               else CDX3 = 4;
              }
             else if(YESKEY == KEY_RIGHT)/*---------------右键---------------*/
              {
               if(CDX3 < 4) CDX3 += 1;
               else CDX3 = 1;
              }
             else if(YESKEY == KEY_ENTER)/*---------------确认键---------------*/
              {
               for(i=0;i<4;i++)
                {
                 DEVICE_NUMBER[i] = KEY_CHAR_TMP[i];
                 Mb85rcxx_WriteByte(ee_device_number0+i,DEVICE_NUMBER[i]);//写入装置编号
                }
               Bottom_Bar_Screen();//刷新一次底部状态栏，以显示设置的新的装置编号
               POPUP_SCRFL = 8;    //弹出操作提示屏幕
               SCRAPPLY    = YES;  //请求全屏刷新
              }
             else if(YESKEY == KEY_ESC)/*---------------取消键---------------*/
              {
               SCRFL    = 6;  //返回系统参数设置屏幕
               SCRAPPLY = YES;//请求全屏刷新
              }
             break;
     default:break;
    }
  }
 DUPDATE = YES;//请求数据刷新 
 YESKEY  = 0;
}
