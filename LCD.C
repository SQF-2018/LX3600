#include "stm32f10x.h"
#include "IOCONFIG.H"
#include "QJBL.H"
#include "LCDDRIVER.H"
#include "RTC.H"


/*******************************************************************************************************************************
测试数据静态数据显示程序
*******************************************************************************************************************************/
void Test_Data_Screen(unsigned char screen)
{
 LCD_Area_Fill(0,28,X_PIXEL_MAX,443,WHITE);//除顶部、底部状态栏位置以外填充白色
 LCD_BACK_COLOR = WHITE;                   //记录当前背景色
 /*--------------------------------------------显示测量结果及测量记录查询结果公共数据部分-------------------------------------*/
 /*显示电容电流测量结果或者显示测试记录时有保存过的测试记录*/
 if(screen == TESTING_SCREEN || (screen == TEST_RECORD_SCREEN && TEST_RECORD_AMOUNT != 0))
  {
   LCD_Draw_Rectangle_Window(  8, 36,631,212,8,BLACK,GBLUE);//绘制黑框灰蓝底圆角窗口，用于显示表格
   LCD_Draw_Line( 10, 66,629, 66,YELLOW); //画横线
   LCD_Draw_Line( 10, 95,629, 95,YELLOW); //画横线
   LCD_Draw_Line( 10,124,629,124,YELLOW); //画横线
   LCD_Draw_Line( 10,153,629,153,YELLOW); //画横线
   LCD_Draw_Line( 10,182,629,182,YELLOW); //画横线
   LCD_Draw_Line(130, 38,130,210,YELLOW); //画竖线
   LCD_Draw_Line(319, 38,319,181,YELLOW); //画竖线
   LCD_Draw_Line(440, 38,440,181,YELLOW); //画竖线   
   LCD_Put_String( 22, 40,"试验编号",FONT_24,YELLOW,GBLUE,1);
   LCD_Put_String( 22, 69,"设备名称",FONT_24,YELLOW,GBLUE,2);
   LCD_Put_String( 22, 98,"额定高压",FONT_24,YELLOW,GBLUE,3);
   LCD_Put_String( 22,127,"母线电压",FONT_24,YELLOW,GBLUE,4);
   LCD_Put_String( 22,156,"P T 方式",FONT_24,YELLOW,GBLUE,5);
   LCD_Put_String( 22,185,"测量时间",FONT_24,YELLOW,GBLUE,6);
   LCD_Put_String(143,185,"20  年  月  日  星期      时  分  秒",FONT_24,WHITE,GBLUE,6);   
   LCD_Put_String(332, 40,"P T 变比",FONT_24,YELLOW,GBLUE,1);
   LCD_Put_String(332, 69,"零序 3U0",FONT_24,YELLOW,GBLUE,2);   
   LCD_Draw_Rectangle_Window(  8,220,631,387,8,BLACK,GBLUE);//绘制黑框灰蓝底圆角窗口，用于显示测量结果
   LCD_Draw_Line( 10,303,629,303,YELLOW); //画横线
   LCD_Draw_Line( 10,304,629,304,YELLOW); //画横线
   LCD_Put_String( 70,246,"电容容量C:",FONT_32,YELLOW,GBLUE,7);
   LCD_Put_String( 70,330,"电容电流I:",FONT_32,YELLOW,GBLUE,7);
   LCD_Put_String(521,246,"uF",FONT_48,YELLOW,GBLUE,7);
   LCD_Put_String(521,330,"A", FONT_48,YELLOW,GBLUE,7);
  }
}

/*------------------------------------------------------------------------------------------------------------------------------
测试数据动态数据显示程序
------------------------------------------------------------------------------------------------------------------------------*/
void Test_Data_Dupscr(unsigned char screen)
{
 unsigned char i,tmp;
 unsigned char *p;
 /*显示测试实时数据或者显示测试记录时有保存过的测试记录*/
 if(screen == TESTING_SCREEN || (screen == TEST_RECORD_SCREEN && TEST_RECORD_AMOUNT != 0))
  {
   /*显示试验编号和设备ID*/
   for(i=0;i<6;i++)
    {
     LCD_Put_Char(TEST_DATA.TEST_NUMBER[i]+0x10,143+i*12, 40,FONT_24,WHITE,GBLUE,1);
     LCD_Put_Char(TEST_DATA.DEVICE_ID[i]  +0x10,143+i*12, 69,FONT_24,WHITE,GBLUE,2);
    }
   /*显示额定高压数值*/
   LCD_IntNum(TEST_DATA.RATED_VOLTAGE,143, 98,NO,3,1,FONT_24,WHITE,GBLUE,3);
   LCD_Put_String(195, 98,"kV",FONT_24,WHITE,GBLUE,3);
   /*显示母线电压数值*/
   LCD_IntNum(TEST_DATA.BUS_VOLTAGE,143,127,NO,3,1,FONT_24,WHITE,GBLUE,4);
   LCD_Put_String(195,127,"kV",FONT_24,WHITE,GBLUE,4);
   /*显示PT连接方式*/
   switch(TEST_DATA.PT_CONNECT_MODE)
    {
     case PT_3PT ://3PT
     default     ://3PT
                  LCD_Put_String(143,156,"3PT ",FONT_24,WHITE,GBLUE,5);
                  break;
     case PT_3PT1://3PT1
                  LCD_Put_String(143,156,"3PT1",FONT_24,WHITE,GBLUE,5);
                  break;
     case PT_3PT2://3PT2
                  LCD_Put_String(143,156,"3PT2",FONT_24,WHITE,GBLUE,5);
                  break;
     case PT_4PT ://4PT
                  LCD_Put_String(143,156,"4PT ",FONT_24,WHITE,GBLUE,5);
                  break;
     case PT_4PT1://4PT1
                  LCD_Put_String(143,156,"4PT1",FONT_24,WHITE,GBLUE,5);
                  break;
     case PT_4PT2://4PT2
                  LCD_Put_String(143,156,"4PT2",FONT_24,WHITE,GBLUE,5);
                  break;
     case PT_1PT ://1PT
                  LCD_Put_String(143,156,"1PT ",FONT_24,WHITE,GBLUE,5);
                  break;
     case PT_C1PT://C1PT
                  LCD_Put_String(143,156,"C1PT",FONT_24,WHITE,GBLUE,5);
                  break;
    }
   /*显示PT变比*/
   LCD_FloatNum(TEST_DATA.PT_TRANS_RAT,453, 40,NO,5,FONT_24,WHITE,GBLUE,1);
   /*显示PT开口电压*/
   if(ANALOG_DATA.VRMS_50HZ < 10) LCD_IntNum(ANALOG_DATA.VRMS_50HZ * 1000,453, 69,NO,5,3,FONT_24,WHITE,GBLUE,2);
   else LCD_IntNum(ANALOG_DATA.VRMS_50HZ * 100,453, 69,NO,5,2,FONT_24,WHITE,GBLUE,2);
   LCD_Put_String(530, 69,"V",FONT_24,WHITE,GBLUE,2); 
   if(TEST_DATA.PT_CONNECT_MODE == PT_C1PT)/*C1PT方式需要显示A、B、C相补偿电容器组电容量*/
    {
     LCD_Put_String(332, 98,"CA电容量",FONT_24,YELLOW,GBLUE,3);
     LCD_Put_String(332,127,"CB电容量",FONT_24,YELLOW,GBLUE,4);
     LCD_Put_String(332,156,"CC电容量",FONT_24,YELLOW,GBLUE,5);
     LCD_Put_String(501, 98,".",FONT_24,WHITE,GBLUE,3);
     LCD_Put_String(501,127,".",FONT_24,WHITE,GBLUE,4);
     LCD_Put_String(501,156,".",FONT_24,WHITE,GBLUE,5);
     LCD_Put_String(541, 98,"uF",FONT_24,WHITE,GBLUE,3);
     LCD_Put_String(541,127,"uF",FONT_24,WHITE,GBLUE,4);
     LCD_Put_String(541,156,"uF",FONT_24,WHITE,GBLUE,5);
     /*显示电容器组电容量*/
     p = &PHASE_A_CAP_KEY1;//显示电容器组A相电容器容量
     for(i=0;i<6;i++)
      {
       if(i<4) LCD_Put_Char(*(p+i)+0x10,453+i*12   , 98,FONT_24,WHITE,GBLUE,3);
       else    LCD_Put_Char(*(p+i)+0x10,453+i*12+12, 98,FONT_24,WHITE,GBLUE,3);
      }
     p = &PHASE_B_CAP_KEY1;//显示电容器组B相电容器容量
     for(i=0;i<6;i++)
      {
       if(i<4) LCD_Put_Char(*(p+i)+0x10,453+i*12   ,127,FONT_24,WHITE,GBLUE,4);
       else    LCD_Put_Char(*(p+i)+0x10,453+i*12+12,127,FONT_24,WHITE,GBLUE,4);
      }
     p = &PHASE_C_CAP_KEY1;//显示电容器组C相电容器容量
     for(i=0;i<6;i++)
      {
       if(i<4) LCD_Put_Char(*(p+i)+0x10,453+i*12   ,156,FONT_24,WHITE,GBLUE,5);
       else    LCD_Put_Char(*(p+i)+0x10,453+i*12+12,156,FONT_24,WHITE,GBLUE,5);
      }
    }
   else /*其它连接方式无需显示补偿电容器组电容量*/
    {
     LCD_Put_String(332, 98,"        ",FONT_24,YELLOW,GBLUE,3);
     LCD_Put_String(332,127,"        ",FONT_24,YELLOW,GBLUE,4);
     LCD_Put_String(332,156,"        ",FONT_24,YELLOW,GBLUE,5);
     LCD_Put_String(453, 98,"          ",FONT_24,YELLOW,GBLUE,3);
     LCD_Put_String(453,127,"          ",FONT_24,YELLOW,GBLUE,4);
     LCD_Put_String(453,156,"          ",FONT_24,YELLOW,GBLUE,5);
    }
   /*显示试验时间*/
   tmp = Week_Day(TEST_DATA.year,TEST_DATA.month,TEST_DATA.date);//计算星期几
   switch(tmp)//星期
    {
     case 1:LCD_Put_String(383,185,"一",FONT_24,WHITE,GBLUE,6);
            break;
     case 2:LCD_Put_String(383,185,"二",FONT_24,WHITE,GBLUE,6);
            break;
     case 3:LCD_Put_String(383,185,"三",FONT_24,WHITE,GBLUE,6);
            break;
     case 4:LCD_Put_String(383,185,"四",FONT_24,WHITE,GBLUE,6);
            break;
     case 5:LCD_Put_String(383,185,"五",FONT_24,WHITE,GBLUE,6);
            break;
     case 6:LCD_Put_String(383,185,"六",FONT_24,WHITE,GBLUE,6);
            break;
     case 7:LCD_Put_String(383,185,"日",FONT_24,WHITE,GBLUE,6);
            break;
    }
   LCD_IntHex(TEST_DATA.year,  167,185,2,FONT_24,WHITE,GBLUE,6);//年
   LCD_IntHex(TEST_DATA.month, 215,185,2,FONT_24,WHITE,GBLUE,6);//月
   LCD_IntHex(TEST_DATA.date,  263,185,2,FONT_24,WHITE,GBLUE,6);//日
   LCD_IntHex(TEST_DATA.hour,  431,185,2,FONT_24,WHITE,GBLUE,6);//时
   LCD_IntHex(TEST_DATA.minute,479,185,2,FONT_24,WHITE,GBLUE,6);//分
   LCD_IntHex(TEST_DATA.second,527,185,2,FONT_24,WHITE,GBLUE,6);//秒
   /*显示电容值及电容电流*/
   LCD_FloatNum(TEST_DATA.CAPACITANCE,280,230,YES,4,FONT_64,YELLOW,GBLUE,7);
   LCD_FloatNum(TEST_DATA.CAP_CURRENT,280,314,YES,4,FONT_64,YELLOW,GBLUE,8);    
  }  
 /*----------------------------------------------显示控制菜单-----------------------------------------------*/
 if(screen == TESTING_SCREEN) /*显示实时测试数据*/
  {
#if BLUETOOTH == ON
   LCD_Draw_Dynamic_Window(  8,394,151,437,8,INDIGO,CDX3-1);//绘制动态窗口1，重新测量
   LCD_Draw_Dynamic_Window(166,394,309,437,8,INDIGO,CDX3-2);//绘制动态窗口2，打印数据
   LCD_Draw_Dynamic_Window(326,394,469,437,8,INDIGO,CDX3-3);//绘制动态窗口3，保存数据
   LCD_Draw_Dynamic_Window(484,394,627,437,8,INDIGO,CDX3-4);//绘制动态窗口4，上传数据
   LCD_Put_String( 16,400,"重新测量",FONT_32,INDIGO,WHITE,CDX3-1);
   LCD_Put_String(174,400,"打印数据",FONT_32,INDIGO,WHITE,CDX3-2);
   LCD_Put_String(334,400,"保存数据",FONT_32,INDIGO,WHITE,CDX3-3);
   LCD_Put_String(492,400,"上传数据",FONT_32,INDIGO,WHITE,CDX3-4);
#else
   LCD_Draw_Dynamic_Window( 44,394,187,437,8,INDIGO,CDX3-1);//绘制动态窗口1，重新测量
   LCD_Draw_Dynamic_Window(248,394,391,437,8,INDIGO,CDX3-2);//绘制动态窗口2，打印数据
   LCD_Draw_Dynamic_Window(452,394,595,437,8,INDIGO,CDX3-3);//绘制动态窗口3，保存数据
   LCD_Put_String( 52,400,"重新测量",FONT_32,INDIGO,WHITE,CDX3-1);
   LCD_Put_String(256,400,"打印数据",FONT_32,INDIGO,WHITE,CDX3-2);
   LCD_Put_String(460,400,"保存数据",FONT_32,INDIGO,WHITE,CDX3-3);
#endif
  }
 else if(screen == TEST_RECORD_SCREEN) /*需要显示测试记录*/
  {
   if(TEST_RECORD_AMOUNT == 0)//无保存的测量记录
    {
     if(SLOW_SHAN_SIG) LCD_Display_String(224,208,"无测试记录！",FONT_32,BLACK,WHITE,1);
     else              LCD_Display_String(224,208,"            ",FONT_32,BLACK,WHITE,1);   
    }
   else //有已保存的测量记录
    {
     LCD_Display_String(232,400,"记录",FONT_32,INDIGO,WHITE,1);
     LCD_Put_Char(TEST_RECORD_NUMBER_KEY/100+0x10,   296,400,FONT_32,INDIGO,WHITE,1);//显示记录编号
     LCD_Put_Char(TEST_RECORD_NUMBER_KEY%100/10+0x10,312,400,FONT_32,INDIGO,WHITE,1);
     LCD_Put_Char(TEST_RECORD_NUMBER_KEY%10+0x10,    328,400,FONT_32,INDIGO,WHITE,1);
     LCD_Put_String(344,400,"/",FONT_32,INDIGO,WHITE,1);
     LCD_Put_Char(TEST_RECORD_AMOUNT/100+0x10,   360,400,FONT_32,INDIGO,WHITE,1);//显示已存储的数量
     LCD_Put_Char(TEST_RECORD_AMOUNT%100/10+0x10,376,400,FONT_32,INDIGO,WHITE,1);
     LCD_Put_Char(TEST_RECORD_AMOUNT%10+0x10,    392,400,FONT_32,INDIGO,WHITE,1);
     if(TEST_RECORD_NUMBER_KEY > 1)
      {
       if(SLOW_SHAN_SIG) LCD_Put_Dot3232(0x03,168,400,INDIGO,WHITE,1);//底栏显示左箭头
       else LCD_Put_String(168,400,"  ",FONT_32,INDIGO,WHITE,1);
      } 
     else LCD_Put_String(168,400,"  ",FONT_32,INDIGO,WHITE,1); 
     if(TEST_RECORD_NUMBER_KEY < TEST_RECORD_AMOUNT)
      {
       if(SLOW_SHAN_SIG) LCD_Put_Dot3232(0x02,440,400,INDIGO,WHITE,1);//底栏显示右箭头
       else LCD_Put_String(440,400,"  ",FONT_32,INDIGO,WHITE,1);
      } 
     else LCD_Put_String(440,400,"  ",FONT_32,INDIGO,WHITE,1);
    }
  } 
}





/*############################################################################################################################## 
顶栏屏幕显示部分
*##############################################################################################################################*/ 
/*******************************************************************************************************************************
屏幕顶部状态栏显示程序
*******************************************************************************************************************************/
void Top_Bar_Screen(void)
{
 LCD_Area_Fill(0,0,639,27,BLUE);//显示蓝色顶部状态栏
 LCD_Put_String(5,2,"20  /  /   星期     :  :  ",FONT_24,WHITE,BLUE,1);
} 
/*------------------------------------------------------------------------------------------------------------------------------
屏幕顶部状态栏数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Top_Bar_Dupscr(void)
{
 /*显示当前时间*/ 
 LCD_IntHex(NOW_TIME.year,   29,2,2,FONT_24,WHITE,BLUE,1);//年,正常显示
 LCD_IntHex(NOW_TIME.month,  65,2,2,FONT_24,WHITE,BLUE,1);//月,正常显示
 LCD_IntHex(NOW_TIME.date,  101,2,2,FONT_24,WHITE,BLUE,1);//日,正常显示
 LCD_IntHex(NOW_TIME.hour,  221,2,2,FONT_24,WHITE,BLUE,1);//时,正常显示
 LCD_IntHex(NOW_TIME.minute,257,2,2,FONT_24,WHITE,BLUE,1);//分,正常显示
 LCD_IntHex(NOW_TIME.second,293,2,2,FONT_24,WHITE,BLUE,1);//秒,正常显示
 /*显示星期*/
 switch(NOW_TIME.weekday)//星期
  {
   case 1:LCD_Put_String(185,2,"一",FONT_24,WHITE,BLUE,1);
          break;
   case 2:LCD_Put_String(185,2,"二",FONT_24,WHITE,BLUE,1);
          break;
   case 3:LCD_Put_String(185,2,"三",FONT_24,WHITE,BLUE,1);
          break;
   case 4:LCD_Put_String(185,2,"四",FONT_24,WHITE,BLUE,1);
          break;
   case 5:LCD_Put_String(185,2,"五",FONT_24,WHITE,BLUE,1);
          break;
   case 6:LCD_Put_String(185,2,"六",FONT_24,WHITE,BLUE,1);
          break;
   case 7:LCD_Put_String(185,2,"日",FONT_24,WHITE,BLUE,1);
          break;
  }
 /*显示USB图标*/
 if(U_DISK_CONNECT == YES) LCD_Put_Dot4824(0x00,351,2,WHITE,BLUE,1);//显示USB图标
 else LCD_Put_String(351,1,"    ",FONT_24,WHITE,BLUE,1);            //消隐USB图标
 /*显示打印机图标*/
 if(PRINTER_CONNECT == YES) LCD_Put_Dot2424(0x02,411,2,WHITE,BLUE,1);//显示打印机图标
 else LCD_Put_String(411,2,"  ",FONT_24,WHITE,BLUE,1);               //消隐打印机图标
 /*显示蓝牙图标*/
 if(BLUETOOTH_CONNECT == YES) LCD_Put_Dot2424(0x00,447,2,WHITE,BLUE,1);//显示蓝牙图标
 else LCD_Put_String(447,2,"  ",FONT_24,WHITE,BLUE,1);                 //消隐蓝牙图标
 /*显示测量记录查询图标*/
 //测试记录查询屏幕显示文件图标
 if(SCRFL == TEST_RECORD_SCREEN) LCD_Put_Dot2424(0x01,483,2,WHITE,BLUE,1);//显示文件图标
 else LCD_Put_String(483,2,"  ",FONT_24,WHITE,BLUE,1);                    //消隐文件图标
 /*显示超级模式图标*/
 //测试记录查询屏幕显示文件图标
 if(PARAMETER_DISPLAY_EN == YES) LCD_Put_Dot2424(0x05,519,2,WHITE,BLUE,1);//显示钥匙图标
 else LCD_Put_String(519,2,"  ",FONT_24,WHITE,BLUE,1);                    //消隐钥匙图标 
 /*显示本地电池电量图标*/ 
 LCD_Battery_Level_4824(ANALOG_DATA.V_BAT,586,2,WHITE,BLUE,1);//显示48*24点阵电池电量图标
}

/*******************************************************************************************************************************
屏幕底部状态栏显示程序
*******************************************************************************************************************************/
void Bottom_Bar_Screen(void)
{
 unsigned char i;
 LCD_Area_Fill(0,444,639,479,BLUE);//显示蓝色底部状态栏
  /*显示仪器软硬件版本号*/
 LCD_Put_String( 10,450,"SV:",FONT_24,WHITE,BLUE,1); 
 LCD_IntNum(SOFTWARE_VERSION, 46,450, NO,3,2,FONT_24,WHITE,BLUE,1);
#if MODEL == MODEL_A
  LCD_Put_String( 94,450,"a",FONT_24,WHITE,BLUE,1);
#elif MODEL == MODEL_B
  LCD_Put_String( 94,450,"b",FONT_24,WHITE,BLUE,1);
#elif MODEL == MODEL_C
  LCD_Put_String( 94,450,"c",FONT_24,WHITE,BLUE,1);
#endif 
 LCD_Put_String(118,450,"HV:",FONT_24,WHITE,BLUE,1);
 LCD_IntNum(HARDWARE_VERSION,154,450, NO,3,2,FONT_24,WHITE,BLUE,1);
 
 LCD_Put_String(232,446,"3U0=      V",FONT_32,WHITE,BLUE,1);//显示PT开口电压，蓝底白字
 
 /*显示仪器编号*/
#if FACTORY == LIXING || FACTORY == FUAN //力兴电子,伏安电力,显示装置编号
 LCD_Put_String(522,450,"编号:",FONT_24,WHITE,BLUE,1);
 for(i=0;i<4;i++)
  {
   LCD_Put_Char(DEVICE_NUMBER[i]+0x10,582+i*12,450,FONT_24,WHITE,BLUE,1);//显示装置编号
  }
#elif FACTORY == AGENT
 if(SCRFL >= SYSTEM_SETTING_SCREEN)/*代理商机型，在系统参数设置所有屏幕中显示装置编号*/
  {
   LCD_Put_String(522,450,"编号:",FONT_24,WHITE,BLUE,1);
   for(i=0;i<4;i++)
    {
     LCD_Put_Char(DEVICE_NUMBER[i]+0x10,582+i*12,450,FONT_24,WHITE,BLUE,1);//显示装置编号
    }
  }
 else /*代理商机型，其它屏幕不显示装置编号*/
  {
   LCD_Put_String(522,450,"         ",FONT_24,WHITE,BLUE,1);
  }
#endif
} 
/*------------------------------------------------------------------------------------------------------------------------------
屏幕底部状态栏数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Bottom_Bar_Dupscr(void)
{
 //显示PT开口电压，蓝底白字
 if(ANALOG_DATA.VRMS_50HZ < 10) LCD_IntNum(ANALOG_DATA.VRMS_50HZ * 1000,296,446,NO,5,3,FONT_32,WHITE,BLUE,1);
 else LCD_IntNum(ANALOG_DATA.VRMS_50HZ * 100,296,446,NO,5,2,FONT_32,WHITE,BLUE,1);
}

/*##############################################################################################################################
突发弹出屏幕显示部分
##############################################################################################################################*/
/*******************************************************************************************************************************
突发弹出屏幕代码3，硬件工作状态异常报警显示屏幕
*******************************************************************************************************************************/
void Burst_Screen1(void)
{
 LCD_Area_Fill(176,167,464,303,WHITE);            //清除动态窗口对应的显示区
 LCD_BACK_COLOR = WHITE;                          //记录当前背景颜色
 LCD_Draw_Dynamic_Window(176,167,464,303,0,RED,1);//绘制红色边框无圆角
} 
/*------------------------------------------------------------------------------------------------------------------------------
突发弹出屏幕代码1，数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Burst_Dupscr1(void)
{
 if(Get_Bit(HARDWARE_WORK_STATE,OV_50HZ_BIT))/*零序50HZ电压过压*/
  {
   LCD_Display_String(208,199," 零序3U0过压! ",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
   LCD_Display_String(208,239," 无法进行测量 ",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
  }
 else if(Get_Bit(HARDWARE_WORK_STATE,SPWM_OSC_BIT))/*变频电源输出短路*/
  {
   LCD_Display_String(208,199,"   输出短路!  ",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
   LCD_Display_String(208,239,"请检查测量接线",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
  }
 else if(Get_Bit(HARDWARE_WORK_STATE,SPWM_OOC_BIT))/*测试回路开路*/
  {
   LCD_Display_String(208,199," 测量回路开路 ",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
   LCD_Display_String(208,239,"请检查测量接线",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
  }
 else if(Get_Bit(HARDWARE_WORK_STATE,FUSE_OOC_BIT))/*保险管熔断*/
  {
   LCD_Display_String(208,199,"输出保险管熔断",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
       LCD_Put_String(208,239,"请更换2A保险管",FONT_32,RED,WHITE,SLOW_SHAN_SIG);//此处用LCD_Put_String，是为了ASCII字符使用ZIKU.C中的字库
  }  
 else if(Get_Bit(HARDWARE_WORK_STATE,SPWM_FAIL_BIT))/*变频电源通讯失败*/
  {                          
   LCD_Display_String(208,199,"   变频电源   ",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
   LCD_Display_String(208,239,"   通讯失败!  ",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
  }
 else if(Get_Bit(HARDWARE_WORK_STATE,BAT_LOW_BIT))/*电池电量低*/
  {
   LCD_Display_String(208,199,"  电池电量低! ",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
   LCD_Display_String(208,239,"  请及时充电! ",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
  }
 else if(Get_Bit(HARDWARE_WORK_STATE,LONG_IDLE_BIT))/*空闲时间过长*/
  {
   LCD_Display_String(208,199," 长时间未使用 ",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
   LCD_Display_String(208,239,"请关机节省电量",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
  }
}





/*##############################################################################################################################
弹出屏幕显示部分
##############################################################################################################################*/
/*******************************************************************************************************************************
弹出屏幕代码1，硬件初始化错误显示屏幕
*******************************************************************************************************************************/
void Popup_Screen1(void)
{
 LCD_Area_Fill(160,128,480,352,BLACK);              //清除动态窗口对应的显示区
 LCD_BACK_COLOR = BLACK;                            //记录当前背景颜色
 LCD_Draw_Dynamic_Window(160,128,480,352,0,WHITE,1);//绘制白色边框无圆角动态窗口
} 
/*------------------------------------------------------------------------------------------------------------------------------
弹出屏幕代码1，数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Popup_Dupscr1(void)
{
 if(Get_Bit(HARDWARE_EEOR_FLAG,RTC_ERR))       LCD_Display_String(176,144,"实时时钟初始化失败",FONT_32,RED,BLACK,1);
 else LCD_Display_String(176,144,"实时时钟初始化成功",FONT_32,GREEN,BLACK,1); 
 if(Get_Bit(HARDWARE_EEOR_FLAG,FM_MEM_ERR))    LCD_Display_String(176,184,"铁电存储初始化失败",FONT_32,RED,BLACK,2);
 else LCD_Display_String(176,184,"铁电存储初始化成功",FONT_32,GREEN,BLACK,2);  
 if(Get_Bit(HARDWARE_EEOR_FLAG,USB_ERR))       LCD_Display_String(176,224,"优盘模块初始化失败",FONT_32,RED,BLACK,3);
 else LCD_Display_String(176,224,"优盘模块初始化成功",FONT_32,GREEN,BLACK,3); 
 if(Get_Bit(HARDWARE_EEOR_FLAG,SPWMPOWER_ERR)) LCD_Display_String(176,264,"变频电源初始化失败",FONT_32,RED,BLACK,4);
 else LCD_Display_String(176,264,"变频电源初始化成功",FONT_32,GREEN,BLACK,4);
#if FACTORY == LIXING || FACTORY == FUAN //只有力兴、伏安品牌带有蓝牙功能 
 if(Get_Bit(HARDWARE_EEOR_FLAG,BLUETOOTH_ERR)) LCD_Display_String(176,304,"蓝牙模块初始化失败",FONT_32,RED,BLACK,5);
 else LCD_Display_String(176,304,"蓝牙模块初始化成功",FONT_32,GREEN,BLACK,5);
#endif
}

/*******************************************************************************************************************************
弹出屏幕代码2，测量过程显示屏幕
*******************************************************************************************************************************/
void Popup_Screen2(void)
{
 if(PARAMETER_DISPLAY_EN == NO)/*不显示详细参数*/
  {
   LCD_Draw_Rectangle_Window(97,173,543,299,0,BLUE,WHITE);//绘制矩形窗口背景色填充白色
   //LCD_Area_Fill(100,176,540,296,WHITE);
   LCD_Put_String(176,192,"正在测量,请稍候...",FONT_32,BLUE,WHITE,1);
  }
 else /*超级调试模式，显示详细参数*/
  {
   LCD_Area_Fill(0,28,X_PIXEL_MAX,443,WHITE);//除顶部、底部状态栏位置以外填充白色
   LCD_BACK_COLOR = WHITE;                   //记录当前背景色
   LCD_Draw_Line(  0, 68,639, 68,BLACK);//画线，横线
   LCD_Draw_Line(  0,326,639,326,BLACK);//画线，横线   
   LCD_Draw_Line(  0,355,639,355,BLACK);//画线，横线
   LCD_Draw_Line(  0,384,639,384,BLACK);//画线，横线
   LCD_Draw_Line(  0,413,639,413,BLACK);//画线，横线
   LCD_Draw_Line(320,327,320,443,BLACK);//画线，竖线
   LCD_Put_String(  8,329,"低频电压:        (      )",FONT_24,BLACK,WHITE,1);
   LCD_Put_String(  8,358,"低频电流:        (      )",FONT_24,BLACK,WHITE,2);
   LCD_Put_String(  8,387,"高频电压:        (      )",FONT_24,BLACK,WHITE,3);
   LCD_Put_String(  8,416,"高频电流:        (      )",FONT_24,BLACK,WHITE,4);
   LCD_Put_String(328,329,"内部阻抗:",FONT_24,BLACK,WHITE,5);
   LCD_Put_String(328,358,"外部阻抗:",FONT_24,BLACK,WHITE,6);
   LCD_Put_String(328,387,"电池电压:",FONT_24,BLACK,WHITE,7);
   LCD_Put_String(328,416,"P T 变比:",FONT_24,BLACK,WHITE,8);
   LCD_Put_String(594,387,"V:",FONT_24,BLACK,WHITE,8);
   LCD_Put_String(594,416,"I:",FONT_24,BLACK,WHITE,8);
   if(POPUP_CDX == 1)
    {
     LCD_Draw_Line(128, 28,128,325,BLACK);//画线，竖线
     LCD_Draw_Line(256, 28,256,325,BLACK);//画线，竖线
     LCD_Draw_Line(384, 28,384,325,BLACK);//画线，竖线
     LCD_Draw_Line(512, 28,512,325,BLACK);//画线，竖线
     LCD_Display_String( 15, 36,"低频阻抗",FONT_24,BLACK,WHITE,1);
     LCD_Display_String(143, 36,"低频角差",FONT_24,BLACK,WHITE,2);
     LCD_Display_String(271, 36,"高频阻抗",FONT_24,BLACK,WHITE,3);
     LCD_Display_String(399, 36,"高频角差",FONT_24,BLACK,WHITE,4);
     LCD_Display_String(539, 36,"电容量",FONT_24,BLACK,WHITE,5);
    }
   else if(POPUP_CDX == 2)
    {
     LCD_Draw_Line(106, 28,106,325,BLACK);//画线，竖线
     LCD_Draw_Line(213, 28,213,325,BLACK);//画线，竖线
     LCD_Draw_Line(320, 28,320,325,BLACK);//画线，竖线
     LCD_Draw_Line(427, 28,427,325,BLACK);//画线，竖线
     LCD_Draw_Line(534, 28,534,325,BLACK);//画线，竖线
     LCD_Display_String(  5, 36,"低频电阻",FONT_24,BLACK,WHITE,1);
     LCD_Display_String(111, 36,"低频容抗",FONT_24,BLACK,WHITE,2);
     LCD_Display_String(218, 36,"低频感抗",FONT_24,BLACK,WHITE,3);
     LCD_Display_String(325, 36,"高频电阻",FONT_24,BLACK,WHITE,4);
     LCD_Display_String(432, 36,"高频容抗",FONT_24,BLACK,WHITE,5);
     LCD_Display_String(539, 36,"高频感抗",FONT_24,BLACK,WHITE,6);
    }   
  } 
} 
/*------------------------------------------------------------------------------------------------------------------------------
弹出屏幕代码2，数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Popup_Dupscr2(void)
{
 unsigned char i;
 if(PARAMETER_DISPLAY_EN == OFF)/*不显示详细参数*/
  {
   LCD_Progress_Bar((float)TEST_PROCESS_FLAG/8,116,232,GREEN,BLUE,WHITE);//显示测量过程进度条
  }
 else /*超级调试模式，显示详细参数*/
  {
   if(POPUP_CDX == 1)
    {
     for(i=0;i<10;i++)
      {
       LCD_FloatNum(TEST_DATA.Z_LF[i]           , 16,77+i*24,YES,5,FONT_24,INDIGO,WHITE,1);
       LCD_FloatNum(TEST_DATA.D_LF[i]*57.2957795,151,77+i*24,YES,5,FONT_24,INDIGO,WHITE,2);//转换为角度值进行显示
       //LCD_FloatNum(TEST_DATA.D_LF[i],151,77+i*24,YES,5,FONT_24,INDIGO,WHITE,2);
       
       LCD_FloatNum(TEST_DATA.Z_HF[i]           ,273,77+i*24,YES,5,FONT_24,INDIGO,WHITE,3);
       LCD_FloatNum(TEST_DATA.D_HF[i]*57.2957795,407,77+i*24,YES,5,FONT_24,INDIGO,WHITE,4);//转换为角度值进行显示
       //LCD_FloatNum(TEST_DATA.D_HF[i],407,77+i*24,YES,5,FONT_24,INDIGO,WHITE,4);
       
       LCD_FloatNum(TEST_DATA.CAP_MEDIAN[i]     ,535,77+i*24,YES,5,FONT_24,INDIGO,WHITE,5);
      }
    }
   else if(POPUP_CDX == 2)
    {
     for(i=0;i<10;i++)
      {
       LCD_FloatNum(TEST_DATA.R_LF[i] ,  5,77+i*24,YES,6,FONT_24,INDIGO,WHITE,1);
       LCD_FloatNum(TEST_DATA.XC_LF[i],111,77+i*24,YES,6,FONT_24,INDIGO,WHITE,2);
       LCD_FloatNum(TEST_DATA.XL_LF[i],218,77+i*24,YES,6,FONT_24,INDIGO,WHITE,3);
       LCD_FloatNum(TEST_DATA.R_HF[i] ,325,77+i*24,YES,6,FONT_24,INDIGO,WHITE,4);
       LCD_FloatNum(TEST_DATA.XC_HF[i],432,77+i*24,YES,6,FONT_24,INDIGO,WHITE,5);
       LCD_FloatNum(TEST_DATA.XL_HF[i],539,77+i*24,YES,6,FONT_24,INDIGO,WHITE,6);       
      }
    }   
   LCD_FloatNum(ANALOG_DATA.VRMS_LF      ,128,329,NO,5,FONT_24,INDIGO,WHITE,1);
   LCD_FloatNum(ANALOG_DATA.VRMS_LF_ORIG ,224,329,NO,5,FONT_24,INDIGO,WHITE,1);   
   LCD_FloatNum(ANALOG_DATA.IRMS_LF      ,128,358,NO,5,FONT_24,INDIGO,WHITE,2);
   LCD_FloatNum(ANALOG_DATA.IRMS_LF_ORIG ,224,358,NO,5,FONT_24,INDIGO,WHITE,2);
   LCD_FloatNum(ANALOG_DATA.VRMS_HF      ,128,387,NO,5,FONT_24,INDIGO,WHITE,3);
   LCD_FloatNum(ANALOG_DATA.VRMS_HF_ORIG ,224,387,NO,5,FONT_24,INDIGO,WHITE,3);
   LCD_FloatNum(ANALOG_DATA.IRMS_HF      ,128,416,NO,5,FONT_24,INDIGO,WHITE,4);
   LCD_FloatNum(ANALOG_DATA.IRMS_HF_ORIG ,224,416,NO,5,FONT_24,INDIGO,WHITE,4);   
   LCD_FloatNum(TEST_DATA.INTER_IMPEDANCE,448,329,NO,5,FONT_24,INDIGO,WHITE,5);
   LCD_FloatNum(TEST_DATA.LOAD_IMPEDANCE ,448,358,NO,5,FONT_24,INDIGO,WHITE,6);
   LCD_FloatNum(ANALOG_DATA.V_BAT        ,448,387,NO,5,FONT_24,INDIGO,WHITE,7);
   LCD_FloatNum(TEST_DATA.PT_TRANS_RAT   ,448,416,NO,5,FONT_24,INDIGO,WHITE,8);   
   LCD_IntNum(VOLTAGE_GAIN_GEAR,618,387,NO,1,0,FONT_24,INDIGO,WHITE,8);
   LCD_IntNum(CURRENT_GAIN_GEAR,618,416,NO,1,0,FONT_24,INDIGO,WHITE,8);
  } 
}

/*******************************************************************************************************************************
弹出屏幕代码3，打印操作显示屏幕
*******************************************************************************************************************************/
void Popup_Screen3(void)
{
 LCD_Area_Fill(160,172,480,308,WHITE);               //清除动态窗口对应的显示区
 LCD_Draw_Dynamic_Window(160,172,480,308,0,INDIGO,1);//绘制靛蓝色边框无圆角动态窗口
} 
/*------------------------------------------------------------------------------------------------------------------------------
弹出屏幕代码3，数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Popup_Dupscr3(void)
{
 if(OPERATION_PROCESS == 0)//打印机未插入
  {
   LCD_Display_String(192,204,"  未插入打印机  ",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
   LCD_Display_String(192,244,"请插入打印机重试",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
  }
 else if(OPERATION_PROCESS == 1)//打印过程
  {
   if(PRINTING_FLAG == YES)//打印过程中
    {
     LCD_Display_String(192,204,"  正在打印数据  ",FONT_32,INDIGO,WHITE,1);
     LCD_Display_String(192,244,"   请稍后...    ",FONT_32,INDIGO,WHITE,2);
    }
   else //打印完成
    {
     LCD_Display_String(192,220,"  数据打印完成  ",FONT_32,INDIGO,WHITE,SLOW_SHAN_SIG);
    }   
  }
}

/*******************************************************************************************************************************
弹出屏幕代码4，测量结果存储操作显示屏幕
*******************************************************************************************************************************/
void Popup_Screen4(void)
{
 LCD_Area_Fill(176,172,464,308,WHITE);               //清除动态窗口对应的显示区
 LCD_Draw_Dynamic_Window(176,172,464,308,0,INDIGO,1);//绘制靛蓝色边框无圆角动态窗口
} 
/*------------------------------------------------------------------------------------------------------------------------------
弹出屏幕代码4，数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Popup_Dupscr4(void)
{
 switch(OPERATION_PROCESS)
  {
   case 0://存储介质选择
          if(SCRFL == TESTING_SCREEN)//实时测试屏幕
           {
            LCD_Display_String(208,204,"  存储至本机  ",FONT_32,INDIGO,WHITE,POPUP_CDX-1);
            LCD_Display_String(208,244,"  存储至优盘  ",FONT_32,INDIGO,WHITE,POPUP_CDX-2);
           }
          else if(SCRFL == TEST_RECORD_SCREEN)//测试记录查询屏幕
           {
            LCD_Display_String(208,224,"  转存至优盘  ",FONT_32,INDIGO,WHITE,POPUP_CDX-2);
           }          
          break;
   case 1://未插入优盘
          LCD_Display_String(208,204,"  未插入优盘  ",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
          LCD_Display_String(208,244,"请插入优盘重试",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
          break;
   case 2://优盘初始化失败
          LCD_Display_String(208,204,"优盘初始化失败",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
          LCD_Display_String(208,244,"  请更换优盘  ",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
          break;
   case 3://测试数据存储中 请稍后。。。
          LCD_Display_String(208,204,"测试数据存储中",FONT_32,INDIGO,WHITE,1);
          LCD_Display_String(208,244,"   请稍后...  ",FONT_32,INDIGO,WHITE,1);
          break;
   case 4://存储成功
          LCD_Display_String(208,224,"   存储成功   ",FONT_32,INDIGO,WHITE,SLOW_SHAN_SIG);
          break;
   case 5://已经存储，无需重新存储
          LCD_Display_String(208,204," 已存储至本机 ",FONT_32,INDIGO,WHITE,SLOW_SHAN_SIG);
          LCD_Display_String(208,244," 无需重新存储 ",FONT_32,INDIGO,WHITE,SLOW_SHAN_SIG);
          break;
  }
}

/*******************************************************************************************************************************
弹出屏幕代码5，测量结果蓝牙上传操作显示屏幕
*******************************************************************************************************************************/
void Popup_Screen5(void)
{
 LCD_Area_Fill(176,172,464,308,WHITE);               //清除动态窗口对应的显示区
 LCD_Draw_Dynamic_Window(176,172,464,308,0,INDIGO,1);//绘制靛蓝色边框无圆角动态窗口
} 
/*------------------------------------------------------------------------------------------------------------------------------
弹出屏幕代码5，数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Popup_Dupscr5(void)
{
 switch(OPERATION_PROCESS)
  {
   case 0://蓝牙未连接
          LCD_Display_String(208,204,"  蓝牙未连接  ",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
          LCD_Display_String(208,244,"请连接蓝牙重试",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
          break;
   case 1://蓝牙上传完毕
          LCD_Display_String(208,220," 蓝牙上传完毕 ",FONT_32,INDIGO,WHITE,SLOW_SHAN_SIG);
          break;
  }
}

/*******************************************************************************************************************************
弹出屏幕代码6，测量记录操作屏幕
*******************************************************************************************************************************/
void Popup_Screen6(void)
{
 LCD_Area_Fill(160,148,480,324,WHITE);               //清除动态窗口对应的显示区
 LCD_Draw_Dynamic_Window(160,148,480,324,0,INDIGO,1);//绘制靛蓝色边框无圆角动态窗口
} 
/*------------------------------------------------------------------------------------------------------------------------------
弹出屏幕代码6，数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Popup_Dupscr6(void)
{
 //OPERATION_PROCESS = 0:操作方法选择
 //OPERATION_PROCESS = 1:未插入打印机
 //OPERATION_PROCESS = 2:打印数据
 //OPERATION_PROCESS = 3:未插入优盘
 //OPERATION_PROCESS = 4:优盘初始化失败
 //OPERATION_PROCESS = 5:数据存储中
 //OPERATION_PROCESS = 6:数据存储完成
 //OPERATION_PROCESS = 7:蓝牙未连接
 //OPERATION_PROCESS = 8:蓝牙上传完毕
 switch(OPERATION_PROCESS)
  {
   case 0://操作方法选择
          LCD_Display_String(192,180,"    打印记录    ",FONT_32,INDIGO,WHITE,POPUP_CDX-1);
          LCD_Display_String(192,220,"    转存优盘    ",FONT_32,INDIGO,WHITE,POPUP_CDX-2);
#if BLUETOOTH == ON //开启了蓝牙上传功能
          LCD_Display_String(192,260,"    蓝牙上传    ",FONT_32,INDIGO,WHITE,POPUP_CDX-3);
#endif
          break;
   case 1://未插入打印机
          LCD_Display_String(192,204,"  未插入打印机  ",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
          LCD_Display_String(192,244,"请插入打印机重试",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
          break;
   case 2://打印数据
          if(PRINTING_FLAG == YES)//打印过程中
           {
            LCD_Display_String(192,204,"  正在打印数据  ",FONT_32,INDIGO,WHITE,1);
            LCD_Display_String(192,244,"    请稍后...   ",FONT_32,INDIGO,WHITE,2);
           }
          else //打印完成
           {
            LCD_Display_String(192,220,"  数据打印完成  ",FONT_32,INDIGO,WHITE,SLOW_SHAN_SIG);
           }
          break;           
   case 3://未插入优盘
          LCD_Display_String(192,204,"   未插入优盘   ",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
          LCD_Display_String(192,244," 请插入优盘重试 ",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
          break;
   case 4://优盘初始化失败
          LCD_Display_String(192,204," 优盘初始化失败 ",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
          LCD_Display_String(192,244,"   请更换优盘   ",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
          break;
   case 5://数据存储中
          LCD_Display_String(192,204," 测试数据存储中 ",FONT_32,INDIGO,WHITE,1);
          LCD_Display_String(192,244,"    请稍后...   ",FONT_32,INDIGO,WHITE,1);
          break;
   case 6://存储成功
          LCD_Display_String(192,220,"    存储成功    ",FONT_32,INDIGO,WHITE,SLOW_SHAN_SIG);
          break;
   case 7://蓝牙未连接
          LCD_Display_String(192,204,"   蓝牙未连接   ",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
          LCD_Display_String(192,244," 请连接蓝牙重试 ",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
          break;
   case 8://蓝牙上传完毕
          LCD_Display_String(192,220,"  蓝牙上传完毕  ",FONT_32,INDIGO,WHITE,SLOW_SHAN_SIG);
          break;
  }
}

/*******************************************************************************************************************************
弹出屏幕代码7，密码输入操作显示屏幕
*******************************************************************************************************************************/
void Popup_Screen7(void)
{
 LCD_Area_Fill(139,129,488,353,WHITE);               //清除动态窗口对应的显示区
 LCD_Draw_Dynamic_Window(151,130,488,348,0,INDIGO,1);//绘制靛蓝色边框无圆角动态窗口
 LCD_Put_String(200,180,"请输入密码",FONT_48,NAVY,WHITE,1);
} 
/*------------------------------------------------------------------------------------------------------------------------------
弹出屏幕代码7，数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Popup_Dupscr7(void)
{
 unsigned char i;
 for(i=0;i<6;i++) LCD_Put_Char(KEY_CHAR_TMP[i]+0x10,236+i*28,252,FONT_48,INDIGO,WHITE,POPUP_CDX-1-i);//显示输入的密码
}

/*******************************************************************************************************************************
弹出屏幕代码8，操作提示显示屏幕
*******************************************************************************************************************************/
void Popup_Screen8(void)
{
 LCD_Area_Fill(176,172,464,308,WHITE);               //清除动态窗口对应的显示区
 LCD_Draw_Dynamic_Window(176,172,464,308,0,INDIGO,1);//绘制靛蓝色边框无圆角动态窗口
} 
/*------------------------------------------------------------------------------------------------------------------------------
弹出屏幕代码8，数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Popup_Dupscr8(void)
{
 if(SCRFL == 7 || SCRFL == 8 || SCRFL == RANGE_CALIBRATION_SCREEN) /*仪器电池电压校准屏幕、零序3U0校准屏幕、量程单位参数校准屏幕*/
  {
   if(OPERATE_STATE) //操作失败
    {
     LCD_Display_String(208,220,"   校准失败   ",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
    }
   else //操作成功
    {
     LCD_Display_String(208,220,"   校准完成   ",FONT_32,INDIGO,WHITE,SLOW_SHAN_SIG);
    }
  }
 else if(SCRFL == 12)/*清存储测量记录屏幕*/
  {
   if(OPERATE_STATE)
    {
     LCD_Display_String(208,204,"   测量记录   ",FONT_32,INDIGO,WHITE,SLOW_SHAN_SIG);
     LCD_Display_String(208,244,"   清除成功   ",FONT_32,INDIGO,WHITE,SLOW_SHAN_SIG);
    }
   else
    {
     LCD_Display_String(208,204," 确认清除所有 ",FONT_32,INDIGO,WHITE,SLOW_SHAN_SIG);
     LCD_Display_String(208,244,"   测量记录?  ",FONT_32,INDIGO,WHITE,SLOW_SHAN_SIG);
    }   
  }
 else if(SCRFL == 13)/*恢复出厂参数设置屏幕*/
  {
   LCD_Display_String(208,220,"   恢复成功   ",FONT_32,INDIGO,WHITE,SLOW_SHAN_SIG);
  }
 else if(SCRFL == 14)/*设置仪器出厂编号屏幕*/
  {
   LCD_Display_String(208,220,"   设置成功   ",FONT_32,INDIGO,WHITE,SLOW_SHAN_SIG);
  }  
}

/*******************************************************************************************************************************
弹出屏幕代码9，校准电阻阻值错误提示显示屏幕
*******************************************************************************************************************************/
void Popup_Screen9(void)
{
 LCD_Area_Fill(160,172,480,308,WHITE);            //清除动态窗口对应的显示区
 LCD_Draw_Dynamic_Window(160,172,480,308,0,RED,1);//绘制红色边框无圆角动态窗口
} 
/*------------------------------------------------------------------------------------------------------------------------------
弹出屏幕代码9，数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Popup_Dupscr9(void)
{
 LCD_Display_String(192,204,"  校准电阻错误  ",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
 LCD_Display_String(192,244," 请检查电阻接线 ",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
}

/*******************************************************************************************************************************
//弹出屏幕代码10，额定阻抗值输入错误提示显示屏幕
*******************************************************************************************************************************/
void Popup_Screen10(void)
{
 LCD_Area_Fill(176,172,464,308,WHITE);            //清除动态窗口对应的显示区
 LCD_Draw_Dynamic_Window(176,172,464,308,0,RED,1);//绘制红色边框无圆角动态窗口
} 
/*------------------------------------------------------------------------------------------------------------------------------
//弹出屏幕代码10，数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Popup_Dupscr10(void)
{
 switch(OPERATE_STATE)
  {
   case 0://额定阻抗值设置超范围
          LCD_Display_String(208,204,"额定阻抗值调整",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
              LCD_Put_String(208,244," 范围为±10%! ",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
          break;
   case 1://设置电压值设置超范围
          LCD_Display_String(208,204,"设置电压值调整",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
              LCD_Put_String(208,244," 范围为±50%! ",FONT_32,RED,WHITE,SLOW_SHAN_SIG);
          break;
  } 
}

/*******************************************************************************************************************************
//弹出屏幕代码11，设置补偿电容器容量显示屏幕
*******************************************************************************************************************************/
void Popup_Screen11(void)
{
 LCD_Draw_Dynamic_Window(176,114,464,358,0,BLUE,1);
 LCD_Area_Fill(178,116,462,356,WHITE);
 LCD_Put_String(192,130,"设置电容器组容量",FONT_32,BLACK,WHITE,1);
 LCD_Put_String(216,170,"A相:    .  uF",FONT_32,INDIGO,WHITE,1);
 LCD_Put_String(216,210,"B相:    .  uF",FONT_32,INDIGO,WHITE,1);
 LCD_Put_String(216,250,"C相:    .  uF",FONT_32,INDIGO,WHITE,1);
} 
/*------------------------------------------------------------------------------------------------------------------------------
弹出屏幕代码11，数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Popup_Dupscr11(void)
{
 unsigned char i;
 unsigned char *p;
 p = &PHASE_A_CAP_KEY1;//显示电容器组A相电容器容量
 for(i=0;i<6;i++)
  {
   if(i<4) LCD_Put_Char(*(p+i)+0x10,280+i*16   ,170,FONT_32,INDIGO,WHITE,POPUP_CDX-i-1);
   else    LCD_Put_Char(*(p+i)+0x10,280+i*16+16,170,FONT_32,INDIGO,WHITE,POPUP_CDX-i-1);
  }
 p = &PHASE_B_CAP_KEY1;//显示电容器组B相电容器容量
 for(i=0;i<6;i++)
  {
   if(i<4) LCD_Put_Char(*(p+i)+0x10,280+i*16   ,210,FONT_32,INDIGO,WHITE,POPUP_CDX-i-7);
   else    LCD_Put_Char(*(p+i)+0x10,280+i*16+16,210,FONT_32,INDIGO,WHITE,POPUP_CDX-i-7);
  }
 p = &PHASE_C_CAP_KEY1;//显示电容器组C相电容器容量
 for(i=0;i<6;i++)
  {
   if(i<4) LCD_Put_Char(*(p+i)+0x10,280+i*16   ,250,FONT_32,INDIGO,WHITE,POPUP_CDX-i-13);
   else    LCD_Put_Char(*(p+i)+0x10,280+i*16+16,250,FONT_32,INDIGO,WHITE,POPUP_CDX-i-13);
  }
 LCD_Draw_Dynamic_Window(248,296,391,343,8,RED,POPUP_CDX-19);//绘制开始测试按钮的动态窗口   
 LCD_Put_String(256,304,"开始测量",FONT_32,RED  ,WHITE,POPUP_CDX-19); 
}

/*******************************************************************************************************************************
弹出屏幕代码12，测量结果为负值提示显示屏幕
*******************************************************************************************************************************/
void Popup_Screen12(void)
{
 LCD_Area_Fill(112,148,528,324,WHITE);            //清除动态窗口对应的显示区
 LCD_Draw_Dynamic_Window(112,148,528,324,0,RED,1);//绘制红色边框无圆角动态窗口
} 
/*------------------------------------------------------------------------------------------------------------------------------
弹出屏幕代码12，数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Popup_Dupscr12(void)
{
 switch(TEST_DATA.PT_CONNECT_MODE)//根据不同的PT连接方式，提示不同信息
  {
   case PT_3PT ://3PT
   case PT_3PT1://3PT1
   case PT_3PT2://3PT2
                LCD_Display_String(144,180,"    零序回路呈感性    ",FONT_32,RED,WHITE,1);
                LCD_Display_String(144,220,"请短接一次高压消谐电阻",FONT_32,RED,WHITE,1);
                LCD_Display_String(144,260,"  退出系统中消弧线圈  ",FONT_32,RED,WHITE,1);
                break;
   case PT_4PT ://4PT
   case PT_4PT1://4PT1
   case PT_4PT2://4PT2
   case PT_1PT ://1PT
   case PT_C1PT://C1PT
                LCD_Display_String(144,180,"    零序回路呈感性    ",FONT_32,RED,WHITE,1);
                LCD_Display_String(144,220," 请退出系统中消弧线圈 ",FONT_32,RED,WHITE,1);
                LCD_Display_String(144,260,"                      ",FONT_32,RED,WHITE,1);
                break;
  }
}





/*##############################################################################################################################
正常屏幕显示部分
##############################################################################################################################*/
/*******************************************************************************************************************************
屏幕代码0，开机显示屏幕
*******************************************************************************************************************************/
void Screen0(void)
{
 unsigned char i;
 LCD_FullScreen_Fill(BLACK);              //整屏填充黑色
 LCD_BACK_COLOR = BLACK;                  //记录当前背景色
 LCD_Draw_Rectangle(0,0,639,479,WHITE,NO);//画整屏矩形框
 LCD_Draw_Line(0,41,639,41,WHITE);        //画装置名称分割横线
 /*显示仪器型号及名称*/
#if FACTORY == LIXING //力兴电子
 LCD_Put_String(112,5,"LX8113手持式电容电流测试仪",FONT_32,WHITE,BLACK,1);
 LCD_Show_Logo(121,100,RED,WHITE);//显示开机公司图标
#elif FACTORY == FUAN //伏安电力
 LCD_Put_String(104,5,"FA-8503手持式电容电流测试仪",FONT_32,WHITE,BLACK,1);
 LCD_Show_Logo(121,100,RED,WHITE);//显示开机公司图标
#elif FACTORY == AGENT //代理商
 LCD_Display_String(176,5,"配网电容电流测试仪",FONT_32,WHITE,BLACK,1);
 LCD_Set_Text_Zoom(FONT_SIZE_X3,FONT_SIZE_X3);//文字水平、垂直方向放大3倍
 LCD_Display_String(129,180,"欢迎使用",FONT_32,CYAN,BLACK,1);
 LCD_Set_Text_Zoom(FONT_SIZE_X1,FONT_SIZE_X1);//文字水平、垂直方向不放大
#endif
 /*显示仪器软硬件版本号*/
 LCD_Put_String( 10,446,"SV:",FONT_24,WHITE,BLACK,1); 
 LCD_IntNum(SOFTWARE_VERSION, 46,446, NO,3,2,FONT_24,WHITE,BLACK,1);
#if MODEL == MODEL_A
  LCD_Put_String( 94,446,"a",FONT_24,WHITE,BLACK,1);
#elif MODEL == MODEL_B
  LCD_Put_String( 94,446,"b",FONT_24,WHITE,BLACK,1);
#elif MODEL == MODEL_C
  LCD_Put_String( 94,446,"c",FONT_24,WHITE,BLACK,1);
#endif 
 LCD_Put_String(130,446,"HV:",FONT_24,WHITE,BLACK,1);
 LCD_IntNum(HARDWARE_VERSION,166,446, NO,3,2,FONT_24,WHITE,BLACK,1);
 /*显示仪器编号*/
#if FACTORY == LIXING || FACTORY == FUAN //力兴电子,伏安电力,显示装置编号
 LCD_Put_String(522,446,"编号:",FONT_24,WHITE,BLACK,1);
 for(i=0;i<4;i++)
  {
   LCD_Put_Char(DEVICE_NUMBER[i]+0x10,582+i*12,446,FONT_24,WHITE,BLACK,1);//显示装置编号
  }
#endif
} 
/*------------------------------------------------------------------------------------------------------------------------------
屏幕代码0，数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Dupscr0(void)
{

}

/*******************************************************************************************************************************
屏幕代码1，待机屏幕
*******************************************************************************************************************************/
void Screen1(void)
{
 LCD_Area_Fill(0,28,X_PIXEL_MAX,443,WHITE);//除顶部、底部状态栏位置以外填充白色
 LCD_BACK_COLOR = WHITE;                   //记录当前背景色
 /*显示仪器型号及名称*/
#if FACTORY == LIXING //力兴电子
 LCD_Put_String(164, 40,"LX8113手持式电容电流测试仪",FONT_24,BLACK,WHITE,1);
 LCD_Display_String(164,412,"保定市力兴电子设备有限公司",FONT_24,BLACK,WHITE,2);
#elif FACTORY == FUAN //伏安电力
 LCD_Put_String(158, 40,"FA-8503手持式电容电流测试仪",FONT_24,BLACK,WHITE,1);
 LCD_Display_String(140,412,"保定市伏安电力科技开发有限公司",FONT_24,BLACK,WHITE,2);
#elif FACTORY == AGENT //代理商
 LCD_Display_String(212, 40,"配网电容电流测试仪",FONT_24,BLACK,WHITE,1);
#endif
} 
/*------------------------------------------------------------------------------------------------------------------------------
屏幕代码1，待机屏幕数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Dupscr1(void)
{
 LCD_Draw_Dynamic_Window(168, 76,471,139,8,INDIGO,CDX1-1);//绘制第1个选项的动态窗口
 LCD_Draw_Dynamic_Window(168,159,471,222,8,INDIGO,CDX1-2);//绘制第2个选项的动态窗口
 LCD_Draw_Dynamic_Window(168,242,471,305,8,INDIGO,CDX1-3);//绘制第3个选项的动态窗口
 LCD_Draw_Dynamic_Window(168,325,471,388,8,INDIGO,CDX1-4);//绘制第4个选项的动态窗口
 //LCD_Set_Text_Zoom(FONT_SIZE_X2,FONT_SIZE_X2);//文字水平、垂直放大2倍
 LCD_Put_String(176, 84,"电容电流测量",FONT_48,INDIGO,WHITE,CDX1-1);
 LCD_Put_String(176,167,"测量记录查询",FONT_48,INDIGO,WHITE,CDX1-2);
 LCD_Put_String(176,250,"实时时钟设置",FONT_48,INDIGO,WHITE,CDX1-3);
 LCD_Put_String(176,333,"系统参数设置",FONT_48,INDIGO,WHITE,CDX1-4);
 //LCD_Set_Text_Zoom(FONT_SIZE_X1,FONT_SIZE_X1);//文字水平、垂直不放大
}

/*******************************************************************************************************************************
屏幕代码2，电容电流测量参数设置屏幕
*******************************************************************************************************************************/
void Screen2(void)
{
 LCD_Area_Fill(0,28,X_PIXEL_MAX,443,WHITE);//除顶部、底部状态栏位置以外填充白色
 LCD_BACK_COLOR = WHITE;                   //记录当前背景色
 LCD_Put_String(  5, 40,"电容电流测量 > 测量参数设置",FONT_32,BLACK,WHITE,1);
 LCD_Put_String(409, 94,"<说明>",FONT_32,BLACK,WHITE,2);
 LCD_Put_String(154,218,"  . kV",FONT_32,INDIGO,WHITE,3);
 LCD_Put_String(154,277,"  . kV",FONT_32,INDIGO,WHITE,4);
 LCD_Draw_Line(  0, 84,639, 84,BLACK); //画横线
 LCD_Draw_Line(  0, 86,639, 86,BLACK); //画横线
 LCD_Draw_Line(  0,145,266,145,BLACK); //画横线
 LCD_Draw_Line(  0,204,266,204,BLACK); //画横线
 LCD_Draw_Line(  0,263,266,263,BLACK); //画横线
 LCD_Draw_Line(  0,322,639,322,BLACK); //画横线
 LCD_Draw_Line(  0,381,639,381,BLACK); //画横线 
 LCD_Draw_Line(138, 87,138,443,BLACK); //画竖线
 LCD_Draw_Line(266, 87,266,321,BLACK); //画竖线
} 
/*------------------------------------------------------------------------------------------------------------------------------
屏幕代码2，电容电流测量参数设置屏幕数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Dupscr2(void)
{
 unsigned char i;
 /*在不显示PT接线图或者有全屏刷新请求时，更新数据显示*/
 //当显示接线图且弹出BURST_SCRFL窗口时，按下任意键关闭BURST_SCRFL窗口，如果此时不进行数据刷新，将会显示一个空白表格
 if(DISPLAY_PT_WIRING_DIAGRAM == OFF || SCRAPPLY == YES)
  {
   LCD_Put_String(  5,100,"试验编号",FONT_32,BLACK,WHITE,CDX2- 1);
   LCD_Put_String(  5,159,"设备名称",FONT_32,BLACK,WHITE,CDX2- 8);//新的蓝牙通讯规约中将“设备ID”改为“设备名称”
   LCD_Put_String(  5,218,"额定高压",FONT_32,BLACK,WHITE,CDX2-15);
   LCD_Put_String(  5,277,"母线电压",FONT_32,BLACK,WHITE,CDX2-19);
   LCD_Put_String(  5,336,"P T 方式",FONT_32,BLACK,WHITE,CDX2-21);
   LCD_Put_String(  5,397,"P T 变比",FONT_32,BLACK,WHITE,5);
   LCD_Draw_Dynamic_Window(386,266,529,313,8,RED,CDX2-24);//绘制开始测试按钮的动态窗口   
   LCD_Put_String(394,274,"开始测量",FONT_32,RED  ,WHITE,CDX2-24);
   /*显示试验编号和设备ID*/
   for(i=0;i<6;i++)
    {
     LCD_Put_Char(TEST_DATA.TEST_NUMBER[i]+0x10,154+i*16,100,FONT_32,INDIGO,WHITE,CDX2-i-2);
     LCD_Put_Char(TEST_DATA.DEVICE_ID[i]  +0x10,154+i*16,159,FONT_32,INDIGO,WHITE,CDX2-i-9);
    }
   /*显示额定高压数值*/
   LCD_Put_Char(RATED_VOLTAGE_KEY1 + 0x10,154,218,FONT_32,INDIGO,WHITE,CDX2-16);
   LCD_Put_Char(RATED_VOLTAGE_KEY2 + 0x10,170,218,FONT_32,INDIGO,WHITE,CDX2-17);
   LCD_Put_Char(RATED_VOLTAGE_KEY3 + 0x10,202,218,FONT_32,INDIGO,WHITE,CDX2-18);
   /*显示母线电压数值*/
   LCD_IntNum(TEST_DATA.BUS_VOLTAGE,154,277,NO,3,1,FONT_32,INDIGO,WHITE,CDX2-20);
   /*显示PT连接方式及变比*/
   LCD_Put_String(154,397,"UL/",FONT_32,INDIGO,WHITE,4);//显示变比前半部分   
   LCD_Put_Dot3232(0x04,202,397,INDIGO,WHITE,4);        //显示根号3
   switch(TEST_DATA.PT_CONNECT_MODE)
    {
     case PT_3PT ://3PT
     default     ://3PT
                  LCD_Put_String(154,336,"3PT ",FONT_32,INDIGO,WHITE,CDX2-22);
                  LCD_Put_String(234,397,":100/3       ",FONT_32,INDIGO,WHITE,4);//显示变比后半部分
                  break;
     case PT_3PT1://3PT1
                  LCD_Put_String(154,336,"3PT1",FONT_32,INDIGO,WHITE,CDX2-22);
                  LCD_Put_String(234,397,":100         ",FONT_32,INDIGO,WHITE,4);//显示变比后半部分
                  break;
     case PT_3PT2://3PT2
                  LCD_Put_String(154,336,"3PT2",FONT_32,INDIGO,WHITE,CDX2-22);
                  LCD_Put_String(234,397,":100/",FONT_32,INDIGO,WHITE,4); //显示变比后半部分
                  LCD_Put_Dot3232(0x04,314,397,INDIGO,WHITE,4);           //显示根号3
                  LCD_Put_String(346,397,"      ",FONT_32,INDIGO,WHITE,4);//显示变比后半部分
                  break;
     case PT_4PT ://4PT
                  LCD_Put_String(154,336,"4PT ",FONT_32,INDIGO,WHITE,CDX2-22);
                  LCD_Put_String(234,397,":100/",FONT_32,INDIGO,WHITE,4); //显示变比后半部分
                  LCD_Put_Dot3232(0x04,314,397,INDIGO,WHITE,4);           //显示根号3
                  LCD_Put_String(346,397,"      ",FONT_32,INDIGO,WHITE,4);//显示变比后半部分
                  break;
     case PT_4PT1://4PT1
                  LCD_Put_String(154,336,"4PT1",FONT_32,INDIGO,WHITE,CDX2-22);
                  LCD_Put_String(234,397,":100/",FONT_32,INDIGO,WHITE,4); //显示变比后半部分
                  LCD_Put_Dot3232(0x04,314,397,INDIGO,WHITE,4);           //显示根号3
                  LCD_Put_String(346,397,"+100/3",FONT_32,INDIGO,WHITE,4);//显示变比后半部分
                  break;
     case PT_4PT2://4PT2
                  LCD_Put_String(154,336,"4PT2",FONT_32,INDIGO,WHITE,CDX2-22);
                  LCD_Put_String(234,397,":100/3       ",FONT_32,INDIGO,WHITE,4);//显示变比后半部分
                  break;
     case PT_1PT ://1PT
                  LCD_Put_String(154,336,"1PT ",FONT_32,INDIGO,WHITE,CDX2-22);
                  LCD_Put_String(234,397,":100/",FONT_32,INDIGO,WHITE,4); //显示变比后半部分
                  LCD_Put_Dot3232(0x04,314,397,INDIGO,WHITE,4);           //显示根号3
                  LCD_Put_String(346,397,"      ",FONT_32,INDIGO,WHITE,4);//显示变比后半部分
                  break;
     case PT_C1PT://C1PT
                  LCD_Put_String(154,336,"C1PT",FONT_32,INDIGO,WHITE,CDX2-22);
                  LCD_Put_String(234,397,":100/",FONT_32,INDIGO,WHITE,4); //显示变比后半部分
                  LCD_Put_Dot3232(0x04,314,397,INDIGO,WHITE,4);           //显示根号3
                  LCD_Put_String(346,397,"      ",FONT_32,INDIGO,WHITE,4);//显示变比后半部分
                  break;
    }
   LCD_Put_String(399,336,"显示接线原理图",FONT_32,INDIGO,WHITE,CDX2-23);
   //显示PT变比实际值
   LCD_Put_String(503,397,"(",FONT_32,INDIGO,WHITE,1);
   LCD_Put_String(615,397,")",FONT_32,INDIGO,WHITE,1);
   LCD_FloatNum(TEST_DATA.PT_TRANS_RAT,519,397,NO,5,FONT_32,INDIGO,WHITE,1);
   if(CDX2 <= 7)/*设置试验编号*/
    {
     LCD_Put_String(278,134,"设置试验编号。               ",FONT_24,BLACK,WHITE,6);
     LCD_Put_String(278,162,"                             ",FONT_24,BLACK,WHITE,6);
     LCD_Put_String(278,190,"                             ",FONT_24,BLACK,WHITE,6);
     LCD_Put_String(278,218,"                             ",FONT_24,BLACK,WHITE,6);
    } 
   else if(CDX2 <= 14)/*设置设备名称*/
    {
     LCD_Put_String(278,134,"设置设备名称。               ",FONT_24,BLACK,WHITE,6);//新的蓝牙通讯规约中将“设备ID”改为“设备名称"
     LCD_Put_String(278,162,"                             ",FONT_24,BLACK,WHITE,6);
     LCD_Put_String(278,190,"                             ",FONT_24,BLACK,WHITE,6);
     LCD_Put_String(278,218,"                             ",FONT_24,BLACK,WHITE,6);
    } 
   else if(CDX2 <= 18)/*设置额定高压*/
    {
     LCD_Put_String(278,134,"设置PT一次电压额定值UL。     ",FONT_24,BLACK,WHITE,6);
     LCD_Put_String(278,162,"                             ",FONT_24,BLACK,WHITE,6);
     LCD_Put_String(278,190,"                             ",FONT_24,BLACK,WHITE,6);
     LCD_Put_String(278,218,"                             ",FONT_24,BLACK,WHITE,6);
    }
   else if(CDX2 <= 20)/*设置母线电压*/
    {
     LCD_Put_String(278,134,"设置一次母线电压实际值,可提高",FONT_24,BLACK,WHITE,6);
     LCD_Put_String(278,162,"电容电流计算精度,对测量的电容",FONT_24,BLACK,WHITE,6);
     LCD_Put_String(278,190,"值精度无影响。               ",FONT_24,BLACK,WHITE,6);
     LCD_Put_String(278,218,"                             ",FONT_24,BLACK,WHITE,6);
    }
   else if(CDX2 <= 22)/*设置PT连接方式*/
    {
     if(CDX2 == 21)
      {
       LCD_Put_String(278,134,"设置PT连接方式及PT变比。     ",FONT_24,BLACK,WHITE,6);
       LCD_Put_String(278,162,"                             ",FONT_24,BLACK,WHITE,6);
       LCD_Put_String(278,190,"                             ",FONT_24,BLACK,WHITE,6);
       LCD_Put_String(278,218,"                             ",FONT_24,BLACK,WHITE,6);
      }
     else
      {
       switch(TEST_DATA.PT_CONNECT_MODE)
        {
         case PT_3PT ://3PT
         case PT_3PT1://3PT1
         case PT_3PT2://3PT2
         default     ://3PT
                      LCD_Put_String(278,134,"3PT连接方式；测量前需退出系统",FONT_24,BLACK,WHITE,6);
                      LCD_Put_String(278,162,"中消弧线圈,短接一次高压消谐电",FONT_24,BLACK,WHITE,6);
                      LCD_Put_String(278,190,"阻。                         ",FONT_24,BLACK,WHITE,6);
                      LCD_Put_String(278,218,"                             ",FONT_24,BLACK,WHITE,6);
                      break;
         case PT_4PT ://4PT
         case PT_4PT1://4PT1
         case PT_4PT2://4PT2
                      LCD_Put_String(278,134,"4PT连接方式；测量前需退出系统",FONT_24,BLACK,WHITE,6);
                      LCD_Put_String(278,162,"中消弧线圈。4PT连接方式，测量",FONT_24,BLACK,WHITE,6);
                      LCD_Put_String(278,190,"精度较低,可采用补偿电容组中性",FONT_24,BLACK,WHITE,6);
                      LCD_Put_String(278,218,"点测试法,以提高测量精度。    ",FONT_24,BLACK,WHITE,6);
                      break;
         case PT_1PT ://1PT
                      LCD_Put_String(278,134,"变压器中性点测试法,测量前需退",FONT_24,BLACK,WHITE,6);
                      LCD_Put_String(278,162,"出系统中消弧线圈。           ",FONT_24,BLACK,WHITE,6);
                      LCD_Put_String(278,190,"                             ",FONT_24,BLACK,WHITE,6);
                      LCD_Put_String(278,218,"                             ",FONT_24,BLACK,WHITE,6);
                      break;
         case PT_C1PT://C1PT
                      LCD_Put_String(278,134,"补偿电容器组中性点测试法,测量",FONT_24,BLACK,WHITE,6);
                      LCD_Put_String(278,162,"前需退出系统中消弧线圈。     ",FONT_24,BLACK,WHITE,6);
                      LCD_Put_String(278,190,"                             ",FONT_24,BLACK,WHITE,6);
                      LCD_Put_String(278,218,"                             ",FONT_24,BLACK,WHITE,6);
                      LCD_Put_String(278,218,"                             ",FONT_24,BLACK,WHITE,6);
                      break;
        }
      }     
    } 
   else if(CDX2 == 23)/*显示接线原理图*/
    {
     LCD_Put_String(278,134,"按确定键显示接线图。         ",FONT_24,BLACK,WHITE,6);
     LCD_Put_String(278,162,"                             ",FONT_24,BLACK,WHITE,6);
     LCD_Put_String(278,190,"                             ",FONT_24,BLACK,WHITE,6);
     LCD_Put_String(278,218,"                             ",FONT_24,BLACK,WHITE,6);
    } 
   else if(CDX2 == 24)/*开始测量*/
    {
     switch(TEST_DATA.PT_CONNECT_MODE)
        {
         case PT_3PT ://3PT
         case PT_3PT1://3PT1
         case PT_3PT2://3PT2
         default     ://3PT
                      LCD_Put_String(278,134,"3PT连接方式；测量前需退出系统",FONT_24,BLACK,WHITE,6);
                      LCD_Put_String(278,162,"中消弧线圈,短接一次高压消谐电",FONT_24,BLACK,WHITE,6);
                      LCD_Put_String(278,190,"阻。按确定键开始测量。       ",FONT_24,BLACK,WHITE,6);
                      LCD_Put_String(278,218,"                             ",FONT_24,BLACK,WHITE,6);
                      break;
         case PT_4PT ://4PT
         case PT_4PT1://4PT1
         case PT_4PT2://4PT2
                      LCD_Put_String(278,134,"4PT连接方式；测量前需退出系统",FONT_24,BLACK,WHITE,6);
                      LCD_Put_String(278,162,"中消弧线圈。                 ",FONT_24,BLACK,WHITE,6);
                      LCD_Put_String(278,190,"按确定键开始测量。           ",FONT_24,BLACK,WHITE,6);
                      LCD_Put_String(278,218,"                             ",FONT_24,BLACK,WHITE,6);
                      break;
         case PT_1PT ://1PT
                      LCD_Put_String(278,134,"变压器中性点测试法,测量前需退",FONT_24,BLACK,WHITE,6);
                      LCD_Put_String(278,162,"出系统中消弧线圈。           ",FONT_24,BLACK,WHITE,6);
                      LCD_Put_String(278,190,"按确定键开始测量。           ",FONT_24,BLACK,WHITE,6);
                      LCD_Put_String(278,218,"                             ",FONT_24,BLACK,WHITE,6);
                      break;
         case PT_C1PT://C1PT
                      LCD_Put_String(278,134,"补偿电容器组中性点测试法,测量",FONT_24,BLACK,WHITE,6);
                      LCD_Put_String(278,162,"前需退出系统中消弧线圈。按确 ",FONT_24,BLACK,WHITE,6);
                      LCD_Put_String(278,190,"定键输入补偿电容器组电容量。 ",FONT_24,BLACK,WHITE,6);
                      LCD_Put_String(278,218,"                             ",FONT_24,BLACK,WHITE,6);
                      break;
        }
    } 
  }
 else /*显示PT接线图*/
  {
   LCD_Draw_Dynamic_Window(142, 97,497,374,0,BLUE,1);//绘制动态窗口
   //显示PT接线图
   switch(TEST_DATA.PT_CONNECT_MODE)
    {
     case PT_3PT ://3PT
     case PT_3PT1://3PT1
     case PT_3PT2://3PT2
     default     ://3PT
                  LCD_Show_Wiring_Diagram(0,144, 99,BLUE);//显示3PT接线图
                  break;
     case PT_4PT ://4PT
     case PT_4PT2://4PT2
                  LCD_Show_Wiring_Diagram(1,144, 99,BLUE);//显示4PT接线图
                  break;
     case PT_4PT1://4PT1
                  LCD_Show_Wiring_Diagram(2,144, 99,BLUE);//显示4PT1接线图
                  break;
     case PT_1PT ://1PT
                  LCD_Show_Wiring_Diagram(3,144, 99,BLUE);//显示1PT接线图
                  break;
     case PT_C1PT://C1PT
                  LCD_Show_Wiring_Diagram(4,144, 99,BLUE);//显示C1PT接线图
                  break;
    }
  }
}

/*******************************************************************************************************************************
屏幕代码3，电容电流测量结果屏幕
*******************************************************************************************************************************/
void Screen3(void)
{
 Test_Data_Screen(TESTING_SCREEN);//显示测试数据静态数据
} 
/*------------------------------------------------------------------------------------------------------------------------------
屏幕代码3，电容电流测量结果屏幕数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Dupscr3(void)
{
 Test_Data_Dupscr(TESTING_SCREEN);//显示测试数据动态数据
}

/*******************************************************************************************************************************
屏幕代码4，测量记录查询屏幕
*******************************************************************************************************************************/
void Screen4(void)
{
 Test_Data_Screen(TEST_RECORD_SCREEN);//显示测试记录静态数据
} 
/*------------------------------------------------------------------------------------------------------------------------------
屏幕代码4，测量记录查询屏幕数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Dupscr4(void)
{
 Test_Data_Dupscr(TEST_RECORD_SCREEN);//显示测试记录动态数据
}

/*******************************************************************************************************************************
屏幕代码5，实时时钟设置屏幕
*******************************************************************************************************************************/
void Screen5(void)
{
 LCD_Area_Fill(0,28,X_PIXEL_MAX,443,WHITE);//除顶部、底部状态栏位置以外填充白色
 LCD_BACK_COLOR = WHITE;                   //记录当前背景色
 LCD_Draw_Line( 50,116,590,116,RED);
 LCD_Draw_Line( 50,124,590,124,RED);
 //LCD_Set_Text_Zoom(FONT_SIZE_X2,FONT_SIZE_X2);//文字水平、垂直放大2倍
 LCD_Put_String(176, 48,"实时时钟设置",FONT_48,BLACK,WHITE,1);
 LCD_Put_String(248,144,"年  月  日",FONT_48,BLACK,WHITE,2);
 LCD_Put_String(248,208,"时  分  秒",FONT_48,BLACK,WHITE,3);
 LCD_Put_String(248,276,"星期",FONT_48,BLACK,WHITE,4);
 //LCD_Set_Text_Zoom(FONT_SIZE_X1,FONT_SIZE_X1);//文字水平、垂直不放大 
} 
/*------------------------------------------------------------------------------------------------------------------------------
屏幕代码5，实时时钟设置屏幕数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Dupscr5(void)
{
 //LCD_Set_Text_Zoom(FONT_SIZE_X2,FONT_SIZE_X2);//文字水平、垂直放大2倍
 //显示当前时间
 LCD_Put_String(152,144,"20",FONT_48,INDIGO,WHITE,CDX2-1); 
 LCD_IntHex(YEAR_KEY,  200,144,2,FONT_48,INDIGO,WHITE,CDX2-1);//年
 LCD_IntHex(MONTH_KEY, 296,144,2,FONT_48,INDIGO,WHITE,CDX2-2);//月
 LCD_IntHex(DATE_KEY,  392,144,2,FONT_48,INDIGO,WHITE,CDX2-3);//日
 LCD_IntHex(HOUR_KEY,  200,208,2,FONT_48,INDIGO,WHITE,CDX2-4);//时
 LCD_IntHex(MINUTE_KEY,296,208,2,FONT_48,INDIGO,WHITE,CDX2-5);//分
 LCD_IntHex(SECOND_KEY,392,208,2,FONT_48,INDIGO,WHITE,CDX2-6);//秒
 switch(WEEKDAY_KEY)//星期
  {
   case 1:LCD_Put_String(344,276,"一",FONT_48,INDIGO,WHITE,4);
          break;
   case 2:LCD_Put_String(344,276,"二",FONT_48,INDIGO,WHITE,4);
          break;
   case 3:LCD_Put_String(344,276,"三",FONT_48,INDIGO,WHITE,4);
          break;
   case 4:LCD_Put_String(344,276,"四",FONT_48,INDIGO,WHITE,4);
          break;
   case 5:LCD_Put_String(344,276,"五",FONT_48,INDIGO,WHITE,4);
          break;
   case 6:LCD_Put_String(344,276,"六",FONT_48,INDIGO,WHITE,4);
          break;
   case 7:LCD_Put_String(344,276,"日",FONT_48,INDIGO,WHITE,4);
          break;
  }
 //LCD_Set_Text_Zoom(FONT_SIZE_X1,FONT_SIZE_X1);//文字水平、垂直不放大 
}

/*******************************************************************************************************************************
屏幕代码6，系统参数设置屏幕
*******************************************************************************************************************************/
void Screen6(void)
{
 LCD_Area_Fill(0,28,X_PIXEL_MAX,443,WHITE);//除顶部、底部状态栏位置以外填充白色
 LCD_BACK_COLOR = WHITE;                   //记录当前背景色
 LCD_Put_String(176, 52,"系统参数设置",FONT_48,BLACK,WHITE,1);
} 
/*------------------------------------------------------------------------------------------------------------------------------
屏幕代码6，系统参数设置屏幕数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Dupscr6(void)
{
 LCD_Draw_Dynamic_Window( 30,124,303,173,8,INDIGO,CDX2-1);//绘制第1个选项的动态窗口,圆角半径8
 LCD_Draw_Dynamic_Window( 30,204,303,253,8,INDIGO,CDX2-2);//绘制第2个选项的动态窗口,圆角半径8 
 LCD_Draw_Dynamic_Window( 30,284,303,333,8,INDIGO,CDX2-3);//绘制第3个选项的动态窗口,圆角半径8
 LCD_Draw_Dynamic_Window( 30,364,303,413,8,INDIGO,CDX2-4);//绘制第4个选项的动态窗口,圆角半径8 
 LCD_Draw_Dynamic_Window(334,124,607,173,8,INDIGO,CDX2-5);//绘制第5个选项的动态窗口,圆角半径8
 LCD_Draw_Dynamic_Window(334,204,607,253,8,INDIGO,CDX2-6);//绘制第6个选项的动态窗口,圆角半径8 
 LCD_Draw_Dynamic_Window(334,284,607,333,8,INDIGO,CDX2-7);//绘制第7个选项的动态窗口,圆角半径8
 LCD_Draw_Dynamic_Window(334,364,607,413,8,INDIGO,CDX2-8);//绘制第8个选项的动态窗口,圆角半径8
 
 LCD_Display_String( 39,133,"仪器电池电压校准",FONT_32,INDIGO,WHITE,CDX2-1);
     LCD_Put_String( 39,213,"零序3U0 电压校准",FONT_32,INDIGO,WHITE,CDX2-2);
 LCD_Display_String( 39,293,"变频逆变电源调试",FONT_32,INDIGO,WHITE,CDX2-3);
 LCD_Display_String( 39,373,"量程档位参数校准",FONT_32,INDIGO,WHITE,CDX2-4); 
 LCD_Display_String(343,133," 测试继电器动作 ",FONT_32,INDIGO,WHITE,CDX2-5);
 LCD_Display_String(343,213,"清除存储测量记录",FONT_32,INDIGO,WHITE,CDX2-6);
 LCD_Display_String(343,293,"恢复出厂参数设置",FONT_32,INDIGO,WHITE,CDX2-7);
 LCD_Display_String(343,373,"仪器出厂编号设置",FONT_32,INDIGO,WHITE,CDX2-8);
}

/*******************************************************************************************************************************
屏幕代码7，仪器电池电压校准屏幕
*******************************************************************************************************************************/
void Screen7(void)
{
 LCD_Area_Fill(0,28,X_PIXEL_MAX,443,WHITE);//除顶部、底部状态栏位置以外填充白色
 LCD_BACK_COLOR = WHITE;                   //记录当前背景色
 LCD_Put_String(  8, 40,"系统参数设置 > 仪器电池电压校准",FONT_24,BLACK,WHITE,1);
 LCD_Draw_Line(  0, 76,639, 76,BLACK);//画横线
 LCD_Put_String(160,130,"实际电压值:        V",FONT_32,BLACK,WHITE,1);
 LCD_Put_String(160,182,"采样电压值:        V",FONT_32,BLACK,WHITE,2);
 LCD_Put_String(160,234,"设置电压值:        V",FONT_32,BLACK,WHITE,3);
} 
/*------------------------------------------------------------------------------------------------------------------------------
屏幕代码7，仪器电池电压校准屏幕数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Dupscr7(void)
{
 unsigned char i;
 LCD_FloatNum(ANALOG_DATA.V_BAT,352,130, NO,5,FONT_32,INDIGO,WHITE,1);//显示电池电压实际值
 LCD_FloatNum(ANALOG_DATA.V_BAT / COE_DATA.COE_V_BAT,352,182, NO,5,FONT_32,INDIGO,WHITE,1);//显示电池电压采样值
 if(MODIFY_STATUS_KEY == NO)/*调整光标状态*/
  {
   for(i=0;i<6;i++)
    {
     LCD_Put_Char(*(&CAL_VBAT_KEY+i)-0x20,352+i*16,234,FONT_32,INDIGO,WHITE,CDX3 - 1);//显示设置电池电压值
    }
  }
 else /*修改参数数值状态*/
  {
   for(i=0;i<6;i++)
    {
     LCD_Put_Char(*(&CAL_VBAT_KEY+i)-0x20,352+i*16,234,FONT_32,INDIGO,WHITE,!(CDX3 == 1 && CDX4-1 == i && SLOW_SHAN_SIG));//显示设置电池电压值
    }
  } 
 LCD_Draw_Dynamic_Window(263,348,376,396,8,INDIGO,CDX3-2);//绘制1个选项的动态窗口,圆角半径8 
 LCD_Display_String(272,357,"校  准",FONT_32,INDIGO,WHITE,CDX3-2);
}

/*******************************************************************************************************************************
屏幕代码8，零序3U0电压校准屏幕
*******************************************************************************************************************************/
void Screen8(void)
{
 LCD_Area_Fill(0,28,X_PIXEL_MAX,443,WHITE);//除顶部、底部状态栏位置以外填充白色
 LCD_BACK_COLOR = WHITE;                   //记录当前背景色
 LCD_Put_String(  8, 40,"系统参数设置 > 零序3U0电压校准",FONT_24,BLACK,WHITE,1);
 LCD_Draw_Line(  0, 76,639, 76,BLACK);//画横线
 LCD_Put_String(160,130,"实际电压值:        V",FONT_32,BLACK,WHITE,1);
 LCD_Put_String(160,182,"采样电压值:        V",FONT_32,BLACK,WHITE,2);
 LCD_Put_String(160,234,"设置电压值:        V",FONT_32,BLACK,WHITE,3);
} 
/*------------------------------------------------------------------------------------------------------------------------------
屏幕代码8，零序3U0电压校准屏幕数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Dupscr8(void)
{
 unsigned char i;
 LCD_FloatNum(ANALOG_DATA.VRMS_50HZ,352,130, NO,5,FONT_32,INDIGO,WHITE,1);//显示零序3U0电压实际值
 LCD_FloatNum(ANALOG_DATA.VRMS_50HZ / COE_DATA.COE_VRMS_50HZ,352,182, NO,5,FONT_32,INDIGO,WHITE,1);//显示零序3U0电压采样值
 if(MODIFY_STATUS_KEY == NO)/*调整光标状态*/
  {
   for(i=0;i<6;i++)
    {
     LCD_Put_Char(*(&CAL_VRMS_50HZ_KEY+i)-0x20,352+i*16,234,FONT_32,INDIGO,WHITE,CDX3 - 1);//显示设置零序3U0电压值
    }
  }
 else /*修改参数数值状态*/
  {
   for(i=0;i<6;i++)
    {
     LCD_Put_Char(*(&CAL_VRMS_50HZ_KEY+i)-0x20,352+i*16,234,FONT_32,INDIGO,WHITE,!(CDX3 == 1 && CDX4-1 == i && SLOW_SHAN_SIG));//显示设置零序3U0电压值
    }
  } 
 LCD_Draw_Dynamic_Window(263,348,376,396,8,INDIGO,CDX3-2);//绘制1个选项的动态窗口,圆角半径8 
 LCD_Display_String(272,357,"校  准",FONT_32,INDIGO,WHITE,CDX3-2);
}

/*******************************************************************************************************************************
屏幕代码9，变频逆变电源调试屏幕
*******************************************************************************************************************************/
void Screen9(void)
{
 LCD_Area_Fill(0,28,X_PIXEL_MAX,443,WHITE);//除顶部、底部状态栏位置以外填充白色
 LCD_BACK_COLOR = WHITE;                   //记录当前背景色
 LCD_Put_String(  8, 40,"系统参数设置 > 变频逆变电源调试",FONT_24,BLACK,WHITE,1);
 LCD_Draw_Line(  0, 76,639, 76,BLACK);//画横线
 LCD_Put_String(184,130,"输出频率:        ",FONT_32,BLACK,WHITE,1);
 LCD_Put_String(184,182,"调制度值:       %",FONT_32,BLACK,WHITE,2);
 LCD_Put_String(184,234,"输出电压:       V",FONT_32,BLACK,WHITE,3);
} 
/*------------------------------------------------------------------------------------------------------------------------------
屏幕代码9，变频逆变电源调试屏幕数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Dupscr9(void)
{
 unsigned short power_out_voltage;
 power_out_voltage = BOOST_POWER_VOLTAGE * 0.0707106781186 * POWER_OUT_MDLT_RAT_KEY;//计算变频逆变电源输出电压 
 if(MODIFY_STATUS_KEY == NO)/*调整光标状态*/
  {
   if(POWER_OUT_FREQENCY_KEY == POWER_OUT_LF)      LCD_Put_String(344,130," 低频 ",FONT_32,INDIGO,WHITE,CDX3-1);
   else if(POWER_OUT_FREQENCY_KEY == POWER_OUT_HF) LCD_Put_String(344,130," 高频 ",FONT_32,INDIGO,WHITE,CDX3-1); 
   LCD_IntNum(POWER_OUT_MDLT_RAT_KEY,344,182,NO,4,1,FONT_32,INDIGO,WHITE,CDX3-2);//显示要输出的调制度
  }
 else /*修改参数数值状态*/
  {
   if(POWER_OUT_FREQENCY_KEY == POWER_OUT_LF)      LCD_Put_String(344,130," 低频 ",FONT_32,INDIGO,WHITE,!(CDX3 == 1 && SLOW_SHAN_SIG));
   else if(POWER_OUT_FREQENCY_KEY == POWER_OUT_HF) LCD_Put_String(344,130," 高频 ",FONT_32,INDIGO,WHITE,!(CDX3 == 1 && SLOW_SHAN_SIG));
   LCD_IntNum(POWER_OUT_MDLT_RAT_KEY,344,182,NO,4,1,FONT_32,INDIGO,WHITE,!(CDX3 == 2 && SLOW_SHAN_SIG));//显示要输出的调制度
  } 
 LCD_IntNum(power_out_voltage     ,344,234,NO,4,2,FONT_32,INDIGO,WHITE,3);//显示目标输出电压 
 LCD_Draw_Dynamic_Window(215,348,424,396,8,INDIGO,CDX3-3);                //绘制1个选项的动态窗口,圆角半径8 
 if(Get_Bit(SPWMPOWER_CTRL_FLAG,SPWMPOWER_TURN_ON_OFF_BIT)) LCD_Display_String(224,357,"停止逆变电源",FONT_32,INDIGO,WHITE,CDX3-3);
 else LCD_Display_String(224,357,"启动逆变电源",FONT_32,INDIGO,WHITE,CDX3-3);
}

/*******************************************************************************************************************************
屏幕代码10，量程档位参数校准屏幕
*******************************************************************************************************************************/
void Screen10(void)
{
 LCD_Area_Fill(0,28,X_PIXEL_MAX,443,WHITE);//除顶部、底部状态栏位置以外填充白色
 LCD_BACK_COLOR = WHITE;                   //记录当前背景色
 LCD_Put_String(  8, 39,"系统参数设置 > 量程档位参数校准",FONT_24,BLACK,WHITE,1);
 LCD_Put_String(527, 39,"V:   I: ",FONT_24,BLACK,WHITE,1);
 LCD_Draw_Line(  0, 74,639, 74,RED);//画横线，标题栏分割线1
 LCD_Draw_Line(  0, 76,639, 76,RED);//画横线，标题栏分割线2，双线效果
 LCD_Draw_Line(  0,125,639,125,RED);//画横线，档位选择分割线
 LCD_Draw_Line(320,126,320,330,RED);//画竖线，两栏数据分割线
 LCD_Draw_Line(  0,282,639,282,RED);//画横线，测试数据与设置数据分割线
 LCD_Draw_Line(  0,331,639,331,RED);//画横线，最下面的分割线
 LCD_Draw_Line(461,332,461,443,RED);//画竖线，按键区域分割线 
 LCD_Put_String(  8,134,"实际角差:          ",FONT_32,BLACK,WHITE,3);
 LCD_Put_String(  8,170,"采样角差:          ",FONT_32,BLACK,WHITE,4);
 LCD_Put_String(  8,206,"实际阻抗:        Ω",FONT_32,BLACK,WHITE,5);
 LCD_Put_String(  8,242,"采样阻抗:        Ω",FONT_32,BLACK,WHITE,6); 
 LCD_Put_String(329,134,"实际电流:        A ",FONT_32,BLACK,WHITE,3);
 LCD_Put_String(329,170,"采样电流:        A ",FONT_32,BLACK,WHITE,4);
 LCD_Put_String(329,206,"实际电压:        V ",FONT_32,BLACK,WHITE,5);
 LCD_Put_String(329,242,"采样电压:        V ",FONT_32,BLACK,WHITE,6); 
 LCD_Put_String(  8,291,"额定阻抗:        Ω",FONT_32,BLACK,WHITE,7);
 LCD_Put_String(329,291,"设置电压:        V ",FONT_32,BLACK,WHITE,7); 
 LCD_Put_String(194,335,"<说明>",FONT_24,BLACK,WHITE,8);
} 
/*------------------------------------------------------------------------------------------------------------------------------
屏幕代码10，量程档位参数校准屏幕数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Dupscr10(void)
{
 unsigned char i;
 float z_coe;
 float voltage,current,voltage_orig,current_orig;
 /*----------------------------显示当前电压、电流档位-------------------------------------*/
 LCD_IntNum(VOLTAGE_GAIN_GEAR,551, 39,NO,1,0,FONT_24,INDIGO,WHITE,1);
 LCD_IntNum(CURRENT_GAIN_GEAR,611, 39,NO,1,0,FONT_24,INDIGO,WHITE,1);
 /*------------------------------选择需要校准的档位---------------------------------------*/
 switch(GAIN_GEAR_KEY)/*根据当前选择档位，显示档位信息，以及确定使用的校准系数*/
  {
   case  0://低频通道档位一，电压档位0，电流档位0
           if(MODIFY_STATUS_KEY == NO) LCD_Display_String(192, 85," 低频通道档位一 ",FONT_32,INDIGO,WHITE,CDX3-1);/*调整光标状态*/
           else LCD_Display_String(192, 85," 低频通道档位一 ",FONT_32,INDIGO,WHITE,!(CDX3 == 1 && SLOW_SHAN_SIG));/*修改参数状态*/
           LCD_Put_String(128, 85,"  ",FONT_32,INDIGO,WHITE,1);//擦除左箭头
           LCD_Put_Dot3232(0x02,480, 85,INDIGO,WHITE,1);       //显示右箭头
           z_coe = COE_DATA.COE_Z_LF_V0I0;
           break;
   case  1://高频通道档位一，电压档位0，电流档位0
           if(MODIFY_STATUS_KEY == NO) LCD_Display_String(192, 85," 高频通道档位一 ",FONT_32,INDIGO,WHITE,CDX3-1);/*调整光标状态*/
           else LCD_Display_String(192, 85," 高频通道档位一 ",FONT_32,INDIGO,WHITE,!(CDX3 == 1 && SLOW_SHAN_SIG));/*修改参数状态*/
           LCD_Put_Dot3232(0x03,128, 85,INDIGO,WHITE,1);//显示左箭头
           LCD_Put_Dot3232(0x02,480, 85,INDIGO,WHITE,1);//显示右箭头
           z_coe = COE_DATA.COE_Z_HF_V0I0;
           break;
   case  2://低频通道档位二，电压档位1，电流档位0
           if(MODIFY_STATUS_KEY == NO) LCD_Display_String(192, 85," 低频通道档位二 ",FONT_32,INDIGO,WHITE,CDX3-1);/*调整光标状态*/
           else LCD_Display_String(192, 85," 低频通道档位二 ",FONT_32,INDIGO,WHITE,!(CDX3 == 1 && SLOW_SHAN_SIG));/*修改参数状态*/
           LCD_Put_Dot3232(0x03,128, 85,INDIGO,WHITE,1);//显示左箭头
           LCD_Put_Dot3232(0x02,480, 85,INDIGO,WHITE,1);//显示右箭头
           z_coe = COE_DATA.COE_Z_LF_V1I0;
           break;
   case  3://高频通道档位二，电压档位1，电流档位0
           if(MODIFY_STATUS_KEY == NO) LCD_Display_String(192, 85," 高频通道档位二 ",FONT_32,INDIGO,WHITE,CDX3-1);/*调整光标状态*/
           else LCD_Display_String(192, 85," 高频通道档位二 ",FONT_32,INDIGO,WHITE,!(CDX3 == 1 && SLOW_SHAN_SIG));/*修改参数状态*/
           LCD_Put_Dot3232(0x03,128, 85,INDIGO,WHITE,1);//显示左箭头
           LCD_Put_Dot3232(0x02,480, 85,INDIGO,WHITE,1);//显示右箭头
           z_coe = COE_DATA.COE_Z_HF_V1I0;
           break;
   case  4://低频通道档位三，电压档位2，电流档位0
           if(MODIFY_STATUS_KEY == NO) LCD_Display_String(192, 85," 低频通道档位三 ",FONT_32,INDIGO,WHITE,CDX3-1);/*调整光标状态*/
           else LCD_Display_String(192, 85," 低频通道档位三 ",FONT_32,INDIGO,WHITE,!(CDX3 == 1 && SLOW_SHAN_SIG));/*修改参数状态*/
           LCD_Put_Dot3232(0x03,128, 85,INDIGO,WHITE,1);//显示左箭头
           LCD_Put_Dot3232(0x02,480, 85,INDIGO,WHITE,1);//显示右箭头
           z_coe = COE_DATA.COE_Z_LF_V2I0;
           break;
   case  5://高频通道档位三，电压档位2，电流档位0
           if(MODIFY_STATUS_KEY == NO) LCD_Display_String(192, 85," 高频通道档位三 ",FONT_32,INDIGO,WHITE,CDX3-1);/*调整光标状态*/
           else LCD_Display_String(192, 85," 高频通道档位三 ",FONT_32,INDIGO,WHITE,!(CDX3 == 1 && SLOW_SHAN_SIG));/*修改参数状态*/
           LCD_Put_Dot3232(0x03,128, 85,INDIGO,WHITE,1);//显示左箭头
           LCD_Put_Dot3232(0x02,480, 85,INDIGO,WHITE,1);//显示右箭头
           z_coe = COE_DATA.COE_Z_HF_V2I0;
           break;
   case  6://低频通道档位四，电压档位0，电流档位1
           if(MODIFY_STATUS_KEY == NO) LCD_Display_String(192, 85," 低频通道档位四 ",FONT_32,INDIGO,WHITE,CDX3-1);/*调整光标状态*/
           else LCD_Display_String(192, 85," 低频通道档位四 ",FONT_32,INDIGO,WHITE,!(CDX3 == 1 && SLOW_SHAN_SIG));/*修改参数状态*/
           LCD_Put_Dot3232(0x03,128, 85,INDIGO,WHITE,1);//显示左箭头
           LCD_Put_Dot3232(0x02,480, 85,INDIGO,WHITE,1);//显示右箭头
           z_coe = COE_DATA.COE_Z_LF_V0I1;
           break;
   case  7://高频通道档位四，电压档位0，电流档位1
           if(MODIFY_STATUS_KEY == NO) LCD_Display_String(192, 85," 高频通道档位四 ",FONT_32,INDIGO,WHITE,CDX3-1);/*调整光标状态*/
           else LCD_Display_String(192, 85," 高频通道档位四 ",FONT_32,INDIGO,WHITE,!(CDX3 == 1 && SLOW_SHAN_SIG));/*修改参数状态*/
           LCD_Put_Dot3232(0x03,128, 85,INDIGO,WHITE,1);//显示左箭头
           LCD_Put_Dot3232(0x02,480, 85,INDIGO,WHITE,1);//显示右箭头
           z_coe = COE_DATA.COE_Z_HF_V0I1;
           break;
   case  8://低频通道档位五，电压档位1，电流档位1
           if(MODIFY_STATUS_KEY == NO) LCD_Display_String(192, 85," 低频通道档位五 ",FONT_32,INDIGO,WHITE,CDX3-1);/*调整光标状态*/
           else LCD_Display_String(192, 85," 低频通道档位五 ",FONT_32,INDIGO,WHITE,!(CDX3 == 1 && SLOW_SHAN_SIG));/*修改参数状态*/
           LCD_Put_Dot3232(0x03,128, 85,INDIGO,WHITE,1);//显示左箭头
           LCD_Put_Dot3232(0x02,480, 85,INDIGO,WHITE,1);//显示右箭头
           z_coe = COE_DATA.COE_Z_LF_V1I1;
           break;
   case  9://高频通道档位五，电压档位1，电流档位1
           if(MODIFY_STATUS_KEY == NO) LCD_Display_String(192, 85," 高频通道档位五 ",FONT_32,INDIGO,WHITE,CDX3-1);/*调整光标状态*/
           else LCD_Display_String(192, 85," 高频通道档位五 ",FONT_32,INDIGO,WHITE,!(CDX3 == 1 && SLOW_SHAN_SIG));/*修改参数状态*/
           LCD_Put_Dot3232(0x03,128, 85,INDIGO,WHITE,1);//显示左箭头
           LCD_Put_Dot3232(0x02,480, 85,INDIGO,WHITE,1);//显示右箭头
           z_coe = COE_DATA.COE_Z_HF_V1I1;
           break;
   case 10://低频通道档位六，电压档位2，电流档位1
           if(MODIFY_STATUS_KEY == NO) LCD_Display_String(192, 85," 低频通道档位六 ",FONT_32,INDIGO,WHITE,CDX3-1);/*调整光标状态*/
           else LCD_Display_String(192, 85," 低频通道档位六 ",FONT_32,INDIGO,WHITE,!(CDX3 == 1 && SLOW_SHAN_SIG));/*修改参数状态*/
           LCD_Put_Dot3232(0x03,128, 85,INDIGO,WHITE,1);//显示左箭头
           LCD_Put_Dot3232(0x02,480, 85,INDIGO,WHITE,1);//显示右箭头
           z_coe = COE_DATA.COE_Z_LF_V2I1;
           break;
   case 11://高频通道档位六，电压档位2，电流档位1
           if(MODIFY_STATUS_KEY == NO) LCD_Display_String(192, 85," 高频通道档位六 ",FONT_32,INDIGO,WHITE,CDX3-1);/*调整光标状态*/
           else LCD_Display_String(192, 85," 高频通道档位六 ",FONT_32,INDIGO,WHITE,!(CDX3 == 1 && SLOW_SHAN_SIG));/*修改参数状态*/
           LCD_Put_Dot3232(0x03,128, 85,INDIGO,WHITE,1);       //显示左箭头
           LCD_Put_String(480, 85,"  ",FONT_32,INDIGO,WHITE,1);//擦除右箭头
           z_coe = COE_DATA.COE_Z_HF_V2I1;
           break;
  }
 if(POWER_OUT_FREQENCY == POWER_OUT_LF)/*当前变频电源输出低频信号*/
   {
    voltage_orig = ANALOG_DATA.VRMS_LF_ORIG;
    current_orig = ANALOG_DATA.IRMS_LF_ORIG;
    voltage      = ANALOG_DATA.VRMS_LF;
    current      = ANALOG_DATA.IRMS_LF;
   }
  else if(POWER_OUT_FREQENCY == POWER_OUT_HF)/*当前变频电源输出高频信号*/
   {
    voltage_orig = ANALOG_DATA.VRMS_HF_ORIG;
    current_orig = ANALOG_DATA.IRMS_HF_ORIG;
    voltage      = ANALOG_DATA.VRMS_HF;
    current      = ANALOG_DATA.IRMS_HF;
   }
 /*--------------------------------显示实际角差----------------------------------------*/ 
 LCD_FloatNum(MEASURED_PHASE_DIFF,168,134,YES,5,FONT_32,MAROON,WHITE,3);//显示实际角差
 /*--------------------------------显示采样角差----------------------------------------*/ 
 LCD_FloatNum(ORIGINAL_PHASE_DIFF,168,170,YES,5,FONT_32,INDIGO,WHITE,4);//显示采样角差
 /*--------------------------------显示实际阻抗----------------------------------------*/ 
 LCD_FloatNum(MEASURED_IMPEDANCE,184,206,NO,5,FONT_32,MAROON,WHITE,5);//显示实际阻抗
 /*--------------------------------显示采样阻抗----------------------------------------*/ 
 LCD_FloatNum(MEASURED_IMPEDANCE / z_coe,184,242,NO,5,FONT_32,INDIGO,WHITE,6);//显示采样阻抗 
 /*--------------------------------显示实际电流----------------------------------------*/ 
 LCD_FloatNum(current,489,134,NO,5,FONT_32,MAROON,WHITE,3);//显示实际电流
 /*--------------------------------显示采样电流----------------------------------------*/ 
 LCD_FloatNum(current_orig,489,170,NO,5,FONT_32,INDIGO,WHITE,4);//显示采样电流
 /*--------------------------------显示实际电压----------------------------------------*/ 
 LCD_FloatNum(voltage,489,206,NO,5,FONT_32,MAROON,WHITE,5);//显示实际电压
 /*--------------------------------显示采样电压----------------------------------------*/ 
 LCD_FloatNum(voltage_orig,489,242,NO,5,FONT_32,INDIGO,WHITE,6);//显示采样电压
 
 /*---------------------------显示额定阻抗和设置电压-----------------------------------*/
 if(MODIFY_STATUS_KEY == NO)/*调整光标状态*/
  {
   for(i=0;i<6;i++)/*显示额定阻抗和设置电压*/
    {
     LCD_Put_Char(*(&EXAMPLE_IMPEDANCE_KEY+i)-0x20,184+i*16,291,FONT_32,INDIGO,WHITE,CDX3-2);//显示额定阻抗
     if(GAIN_GEAR_KEY < 6)//档位4之前的档位需要显示设置电压
       LCD_Put_Char(*(&EXAMPLE_VOLTAGE_KEY+i)-0x20,489+i*16,291,FONT_32,INDIGO,WHITE,CDX3-3);//显示设置电压
     else
       LCD_Put_Char(0x00,489+i*16,291,FONT_32,INDIGO,WHITE,1);//擦除前面显示的设置电压值
    }
  }
 else /*修改参数数值状态*/
  {
   for(i=0;i<6;i++)/*显示额定阻抗和设置电压*/
    {
     LCD_Put_Char(*(&EXAMPLE_IMPEDANCE_KEY+i)-0x20,184+i*16,291,FONT_32,INDIGO,WHITE,!(CDX3 == 2 && CDX4-1 == i && SLOW_SHAN_SIG));//显示额定阻抗
     if(GAIN_GEAR_KEY < 6)//档位4之前的档位需要显示设置电压
       LCD_Put_Char(*(&EXAMPLE_VOLTAGE_KEY+i)-0x20,489+i*16,291,FONT_32,INDIGO,WHITE,!(CDX3 == 3 && CDX4-1 == i && SLOW_SHAN_SIG));//显示设置电压
     else
       LCD_Put_Char(0x00,489+i*16,291,FONT_32,INDIGO,WHITE,1);//擦除前面显示的设置电压值
    }
  }
 /*------------------------显示"启动输出"、"开始校准"按钮----------------------------------*/
 LCD_Draw_Dynamic_Window(478,338,623,381,8,RED,CDX3-4);//绘制"启动输出"动态窗口,圆角半径8
 if(Get_Bit(SPWMPOWER_CTRL_FLAG,SPWMPOWER_TURN_ON_OFF_BIT)) LCD_Display_String(487,344,"停止输出",FONT_32,RED,WHITE,CDX3-4);
 else LCD_Display_String(487,344,"启动输出",FONT_32,RED,WHITE,CDX3-4);
 
 if(CALI_PROCESS_FLAG >= 4)/*变频逆变电源已经启动且电流调节完毕，可以进行校准*/
  {
   LCD_Draw_Dynamic_Window(478,391,623,434,8,DGREEN,CDX3-5);//绘制"开始校准"动态窗口,圆角半径8
   LCD_Display_String(487,397,"开始校准",FONT_32,DGREEN,WHITE,CDX3-5);
  }
 else /*没有启动输出或者电流调节没有完成，不可以进行校准*/
  {
   LCD_Area_Fill(478,391,627,439,WHITE);//擦除"开始校准"动态窗口
  }
 /*---------------------------------------显示说明帮助信息-------------------------------------*/
 switch(CDX3)
  {
   case 1:/*档位选择*/
          LCD_Display_String( 14,363,"选择需要校准的档位。下一步设置额定阻",FONT_24,INDIGO,WHITE,8);
          LCD_Display_String( 14,389,"抗值。启动变频逆变电源输出后，此选项",FONT_24,INDIGO,WHITE,8);
          LCD_Display_String( 14,415,"将不可更改。                        ",FONT_24,INDIGO,WHITE,8); 
          break;
   case 2:/*额定阻抗设置*/
          LCD_Display_String( 14,363,"根据校准工装的标准电阻的阻值设置额定",FONT_24,INDIGO,WHITE,8);
          LCD_Display_String( 14,389,"阻抗。下一步启动变频逆变电源输出。启",FONT_24,INDIGO,WHITE,8);
          LCD_Display_String( 14,415,"动输出后，此选项将不可更改。        ",FONT_24,INDIGO,WHITE,8);
          break;
   case 3:/*设置电压设置*/
          LCD_Display_String( 14,363,"根据万用表测得的电压值进行设置。下一",FONT_24,INDIGO,WHITE,8);
          LCD_Display_String( 14,389,"步开始校准。变频逆变电源启动输出前，",FONT_24,INDIGO,WHITE,8);
          LCD_Display_String( 14,415,"此选项不可更改。                    ",FONT_24,INDIGO,WHITE,8);
          break;
   case 4:/*启动、停止按钮*/
          if(CALI_PROCESS_FLAG >= 4)/*变频逆变电源已经启动且电流调节完毕，可以设置电压值*/
           {
            if(GAIN_GEAR_KEY < 6)//档位4之前的档位需要校准电压和电流系数
             {
              LCD_Display_String( 14,363,"输出电流调节完成，请测量并设置输出电",FONT_24,INDIGO,WHITE,8);
              LCD_Display_String( 14,389,"电压值。设置完成后即可进行校准。    ",FONT_24,INDIGO,WHITE,8);
              LCD_Display_String( 14,415,"按确认键停止变频逆变电源输出。      ",FONT_24,   RED,WHITE,8);
             }
            else
             {
              LCD_Display_String( 14,363,"输出电流调节完成，待数据稳定后，即可",FONT_24,INDIGO,WHITE,8);
              LCD_Display_String( 14,389,"进行校准。                          ",FONT_24,INDIGO,WHITE,8);
              LCD_Display_String( 14,415,"按确认键停止变频逆变电源输出。      ",FONT_24,   RED,WHITE,8);
             }            
           }
          else
           {          
            LCD_Display_String( 14,363,"启动或停止变频逆变电源输出。启动输出",FONT_24,RED,WHITE,8);
            LCD_Display_String( 14,389,"前，应检查连接的校准工装的标准电阻值",FONT_24,RED,WHITE,8);
            LCD_Display_String( 14,415,"是否正确。电阻值参考额定阻抗。      ",FONT_24,RED,WHITE,8);
           } 
          break;
   case 5:/*开始校准按钮*/
          if(GAIN_GEAR_KEY < 6)//档位4之前的档位需要校准电压和电流系数
           {
            LCD_Display_String( 14,363,"开始校准前，应先等待所有数据稳定。  ",FONT_24,RED,WHITE,8);
            LCD_Display_String( 14,389,"之后，请先修改设置电压！然后，即可进",FONT_24,RED,WHITE,8);
            LCD_Display_String( 14,415,"行校准操作。                        ",FONT_24,RED,WHITE,8);
           }
          else
           {
            LCD_Display_String( 14,363,"开始校准前，应先等待所有数据稳定。  ",FONT_24,RED,WHITE,8);
            LCD_Display_String( 14,389,"之后，即可进行校准操作。            ",FONT_24,RED,WHITE,8);
            LCD_Display_String( 14,415,"                                    ",FONT_24,RED,WHITE,8);
           }
          break;
  } 
}

/*******************************************************************************************************************************
屏幕代码11，测试继电器动作屏幕
*******************************************************************************************************************************/
void Screen11(void)
{
 LCD_Area_Fill(0,28,X_PIXEL_MAX,443,WHITE);//除顶部、底部状态栏位置以外填充白色
 LCD_BACK_COLOR = WHITE;                   //记录当前背景色
 LCD_Put_String(  8, 40,"系统参数设置 > 测试继电器动作",FONT_24,BLACK,WHITE,1);
 LCD_Draw_Line(  0, 76,639, 76,BLACK);//画横线
 LCD_Put_String(224,148,"输出控制: ",FONT_32,BLACK,WHITE,2);
 LCD_Put_String(224,196,"分压切换: ",FONT_32,BLACK,WHITE,3);
 LCD_Put_String(224,244,"滤波切换: ",FONT_32,BLACK,WHITE,4);
 LCD_Put_String(224,292,"电压增益: ",FONT_32,BLACK,WHITE,5);
 LCD_Put_String(224,340,"电流增益: ",FONT_32,BLACK,WHITE,6);
} 
/*------------------------------------------------------------------------------------------------------------------------------
屏幕代码11，测试继电器动作屏幕数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Dupscr11(void)
{
 if(Get_Bit(RELAY_CTRL,JDQ_OUT)) LCD_Put_Dot3232(0x05,384,148,INDIGO,WHITE,CDX3-1);//输出控制继电器吸合
 else LCD_Put_Dot3232(0x06,384,148,INDIGO,WHITE,CDX3-1);//输出控制继电器释放
 if(Get_Bit(RELAY_CTRL,JDQ_FY)) LCD_Put_Dot3232(0x05,384,196,INDIGO,WHITE,CDX3-2);//分压电阻控制继电器吸合
 else LCD_Put_Dot3232(0x06,384,196,INDIGO,WHITE,CDX3-2);//分压电阻控制继电器释放
 if(Get_Bit(RELAY_CTRL,JDQ_QH)) LCD_Put_Dot3232(0x05,384,244,INDIGO,WHITE,CDX3-3);//滤波电路切换控制继电器吸合
 else LCD_Put_Dot3232(0x06,384,244,INDIGO,WHITE,CDX3-3);//滤波电路切换控制继电器释放
 if(Get_Bit(RELAY_CTRL,JDQ_VZY)) LCD_Put_Dot3232(0x05,384,292,INDIGO,WHITE,CDX3-4);//电压增益控制继电器吸合
 else LCD_Put_Dot3232(0x06,384,292,INDIGO,WHITE,CDX3-4);//电压增益控制继电器释放
 if(Get_Bit(RELAY_CTRL,JDQ_IZY)) LCD_Put_Dot3232(0x05,384,340,INDIGO,WHITE,CDX3-5);//电流增益控制继电器吸合
 else LCD_Put_Dot3232(0x06,384,340,INDIGO,WHITE,CDX3-5);//电流增益控制继电器释放
}

/*******************************************************************************************************************************
屏幕代码12，清除存储测量记录屏幕
*******************************************************************************************************************************/
void Screen12(void)
{
 LCD_Area_Fill(0,28,X_PIXEL_MAX,443,WHITE);//除顶部、底部状态栏位置以外填充白色
 LCD_BACK_COLOR = WHITE;                   //记录当前背景色
 LCD_Put_String(  8, 40,"系统参数设置 > 清除存储测量记录",FONT_24,BLACK,WHITE,1);
 LCD_Draw_Line(  0, 76,639, 76,BLACK);//画横线
 LCD_Put_String(144,150,"可存储测量记录:     条",FONT_32,INDIGO,WHITE,1);
 LCD_Put_String(144,202,"已存储测量记录:     条",FONT_32,INDIGO,WHITE,2);
 LCD_Put_String(144,254,"      剩余空间:     条",FONT_32,INDIGO,WHITE,3);
} 
/*------------------------------------------------------------------------------------------------------------------------------
屏幕代码12，清除存储测量记录屏幕数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Dupscr12(void)
{
 LCD_IntNum(TEST_RECORD_AMOUNT_MAX,                   400,150, NO,3,0,FONT_32,INDIGO,WHITE,1);//显示可存储测试记录总条数
 LCD_IntNum(TEST_RECORD_AMOUNT,                       400,202, NO,3,0,FONT_32,INDIGO,WHITE,1);//显示已经保存的测试记录条数
 LCD_IntNum(TEST_RECORD_AMOUNT_MAX-TEST_RECORD_AMOUNT,400,254, NO,3,0,FONT_32,INDIGO,WHITE,1);//显示剩余空间
 /*----------------------------------------显示控制按钮------------------------------------------*/
 LCD_Draw_Dynamic_Window(183,358,456,406,8,INDIGO,CDX3-1);//绘制1个选项的动态窗口,圆角半径8 
 LCD_Display_String(192,367,"清除所有测试记录",FONT_32,INDIGO,WHITE,CDX3-1);
}

/*******************************************************************************************************************************
屏幕代码13，恢复出厂参数设置屏幕
*******************************************************************************************************************************/
void Screen13(void)
{
 LCD_Area_Fill(0,28,X_PIXEL_MAX,443,WHITE);//除顶部、底部状态栏位置以外填充白色
 LCD_BACK_COLOR = WHITE;                   //记录当前背景色
 LCD_Put_String(  8, 40,"系统参数设置 > 恢复出厂参数设置",FONT_24,BLACK,WHITE,1);
 LCD_Draw_Line(  0, 76,639, 76,BLACK);//画横线
} 
/*------------------------------------------------------------------------------------------------------------------------------
屏幕代码13，恢复出厂参数设置屏幕数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Dupscr13(void)
{
 LCD_Display_String(208, 97,"测量参数设置    ",FONT_32,INDIGO,WHITE,CDX3-1);
 LCD_Display_String(208,149,"电池电压校准系数",FONT_32,INDIGO,WHITE,CDX3-2);
     LCD_Put_String(208,201,"零序3U0 校准系数",FONT_32,INDIGO,WHITE,CDX3-3); 
 LCD_Display_String(208,253,"量程档位校准系数",FONT_32,INDIGO,WHITE,CDX3-4);
 if(Get_Bit(RECOVERY_KEY,TESTING_SET_BIT))  LCD_Put_Dot3232(0x01,176, 97,INDIGO,WHITE,CDX3-1);//●符号
 else                                       LCD_Put_Dot3232(0x00,176, 97,INDIGO,WHITE,CDX3-1);//○符号
 if(Get_Bit(RECOVERY_KEY,BAT_COE_BIT))      LCD_Put_Dot3232(0x01,176,149,INDIGO,WHITE,CDX3-2);//●符号
 else                                       LCD_Put_Dot3232(0x00,176,149,INDIGO,WHITE,CDX3-2);//○符号
 if(Get_Bit(RECOVERY_KEY,U03_COE_BIT))      LCD_Put_Dot3232(0x01,176,201,INDIGO,WHITE,CDX3-3);//●符号
 else                                       LCD_Put_Dot3232(0x00,176,201,INDIGO,WHITE,CDX3-3);//○符号
 if(Get_Bit(RECOVERY_KEY,RAN_COE_BIT))      LCD_Put_Dot3232(0x01,176,253,INDIGO,WHITE,CDX3-4);//●符号
 else                                       LCD_Put_Dot3232(0x00,176,253,INDIGO,WHITE,CDX3-4);//○符号
 /*----------------------------------------显示控制按钮------------------------------------------*/
 LCD_Draw_Dynamic_Window(215,350,424,398,8,INDIGO,CDX3-5);//绘制1个选项的动态窗口,圆角半径8 
 LCD_Display_String(224,359,"恢复出厂参数",FONT_32,INDIGO,WHITE,CDX3-5);
}

/*******************************************************************************************************************************
屏幕代码14，设置仪器出厂编号屏幕
*******************************************************************************************************************************/
void Screen14(void)
{
 LCD_Area_Fill(0,28,X_PIXEL_MAX,443,WHITE);//除顶部、底部状态栏位置以外填充白色
 LCD_BACK_COLOR = WHITE;                   //记录当前背景色
 LCD_Put_String(  8, 40,"系统参数设置 > 设置仪器出厂编号",FONT_24,BLACK,WHITE,1);
 LCD_Draw_Line(  0, 76,639, 76,BLACK);//画横线
 LCD_Put_String(200,180,"请输入编号",FONT_48,NAVY,WHITE,1);
} 
/*------------------------------------------------------------------------------------------------------------------------------
屏幕代码14，设置仪器出厂编号屏幕数据刷新                                                     
------------------------------------------------------------------------------------------------------------------------------*/
void Dupscr14(void)
{
 unsigned char i;
 for(i=0;i<4;i++) LCD_Put_Char(KEY_CHAR_TMP[i]+0x10,266+i*28,252,FONT_48,INDIGO,WHITE,CDX3-1-i);//显示输入的编号
}






/*##############################################################################################################################   
液晶屏刷新处理程序
##############################################################################################################################*/
void LCD_Update(void)
{
 /*------------------------------------------------------------------------------------------------------------------------------
 全屏刷新
 ------------------------------------------------------------------------------------------------------------------------------*/
 if(SCRAPPLY == YES)
  {
   if(BURST_SCRFL)     //突发屏幕编号不为0，有突发屏幕显示请求
    {
     switch(BURST_SCRFL)
      {
       case 1:Burst_Screen1();
              Burst_Dupscr1();
              break;
      }
    }
   else if(POPUP_SCRFL)//弹出屏幕编号不为0，有弹出屏幕显示请求
    {
     switch(POPUP_SCRFL)
      {
       case  1:Popup_Screen1();
               Popup_Dupscr1();
               break;
       case  2:Popup_Screen2();
               Popup_Dupscr2();
               break;
       case  3:Popup_Screen3();
               Popup_Dupscr3();
               break;
       case  4:Popup_Screen4();
               Popup_Dupscr4();
               break;
       case  5:Popup_Screen5();
               Popup_Dupscr5();
               break;
       case  6:Popup_Screen6();
               Popup_Dupscr6();
               break;
       case  7:Popup_Screen7();
               Popup_Dupscr7();
               break;
       case  8:Popup_Screen8();
               Popup_Dupscr8();
               break;
       case  9:Popup_Screen9();
               Popup_Dupscr9();
               break;
       case 10:Popup_Screen10();
               Popup_Dupscr10();
               break;
       case 11:Popup_Screen11();
               Popup_Dupscr11();
               break;
       case 12:Popup_Screen12();
               Popup_Dupscr12();
               break;
       default:break;
      }
    }
   else    //正常屏幕显示
    {
     switch(SCRFL)
      {
       case  0:Screen0();
               Dupscr0();
               break;
       case  1:Screen1();
               Dupscr1();
               break;
       case  2:Screen2();
               Dupscr2();
               break;
       case  3:Screen3();
               Dupscr3();
               break;
       case  4:Screen4();
               Dupscr4();
               break;
       case  5:Screen5();
               Dupscr5();
               break;
       case  6:Screen6();
               Dupscr6();
               break;
       case  7:Screen7();
               Dupscr7();
               break;
       case  8:Screen8();
               Dupscr8();
               break;
       case  9:Screen9();
               Dupscr9();
               break;
       case 10:Screen10();
               Dupscr10();
               break;
       case 11:Screen11();
               Dupscr11();
               break;
       case 12:Screen12();
               Dupscr12();
               break;
       case 13:Screen13();
               Dupscr13();
               break;
       case 14:Screen14();
               Dupscr14();
               break;
       default:break;
      }
    }
   SCRAPPLY = NO;
   DUPDATE  = NO; //全屏刷新执行完毕后不再执行下面的数据刷新
  }
 /*------------------------------------------------------------------------------------------------------------------------------
 数据刷新
 ------------------------------------------------------------------------------------------------------------------------------*/
 if(DUPDATE == YES)
  {
   if(BURST_SCRFL)     //突发屏幕编号不为0，有突发屏幕显示请求
    {
     switch(BURST_SCRFL)
      {
       case 1:Burst_Dupscr1();
              break;
      }
    }
   else if(POPUP_SCRFL)//弹出屏幕编号不为0，有弹出屏幕显示请求
    {
     switch(POPUP_SCRFL)
      {
       case  1:Popup_Dupscr1();
               break;
       case  2:Popup_Dupscr2();
               break;
       case  3:Popup_Dupscr3();
               break;
       case  4:Popup_Dupscr4();
               break;
       case  5:Popup_Dupscr5();
               break;
       case  6:Popup_Dupscr6();
               break;
       case  7:Popup_Dupscr7();
               break;
       case  8:Popup_Dupscr8();
               break;
       case  9:Popup_Dupscr9();
               break;
       case 10:Popup_Dupscr10();
               break;
       case 11:Popup_Dupscr11();
               break;
       case 12:Popup_Dupscr12();
               break;
       default:break;
      }
    }
   else    //正常屏幕显示
    {
     switch(SCRFL)
      {
       case  0:Dupscr0();
               break;
       case  1:Dupscr1();
               break;
       case  2:Dupscr2();
               break;
       case  3:Dupscr3();
               break;
       case  4:Dupscr4();
               break;
       case  5:Dupscr5();
               break;
       case  6:Dupscr6();
               break;
       case  7:Dupscr7();
               break;
       case  8:Dupscr8();
               break;
       case  9:Dupscr9();
               break;
       case 10:Dupscr10();
               break;
       case 11:Dupscr11();
               break;
       case 12:Dupscr12();
               break;
       case 13:Dupscr13();
               break;
       case 14:Dupscr14();
               break;
       default:break;
      }
    }
   DUPDATE = NO;
  }
}
