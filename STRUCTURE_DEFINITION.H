#ifndef STRUCTURE_DEFNITION_H
#define STRUCTURE_DEFNITION_H

/*******************************************************************************************************************************
���Խ�������Բ������ñ���
*******************************************************************************************************************************/
typedef struct //�������ݽṹ����
{
 unsigned char  TEST_NUMBER[6]; //������
 unsigned char  DEVICE_ID[6];   //�豸ID
 unsigned short RATED_VOLTAGE;  //���ѹ
 unsigned short BUS_VOLTAGE;    //ĸ��ʵ�ʵ�ѹֵ
 unsigned char  PT_CONNECT_MODE;//PT���ӷ�ʽ
 float          PT_TRANS_RAT;   //PT���ֵ
 float          PHASE_A_CAP;    //A�ಹ�������������
 float          PHASE_B_CAP;    //B�ಹ���������������
 float          PHASE_C_CAP;    //C�ಹ���������������
 float          Z_LF[10];       //��Ƶ�źŻ�·�迹
 float          D_LF[10];       //��Ƶ�źŵ�ѹ��������ǲ�
 float          R_LF[10];       //��Ƶ����Ƶ����ϵͳ�Ĵ�����
 float          XC_LF[10];      //��Ƶ����Ƶ����ϵͳ���ݿ�
 float          XL_LF[10];      //��Ƶ����Ƶ����ϵͳ�ĸп�
 float          Z_HF[10];       //��Ƶ�źŻ�·�迹
 float          D_HF[10];       //��Ƶ�źŵ�ѹ��������ǲ�
 float          R_HF[10];       //��Ƶ����Ƶ����ϵͳ�Ĵ�����
 float          XC_HF[10];      //��Ƶ����Ƶ����ϵͳ���ݿ�
 float          XL_HF[10];      //��Ƶ����Ƶ����ϵͳ�ĸп�
 float          VRMS_50HZ;      //����������PT�����ѹ
 float          CAP_MEDIAN[10]; //��������ĵ������м�ֵ
 float          CAPACITANCE;    //������������յ�����
 float          CAP_CURRENT;    //���������ĵ��ݵ���
 float          INTER_IMPEDANCE;//��Ƶ��Դ�������
 float          LOAD_IMPEDANCE; //���⸺���迹
 unsigned char  year;           //����ʱ�����
 unsigned char  month;          //����ʱ���·�
 unsigned char  date;           //����ʱ������
 unsigned char  hour;           //����ʱ��Сʱ
 unsigned char  minute;         //����ʱ�����
 unsigned char  second;         //����ʱ������
}TEST_DATA_STRUCT;

/*******************************************************************************************************************************
У׼ϵ���ṹ��
*******************************************************************************************************************************/
typedef struct //У׼ϵ���ṹ��
{
 float COE_Z_LF_V0I0;      //��Ƶ�迹У��ϵ������ѹ��λ0��������λ0
 float COE_Z_LF_V1I0;      //��Ƶ�迹У��ϵ������ѹ��λ1��������λ0
 float COE_Z_LF_V2I0;      //��Ƶ�迹У��ϵ������ѹ��λ2��������λ0
 float COE_Z_LF_V0I1;      //��Ƶ�迹У��ϵ������ѹ��λ0��������λ1
 float COE_Z_LF_V1I1;      //��Ƶ�迹У��ϵ������ѹ��λ1��������λ1
 float COE_Z_LF_V2I1;      //��Ƶ�迹У��ϵ������ѹ��λ2��������λ1
 float COE_PHA_DIF_LF_V0I0;//��Ƶ��ѹ��������ǲ�У��ϵ������ѹ��λ0��������λ0
 float COE_PHA_DIF_LF_V1I0;//��Ƶ��ѹ��������ǲ�У��ϵ������ѹ��λ1��������λ0
 float COE_PHA_DIF_LF_V2I0;//��Ƶ��ѹ��������ǲ�У��ϵ������ѹ��λ2��������λ0
 float COE_PHA_DIF_LF_V0I1;//��Ƶ��ѹ��������ǲ�У��ϵ������ѹ��λ0��������λ1
 float COE_PHA_DIF_LF_V1I1;//��Ƶ��ѹ��������ǲ�У��ϵ������ѹ��λ1��������λ1
 float COE_PHA_DIF_LF_V2I1;//��Ƶ��ѹ��������ǲ�У��ϵ������ѹ��λ2��������λ1
 float COE_V_LF_V0;        //��Ƶ��ѹ��λ0У��ϵ��
 float COE_V_LF_V1;        //��Ƶ��ѹ��λ1У��ϵ��
 float COE_V_LF_V2;        //��Ƶ��ѹ��λ2У��ϵ�� 
 float COE_I_LF_I0;        //��Ƶ������λ0У��ϵ��
 float COE_I_LF_I1;        //��Ƶ������λ1У��ϵ�� 
 float COE_Z_HF_V0I0;      //��Ƶ�迹У��ϵ������ѹ��λ0��������λ0
 float COE_Z_HF_V1I0;      //��Ƶ�迹У��ϵ������ѹ��λ1��������λ0
 float COE_Z_HF_V2I0;      //��Ƶ�迹У��ϵ������ѹ��λ2��������λ0
 float COE_Z_HF_V0I1;      //��Ƶ�迹У��ϵ������ѹ��λ0��������λ1
 float COE_Z_HF_V1I1;      //��Ƶ�迹У��ϵ������ѹ��λ1��������λ1
 float COE_Z_HF_V2I1;      //��Ƶ�迹У��ϵ������ѹ��λ2��������λ1
 float COE_PHA_DIF_HF_V0I0;//��Ƶ��ѹ��������ǲ�У��ϵ������ѹ��λ0��������λ0
 float COE_PHA_DIF_HF_V1I0;//��Ƶ��ѹ��������ǲ�У��ϵ������ѹ��λ1��������λ0
 float COE_PHA_DIF_HF_V2I0;//��Ƶ��ѹ��������ǲ�У��ϵ������ѹ��λ2��������λ0
 float COE_PHA_DIF_HF_V0I1;//��Ƶ��ѹ��������ǲ�У��ϵ������ѹ��λ0��������λ1
 float COE_PHA_DIF_HF_V1I1;//��Ƶ��ѹ��������ǲ�У��ϵ������ѹ��λ1��������λ1
 float COE_PHA_DIF_HF_V2I1;//��Ƶ��ѹ��������ǲ�У��ϵ������ѹ��λ2��������λ1
 float COE_V_HF_V0;        //��Ƶ��ѹ��λ0У��ϵ��
 float COE_V_HF_V1;        //��Ƶ��ѹ��λ1У��ϵ��
 float COE_V_HF_V2;        //��Ƶ��ѹ��λ2У��ϵ�� 
 float COE_I_HF_I0;        //��Ƶ������λ0У��ϵ��
 float COE_I_HF_I1;        //��Ƶ������λ1У��ϵ��
 float COE_VRMS_50HZ;      //50HZ��ѹ�ź���ЧֵУ��ϵ��
 float COE_V_BAT;          //��ص�ѹУ��ϵ�� 
}COE_DATA_STRUCT;

/*******************************************************************************************************************************
ģ�����������ṹ����
*******************************************************************************************************************************/
typedef struct //ģ�����������ṹ����
{
 float VRMS_LF;        //��Ƶ��ѹ�ź���Чֵʵ��ֵ
 float VRMS_LF_ORIG;   //��Ƶ��ѹ�ź���Чֵԭʼֵ
 float VPHA_LF;        //��Ƶ��ѹ�źų����
 float IRMS_LF;        //��Ƶ�����ź���Чֵʵ��ֵ
 float IRMS_LF_ORIG;   //��Ƶ�����ź���Чֵԭʼֵ
 float IPHA_LF;        //��Ƶ�����źų����
 float PHA_DIF_LF;     //��Ƶ�źŵ�ѹ��������ǲ�ʵ��ֵ
 float PHA_DIF_LF_ORIG;//��Ƶ�źŵ�ѹ��������ǲ�ԭʼֵ
 float VRMS_HF;        //��Ƶ��ѹ�ź���Чֵʵ��ֵ
 float VRMS_HF_ORIG;   //��Ƶ��ѹ�ź���Чֵԭʼֵ
 float VPHA_HF;        //��Ƶ��ѹ�źų����
 float IRMS_HF;        //��Ƶ�����ź���Чֵʵ��ֵ
 float IRMS_HF_ORIG;   //��Ƶ�����ź���Чֵԭʼֵ
 float IPHA_HF;        //��Ƶ�����źų����
 float PHA_DIF_HF;     //��Ƶ�źŵ�ѹ��������ǲ�ʵ��ֵ
 float PHA_DIF_HF_ORIG;//��Ƶ�źŵ�ѹ��������ǲ�ԭʼֵ
 float VRMS_50HZ;      //50HZ��ѹ�ź���Чֵʵ��ֵ
 //float VPHA_50HZ;      //50HZ��ѹ�źų����
 float V_BAT;          //��ص�ѹʵ��ֵ
}ANALOG_VALUE_STRUCT;

/*******************************************************************************************************************************
ʱ��ṹ�����ṹ����
*******************************************************************************************************************************/
typedef struct //ʱ��ṹ����
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