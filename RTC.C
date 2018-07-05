/*************************************************************************************************************
����:   BQ32000RTC��������(V1.1)
�ļ���: RTC.c
������: STM32F103ϵ��CPU
������: RealView MDK
����:   ���
��Ȩ:   ���������˵����豸���޹�˾
���ڣ�  2015.12.8
*************************************************************************************************************/
#include "stm32f10x.h"
#include "IOCONFIG.H"
#include "QJBL.H"


#define IO_TYPE 0   //IO�����ͣ�0������©�����1���������Ƴ�

#define BUSIN   0
#define BUSOUT  1

#define DELAYTIME 12 //��ʱԼ1.3us,ʱ����Ƶ����380KHZ����

#define REG_READ 	0xd1
#define REG_WRITE 	0xd0
#define SFR 	    0x22 //Special function register
#define SF_KEY2 	0x21 //Special function key 2
#define SF_KEY1 	0x20 //Special function key 1
#define CFG2 		0x09 //Configuration 2
#define TCH2 		0x08 //Trickle charge enable
#define CAL_CFG1 	0x07 //Calibration and configuration
#define YEARS 		0x06 //Clock years
#define MONTH  		0x05 //Clock month
#define DATE 		0x04 //Clock date
#define DAY 	    0x03 //Clock day
#define CENT_HOURS 	0x02 //Clock hours, century, and CENT_EN bit
#define MINUTES 	0x01 //Clock minutes
#define SECONDS 	0x00 //Clock seconds and STOP bit

unsigned char RTC_IFACK; //record the SDA state to confirm if ACK has happened 
unsigned char ERR = 0; 
unsigned char RTC_DATA_ERR = 0;   //RTC���ݴ���
unsigned char RTC_OSC_FAIL = OFF; //RTC����ʧЧ��־

/************************************************************************************************************/
//��������void IIC_RTCDATA_DIR(unsigned char dir)
//Ŀ�ģ�	SDATA�������
//���룺	dir	����ֵ
//		    1:���
//		    0:����
//DATA��ʹ��PE1
/************************************************************************************************************/
void IIC_RTCDATA_DIR(unsigned char dir)
{
 unsigned int tmpreg;
 if(dir == BUSIN) 
  {
   tmpreg  = GPIOE->CRL;
   tmpreg &= 0xffffff0f;//���PE1��IO������
   tmpreg |= 0x00000040;//����Ϊ��©����
   GPIOE->CRL = tmpreg;
  }
 else
  {
   tmpreg  = GPIOE->CRL;
   tmpreg &= 0xffffff0f;//���PE1��IO������
   tmpreg |= 0x00000070;//����Ϊ��©������������ٶ�50MHZ
   GPIOE->CRL = tmpreg;
  }
}

/************************************************************************************************************/
/*IIC�������һ����ʼ�����ź�*/
/************************************************************************************************************/
void IIC_RTC_Start(void) 
{
 RTC_SDA_1;
 IIC_RTCDATA_DIR(BUSOUT);//����������Ϊ���
 Delay(DELAYTIME);
 RTC_SCL_H;	  
 Delay(DELAYTIME);
 RTC_SDA_0;
 Delay(DELAYTIME);
 RTC_SCL_L;
}

/************************************************************************************************************/
/*IIC�������һ��ֹͣ�����ź�*/
/************************************************************************************************************/
void IIC_RTC_Stop(void)
{
 RTC_SDA_0;
 IIC_RTCDATA_DIR(BUSOUT);//����������Ϊ���
 Delay(DELAYTIME);
 RTC_SCL_H;
 Delay(DELAYTIME);
 RTC_SDA_1;
 Delay(DELAYTIME);
}

/************************************************************************************************************/
/*IIC�������һ��ACK�ź�*/
/************************************************************************************************************/
void IIC_RTC_Ack(void)
{
 RTC_SDA_0;
 Delay(DELAYTIME);
 RTC_SCL_H;
 Delay(DELAYTIME);
 RTC_SCL_L;
 Delay(DELAYTIME);
}

/************************************************************************************************************/
/*IIC���߽���һ��ACK�źŻ������һ��NO_ACK�ź�*/
/************************************************************************************************************/
void IIC_RTC_Nack(unsigned char ack)
{
 IIC_RTCDATA_DIR(BUSIN);//����������Ϊ����
 Delay(DELAYTIME);
 RTC_SCL_H;
 Delay(DELAYTIME);
 if(RTC_SDA_STAT) RTC_IFACK = 1;
 else RTC_IFACK = 0; 
 RTC_SCL_L;
 if(ack == 1 && RTC_IFACK == 1) ERR += 1;
}

/************************************************************************************************************/
/*IIC��������һ���ֽڣ���λ*/
/************************************************************************************************************/
unsigned char IIC_RTC_InByte(void) 
{
 unsigned char i,val=0;
 IIC_RTCDATA_DIR(BUSIN);//����������Ϊ����
 for(i=0x80;i>0;i/=2) //shift bit for masking
  { 
   Delay(DELAYTIME);
   RTC_SCL_H;
   Delay(DELAYTIME);
   if(RTC_SDA_STAT) val=(val | i); //read bit
   RTC_SCL_L;
  }    
 return(val);
}

/************************************************************************************************************/
/*IIC�������һ���ֽڣ���λ*/
/************************************************************************************************************/
void IIC_RTC_OutByte(unsigned char data) 
{
 unsigned char i;
 IIC_RTCDATA_DIR(BUSOUT);//����������Ϊ���
 for(i=0x80;i>0;i/=2) //shift bit for masking
  { 
   if(i & data) RTC_SDA_1; //masking value with i , write to SENSI-BUS
   else RTC_SDA_0;
   Delay(DELAYTIME);
   RTC_SCL_H; //clk for SENSI-BUS
   Delay(DELAYTIME);
   RTC_SCL_L;
  }
 #if IO_TYPE == 0	//����ǿ�©���
  RTC_SDA_1;//�ͷ�������
 #elif IO_TYPE == 1	//������������������͵�ƽ����ֹ��������͵�ƽ��Ӧ��оƬ
  RTC_SDA_0;
 #endif
}

/************************************************************************************************************/
/*д�Ĵ���*/
/************************************************************************************************************/
void RTC_WriteReg(unsigned char add,unsigned char wbyte)
{
 do
  {
   IIC_RTC_Start();
   IIC_RTC_OutByte(REG_WRITE);
   IIC_RTC_Nack(1);
   IIC_RTC_OutByte(add);
   IIC_RTC_Nack(1);
   IIC_RTC_OutByte(wbyte);
   IIC_RTC_Nack(1);
   IIC_RTC_Stop();
   if(ERR != 0)
    {
     ERR = 0;
     RTC_DATA_ERR += 1;
     if(RTC_DATA_ERR >= 5)
      {
	   RTC_DATA_ERR = 0;
	  }
    }
   else
    {
     RTC_DATA_ERR = 0;
    }
  }
 while(RTC_DATA_ERR);
}

/************************************************************************************************************/
/*���Ĵ���*/
/************************************************************************************************************/
unsigned char RTC_ReadReg (unsigned char add)
{
 unsigned char temp;
 do
  {
   IIC_RTC_Start();
   IIC_RTC_OutByte(REG_WRITE);
   IIC_RTC_Nack(1);
   IIC_RTC_OutByte(add);
   IIC_RTC_Nack(1);
   IIC_RTC_Start();
   IIC_RTC_OutByte(REG_READ);
   IIC_RTC_Nack(1);
   temp=IIC_RTC_InByte();
   IIC_RTC_Nack(0);
   IIC_RTC_Stop();
   if(ERR != 0)
    {
     ERR = 0;
     RTC_DATA_ERR += 1;
     if(RTC_DATA_ERR >= 5)
      {
	   RTC_DATA_ERR = 0;
	  }
    }
   else
    {
     RTC_DATA_ERR = 0;
    }
  }
 while(RTC_DATA_ERR);
 return(temp);
}

/************************************************************************************************************/
/*RTC��ʼ��*/
/************************************************************************************************************/
void RTC_Init(void)
{
 unsigned char tmp;
 Delay_ms(1);  //A master device must wait at least 60 us after the RTC 
               //exits backup mode to generate a START condition.
 RTC_WriteReg(CFG2,0x0A);    //�����
 RTC_WriteReg(TCH2,0x00);
 RTC_WriteReg(CAL_CFG1,0x80);//IRQ�߼����1��������Ƶ�ʲ������
 tmp = RTC_ReadReg(SECONDS); //����ǰ������ֵ����
 if(tmp & 0x80)//���STOPλΪ1(����ֹͣ)������������
  {
   tmp &= 0x7F;                //��STOPλΪ0
   RTC_WriteReg(SECONDS,0x80); //ǿ�����������ϵ������STOPλΪ1��Ȼ����STOPλΪ0
   RTC_WriteReg(SECONDS,tmp);  //��STOPλΪ0�������ϵ�ʱ��������ֵд��
  }
 tmp = RTC_ReadReg(MINUTES);   //����ǰ������ֵ����
 if(tmp & 0x80)//����ʧЧ��־OF����λ
  {
   tmp &= 0x7F;                //��OF(����ʧЧ)λΪ0�����OF��־
   RTC_WriteReg(MINUTES,tmp);  //���ϵ�ʱ�ķ�����ֵд��
  }
}

/************************************************************************************************************/
/*RTC��ȡʱ��ֵ*/
/************************************************************************************************************/
void RTC_ReadTime(void)
{
 unsigned char tmp;
 NOW_TIME.year    = RTC_ReadReg(YEARS);
 NOW_TIME.month   = RTC_ReadReg(MONTH);
 NOW_TIME.date    = RTC_ReadReg(DATE);
 NOW_TIME.weekday = RTC_ReadReg(DAY);
 NOW_TIME.hour    = RTC_ReadReg(CENT_HOURS);
 tmp              = RTC_ReadReg(MINUTES);
 NOW_TIME.minute  = tmp & 0x7f;//���ε�OF��־
 NOW_TIME.second  = RTC_ReadReg(SECONDS);
 //���OF��־
 if(tmp >> 7) RTC_OSC_FAIL = ON;//RTC����ʧЧ
 else RTC_OSC_FAIL = OFF;
}

/************************************************************************************************************/
/*RTC����ʱ��ֵ*/
/************************************************************************************************************/
void RTC_WriteTime(unsigned char year,unsigned char month,unsigned char date,unsigned char weekday,
                   unsigned char hour,unsigned char minute,unsigned char second)
{
 RTC_WriteReg(YEARS,year);
 RTC_WriteReg(MONTH,month);
 RTC_WriteReg(DATE,date);
 RTC_WriteReg(DAY,weekday);
 RTC_WriteReg(CENT_HOURS,hour);
 RTC_WriteReg(MINUTES,minute);
 RTC_WriteReg(SECONDS,second);
}

/************************************************************************************************************/
/*�������*/
/************************************************************************************************************/
unsigned char Leap_Year(unsigned char year)
{
 unsigned short yeartmp;
 yeartmp  = 2000+Bcd_To_Hex(year);
 return ((yeartmp%4==0 && yeartmp%100!=0) || (yeartmp%400==0)); 
}

/************************************************************************************************************/
/*���ڼ���*/
/************************************************************************************************************/
unsigned char Week_Day(unsigned char year,unsigned char month,unsigned char date)
{
 unsigned short yeartmp;
 unsigned char monthtmp,datetmp,daytmp;
 yeartmp  = 2000+Bcd_To_Hex(year);
 monthtmp = Bcd_To_Hex(month);
 if(monthtmp==1||monthtmp==2){monthtmp+=12;yeartmp--;}
 datetmp  = Bcd_To_Hex(date);
 //��ķ����ɭ���㹫ʽ,�������ڼ�
 daytmp = ((datetmp+2*monthtmp+3*(monthtmp+1)/5
          +yeartmp+yeartmp/4-yeartmp/100+yeartmp/400) % 7) + 1;
 return (daytmp); 
}