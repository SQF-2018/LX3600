/********************************************************************************************************************************
����:   AD7606���������򼰲�������
�ļ���: AD7606.C 
������: STM32F103ϵ��CPU
������: Keil C Version 5.05
����:   ��Ԫ
��Ȩ:   ���������˵����豸���޹�˾
���ڣ�  2017.08.08
********************************************************************************************************************************/
#include "stm32f10x.h"
#include "QJBL.H"
#include "IOCONFIG.H"

/********************************************************************************************************************************
��AD7606ͨѶʹ�õ�SPI�ڳ�ʼ������
SPI2��APB1���ߣ����ʱ��Ƶ��36MHz
********************************************************************************************************************************/
void AD7606_SPI_Configuration(void)
{
 SPI_InitTypeDef SPI_InitStructure;//����SPI�����ݽṹ��
 SPI_Cmd(SPI2, DISABLE);           //��ֹSPI2
 SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;//����Ϊ˫��˫��ȫ˫��ģʽ
 SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;                //����Ϊ���豸
 SPI_InitStructure.SPI_DataSize          = SPI_DataSize_16b;               //16λ���ݴ���
 SPI_InitStructure.SPI_CPOL              = SPI_CPOL_High;                  //����ʱSCK���ָߵ�ƽ
 SPI_InitStructure.SPI_CPHA              = SPI_CPHA_2Edge;                 //���ݴӵ�һ��ʱ���ؿ�ʼ���˴�Ϊ�����ؿ�ʼ
 SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;                   //�������豸����
 SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;        //������= 36M/16 = 4.5MHz
 SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;		       //����λ��λ��ǰ
 SPI_InitStructure.SPI_CRCPolynomial     = 7;						       //��CRC����ֵ
 SPI_Init(SPI2, &SPI_InitStructure);
 SPI_I2S_ITConfig(SPI2, SPI_IT_CRCERR, DISABLE);//�ر�CRC�����ж�
 SPI_Cmd(SPI2, ENABLE);                         //ʹ��SPI2
}

/********************************************************************************************************************************
�ⲿ�жϳ�ʼ��
AD7606-BUSY����
BUSY:PD8
********************************************************************************************************************************/
void AD7606_EXTI_Configuration(void)
{
 EXTI_InitTypeDef EXTI_InitStructure; 
 GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource8);//PD8Ϊ�ⲿ�ж��������� 
 EXTI_InitStructure.EXTI_Line    = EXTI_Line8;          //�����ⲿ�ж���8
 EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt; //����Ϊ�ж�ģʽ 
 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//����Ϊ�½����ж�
 EXTI_InitStructure.EXTI_LineCmd = ENABLE;              //�����ⲿ�ж�
 EXTI_Init(&EXTI_InitStructure);                        //�ⲿ�жϳ�ʼ��
}

///********************************************************************************************************************************
//���ö�ʱ��8
//����:base_freq �������������źŻ���Ƶ��,��λHz
//����:data_len  ÿ���ܲ������ĳ���,����������
//�����ⲿADC����������߲��������й�
//��ʱ��8��CC1�¼���������AD7606������TIM8_CH1(PC6)���ŵ������ؿ�������ADת��
//********************************************************************************************************************************/
//void TIM8_Configuration(float base_freq,unsigned int data_len)
//{
// TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//���嶨ʱ�����ݽṹ��
// TIM_OCInitTypeDef        TIM_OCInitStructure;  //���嶨ʱ���Ƚ�������ݽṹ��
// TIM_BDTRInitTypeDef      TIM_BDTRInitStructure;//���嶨ʱ��BDTR���ݽṹ��
// float tim_prescal=0,tim_period,max_sample_rat; //��ʱ��Ԥ��Ƶϵ�������ڡ�AD��������������

// switch(AD7606_OS_RAT)
//  {
//   case  0:max_sample_rat = 200000;//200KHz
//           break;
//   case  2:max_sample_rat = 100000;//100KHz
//           break;
//   case  4:max_sample_rat = 50000; //50KHz
//           break;
//   case  8:max_sample_rat = 25000; //25KHz
//           break;
//   case 16:max_sample_rat = 12500; //12.5KHz
//           break;
//   case 32:max_sample_rat = 6250;  //6.25KHz
//           break;
//   case 64:max_sample_rat = 3125;  //3.125KHz
//           break;
//   default:max_sample_rat = 200000;//200KHz
//           break;
//  }
// if(base_freq * data_len <= max_sample_rat)//�����ʲ��ô�����������Ĳ�����
//  {
//   do
//    {
//     tim_period = 72000000 / (1 + tim_prescal++) / (base_freq * data_len) + 0.5;//�������趨ʱ������ֵ����������
//    }
//   while(tim_period > 65535 || tim_prescal > 65535);//�ж����������ֵ�Ƿ�Խ��

//   TIM_DeInit(TIM8);//��λTIM8��ʱ��

//   //���ö�ʱ��8
//   TIM_TimeBaseStructure.TIM_Period        = tim_period  - 1;   //���ڼĴ���      
//   TIM_TimeBaseStructure.TIM_Prescaler     = tim_prescal - 1;   //Ԥ��Ƶϵ��
//   //TIM8�ڸ���APB2���ߣ��ٶ�Ϊ72MHZ       
//   TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;               // ʱ�ӷָ�  
//   TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;//�����������ϼ���
//   TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
//   //����TIM8ͨ��1��CC1�¼�
//   TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1;         //TIM8������ȵ���ģʽ1;���ϼ���ʱ��һ��TIMX_CNT<TIMX_CCRXʱͨ��Ϊ��Ч��ƽ������Ϊ��Ч��ƽ�� 
//   TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable;  //ʹ�����״̬
//   TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//��ֹ�������״̬
//   TIM_OCInitStructure.TIM_Pulse        = tim_period / 2;          //���ò���/�ȽϼĴ�����ֵ 
//   TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_Low;      //����OCX�������
//   TIM_OCInitStructure.TIM_OCNPolarity  = TIM_OCPolarity_Low;      //����OCNX�������         
//   TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Reset;   //�����������״̬����MOE=0ʱ�����ʵ����OCXN����������OCX=0
//   TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;  //�����������״̬����MOE=0ʱ��������OCXN=0      
//   TIM_OC1Init(TIM8, &TIM_OCInitStructure);                        //����TIMX��OC1���ͨ��
//   
//   //����ɲ�����ԣ�����ʱ�䣬����ƽ��OSSI��OSSR״̬��AOE���Զ����ʹ�ܣ�
//   TIM_BDTRInitStructure.TIM_OSSRState       = TIM_OSSRState_Enable;      
//   TIM_BDTRInitStructure.TIM_OSSIState       = TIM_OSSIState_Enable;      
//   TIM_BDTRInitStructure.TIM_LOCKLevel       = TIM_LOCKLevel_3;//ʹ��3������
//   //����TIMx_BDTR�Ĵ�����DTG��BKE��BKP��AOEλ������TIMx_CR2�Ĵ�����OISx��OISxNλ
//   //����TIMx_CCER�Ĵ�����CCxP��CCNxPλ������TIMx_BDTR�Ĵ�����OSSR��OSSIλ
//   //����TIMx_CCMRx�Ĵ�����OCxM��OCxPEλ      
//   TIM_BDTRInitStructure.TIM_DeadTime        = 0;                          //��������ʱ��Ϊ0
//   TIM_BDTRInitStructure.TIM_Break           = TIM_Break_Disable;          //��ֹɲ������
//   TIM_BDTRInitStructure.TIM_BreakPolarity   = TIM_BreakPolarity_High;     //ɲ���źż���Ϊ�ߵ�ƽ��Ч  
//   TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;//��ֹ�Զ����ʹ�ܹ���
//   TIM_BDTRConfig(TIM8, &TIM_BDTRInitStructure);

//   //����TIM8����
//   TIM_Cmd(TIM8, ENABLE);
//   //TIM_CtrlPWMOutputs(TIM8, DISABLE);//��ֹPWM���������MOEλ
//   TIM_CtrlPWMOutputs(TIM8, ENABLE);//ʹ��PWM�������λMOEλ
//  }
//}

///********************************************************************************************************************************
//PC4(AD-BUSY)�ⲿ�жϲ�������
//********************************************************************************************************************************/
//void EXTI4_IRQHandler(void)
//{
// if(AD7606_DONE == NO)
//  {
//   AD_CS_L;//ʹ��AD7606Ƭѡ�ź�
//   /*��ȡ��ѹͨ������*/
//   SPI1->DR = 0x0000;//��������0x0000��Ŀ���Ƿ���16��ʱ���ź�   
//   while((SPI1->SR & SPI_I2S_FLAG_RXNE) == 0);//�ȴ����ݷ�����ɣ����ȴ����ݽ������	
//    { 
//     AD7606_DATA_BUF[VOLTAGE_CHANNAL][AD7606_POINT] = SPI1->DR;//����ȡ�������ݴ洢����������
//    }
//   /*��ȡ����ͨ������*/ 
//   SPI1->DR = 0x0000;//��������0x0000��Ŀ���Ƿ���16��ʱ���ź�   
//   while((SPI1->SR & SPI_I2S_FLAG_RXNE) == 0);//�ȴ����ݷ�����ɣ����ȴ����ݽ������	
//    { 
//     AD7606_DATA_BUF[CURRENT_CHANNAL][AD7606_POINT] = SPI1->DR;//����ȡ�������ݴ洢����������
//    }
//   /*��ȡ�����ѹͨ������*/ 
//   SPI1->DR = 0x0000;//��������0x0000��Ŀ���Ƿ���16��ʱ���ź�   
//   while((SPI1->SR & SPI_I2S_FLAG_RXNE) == 0);//�ȴ����ݷ�����ɣ����ȴ����ݽ������	
//    { 
//     AD7606_DATA_BUF[VOLTAGE_50HZ_CHANNAL][AD7606_POINT] = SPI1->DR;//����ȡ�������ݴ洢����������
//    } 
//   AD_CS_H; //��ֹAD7606Ƭѡ�ź�
//   AD7606_POINT += 1;
//   if(AD7606_POINT >= FFT_POINT)
//    {
//     TIM_Cmd(TIM8, DISABLE);           //ֹͣTIM8����
//     TIM_CtrlPWMOutputs(TIM8, DISABLE);//��ֹPWM���������MOEλ
//     AD7606_POINT = 0;                 //����AD7606��������������
//     AD7606_DONE  = YES;               //AD7606һ�ܲ��������
//    }
//  }
// EXTI_ClearITPendingBit(EXTI_Line4);//�������Ĵ������жϱ�־
//}

///********************************************************************************************************************************
//AD7606���������ó���
//����:rat ���������ʣ�ȡֵ0��2��4��8��16��32��64
//********************************************************************************************************************************/
//void AD7606_OverSample_Rat(unsigned char rat)
//{
////�޹�����,SNR 5V 89dB,SNR 10V 90dB,BW 5V 15KHz,BW 10V 22KHz,CONVST Frequency 200K
////2��������,SNR 5V 91.2dB,SNR 10V 92dB,BW 5V 15KHz,BW 10V 22KHz,CONVST Frequency 100K
////4��������,SNR 5V 92.6dB,SNR 10V 93.6dB,BW 5V 13.7KHz,BW 10V 18.5KHz,CONVST Frequency 50K
////8��������,SNR 5V 94.2dB,SNR 10V 95dB,BW 5V 10.3KHz,BW 10V 11.9KHz,CONVST Frequency 25K
////16��������,SNR 5V 95.5dB,SNR 10V 96dB,BW 5V 6KHz,BW 10V 6KHz,CONVST Frequency 12.5K
////32��������,SNR 5V 96.4dB,SNR 10V 96.7dB,BW 5V 3KHz,BW 10V 3KHz,CONVST Frequency 6.25K
////64��������,SNR 5V 96.9dB,SNR 10V 97dB,BW 5V 1.5KHz,BW 10V 1.5KHz,CONVST Frequency 3.125K
////�޹�����,SNR 5V 89dB,SNR 10V 90dB,BW 5V 15KHz,BW 10V 22KHz,CONVST Frequency 200K
////������õĹ��������ʳ���Χ����ʹ��default���ã�������AD7606_OS_RAT = 0�Ա�����֪AD7606��Ĭ�����õĹ�������
//}

///********************************************************************************************************************************
//AD7606����ģʽ���ó���
//********************************************************************************************************************************/
//void AD7606_STBY_Mode(unsigned char mode)
//{
// //��������ģʽ��5V����
// //AD_STBY_H;
// //AD_RANGE_5V_L;
// Delay_ms(50); //��ʱ50ms����֤AD7606�ϵ���ȫ���ֲ�Ҫ��30ms
// AD_RESET_H;   //��λоƬ
// Delay_ms(1);  //��ʱ1ms
// AD_RESET_L;   //��λ���
// Delay_ms(1);  //��ʱ1ms
//}

///********************************************************************************************************************************
//��ѹͨ�������л����Ƴ���
//����: gear ���浵λ��0��1��2
//********************************************************************************************************************************/
//void Voltage_Gain_Switch(unsigned char gear)
//{
// switch(gear)
//  {
//   case 0://���浵λ0
//          JDQ_FY_OFF; //��ѹ���������л�������ѹ��
//          JDQ_VZY_OFF;//��ѹ��������Ϊ1��
//          VOLTAGE_GAIN_GEAR = 0;
//          break;
//   case 1://���浵λ1
//          JDQ_FY_ON;  //��ѹ���������л���С��ѹ��
//          JDQ_VZY_OFF;//��ѹ��������Ϊ1��
//          VOLTAGE_GAIN_GEAR = 1;
//          break;
//   case 2://���浵λ2
//          JDQ_FY_ON; //��ѹ���������л���С��ѹ��
//          JDQ_VZY_ON;//��ѹ��������Ϊ10��
//          VOLTAGE_GAIN_GEAR = 2;
//          break;
//  }
//}

///********************************************************************************************************************************
//����ͨ�������л����Ƴ���
//����: gear ���浵λ��0��1
//********************************************************************************************************************************/
//void Current_Gain_Switch(unsigned char gear)
//{
// switch(gear)
//  {
//   case 0://���浵λ0
//          JDQ_IZY_OFF;//������������Ϊ5��
//          CURRENT_GAIN_GEAR = 0;
//          break;
//   case 1://���浵λ1
//          JDQ_IZY_ON;//������������Ϊ41��
//          CURRENT_GAIN_GEAR = 1;
//          break;
//  }
//}

///********************************************************************************************************************************
//ADģ���������л����Ƴ���
//����: ctrl_en �л�����������־��0:��ֹ�л�����ѹ�����л��������������;1:�����Զ��л�����
//********************************************************************************************************************************/
//void AD_Analog_Gain_Switch (unsigned char ctrl_en)
//{
// unsigned short i;
// unsigned short u_over_upper_limit,u_over_lower_limit,i_over_upper_limit,i_over_lower_limit;
// u_over_upper_limit = 0;//��ѹͨ������ֵ������߷�ֵ����
// u_over_lower_limit = 0;//��ѹͨ������ֵ������ͷ�ֵ����
// i_over_upper_limit = 0;
// i_over_lower_limit = 0;
// /*----------------------------------------------�����Զ��л�����---------------------------------------------*/
// if(ctrl_en)
//  {
//   for(i=0;i<FFT_POINT;i++)/*������ѹ����������ֵ�Ƿ���*/
//    {
//     switch(VOLTAGE_GAIN_GEAR)/*����ѹͨ������ֵ�Ƿ�Խ��*/
//      {
//       case 0://��ѹ��0.135643166579��62K+9.729634802135K��
//              //��������6.3825��25.0224Vrms�����������ޱ�6.8825Vrms��0.5Vrms�������غϲ��֣�
//              //��ѹ���0.8657��3.3941Vrms
//              //AD����  ��1.2243��AD������ֵ��8023������4.8Vpp��AD������ֵ��31457��
//              /*��ѹAD����ֵС�ڡ�8023����Ҫ�л����������ĵ�λ*/
//              if(AD7606_DATA_BUF[VOLTAGE_CHANNAL][i] > 8023 || AD7606_DATA_BUF[VOLTAGE_CHANNAL][i] < -8023) u_over_lower_limit++;
//              break;
//       case 1://��ѹ��0.493148246266��10K+9.729634802135K����1���Ŵ�
//              //��������0.63825��6.8825Vrms
//              //��ѹ���0.31475��3.39409Vrms
//              //AD����  ��0.4451��AD������ֵ��2917������4.8Vpp��AD������ֵ��31457��
//              /*��ѹAD����ֵ���ڡ�31457����Ҫ�л��������С�ĵ�λ*/
//              if(AD7606_DATA_BUF[VOLTAGE_CHANNAL][i] > 31457 || AD7606_DATA_BUF[VOLTAGE_CHANNAL][i] < -31457) u_over_upper_limit++;
//              /*��ѹAD����ֵС�ڡ�2917����Ҫ�л����������ĵ�λ*/
//              else if(AD7606_DATA_BUF[VOLTAGE_CHANNAL][i] > 2917 || AD7606_DATA_BUF[VOLTAGE_CHANNAL][i] < -2917) u_over_lower_limit++;
//              break;
//       case 2://��ѹ��0.493148246266��10K+9.729634802135K����10���Ŵ�
//              //�������� 0��0.68825Vrms
//              //��ѹ��� 0��0.33941Vrms
//              //AD����   0����4.8Vpp��AD������ֵ��31457��
//              /*��ѹAD����ֵ���ڡ�31457����Ҫ�л��������С�ĵ�λ*/
//              if(AD7606_DATA_BUF[VOLTAGE_CHANNAL][i] > 31457 || AD7606_DATA_BUF[VOLTAGE_CHANNAL][i] < -31457) u_over_upper_limit++;
//              break;
//      }
//     switch(CURRENT_GAIN_GEAR)/*������ͨ������ֵ�Ƿ�Խ��*/
//      {
//       case 0://0.25��ȡ�����裬5���Ŵ�
//              //��������     281.1317mArms��2.71528Arms
//              //ȡ��������� 70.28293 �� 678.82mVrms
//              //AD����       ��496.97536mVrms��AD������ֵ��3257���� ��4.8Vpp��AD������ֵ��31457��
//              /*AD����ֵС�ڡ�3257����Ҫ�л����������ĵ�λ*/
//              if(AD7606_DATA_BUF[CURRENT_CHANNAL][i] > 3257 || AD7606_DATA_BUF[CURRENT_CHANNAL][i] < -3257) i_over_lower_limit++;
//              break;
//       case 1://0.25��ȡ�����裬41���Ŵ�
//              //��������     0 �� 331.1317mArms
//              //ȡ��������� 0 �� 82.78292mVrms
//              //AD����       0 �� ��4.8Vpp��AD������ֵ��31457��
//              /*AD����ֵ���ڡ�31457����Ҫ�л��������С�ĵ�λ*/
//              if(AD7606_DATA_BUF[CURRENT_CHANNAL][i] > 31457 || AD7606_DATA_BUF[CURRENT_CHANNAL][i] < -31457) i_over_upper_limit++;
//              break;
//      }
//    }      
//   /*----------------------------------��ѹ�����Զ��л�------------------------------------*/   
//   if(u_over_upper_limit > 0)/*����ֵ������һ���㳬�����������ֵ����Ҫ�л��������С�ĵ�λ*/
//    {
//     if(VOLTAGE_GAIN_GEAR == 1)
//      {
//       Voltage_Gain_Switch(0);  //��ѹͨ�������л�����λ0 
//       JDQ_FY_ACTION_FLAG = YES;//�����ѹ����������л���������Ҫ��ʱ����
//       AD_GAIN_SWITCHING  = YES;//�����б仯 
//       Delay_ms(5);             //��ʱ5ms���ȴ��̵��������ͷ����
//      }
//     else if(VOLTAGE_GAIN_GEAR == 2)
//      {
//       Voltage_Gain_Switch(1); //��ѹͨ�������л�����λ1
//       AD_GAIN_SWITCHING = YES;//�����б仯 
//       Delay_ms(5);            //��ʱ5ms���ȴ��̵��������ͷ����
//      } 
//    }
//   else if(u_over_lower_limit == 0)/*����ֵ���е㶼û�г���������Сֵ����Ҫ�л����������ĵ�λ*/
//    {
//     if(VOLTAGE_GAIN_GEAR == 0)
//      {
//       Voltage_Gain_Switch(1);  //��ѹͨ�������л�����λ1
//       JDQ_FY_ACTION_FLAG = YES;//�����ѹ����������л���������Ҫ��ʱ����
//       AD_GAIN_SWITCHING  = YES;//�����б仯
//       Delay_ms(5);             //��ʱ5ms���ȴ��̵��������ͷ����
//      }
//     else if(VOLTAGE_GAIN_GEAR == 1)
//      {
//       Voltage_Gain_Switch(2); //��ѹͨ�������л�����λ2
//       AD_GAIN_SWITCHING = YES;//�����б仯
//       Delay_ms(5);            //��ʱ5ms���ȴ��̵��������ͷ����
//      }     
//    }
//   /*----------------------------------���������Զ��л�------------------------------------*/   
//   if(i_over_upper_limit > 0)/*����ֵ������һ���㳬�����������ֵ����Ҫ�л��������С�ĵ�λ*/
//    {
//     if(CURRENT_GAIN_GEAR == 1)
//      {
//       Current_Gain_Switch(0); //����ͨ�������л�����λ0
//       AD_GAIN_SWITCHING = YES;//�����б仯
//       Delay_ms(5);            //��ʱ5ms���ȴ��̵��������ͷ����
//      }
//    }
//   else if(i_over_lower_limit == 0)/*����ֵ���е㶼û�г���������Сֵ����Ҫ�л����������ĵ�λ*/
//    {
//     if(CURRENT_GAIN_GEAR == 0)
//      {
//       Current_Gain_Switch(1); //����ͨ�������л�����λ1
//       AD_GAIN_SWITCHING = YES;//�����б仯
//       Delay_ms(5);            //��ʱ5ms���ȴ��̵��������ͷ����
//      }
//    }   
//  }
// else /*-------------------------------------��ֹ�Զ��л�����-------------------------------------*/
//  {
//   if(RELAY_DETECT_EN == OFF)//û�н��м̵�����⣬���ԶԼ̵������п���
//    {
//     Voltage_Gain_Switch(0);//��ѹͨ�������л�����λ0
//     Current_Gain_Switch(0);//����ͨ�������л�����λ0
//    }
//  }
//}

/********************************************************************************************************************************
AD7606��ʼ������
ADI���������ĵ�˵��AD7606�ϵ��ʼ����Ҫ�ȹض�AD7606�ٿ�����ͬʱ����λ�źţ��������Ա�֤AD��������
********************************************************************************************************************************/
void AD7606_Init(void)
{
// AD_Analog_Gain_Switch(0);            //��ʼ����ѹͨ���������
// AD7606_OS_RAT = 8;                   //���ù���������Ϊ8����12HZʱ��2HZΪ��Ƶ(4096HZ),180HZʱ��10HZΪ��Ƶ(20480HZ)
// AD7606_SPI_Configuration();          //����STM32��AD7606��SPI�ӿ�
// AD7606_EXTI_Configuration();         //����AD-BUSY�����ⲿ�ж�
// //AD7606_OverSample_Rat(AD7606_OS_RAT);//���ù���������
// //AD7606_STBY_Mode('G');               //��ȫ�ض�AD7606
// AD7606_STBY_Mode('A');               //����AD7606
// TIM8_Configuration(FFT_BASE_FREQENCY,FFT_POINT);//��ʼ��TIM8,���û���Ƶ��FFT_BASE_FREQENCY,����AD_SAMPLE_POINT��,������ʼ
}
