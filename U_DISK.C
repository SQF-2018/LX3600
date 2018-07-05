/*************************************************************************************************/
//文件名:	U_DISK.c		USB数据存储部分
//设计者:	程元
//日期:	2014/12/25
/*************************************************************************************************/
#include "QJBL.H"
#include "FILE_SYS.H"
#include "stdio.h"

/*************************************************************************************************/
//向U盘文件写入字节数据
//字节数据以ASCII码形式写入
/*************************************************************************************************/
void U_Disk_Write_Number(unsigned char number)
{   
 number += 0x30;
 CH376ByteWrite(&number,1,NULL);
}
/*************************************************************************************************/
//向U盘文件写入回车换行符
//参数: n 回车换行符的数量
/*************************************************************************************************/
void U_Disk_Write_Enter(unsigned char n)
{   
 unsigned char i;
 for(i=0;i<n;i++)
  {
   CH376ByteWrite("\n",2,NULL);
  }
}
/*************************************************************************************************/
//向U盘相应文件写入测量结果数据
/*************************************************************************************************/
void File_Of_Test_Data(void)
{
 unsigned int tmp;
 /*--------------------------保存表头--------------------------------*/
 CH376ByteWrite("* * * * 电容电流测量结果 * * * *",32,NULL);
 U_Disk_Write_Enter(1);//换行
 /*------------------------保存试验时间------------------------------*/
 CH376ByteWrite("测量时间:",9,NULL);
 U_Disk_Write_Number(2);
 U_Disk_Write_Number(0);
 U_Disk_Write_Number((TEST_DATA.year & 0xf0)>>4);
 U_Disk_Write_Number(TEST_DATA.year & 0x0f);
 CH376ByteWrite("年",2,NULL);
 U_Disk_Write_Number((TEST_DATA.month & 0xf0)>>4);
 U_Disk_Write_Number(TEST_DATA.month & 0x0f);
 CH376ByteWrite("月",2,NULL);
 U_Disk_Write_Number((TEST_DATA.date & 0xf0)>>4);
 U_Disk_Write_Number(TEST_DATA.date & 0x0f);
 CH376ByteWrite("日 ",2,NULL);
 U_Disk_Write_Enter(1);
 CH376ByteWrite("         ",9,NULL);
 U_Disk_Write_Number((TEST_DATA.hour & 0xf0)>>4);
 U_Disk_Write_Number(TEST_DATA.hour & 0x0f);
 CH376ByteWrite("时",2,NULL);
 U_Disk_Write_Number((TEST_DATA.minute & 0xf0)>>4);
 U_Disk_Write_Number(TEST_DATA.minute & 0x0f);
 CH376ByteWrite("分",2,NULL);
 U_Disk_Write_Number((TEST_DATA.second & 0xf0)>>4);
 U_Disk_Write_Number(TEST_DATA.second & 0x0f);
 CH376ByteWrite("秒",2,NULL);
 U_Disk_Write_Enter(1);//换行
 CH376ByteWrite("试验编号:",9,NULL);
 U_Disk_Write_Number(TEST_DATA.TEST_NUMBER[0]);
 U_Disk_Write_Number(TEST_DATA.TEST_NUMBER[1]);
 U_Disk_Write_Number(TEST_DATA.TEST_NUMBER[2]);
 U_Disk_Write_Number(TEST_DATA.TEST_NUMBER[3]);
 U_Disk_Write_Number(TEST_DATA.TEST_NUMBER[4]);
 U_Disk_Write_Number(TEST_DATA.TEST_NUMBER[5]);
 U_Disk_Write_Enter(1);//换行
 //CH376ByteWrite("设备 I D:",9,NULL);
 CH376ByteWrite("设备名称:",9,NULL);//新的蓝牙通讯规约中将“设备ID”改为“设备名称”
 U_Disk_Write_Number(TEST_DATA.DEVICE_ID[0]);
 U_Disk_Write_Number(TEST_DATA.DEVICE_ID[1]);
 U_Disk_Write_Number(TEST_DATA.DEVICE_ID[2]);
 U_Disk_Write_Number(TEST_DATA.DEVICE_ID[3]);
 U_Disk_Write_Number(TEST_DATA.DEVICE_ID[4]);
 U_Disk_Write_Number(TEST_DATA.DEVICE_ID[5]);
 U_Disk_Write_Enter(1);//换行
 CH376ByteWrite("额定高压:",9,NULL);
 U_Disk_Write_Number(TEST_DATA.RATED_VOLTAGE/100);
 U_Disk_Write_Number(TEST_DATA.RATED_VOLTAGE%100/10);
 CH376ByteWrite(".",1,NULL);
 U_Disk_Write_Number(TEST_DATA.RATED_VOLTAGE%100%10);
 CH376ByteWrite("KV",2,NULL);
 U_Disk_Write_Enter(1);//换行 
 CH376ByteWrite("母线电压:",9,NULL);
 U_Disk_Write_Number(TEST_DATA.BUS_VOLTAGE/100);
 U_Disk_Write_Number(TEST_DATA.BUS_VOLTAGE%100/10);
 CH376ByteWrite(".",1,NULL);
 U_Disk_Write_Number(TEST_DATA.BUS_VOLTAGE%100%10);
 CH376ByteWrite("KV",2,NULL);
 U_Disk_Write_Enter(1);//换行 
 /*------------------------保存连接方式------------------------------*/
 CH376ByteWrite("连接方式:",9,NULL);
 switch(TEST_DATA.PT_CONNECT_MODE)
  {
   case PT_3PT ://3PT
   default     ://3PT
                CH376ByteWrite("3PT ",4,NULL);
                break;
   case PT_3PT1://3PT1
                CH376ByteWrite("3PT1",4,NULL);
                break;
   case PT_3PT2://3PT2
                CH376ByteWrite("3PT2",4,NULL);
                break;
   case PT_4PT ://4PT
                CH376ByteWrite("4PT ",4,NULL);
                break;
   case PT_4PT1://4PT1
                CH376ByteWrite("4PT1",4,NULL);
                break;
   case PT_4PT2://4PT2
                CH376ByteWrite("4PT2",4,NULL);
                break;
   case PT_1PT ://1PT
                CH376ByteWrite("1PT ",4,NULL);
                break;
   case PT_C1PT://C1PT
                CH376ByteWrite("C1PT",4,NULL);
                break;
  }
 U_Disk_Write_Enter(1);//换行
 /*------------------------保存PT变比------------------------------*/
 CH376ByteWrite("P T 变比:",9,NULL);
 if(TEST_DATA.PT_TRANS_RAT < 10)
  {
   tmp = TEST_DATA.PT_TRANS_RAT * 10000;
   U_Disk_Write_Number(tmp/10000);
   CH376ByteWrite(".",1,NULL);
   U_Disk_Write_Number(tmp%10000/1000);
   U_Disk_Write_Number(tmp%10000%1000/100);
   U_Disk_Write_Number(tmp%10000%1000%100/10);
   U_Disk_Write_Number(tmp%10000%1000%100%10);
  }
 else if(TEST_DATA.PT_TRANS_RAT < 100)
  {
   tmp = TEST_DATA.PT_TRANS_RAT * 1000;
   U_Disk_Write_Number(tmp/10000);
   U_Disk_Write_Number(tmp%10000/1000);
   CH376ByteWrite(".",1,NULL);
   U_Disk_Write_Number(tmp%10000%1000/100);
   U_Disk_Write_Number(tmp%10000%1000%100/10);
   U_Disk_Write_Number(tmp%10000%1000%100%10);
  }
 else if(TEST_DATA.PT_TRANS_RAT < 1000)  
  {
   tmp = TEST_DATA.PT_TRANS_RAT * 100;
   U_Disk_Write_Number(tmp/10000);
   U_Disk_Write_Number(tmp%10000/1000);
   U_Disk_Write_Number(tmp%10000%1000/100);
   CH376ByteWrite(".",1,NULL);
   U_Disk_Write_Number(tmp%10000%1000%100/10);
   U_Disk_Write_Number(tmp%10000%1000%100%10);
  }
 else if(TEST_DATA.PT_TRANS_RAT < 10000) 
  {
   tmp = TEST_DATA.PT_TRANS_RAT * 10;
   U_Disk_Write_Number(tmp/10000);
   U_Disk_Write_Number(tmp%10000/1000);
   U_Disk_Write_Number(tmp%10000%1000/100);
   U_Disk_Write_Number(tmp%10000%1000%100/10);
   CH376ByteWrite(".",1,NULL);
   U_Disk_Write_Number(tmp%10000%1000%100%10);
  }
 else
  {
   tmp = TEST_DATA.PT_TRANS_RAT;
   U_Disk_Write_Number(tmp/10000);
   U_Disk_Write_Number(tmp%10000/1000);
   U_Disk_Write_Number(tmp%10000%1000/100);
   U_Disk_Write_Number(tmp%10000%1000%100/10);
   U_Disk_Write_Number(tmp%10000%1000%100%10);
  }
 U_Disk_Write_Enter(1);//换行
 /*------------------------保存零序3U0------------------------------*/
 CH376ByteWrite("零序 3U0:",9,NULL);
 if(TEST_DATA.VRMS_50HZ < 10) 
  {
   tmp = TEST_DATA.VRMS_50HZ * 1000;
   U_Disk_Write_Number(tmp/1000);
   CH376ByteWrite(".",1,NULL);
   U_Disk_Write_Number(tmp%1000/100);
   U_Disk_Write_Number(tmp%1000%100/10);
   U_Disk_Write_Number(tmp%1000%100%10);
  }
 else if(TEST_DATA.VRMS_50HZ < 100) 
  {
   tmp = TEST_DATA.VRMS_50HZ * 100;
   U_Disk_Write_Number(tmp/1000);
   U_Disk_Write_Number(tmp%1000/100);
   CH376ByteWrite(".",1,NULL);
   U_Disk_Write_Number(tmp%1000%100/10);
   U_Disk_Write_Number(tmp%1000%100%10);
  }
 else 
  {
   tmp = TEST_DATA.VRMS_50HZ * 100;
   U_Disk_Write_Number(tmp/10000);
   U_Disk_Write_Number(tmp%10000/1000);
   U_Disk_Write_Number(tmp%10000%1000/100);
   CH376ByteWrite(".",1,NULL);
   U_Disk_Write_Number(tmp%10000%1000%100/10);
   U_Disk_Write_Number(tmp%10000%1000%100%10);
  }
 CH376ByteWrite("V",1,NULL);
 U_Disk_Write_Enter(1);//换行
 /*---------------------保存电容器组容量----------------------------*/
 if(TEST_DATA.PT_CONNECT_MODE == PT_C1PT)//C1PT接线方式，保存电容器组容量
  {   
   CH376ByteWrite("A相电容器组:",12,NULL);
   U_Disk_Write_Number(PHASE_A_CAP_KEY1);
   U_Disk_Write_Number(PHASE_A_CAP_KEY2);
   U_Disk_Write_Number(PHASE_A_CAP_KEY3);
   U_Disk_Write_Number(PHASE_A_CAP_KEY4);
   CH376ByteWrite(".",1,NULL);
   U_Disk_Write_Number(PHASE_A_CAP_KEY5);
   U_Disk_Write_Number(PHASE_A_CAP_KEY6);
   CH376ByteWrite("uF",2,NULL);
   U_Disk_Write_Enter(1);//换行
   CH376ByteWrite("B相电容器组:",12,NULL);
   U_Disk_Write_Number(PHASE_B_CAP_KEY1);
   U_Disk_Write_Number(PHASE_B_CAP_KEY2);
   U_Disk_Write_Number(PHASE_B_CAP_KEY3);
   U_Disk_Write_Number(PHASE_B_CAP_KEY4);
   CH376ByteWrite(".",1,NULL);
   U_Disk_Write_Number(PHASE_B_CAP_KEY5);
   U_Disk_Write_Number(PHASE_B_CAP_KEY6);
   CH376ByteWrite("uF",2,NULL);
   U_Disk_Write_Enter(1);//换行
   CH376ByteWrite("C相电容器组:",12,NULL);
   U_Disk_Write_Number(PHASE_C_CAP_KEY1);
   U_Disk_Write_Number(PHASE_C_CAP_KEY2);
   U_Disk_Write_Number(PHASE_C_CAP_KEY3);
   U_Disk_Write_Number(PHASE_C_CAP_KEY4);
   CH376ByteWrite(".",1,NULL);
   U_Disk_Write_Number(PHASE_C_CAP_KEY5);
   U_Disk_Write_Number(PHASE_C_CAP_KEY6);
   CH376ByteWrite("uF",2,NULL);
   U_Disk_Write_Enter(1);//换行
  }
 /*------------------------保存电容量C------------------------------*/
 CH376ByteWrite("- - - - - - - - - - - -",23,NULL);
 U_Disk_Write_Enter(1);//换行
 CH376ByteWrite("  电容容量C: ",13,NULL);
 if(TEST_DATA.CAPACITANCE < 10)
  {
   tmp = TEST_DATA.CAPACITANCE * 1000;
   U_Disk_Write_Number(tmp/1000);
   CH376ByteWrite(".",1,NULL);
   U_Disk_Write_Number(tmp%1000/100);
   U_Disk_Write_Number(tmp%1000%100/10);
   U_Disk_Write_Number(tmp%1000%100%10);
  }
 else if(TEST_DATA.CAPACITANCE < 100)
  {
   tmp = TEST_DATA.CAPACITANCE * 100;
   U_Disk_Write_Number(tmp/1000);
   U_Disk_Write_Number(tmp%1000/100);
   CH376ByteWrite(".",1,NULL);
   U_Disk_Write_Number(tmp%1000%100/10);
   U_Disk_Write_Number(tmp%1000%100%10);
  }
 else if(TEST_DATA.CAPACITANCE < 1000)
  {
   tmp = TEST_DATA.CAPACITANCE * 10;
   U_Disk_Write_Number(tmp/1000);
   U_Disk_Write_Number(tmp%1000/100);
   U_Disk_Write_Number(tmp%1000%100/10);
   CH376ByteWrite(".",1,NULL);
   U_Disk_Write_Number(tmp%1000%100%10);
  }
 else
  {
   tmp = TEST_DATA.CAPACITANCE;
   U_Disk_Write_Number(tmp/1000);
   U_Disk_Write_Number(tmp%1000/100);
   U_Disk_Write_Number(tmp%1000%100/10);
   U_Disk_Write_Number(tmp%1000%100%10);
  }
 CH376ByteWrite(" uF",3,NULL);
 U_Disk_Write_Enter(1);//换行
 /*------------------------保存电容电流------------------------------*/
 CH376ByteWrite("  电容电流I: ",13,NULL);
 if(TEST_DATA.CAP_CURRENT < 10)
  {
   tmp = TEST_DATA.CAP_CURRENT * 1000;
   U_Disk_Write_Number(tmp/1000);
   CH376ByteWrite(".",1,NULL);
   U_Disk_Write_Number(tmp%1000/100);
   U_Disk_Write_Number(tmp%1000%100/10);
   U_Disk_Write_Number(tmp%1000%100%10);
  }
 else if(TEST_DATA.CAP_CURRENT < 100)
  {
   tmp = TEST_DATA.CAP_CURRENT * 100;
   U_Disk_Write_Number(tmp/1000);
   U_Disk_Write_Number(tmp%1000/100);
   CH376ByteWrite(".",1,NULL);
   U_Disk_Write_Number(tmp%1000%100/10);
   U_Disk_Write_Number(tmp%1000%100%10);
  }
 else if(TEST_DATA.CAP_CURRENT < 1000)
  {
   tmp = TEST_DATA.CAP_CURRENT * 10;
   U_Disk_Write_Number(tmp/1000);
   U_Disk_Write_Number(tmp%1000/100);
   U_Disk_Write_Number(tmp%1000%100/10);
   CH376ByteWrite(".",1,NULL);
   U_Disk_Write_Number(tmp%1000%100%10);
  }
 else
  {
   tmp = TEST_DATA.CAP_CURRENT;
   U_Disk_Write_Number(tmp/1000);
   U_Disk_Write_Number(tmp%1000/100);
   U_Disk_Write_Number(tmp%1000%100/10);
   U_Disk_Write_Number(tmp%1000%100%10);
  }
 CH376ByteWrite(" A",2,NULL);
}
/*************************************************************************************************/
//向U盘写入测量结果文件
/*************************************************************************************************/
void U_Disk_Write_Test_Data_File(void)
{
 unsigned char  buf[25];
 unsigned short FileDate,FileTime;
 //将测量结果数据中的年月日插入到字符串“/DR”之后(目录名不能超过8个字符)
 sprintf((char*)buf,"/DR%d%d%d%d%d%d",(TEST_DATA.year&0xf0)>>4,TEST_DATA.year&0x0f,(TEST_DATA.month&0xf0)>>4,TEST_DATA.month&0x0f,(TEST_DATA.date&0xf0)>>4,TEST_DATA.date&0x0f);
 CH376DirCreate(buf);//在根目录下新建文件夹并打开，如果存在则直接打开
 //读取当前文件的目录信息FAT_DIR_INFO,将相关数据调到内存中
 if(CH376DirInfoRead() == USB_INT_SUCCESS) //如果读取成功再更改属性
  {
   FileDate = MAKE_FILE_DATE(2000+Bcd_To_Hex(TEST_DATA.year),Bcd_To_Hex(TEST_DATA.month),Bcd_To_Hex(TEST_DATA.date));
   FileTime = MAKE_FILE_TIME(Bcd_To_Hex(TEST_DATA.hour),Bcd_To_Hex(TEST_DATA.minute),Bcd_To_Hex(TEST_DATA.second));
   buf[0] = FileDate & 0x00ff;//文件创建的时间,低8位在前,高8位在后
   buf[1] = (FileDate >> 8) & 0x00ff;
   CH376WriteOfsBlock(buf,0x10,2);//修改文件创建日期
   CH376WriteOfsBlock(buf,0x18,2);//修改文件修改日期
   buf[0] = FileTime & 0x00ff;
   buf[1] = (FileTime >> 8) & 0x00ff;
   CH376WriteOfsBlock(buf,0x0e,2);//修改文件创建时间
   CH376WriteOfsBlock(buf,0x16,2);//修改文件修改时间
   CH376DirInfoSave(); //保存文件的目录信息 
  }
 //将测量结果数据中的时分秒插入到字符串“.DOC”之前
 sprintf((char*)buf,"%d%d%d%d%d%d.DOC",(TEST_DATA.hour&0xf0)>>4,TEST_DATA.hour&0x0f,(TEST_DATA.minute&0xf0)>>4,TEST_DATA.minute&0x0f,(TEST_DATA.second&0xf0)>>4,TEST_DATA.second&0x0f);
 CH376FileCreate(buf);//新建文件并打开,如果文件已经存在则先删除再新建  
 File_Of_Test_Data(); //向新建并打开的文件中写入测量结果数据
 //读取当前文件的目录信息FAT_DIR_INFO,将相关数据调到内存中
 if(CH376DirInfoRead() == USB_INT_SUCCESS)//如果读取成功在更改属性 
  {
   FileDate = MAKE_FILE_DATE(2000+Bcd_To_Hex(TEST_DATA.year),Bcd_To_Hex(TEST_DATA.month),Bcd_To_Hex(TEST_DATA.date));
   FileTime = MAKE_FILE_TIME(Bcd_To_Hex(TEST_DATA.hour),Bcd_To_Hex(TEST_DATA.minute),Bcd_To_Hex(TEST_DATA.second));
   buf[0] = FileDate & 0x00ff;
   buf[1] = (FileDate >> 8) & 0x00ff;
   CH376WriteOfsBlock(buf,0x10,2);//修改文件创建日期
   CH376WriteOfsBlock(buf,0x18,2);//修改文件修改日期
   buf[0] = FileTime & 0x00ff;
   buf[1] = (FileTime >> 8) & 0x00ff;
   CH376WriteOfsBlock(buf,0x0e,2);//修改文件创建时间
   CH376WriteOfsBlock(buf,0x16,2);//修改文件修改时间
   CH376DirInfoSave(); //保存文件的目录信息 
  }
 CH376FileClose(TRUE);  //关闭文件,自动计算文件长度,以字节为单位写文件,建议让程序库关闭文件以便自动更新文件长度
}
