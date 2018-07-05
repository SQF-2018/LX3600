/*************************************************************************************************/
//CH376оƬ Ӳ����׼SPI�������ӵ�Ӳ������� V1.0 
//�ṩI/O�ӿ��ӳ���
//�޸ģ���Ԫ
//�޸����ڣ�2014.10.29
/*************************************************************************************************/

#include "stm32f10x.h"
#include "IOCONFIG.H"
#include "QJBL.H"
#include "FILE_SYS.H"

/*************************************************************************************************/
//�û����壬���ż����ܿ��Ƶĺ�
/*************************************************************************************************/
#define CH376_INT_WIRE //����CH376_INT_WIRE�꣬ȷ��ʹ��CH376��INT����

/*******************************************************************************************************************************
����SPI3��������CH376ͨѶ
SPI3��APB1���ߣ����ʱ��Ƶ��36MHz
*******************************************************************************************************************************/
void SPI3_Configuration(void)
{
 SPI_InitTypeDef SPI_InitStructure;//����SPI�����ݽṹ��
 SPI_Cmd(SPI3, DISABLE);           //��ֹSPI3
 SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;//����Ϊ˫��˫��ȫ˫��ģʽ
 SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;                //����Ϊ���豸
 SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;                //8λ���ݴ���
 SPI_InitStructure.SPI_CPOL              = SPI_CPOL_High;                  //����ʱSCK���ָߵ�ƽ
 SPI_InitStructure.SPI_CPHA              = SPI_CPHA_2Edge;                 //���ݴӵڶ���ʱ���ؿ�ʼ
 SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;                   //�������豸����
 SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;        //������=36M/4=9M
 SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;               //����λ��λ��ǰ
 SPI_InitStructure.SPI_CRCPolynomial     = 7;                              //��CRC����ֵ
 SPI_Init(SPI3, &SPI_InitStructure);
 SPI_I2S_ITConfig(SPI3, SPI_IT_CRCERR, DISABLE);//�ر�CRC�����ж�
 SPI_Cmd(SPI3, ENABLE);                         //ʹ��SPI3
}

/*************************************************************************************************/
//Ӳ��SPI���������8��λ����
/*************************************************************************************************/
unsigned char Spi376Exchange(unsigned char d)  
{  /* Ϊ������ٶ�,���Խ����ӳ������ɺ��Լ����ӳ�����ò�� */
 unsigned char i;
 SPI3->DR = d;
 while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_RXNE) == 0);//�ȴ����ݽ������
 i = SPI3->DR;//��ȡ�յ�������
 return i&0xff;
}

/*************************************************************************************************/
//��CH376д����
/*************************************************************************************************/
void xWriteCH376Cmd(unsigned char mCmd)  
{
 CH376_CS_H;
 CH376_CS_L;            /* SPIƬѡ��Ч */
 Spi376Exchange(mCmd);  /* ���������� */
 while(CH376_BUSY_STAT);
}

/*************************************************************************************************/
//��CH376д����
/*************************************************************************************************/
void xWriteCH376Data(unsigned char mData)  
{
 Spi376Exchange(mData);
 while(CH376_BUSY_STAT);
 //Delay(50);//ȷ��д���ڴ���0.6us
}

/*************************************************************************************************/
//��CH376������
/*************************************************************************************************/
unsigned char xReadCH376Data(void) 
{
 //Delay(50);//ȷ�������ڴ���0.6us
 while(CH376_BUSY_STAT);
 return(Spi376Exchange(0xFF));
}

/*************************************************************************************************/
//��ѯCH376�ж�(INT#�͵�ƽ)
/*************************************************************************************************/
unsigned char Query376Interrupt(void)
{
#ifdef CH376_INT_WIRE
 return(CH376_INT_STAT ? FALSE : TRUE );   //���������CH376���ж�������ֱ�Ӳ�ѯ�ж�����
#else
 return(CH376_SPIMISO_STAT ? FALSE : TRUE);//���δ����CH376���ж��������ѯ�����ж������SDO����״̬
#endif
}

/*************************************************************************************************/
//�ϵ��ʼ��CH376
//��ʼ����������ʽ
/*************************************************************************************************/
unsigned char CH376_Init_Host(void)  
{
 unsigned char res,init_err=0;
 SPI3_Configuration();             //��ʼ����CH376ͨѶ��SPI3��
 Delay_ms(100);                    //�ϵ���ʱ100ms���ȴ�CH376��λ���
 xWriteCH376Cmd(CMD00_RESET_ALL);  //�ϵ��ִ��һ��Ӳ����λָ��������λ����ĳЩ����£�����Ƭ����λ�����ϵ�ͨѶ���Ի����
 xEndCH376Cmd();                   //��CH376Ƭѡ�ߵ�ƽ
 Delay_ms(100);                    //�ϵ���ʱ100ms
 xWriteCH376Cmd(CMD11_CHECK_EXIST);//���Ե�Ƭ����CH376֮���ͨѶ�ӿ�
 xWriteCH376Data(0x65);            //��������0x65��Ӧ����0x9a
 res = xReadCH376Data();           //����������
 xEndCH376Cmd();                   //��CH376Ƭѡ�ߵ�ƽ
 if(res != 0x9A) init_err = ERR_USB_UNKNOWN;//ͨѶ�ӿڲ�����,����ԭ����:�ӿ������쳣,�����豸Ӱ��(Ƭѡ��Ψһ),���ڲ�����,һֱ�ڸ�λ,���񲻹��� 
 else
  {
   xWriteCH376Cmd(CMD11_SET_USB_MODE);//�豸USB����ģʽ
   xWriteCH376Data(0x06);             //����USB����ģʽ06H�������õ�USB������ʽ���Զ�����SOF��
   Delay(200);                        //��ʱ17us�ȴ��������(�ĵ���Ҫ��10us)
   res = xReadCH376Data();            //������״̬����
   xEndCH376Cmd();                    //��CH376Ƭѡ�ߵ�ƽ
#ifndef CH376_INT_WIRE
 #ifdef    CH376_SPI_SDO
   xWriteCH376Cmd(CMD20_SET_SDO_INT); //����SPI��SDO���ŵ��жϷ�ʽ */
   xWriteCH376Data(0x16);
   xWriteCH376Data(0x90);             //SDO������SCSƬѡ��Чʱ�����ж�������� */
   xEndCH376Cmd();
 #endif
#endif
   if(res == CMD_RET_SUCCESS) 
    {
     init_err = USB_INT_SUCCESS;//USB��ʼ����ȷ
     //����ϵ�ǰ�Ƿ��Ѿ���U�̲��룬���û��U����ʹCH376����˯��ģʽ
     if(CH376DiskConnect() != USB_INT_SUCCESS)
      {
       xWriteCH376Cmd(CMD00_ENTER_SLEEP);//CH376����˯��ģʽ
       xEndCH376Cmd();                   //��CH376Ƭѡ�ߵ�ƽ
      }
    }
   else init_err = ERR_USB_UNKNOWN;//����ģʽ����
  }
 return(init_err);
}