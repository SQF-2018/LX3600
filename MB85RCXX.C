/***********************************************************************************
����:   MB85RCxxϵ��IIC����洢����������(V1.0)
�ļ���: MB85RCxx.c
������: STM32F1.3ϵ��CPU
������: Keil C Version 4.03
����:   ��Ԫ
��Ȩ:   ���������˵����豸���޹�˾
���ڣ�  2014.10.29
***********************************************************************************/
#include "stm32f10x.h"
#include "IOCONFIG.H"
#include "QJBL.H"
#include "FUNCTION.H"

#define IO_TYPE 0   //IO�����ͣ�0������©�����1���������Ƴ�

#define BUSIN   0
#define BUSOUT  1

#define FM_DELAYTIME 12 //��ʱ1.29us,�ߵ�ƽ1.29us,ʱ����Ƶ����387KHZ����

#define DATA_READ     0xa1
#define DATA_WRITE    0xa0

unsigned char IFACK;//record the SDA state to confirm if ACK has happened
unsigned char IIC_ERR = 0; 
unsigned char DATA_ERR = 0;

/*******************************************************************************************************************************
�ı�SDA�������ݷ�����
����:dir ����ֵ��0:���룻1:���
DATA��ʹ��PC1
*******************************************************************************************************************************/
void IIC_DATA_DIR(unsigned char dir)
{
 unsigned int tmpreg;
 if(dir == BUSIN) 
  {
   tmpreg = GPIOC->CRL;
   tmpreg &= 0xffffff0f;//���PC1��IO������
   tmpreg |= 0x00000040;//����Ϊ��©����
   GPIOC->CRL = tmpreg;
   GPIOC->ODR |= 0x00000080;//�ڲ�������Ч��
  }
 else
  {
   tmpreg = GPIOC->CRL;
   tmpreg &= 0xffffff0f;//���PC1��IO������
   tmpreg |= 0x00000070;//����Ϊ��©������������ٶ�50MHZ
   GPIOC->CRL = tmpreg;
  }
}

/*************************************************************************************************/
/*IIC�������һ����ʼ�����ź�*/
/*************************************************************************************************/
void IIC_Start(void) 
{
 FM_SDA_1;
 IIC_DATA_DIR(BUSOUT);//����������Ϊ���
 Delay(FM_DELAYTIME);
 FM_SCL_H;
 Delay(FM_DELAYTIME);
 FM_SDA_0;
 Delay(FM_DELAYTIME);
 FM_SCL_L;
}

/*************************************************************************************************/
/*IIC�������һ��ֹͣ�����ź�*/
/*************************************************************************************************/
void IIC_Stop(void)
{
 FM_SDA_0;
 IIC_DATA_DIR(BUSOUT);//����������Ϊ���
 Delay(FM_DELAYTIME);
 FM_SCL_H;
 Delay(FM_DELAYTIME);
 FM_SDA_1;
 Delay(FM_DELAYTIME);
}

/*************************************************************************************************/
/*IIC�������һ��ACK�ź�*/
/*************************************************************************************************/
void IIC_Ack(void)
{
 FM_SDA_0;
 Delay(FM_DELAYTIME);
 FM_SCL_H;
 Delay(FM_DELAYTIME);
 FM_SCL_L;
 Delay(FM_DELAYTIME);
}

/*************************************************************************************************/
/*IIC���߽���һ��ACK�źŻ������һ��NO_ACK�ź�*/
/*************************************************************************************************/
void IIC_Nack(unsigned char ack)
{
 IIC_DATA_DIR(BUSIN);//����������Ϊ����
 Delay(FM_DELAYTIME);
 FM_SCL_H;
 Delay(FM_DELAYTIME);
 if(FM_SDA_STAT) IFACK = 1;
 else IFACK = 0; 
 FM_SCL_L;
 if(ack == 1 && IFACK == 1) IIC_ERR += 1;
}

/*************************************************************************************************/
/*IIC��������һ���ֽڣ���λ*/
/*************************************************************************************************/
unsigned char IIC_InByte(void) 
{
 unsigned char i,val=0;
 IIC_DATA_DIR(BUSIN);//����������Ϊ����
 for(i=0x80;i>0;i/=2) //shift bit for masking
  { 
   Delay(FM_DELAYTIME);
   FM_SCL_H;
   Delay(FM_DELAYTIME);
   if(FM_SDA_STAT) val=(val | i); //read bit
   FM_SCL_L;
  }    
 return(val);
}

/*************************************************************************************************/
/*IIC�������һ���ֽڣ���λ*/
/*************************************************************************************************/
void IIC_OutByte(unsigned char data) 
{
 unsigned char i;
 IIC_DATA_DIR(BUSOUT);//����������Ϊ���
 for(i=0x80;i>0;i/=2) //shift bit for masking
  { 
   if(i & data) FM_SDA_1; //masking value with i , write to SENSI-BUS
   else FM_SDA_0;
   Delay(FM_DELAYTIME);
   FM_SCL_H; //clk for SENSI-BUS
   Delay(FM_DELAYTIME);
   FM_SCL_L;
  }
 #if IO_TYPE == 0    //����ǿ�©���
  FM_SDA_1;//�ͷ�������
 #elif IO_TYPE == 1    //������������������͵�ƽ����ֹ��������͵�ƽ��Ӧ��оƬ
  FM_SDA_0;
 #endif
}

/*************************************************************************************************/
/*дһ�ֽ�����*/
/*************************************************************************************************/
void Mb85rcxx_WriteByte(unsigned short add,unsigned char wbyte)
{
 do
  {
   IIC_Start();
   IIC_OutByte(DATA_WRITE);
   IIC_Nack(1);
   IIC_OutByte(add>>8);
   IIC_Nack(1);
   IIC_OutByte(add);
   IIC_Nack(1);
   IIC_OutByte(wbyte);
   IIC_Nack(1);
   IIC_Stop();
   if(IIC_ERR != 0)
    {
     IIC_ERR = 0;
     DATA_ERR += 1;
     if(DATA_ERR >= 5)//�����������ѭ������5��
      {
       DATA_ERR = 0;
      }
    }
   else
    {
     DATA_ERR = 0;
    }
  }
 while(DATA_ERR);
}

/*************************************************************************************************/
/*��һ�ֽ�����*/
/*************************************************************************************************/
unsigned char Mb85rcxx_ReadByte (unsigned short add)
{
 unsigned char temp;
 do
  {
   IIC_Start();
   IIC_OutByte(DATA_WRITE);
   IIC_Nack(1);
   IIC_OutByte(add>>8);
   IIC_Nack(1);
   IIC_OutByte(add);
   IIC_Nack(1);
   IIC_Start();
   IIC_OutByte(DATA_READ);
   IIC_Nack(1);
   temp=IIC_InByte();
   IIC_Nack(0);
   IIC_Stop();
   if(IIC_ERR != 0)
    {
     IIC_ERR = 0;
     DATA_ERR += 1;
     if(DATA_ERR >= 5)
      {
       DATA_ERR = 0;
      }
    }
   else
    {
     DATA_ERR = 0;
    }
  }
 while(DATA_ERR);
 return(temp);
}

/*************************************************************************************************/
/*��FLOAT�ͱ������뵽������ȥ
  data:Ҫ����ĸ����� add:Ҫ����ĵ�ַ��һ���������ĸ��ֽ�(һ���ֽ�8λ)*/
/*************************************************************************************************/
void Mb85rcxx_Write_Float(unsigned short add,float data) 
{ 
 unsigned char i;
 union float_to_char //ʹ�����϶��巽ʽȡ�ø�������4�ֽ�����,data��px[4]����һ��4�ֽ��ڴ�ռ�
  {
   float         data_f;
   unsigned char px[4];
  }tmp;
 tmp.data_f = data;
 for(i=0;i<4;i++)
  { 
   Mb85rcxx_WriteByte(add+i,tmp.px[i]); 
  } 
}

/*************************************************************************************************/
/*���洢��FLOAT�ͱ����������ж���
  add:Ҫ��ȡ�ĵ�ַ*/
/*************************************************************************************************/
float Mb85rcxx_Read_Float(unsigned short add)       
{ 
 unsigned char i;
 union float_to_char //ʹ�����϶��巽ʽȡ�ø�������4�ֽ�����,data��px[4]����һ��4�ֽ��ڴ�ռ�
  {
   float         data_f;
   unsigned char px[4];
  }tmp;
 for(i=0;i<4;i++)
  { 
   tmp.px[i] = Mb85rcxx_ReadByte(add+i); 
  } 
 return(tmp.data_f); 
}

/*************************************************************************************************/
//д�������¼
/*************************************************************************************************/
void Mb85rcxx_Write_Test_Record(void)
{

}

/*************************************************************************************************/
//��ȡ������¼
//����:    now_record_number Ҫ��ȡ�Ĳ�����¼��ţ������±���Ĳ�����¼���Ϊ����
/*************************************************************************************************/
void Mb85rcxx_Read_Test_Record(unsigned char now_record_number)
{
 
}

/*************************************************************************************************/
/*��ȡ�������õĲ���ֵ*/
/*************************************************************************************************/
void Mb85rcxx_Read_Test_Set(void)
{

}

/*************************************************************************************************/
/*д���������õĲ���ֵ*/
/*************************************************************************************************/
void Mb85rcxx_Write_Test_Set(void)
{

}

/*************************************************************************************************/
/*д���������õĲ���Ĭ��ֵ*/
/*************************************************************************************************/
void Mb85rcxx_Write_Test_Set_Default(void)
{
 
}

/*************************************************************************************************/
/*��ȡģ����У��ϵ��*/
/*************************************************************************************************/
void Mb85rcxx_Read_Correction_Coefficient(void)
{
 
}

/*************************************************************************************************/
/*д��ģ����У��ϵ��Ĭ��ֵ*/
/*************************************************************************************************/
void Mb85rcxx_Write_Correction_Coefficient_Default(unsigned short control_bit)
{
 
}

/*************************************************************************************************/
/*������ȡ���õĲ���ֵ*/
/*************************************************************************************************/
void Mb85rcxx_ReadSheZhi(void)
{
 unsigned short tmp1,tmp2;
 unsigned char i;
 /*----------------------------��ȡ�洢���ձ�־--------------------------------------------------*/
 tmp1 = Mb85rcxx_ReadByte(ee_data_sig1);//��ȡ�洢���ձ�־1
 tmp2 = Mb85rcxx_ReadByte(ee_data_sig2);//��ȡ�洢���ձ�־2
 /*--------------------------����洢��Ϊ�գ���д��Ĭ��ֵ----------------------------------------*/
 //���ò����洢���գ���Ҫ����д��Ĭ������
 if(tmp1 != 0x1a || tmp2 != 0xa1)//�洢��Ϊ��Ƭ
  {
   Mb85rcxx_WriteByte(ee_data_sig1,0x1a);//д��洢���ձ�־1
   Mb85rcxx_WriteByte(ee_data_sig2,0xa1);//д��洢���ձ�־2
   tmp1 = Mb85rcxx_ReadByte(ee_data_sig1);//���¶�ȡ�洢���ձ�־1
   tmp2 = Mb85rcxx_ReadByte(ee_data_sig2);//���¶�ȡ�洢���ձ�־2
   if(tmp1 != 0x1a || tmp2 != 0xa1)//������������Ȼ����
    {
     Set_Bit(HARDWARE_EEOR_FLAG,FM_MEM_ERR);//����洢��Ӳ������
     return;
    }
   for(i=0;i<4;i++)
    {
     Mb85rcxx_WriteByte(ee_device_number0 + i,0);//Ĭ��װ�ñ��0000
    }
   Mb85rcxx_Write_Test_Set_Default();                     //д�������������Ĭ��ֵ
   Mb85rcxx_Write_Correction_Coefficient_Default(ALL_BIT);//д������У׼ֵĬ������
   Mb85rcxx_WriteByte(ee_test_record_number,0);           //Ĭ�����²�����¼���Ϊ0
   Mb85rcxx_WriteByte(ee_test_record_amount,0);           //Ĭ���ѱ���Ĳ�����¼����Ϊ0
  }
 /*-------------------------------��ȡ��������---------------------------------------------------*/
 //��ȡװ�ñ��
 for(i=0;i<4;i++)
  {
   DEVICE_NUMBER[i] = Mb85rcxx_ReadByte(ee_device_number0 + i);
  }
 //��ȡ���һ�β��Բ�������
 Mb85rcxx_Read_Test_Set();
 /*-----------------------------��ȡģ����У׼ϵ��------------------------------------------------*/
 Mb85rcxx_Read_Correction_Coefficient();//��ȡģ����У׼ϵ��
 /*------------------------��ȡ����Ĳ�����¼��ż�����-------------------------------------------*/
 TEST_RECORD_NUMBER = Mb85rcxx_ReadByte(ee_test_record_number);//��ȡ���²�����¼���
 TEST_RECORD_AMOUNT = Mb85rcxx_ReadByte(ee_test_record_amount);//��ȡ�ѱ���Ĳ�����¼����
}