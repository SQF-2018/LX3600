/*************************************************************************************************/
//�ļ���:	U_DISK.c		USB���ݴ洢����
//�����:	��Ԫ
//����:	2014/12/25
/*************************************************************************************************/
#include "QJBL.H"
#include "FILE_SYS.H"
#include "stdio.h"

/*************************************************************************************************/
//��U���ļ�д���ֽ�����
//�ֽ�������ASCII����ʽд��
/*************************************************************************************************/
void U_Disk_Write_Number(unsigned char number)
{   
 number += 0x30;
 CH376ByteWrite(&number,1,NULL);
}
/*************************************************************************************************/
//��U���ļ�д��س����з�
//����: n �س����з�������
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
//��U����Ӧ�ļ�д������������
/*************************************************************************************************/
void File_Of_Test_Data(void)
{
 unsigned int tmp;
 /*--------------------------�����ͷ--------------------------------*/
 CH376ByteWrite("* * * * ���ݵ���������� * * * *",32,NULL);
 U_Disk_Write_Enter(1);//����
 /*------------------------��������ʱ��------------------------------*/
 CH376ByteWrite("����ʱ��:",9,NULL);
 U_Disk_Write_Number(2);
 U_Disk_Write_Number(0);
 U_Disk_Write_Number((TEST_DATA.year & 0xf0)>>4);
 U_Disk_Write_Number(TEST_DATA.year & 0x0f);
 CH376ByteWrite("��",2,NULL);
 U_Disk_Write_Number((TEST_DATA.month & 0xf0)>>4);
 U_Disk_Write_Number(TEST_DATA.month & 0x0f);
 CH376ByteWrite("��",2,NULL);
 U_Disk_Write_Number((TEST_DATA.date & 0xf0)>>4);
 U_Disk_Write_Number(TEST_DATA.date & 0x0f);
 CH376ByteWrite("�� ",2,NULL);
 U_Disk_Write_Enter(1);
 CH376ByteWrite("         ",9,NULL);
 U_Disk_Write_Number((TEST_DATA.hour & 0xf0)>>4);
 U_Disk_Write_Number(TEST_DATA.hour & 0x0f);
 CH376ByteWrite("ʱ",2,NULL);
 U_Disk_Write_Number((TEST_DATA.minute & 0xf0)>>4);
 U_Disk_Write_Number(TEST_DATA.minute & 0x0f);
 CH376ByteWrite("��",2,NULL);
 U_Disk_Write_Number((TEST_DATA.second & 0xf0)>>4);
 U_Disk_Write_Number(TEST_DATA.second & 0x0f);
 CH376ByteWrite("��",2,NULL);
 U_Disk_Write_Enter(1);//����
 CH376ByteWrite("������:",9,NULL);
 U_Disk_Write_Number(TEST_DATA.TEST_NUMBER[0]);
 U_Disk_Write_Number(TEST_DATA.TEST_NUMBER[1]);
 U_Disk_Write_Number(TEST_DATA.TEST_NUMBER[2]);
 U_Disk_Write_Number(TEST_DATA.TEST_NUMBER[3]);
 U_Disk_Write_Number(TEST_DATA.TEST_NUMBER[4]);
 U_Disk_Write_Number(TEST_DATA.TEST_NUMBER[5]);
 U_Disk_Write_Enter(1);//����
 //CH376ByteWrite("�豸 I D:",9,NULL);
 CH376ByteWrite("�豸����:",9,NULL);//�µ�����ͨѶ��Լ�н����豸ID����Ϊ���豸���ơ�
 U_Disk_Write_Number(TEST_DATA.DEVICE_ID[0]);
 U_Disk_Write_Number(TEST_DATA.DEVICE_ID[1]);
 U_Disk_Write_Number(TEST_DATA.DEVICE_ID[2]);
 U_Disk_Write_Number(TEST_DATA.DEVICE_ID[3]);
 U_Disk_Write_Number(TEST_DATA.DEVICE_ID[4]);
 U_Disk_Write_Number(TEST_DATA.DEVICE_ID[5]);
 U_Disk_Write_Enter(1);//����
 CH376ByteWrite("���ѹ:",9,NULL);
 U_Disk_Write_Number(TEST_DATA.RATED_VOLTAGE/100);
 U_Disk_Write_Number(TEST_DATA.RATED_VOLTAGE%100/10);
 CH376ByteWrite(".",1,NULL);
 U_Disk_Write_Number(TEST_DATA.RATED_VOLTAGE%100%10);
 CH376ByteWrite("KV",2,NULL);
 U_Disk_Write_Enter(1);//���� 
 CH376ByteWrite("ĸ�ߵ�ѹ:",9,NULL);
 U_Disk_Write_Number(TEST_DATA.BUS_VOLTAGE/100);
 U_Disk_Write_Number(TEST_DATA.BUS_VOLTAGE%100/10);
 CH376ByteWrite(".",1,NULL);
 U_Disk_Write_Number(TEST_DATA.BUS_VOLTAGE%100%10);
 CH376ByteWrite("KV",2,NULL);
 U_Disk_Write_Enter(1);//���� 
 /*------------------------�������ӷ�ʽ------------------------------*/
 CH376ByteWrite("���ӷ�ʽ:",9,NULL);
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
 U_Disk_Write_Enter(1);//����
 /*------------------------����PT���------------------------------*/
 CH376ByteWrite("P T ���:",9,NULL);
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
 U_Disk_Write_Enter(1);//����
 /*------------------------��������3U0------------------------------*/
 CH376ByteWrite("���� 3U0:",9,NULL);
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
 U_Disk_Write_Enter(1);//����
 /*---------------------���������������----------------------------*/
 if(TEST_DATA.PT_CONNECT_MODE == PT_C1PT)//C1PT���߷�ʽ�����������������
  {   
   CH376ByteWrite("A���������:",12,NULL);
   U_Disk_Write_Number(PHASE_A_CAP_KEY1);
   U_Disk_Write_Number(PHASE_A_CAP_KEY2);
   U_Disk_Write_Number(PHASE_A_CAP_KEY3);
   U_Disk_Write_Number(PHASE_A_CAP_KEY4);
   CH376ByteWrite(".",1,NULL);
   U_Disk_Write_Number(PHASE_A_CAP_KEY5);
   U_Disk_Write_Number(PHASE_A_CAP_KEY6);
   CH376ByteWrite("uF",2,NULL);
   U_Disk_Write_Enter(1);//����
   CH376ByteWrite("B���������:",12,NULL);
   U_Disk_Write_Number(PHASE_B_CAP_KEY1);
   U_Disk_Write_Number(PHASE_B_CAP_KEY2);
   U_Disk_Write_Number(PHASE_B_CAP_KEY3);
   U_Disk_Write_Number(PHASE_B_CAP_KEY4);
   CH376ByteWrite(".",1,NULL);
   U_Disk_Write_Number(PHASE_B_CAP_KEY5);
   U_Disk_Write_Number(PHASE_B_CAP_KEY6);
   CH376ByteWrite("uF",2,NULL);
   U_Disk_Write_Enter(1);//����
   CH376ByteWrite("C���������:",12,NULL);
   U_Disk_Write_Number(PHASE_C_CAP_KEY1);
   U_Disk_Write_Number(PHASE_C_CAP_KEY2);
   U_Disk_Write_Number(PHASE_C_CAP_KEY3);
   U_Disk_Write_Number(PHASE_C_CAP_KEY4);
   CH376ByteWrite(".",1,NULL);
   U_Disk_Write_Number(PHASE_C_CAP_KEY5);
   U_Disk_Write_Number(PHASE_C_CAP_KEY6);
   CH376ByteWrite("uF",2,NULL);
   U_Disk_Write_Enter(1);//����
  }
 /*------------------------���������C------------------------------*/
 CH376ByteWrite("- - - - - - - - - - - -",23,NULL);
 U_Disk_Write_Enter(1);//����
 CH376ByteWrite("  ��������C: ",13,NULL);
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
 U_Disk_Write_Enter(1);//����
 /*------------------------������ݵ���------------------------------*/
 CH376ByteWrite("  ���ݵ���I: ",13,NULL);
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
//��U��д���������ļ�
/*************************************************************************************************/
void U_Disk_Write_Test_Data_File(void)
{
 unsigned char  buf[25];
 unsigned short FileDate,FileTime;
 //��������������е������ղ��뵽�ַ�����/DR��֮��(Ŀ¼�����ܳ���8���ַ�)
 sprintf((char*)buf,"/DR%d%d%d%d%d%d",(TEST_DATA.year&0xf0)>>4,TEST_DATA.year&0x0f,(TEST_DATA.month&0xf0)>>4,TEST_DATA.month&0x0f,(TEST_DATA.date&0xf0)>>4,TEST_DATA.date&0x0f);
 CH376DirCreate(buf);//�ڸ�Ŀ¼���½��ļ��в��򿪣����������ֱ�Ӵ�
 //��ȡ��ǰ�ļ���Ŀ¼��ϢFAT_DIR_INFO,��������ݵ����ڴ���
 if(CH376DirInfoRead() == USB_INT_SUCCESS) //�����ȡ�ɹ��ٸ�������
  {
   FileDate = MAKE_FILE_DATE(2000+Bcd_To_Hex(TEST_DATA.year),Bcd_To_Hex(TEST_DATA.month),Bcd_To_Hex(TEST_DATA.date));
   FileTime = MAKE_FILE_TIME(Bcd_To_Hex(TEST_DATA.hour),Bcd_To_Hex(TEST_DATA.minute),Bcd_To_Hex(TEST_DATA.second));
   buf[0] = FileDate & 0x00ff;//�ļ�������ʱ��,��8λ��ǰ,��8λ�ں�
   buf[1] = (FileDate >> 8) & 0x00ff;
   CH376WriteOfsBlock(buf,0x10,2);//�޸��ļ���������
   CH376WriteOfsBlock(buf,0x18,2);//�޸��ļ��޸�����
   buf[0] = FileTime & 0x00ff;
   buf[1] = (FileTime >> 8) & 0x00ff;
   CH376WriteOfsBlock(buf,0x0e,2);//�޸��ļ�����ʱ��
   CH376WriteOfsBlock(buf,0x16,2);//�޸��ļ��޸�ʱ��
   CH376DirInfoSave(); //�����ļ���Ŀ¼��Ϣ 
  }
 //��������������е�ʱ������뵽�ַ�����.DOC��֮ǰ
 sprintf((char*)buf,"%d%d%d%d%d%d.DOC",(TEST_DATA.hour&0xf0)>>4,TEST_DATA.hour&0x0f,(TEST_DATA.minute&0xf0)>>4,TEST_DATA.minute&0x0f,(TEST_DATA.second&0xf0)>>4,TEST_DATA.second&0x0f);
 CH376FileCreate(buf);//�½��ļ�����,����ļ��Ѿ���������ɾ�����½�  
 File_Of_Test_Data(); //���½����򿪵��ļ���д������������
 //��ȡ��ǰ�ļ���Ŀ¼��ϢFAT_DIR_INFO,��������ݵ����ڴ���
 if(CH376DirInfoRead() == USB_INT_SUCCESS)//�����ȡ�ɹ��ڸ������� 
  {
   FileDate = MAKE_FILE_DATE(2000+Bcd_To_Hex(TEST_DATA.year),Bcd_To_Hex(TEST_DATA.month),Bcd_To_Hex(TEST_DATA.date));
   FileTime = MAKE_FILE_TIME(Bcd_To_Hex(TEST_DATA.hour),Bcd_To_Hex(TEST_DATA.minute),Bcd_To_Hex(TEST_DATA.second));
   buf[0] = FileDate & 0x00ff;
   buf[1] = (FileDate >> 8) & 0x00ff;
   CH376WriteOfsBlock(buf,0x10,2);//�޸��ļ���������
   CH376WriteOfsBlock(buf,0x18,2);//�޸��ļ��޸�����
   buf[0] = FileTime & 0x00ff;
   buf[1] = (FileTime >> 8) & 0x00ff;
   CH376WriteOfsBlock(buf,0x0e,2);//�޸��ļ�����ʱ��
   CH376WriteOfsBlock(buf,0x16,2);//�޸��ļ��޸�ʱ��
   CH376DirInfoSave(); //�����ļ���Ŀ¼��Ϣ 
  }
 CH376FileClose(TRUE);  //�ر��ļ�,�Զ������ļ�����,���ֽ�Ϊ��λд�ļ�,�����ó����ر��ļ��Ա��Զ������ļ�����
}