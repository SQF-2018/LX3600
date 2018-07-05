#ifndef __QJBL_H
#define __QJBL_H

#include "MACRO_DEFINITION.H"
#include "STRUCTURE_DEFINITION.H"

/*************************************************************************************************/
//�洢���洢��������
/*************************************************************************************************/
extern unsigned char  DEVICE_NUMBER[4];  //װ�ñ��
extern unsigned char  TEST_RECORD_NUMBER;//���²�����¼���
extern unsigned char  TEST_RECORD_AMOUNT;//�Ѿ�����Ĳ�����¼���ܸ���

/*************************************************************************************************/
//����ͨ�ó��򲿷�����
/*************************************************************************************************/
extern BCD_TIME NOW_TIME;                    //ʱ������
extern unsigned char  RELAY_DETECT_EN;       //�̵�������ʹ�ܱ�־
extern unsigned char  RELAY_DETECT;          //�̵������Կ��ƼĴ���
extern unsigned char  RELAY_CTRL;            //�̵������ƼĴ���
extern unsigned char  HARDWARE_EEOR_FLAG;    //Ӳ�������־
extern float          RTC_CORRECT_COE;       //RTCʱ��У��ϵ��
extern unsigned char  POWERON_TIME_COUNT;    //�����ϵ�ʱ���ʱ
extern unsigned char  U_DISK_CONNECT;        //U�̲����־
extern unsigned char  U_DISK_MOUNT;          //U�̳�ʼ����־
extern unsigned char  U_DISK_MOUNT_TIME;     //U�̳�ʼ����ʱ��ʱ
extern unsigned char  PRINTING_FLAG;         //��ǰ���ڴ�ӡ��־
extern unsigned char  OPERATION_PROCESS;     //�������̱�־
extern unsigned char  RECORD_STORAGED_INTER; //���������¼�ѱ�����������־
extern unsigned char  BLUETOOTH_MODLE_INIT;  //����ģ���ʼ����־
extern unsigned char  BLUETOOTH_CONNECT;     //�������ӱ�־
extern unsigned short HARDWARE_WORK_STATE;   //Ӳ������״̬
extern unsigned char  WRITE_DEVICE_NUMBER_EN;//����дװ�ñ�ű�־,ON:����;OFF:��ֹ
extern unsigned char  OPERATE_STATE;         //����״̬��־,��Ҫ����ϵͳ�������õ���Ļ

/*******************************************************************************************************************************
ADC�������ֱ���
*******************************************************************************************************************************/
extern unsigned char  AD7606_DONE;                      //����AD7606һ�ܲ�������ɱ�־
extern unsigned short AD7606_POINT;                     //����AD7606��������������

/*******************************************************************************************************************************
���Խ�������Բ������ñ���
*******************************************************************************************************************************/
extern TEST_DATA_STRUCT TEST_DATA;//���Խ�������Բ������ýṹ��

/*******************************************************************************************************************************
У׼ϵ���ṹ��
*******************************************************************************************************************************/
extern COE_DATA_STRUCT COE_DATA;//У׼ϵ���ṹ��

/*******************************************************************************************************************************
Һ����ʾ���ֱ���
*******************************************************************************************************************************/
extern unsigned short LCD_FORE_COLOR;//������ʾ��ǰ��ɫ������ͳһ����
extern unsigned short LCD_BACK_COLOR;//������ʾ�ı���ɫ������ͳһ����
extern unsigned char  LCD_BRIGHTNESS;//Һ������������ֵ
extern unsigned char  SCRFL;         //��Ļ���
extern unsigned char  POPUP_SCRFL;   //������Ļ���
extern unsigned char  BURST_SCRFL;   //ͻ��������Ļ���
extern unsigned char  SCRAPPLY;      //����ȫ��ˢ�±�־
extern unsigned char  DUPDATE;       //��������ˢ�±�־
extern unsigned char  CDX1;          //��һ���˵����λ��
extern unsigned char  CDX2;          //�ڶ����˵����λ�� 
extern unsigned char  CDX3;          //�������˵����λ��
extern unsigned char  CDX4;          //���ļ��˵����λ��
extern unsigned char  POPUP_CDX;     //�����˵����λ��
extern unsigned char  DISPLAY_PT_WIRING_DIAGRAM;//��ʾPT����ͼʹ�ܿ��Ʊ�־
extern unsigned char  PARAMETER_DISPLAY_EN;     //����������ϸ������ʾ���Ʊ�־

/*-----------------------------------------------------------------------------------------------*/
//�������ִ�������
/*-----------------------------------------------------------------------------------------------*/
extern unsigned char  KEY1;            //��һ�ΰ�������ֵ
extern unsigned char  KEY2;            //�ڶ��ΰ�������ֵ
extern unsigned char  YESKEY;          //ȷ���İ�����ֵ
extern unsigned short KEYTIME;         //һ�������������µ�ʱ��
extern unsigned short DKEYTIME;        //�ް������³���ʱ��
//������Ļ����������ʱ����
extern unsigned char  KEY_CHAR_TMP[25];//������Ļ����������char����ʱ����
extern unsigned short KEY_SHORT_TMP[5];//������Ļ����������short����ʱ����
    
#define YEAR_KEY               KEY_CHAR_TMP[0]//ʵʱʱ��������Ļ������ʱ����
#define MONTH_KEY              KEY_CHAR_TMP[1]
#define DATE_KEY               KEY_CHAR_TMP[2]
#define WEEKDAY_KEY            KEY_CHAR_TMP[3]
#define HOUR_KEY               KEY_CHAR_TMP[4]
#define MINUTE_KEY             KEY_CHAR_TMP[5]
#define SECOND_KEY             KEY_CHAR_TMP[6]
#define TEST_RECORD_NUMBER_KEY KEY_CHAR_TMP[0]//��ȡ������¼��Ű�����ʱ����
#define MIMA0                  KEY_CHAR_TMP[0]//����������滻
#define MIMA1                  KEY_CHAR_TMP[1]
#define MIMA2                  KEY_CHAR_TMP[2]
#define MIMA3                  KEY_CHAR_TMP[3]
#define MIMA4                  KEY_CHAR_TMP[4]
#define MIMA5                  KEY_CHAR_TMP[5]


/*************************************************************************************************/
//�������̿��Ʋ��ֱ�������
/*************************************************************************************************/
extern unsigned char  TEST_PROCESS_EN;  //ʹ�ܽ�ֹ�������̱�־
extern unsigned char  TEST_PROCESS_FLAG;//�������̿��Ʊ�־
extern unsigned short TEST_PROCESS_TIME;//����������ÿ���׶���ʱ��ʱ����λ��10ms

/*************************************************************************************************/
//����ת����ͨѶ��UART4�����ֱ���
/*************************************************************************************************/
extern unsigned char BLUETOOTH_RXBUF[50]; //�첽����ͨѶ���ջ�����
extern unsigned char BLUETOOTH_TXBUF[70]; //�첽����ͨѶ���ͻ�����
extern unsigned char BLUETOOTH_RXNO;      //ͨѶ���յ����ֽ���Ŀ
extern unsigned char BLUETOOTH_TXNO;      //ͨѶ��������ָ��
extern unsigned char BLUETOOTH_TXEND;     //ͨѶҪ���͵������ܸ���
extern unsigned char BLUETOOTH_RXTIME;    //ͨѶ���յ����һ���ֽں�ľ�ֹʱ��
extern unsigned char BLUETOOTH_RXTIME_EN; //����ͨѶ���վ�ֹʱ���ʱ��־
extern unsigned char BLUETOOTH_RXTIME_END;//ͨѶ���վ�ֹʱ�䵽ʱ��־

/*************************************************************************************************/
//��ʱ�ñ���
/*************************************************************************************************/
extern unsigned char HALF_SECOND_SIG;    //�����Ӷ�ʱ����ʱ��־
extern unsigned char HUNDRED_MSECOND_SIG;//һ�ٺ��붨ʱ����ʱ��־
extern unsigned char KAIJI_TIME;         //����������ʾ��ʱ��
extern unsigned char SLOW_SHAN_SIG;      //����˸��־����ʱ������
extern unsigned char FAST_SHAN_SIG;      //����˸��־����ʱ100ms

/*************************************************************************************************/
//������ʱ����
//50  ��ʱ4.52us
//100 ��ʱ8.68us
//200 ��ʱ17us
//500 ��ʱ42us
//1000��ʱ83.8us
/*************************************************************************************************/
extern void Delay(unsigned int count);
/*************************************************************************************************/
//����������ʱ����
/*************************************************************************************************/
extern void Delay_ms(unsigned int ms);
/*************************************************************************************************/
// Name: hex_to_bcd
// Description: ʮ��������ת����BDC���ʽ
// Calls: None
// Input: ��Ҫת����ʮ��������
// Outputs: ת�����BCD��
/*************************************************************************************************/
extern unsigned short Hex_To_Bcd(unsigned short data);
/*************************************************************************************************/
//Name: bcd_to_hex
//Description: BCD���ʽת����ʮ��������
//Calls: None
//Input: ��Ҫת����BCD��
//Outputs: ת�����ʮ��������
/*************************************************************************************************/
extern unsigned short Bcd_To_Hex(unsigned short data);

/*************************************************************************************************/
#define Set_Bit(val, bitn)    (val |=(1<<(bitn)))
#define Clr_Bit(val, bitn)    (val&=~(1<<(bitn)))
#define Get_Bit(val, bitn)    (val &(1<<(bitn)) )

/*************************************************************************************************/
//����CRC16����
/*************************************************************************************************/
extern unsigned short int CRC16(unsigned char *puchMsg, unsigned short int usDataLen);

/*************************************************************************************************/
//�������������ƴ�������
/*************************************************************************************************/
extern void Alarm_BEEP(void);

/*************************************************************************************************/
//�̵������Ƴ���
/*************************************************************************************************/
extern void Relay_Control(void);

/*******************************************************************************************************************************
��������N�η����㺯��(Դ��SGI STL�㷨��)
*******************************************************************************************************************************/
extern int Int_Power(int x, unsigned int n);

/*******************************************************************************************************************************
������ת�����ַ������麯�� 
����  data_f   :Ҫת���ĸ�����
      *data_i  :����ת�����ݵ��ַ�������
      symbol_en:�Ƿ���ʾ����λ,0:����ʾ,��0:��ʾ����λ
      length   :Ҫת���ĳ���,����С���ͷ���λ���ڵĳ���
*******************************************************************************************************************************/
extern void Float_To_Char(float data_f,unsigned char *data_i,unsigned char symbol_en,unsigned char length);

/*******************************************************************************************************************************
������ת���ɹ̶�С�������ַ������麯�� 
����  data_f   :Ҫת���ĸ�����
      *data_i  :����ת�����ݵ��ַ�������
      symbol_en:�Ƿ���ʾ����λ,0:����ʾ,��0:��ʾ����λ
      length   :Ҫת���ĳ���,����С���ͷ���λ���ڵĳ���,��󳤶�10λ(С����1λ,����λ1λ,��Ч����7λ,С��0����С����ǰ��0ռһλ)
      dot      :��Ҫ��С������
*******************************************************************************************************************************/
extern void Float_To_Fixed_Char(float data_f,unsigned char *data_i,unsigned char symbol_en,unsigned char length,unsigned char dot);  

/*******************************************************************************************************************************
�ַ�������ת���ɸ��������� 
������data      Ҫת�����ַ���ָ��
      length    Ҫת�����ַ�����
����data[6]={'1','2','3','.','4','5'},ת����ɺ�=123.45
*******************************************************************************************************************************/
extern float Char_To_Float(unsigned char *data,unsigned char length);

#endif