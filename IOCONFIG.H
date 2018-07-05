#ifndef __IOFONFIG_H
#define __IOFONFIG_H

#include "stm32f10x_gpio.h"
/*ָʾ��IO����************************************************************************************/
//����ָʾ�� PE5
#define RUNLED_ON    GPIOE->BRR  = 0x0020//��
#define RUNLED_OFF   GPIOE->BSRR = 0x0020//��

/*�̵�������IO����********************************************************************************/
//������Դ������Ƽ̵��� PE4
#define JDQ_POWER_CTRL_ON   GPIOE->BSRR = 0x0010//����
#define JDQ_POWER_CTRL_OFF  GPIOE->BRR  = 0x0010//�ͷ�
//�����Ƽ̵��� PD14
#define JDQ_MASTER_CTRL_ON  GPIOD->BSRR = 0x4000//����,���ʵ�Դ���
#define JDQ_MASTER_CTRL_OFF GPIOD->BRR  = 0x4000//�ͷ�,���ʵ�Դ�Ͽ�
//����Դ������Ƽ̵���1 PA12
#define JDQ1_ON   GPIOA->BSRR = 0x1000//����
#define JDQ1_OFF  GPIOA->BRR  = 0x1000//�ͷ�
//����Դ������Ƽ̵���2 PA11
#define JDQ2_ON   GPIOA->BSRR = 0x0800//����
#define JDQ2_OFF  GPIOA->BRR  = 0x0800//�ͷ�
//����Դ������Ƽ̵���3 PA10
#define JDQ3_ON   GPIOA->BSRR = 0x0400//����
#define JDQ3_OFF  GPIOA->BRR  = 0x0400//�ͷ�
//����Դ������Ƽ̵���4 PA9
#define JDQ4_ON   GPIOA->BSRR = 0x0200//����
#define JDQ4_OFF  GPIOA->BRR  = 0x0200//�ͷ�
//����Դ������Ƽ̵���5 PA8
#define JDQ5_ON   GPIOA->BSRR = 0x0100//����
#define JDQ5_OFF  GPIOA->BRR  = 0x0100//�ͷ�
//����Դ������Ƽ̵���6 PC9
#define JDQ6_ON   GPIOC->BSRR = 0x0200//����
#define JDQ6_OFF  GPIOC->BRR  = 0x0200//�ͷ�
//����Դ������Ƽ̵���7 PC8
#define JDQ7_ON   GPIOC->BSRR = 0x0100//����
#define JDQ7_OFF  GPIOC->BRR  = 0x0100//�ͷ�
//����Դ������Ƽ̵���8 PC7
#define JDQ8_ON   GPIOC->BSRR = 0x0080//����
#define JDQ8_OFF  GPIOC->BRR  = 0x0080//�ͷ�
//��ѹ������Ƽ̵���1 PD9
#define AD_JDQ1_ON   GPIOD->BSRR = 0x0200//����
#define AD_JDQ1_OFF  GPIOD->BRR  = 0x0200//�ͷ�
//��ѹ������Ƽ̵���2 PD10
#define AD_JDQ2_ON   GPIOD->BSRR = 0x0400//����
#define AD_JDQ2_OFF  GPIOD->BRR  = 0x0400//�ͷ�
//��ѹ������Ƽ̵���3 PD11
#define AD_JDQ3_ON   GPIOD->BSRR = 0x0800//����
#define AD_JDQ3_OFF  GPIOD->BRR  = 0x0800//�ͷ�
//��ѹ������Ƽ̵���4 PD12
#define AD_JDQ4_ON   GPIOD->BSRR = 0x1000//����
#define AD_JDQ4_OFF  GPIOD->BRR  = 0x1000//�ͷ�

/*��������IO����**********************************************************************************/
//KESC����   PE9
#define KESC_STAT   (GPIOE->IDR & 0x0200)
//KENTER���� PE11
#define KENTER_STAT (GPIOE->IDR & 0x0800)
//KRIGHT���� PE14
#define KRIGHT_STAT (GPIOE->IDR & 0x4000)
//KLEFT����  PE12
#define KLEFT_STAT  (GPIOE->IDR & 0x1000)
//KDOWN����  PE10
#define KDOWN_STAT  (GPIOE->IDR & 0x0400)
//KUP����    PE13
#define KUP_STAT    (GPIOE->IDR & 0x2000)
/*��ת������IO����*******************************************************************************/
//A������  PB1
#define ENCODE_A   GPIOB->IDR & 0x0002
//B������  PE7
#define ENCODE_B   GPIOE->IDR & 0x0080
//�������� PE8
#define KEY_SET_IN GPIOE->IDR & 0x0100

/*����IO����**************************************************************************************/
//COOL-FAN�����ź� PD13
#define COOL_FAN_ON   GPIOD->BSRR = 0x2000//��������
#define COOL_FAN_OFF  GPIOD->BRR  = 0x2000//����ֹͣ

/*X9C103���ֵ�λ��IO����**************************************************************************/
//INC�����ź� PD15
#define X9C103_INC_H  GPIOD->BRR  = 0x8000//�ߵ�ƽ
#define X9C103_INC_L  GPIOD->BSRR = 0x8000//�͵�ƽ
//U/D�����ź� PC6
#define X9C103_UD_H   GPIOC->BRR  = 0x0040//�ߵ�ƽ
#define X9C103_UD_L   GPIOC->BSRR = 0x0040//�͵�ƽ

/*Һ��������IO����********************************************************************************/
//Ƭѡ�ź�CS       PA4
#define LCD_CS_H   GPIOA->BSRR = 0x0010//�ߵ�ƽ
#define LCD_CS_L   GPIOA->BRR  = 0x0010//�͵�ƽ
//��λ�ź�RST      PC4
#define LCD_RST_H  GPIOC->BSRR = 0x0010//�ߵ�ƽ
#define LCD_RST_L  GPIOC->BRR  = 0x0010//�͵�ƽ
//æ�ź�BUSY       PB0
#define LCD_BUSY_STAT (GPIOB->IDR  & 0x0001)//ȡBUSY�ŵ�ǰ״̬
//�ж��ź�INT      PC5
#define LCD_INT_STAT  (GPIOC->IDR  & 0x0020)//ȡINT�ŵ�ǰ״̬

/*����洢��IIC�ӿ�IO����*************************************************************************/
//FM_SCLʱ���� PC0
#define FM_SCL_H    GPIOC->BSRR = 0x0001//�ø�
#define FM_SCL_L    GPIOC->BRR  = 0x0001//�õ�
//FM_SDA������ PC1
#define FM_SDA_1    GPIOC->BSRR = 0x0002//�ø�
#define FM_SDA_0    GPIOC->BRR  = 0x0002//�õ�
#define FM_SDA_STAT (GPIOC->IDR  & 0x0002)//ȡSDA���ŵ�ǰ״̬ 

/*RTCʵʱʱ��IIC�ӿ�IO����************************************************************************/
//RTC_SDA������ PE1
#define RTC_SDA_1    GPIOE->BSRR = 0x0002//�ø�
#define RTC_SDA_0    GPIOE->BRR  = 0x0002//�õ�
#define RTC_SDA_STAT GPIOE->IDR  & 0x0002//ȡSDA���ŵ�ǰ״̬
//RTC_SCLʱ���� PE0
#define RTC_SCL_H    GPIOE->BSRR = 0x0001//�ø�
#define RTC_SCL_L    GPIOE->BRR  = 0x0001//�õ�
//RTC_IRQ������ PB9
#define RTC_IRQ_STAT (GPIOB->IDR  & 0x0200)//ȡIRQ���ŵ�ǰ״̬

/*����������IO����********************************************************************************/
//BELL�����ź� PE2
#define BELL_ON    GPIOE->BSRR = 0x0004//��
#define BELL_OFF   GPIOE->BRR  = 0x0004//��

/*��ӡ��æ�ź�IO����******************************************************************************/
//PRINT-BUSY�ź� PB7
#define PRINT_BUSY_STAT (GPIOB->IDR & 0x0080)//ȡPRINT-BUSY���ŵ�ǰ״̬

/*485ͨѶ���Ϳ���IO����***************************************************************************/
//���Ϳ����ź� PA1
#define RS485TXD_EN_ON    GPIOA->BRR  = 0x0002//����
#define RS485TXD_EN_OFF   GPIOA->BSRR = 0x0002//����

/*U�̿���оƬ�ź�IO����***************************************************************************/
//Ӧ���ź�ACT PD7
#define CH376_ACT_STAT     (GPIOD->IDR  & 0x0080)//ȡUSB-ACT���ŵ�ǰ״̬
//�ж��ź�INT PD4
#define CH376_INT_STAT     (GPIOD->IDR  & 0x0010)//ȡUSB-INT���ŵ�ǰ״̬
//æ�ź�BUSY  PD5
#define CH376_BUSY_STAT    (GPIOD->IDR  & 0x0020)//ȡUSB-BUSY���ŵ�ǰ״̬
//Ƭѡ�ź�CS  PD6
#define CH376_CS_H         GPIOD->BSRR = 0x0040//�ߵ�ƽ
#define CH376_CS_L         GPIOD->BRR  = 0x0040//�͵�ƽ
//SPI-MISO����״̬ PB4
#define CH376_SPIMISO_STAT (GPIOB->IDR  & 0x0010)//ȡUSB-SPIMISO���ŵ�ǰ״̬

/*AD����оƬ�ӿ�IO����****************************************************************************/
//æ�ź�BUSY     PD8
#define AD_BUSY_STAT   (GPIOD->IDR  & 0x0100)//ȡAD-BUSY���ŵ�ǰ״̬
//��λ�ź�RESET  PB15
#define AD_RESET_H     GPIOB->BSRR = 0x8000//�ߵ�ƽ
#define AD_RESET_L     GPIOB->BRR  = 0x8000//�͵�ƽ
//Ƭѡ�ź�CS     PB12
#define AD_CS_H        GPIOB->BSRR = 0x1000//�ߵ�ƽ
#define AD_CS_L        GPIOB->BRR  = 0x1000//�͵�ƽ

/*����ģ����ƽӿ�IO����**************************************************************************/
//����ģʽ�����ź�MODE  PC12
#define BT_MODE_H     GPIOC->BSRR = 0x1000//�ߵ�ƽ
#define BT_MODE_L     GPIOC->BRR  = 0x1000//�͵�ƽ
//����״̬�ź�PIO9      PA15
#define BT_LINK_STAT  (GPIOA->IDR  & 0x8000)//ȡPIO9���ŵ�ǰ״̬



#endif