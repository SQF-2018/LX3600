#include "stm32f10x.h"
#include "QJBL.H"
#include "IOCONFIG.H"
#include "RTC.H"
#include "stdio.h"

/********************************************************************************************************************************
HC-05����ת����ģ����Ƽ�ͨѶ��������
ʹ��UART4����,TTL232�ӿ�
********************************************************************************************************************************/

/********************************************************************************************************************************
����ģ��ʹ�õ�UART�ڳ�ʼ������
********************************************************************************************************************************/
void BlueTooth_UART_Configuration(unsigned int baudrate)
{
 USART_InitTypeDef USART_InitStructure;
 USART_InitStructure.USART_BaudRate            = baudrate;           //����ͨѶ����
 USART_InitStructure.USART_WordLength          = USART_WordLength_8b;//8λ����λ
 USART_InitStructure.USART_StopBits            = USART_StopBits_1;   //1λֹͣλ
 USART_InitStructure.USART_Parity              = USART_Parity_No ;   //����żУ��
 USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��ֹӲ��������
 USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx; //���͡�����˫����ģʽ
 USART_Init(UART4, &USART_InitStructure);
 USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//ʹ�ܽ����ж�
 USART_Cmd(UART4, ENABLE);                    //ʹ��UART4
 BLUETOOTH_RXNO  = 0;
 BLUETOOTH_TXNO  = 0;
 BLUETOOTH_TXEND = 0;    
}

/********************************************************************************************************************************
UART4�ж��ӳ���
********************************************************************************************************************************/
void USART2_IRQHandler(void) 
{
 //���ռĴ����ǿ���Ϊ�����ж�
 if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
  {     
   if(BLUETOOTH_RXNO >= 50) BLUETOOTH_RXNO = 0;
   BLUETOOTH_RXBUF[BLUETOOTH_RXNO++] = UART4->DR;
   BLUETOOTH_RXTIME                  = 0;  //��ֹʱ���ʱ������
   BLUETOOTH_RXTIME_END              = NO; //��ֹʱ�䵽ʱ��־����
   BLUETOOTH_RXTIME_EN               = YES;//������ֹʱ���ʱ
  }
 //���ͼĴ������жϣ����ͼĴ����������Ѿ�ת�Ƶ�������λ�Ĵ����У�
 if(USART_GetITStatus(UART4, USART_IT_TXE) != RESET)
  {
   if(BLUETOOTH_TXNO <= BLUETOOTH_TXEND)
    {
     UART4->DR = BLUETOOTH_TXBUF[BLUETOOTH_TXNO];//��������ͨѶ;
     if(BLUETOOTH_TXNO == BLUETOOTH_TXEND)
      {
       USART_ITConfig(UART4, USART_IT_TXE, DISABLE);//��ֹ���ͻ��������ж�
       USART_ClearFlag(UART4, USART_FLAG_TC);       //���TC��־���ȴ���λ�Ĵ����պ�Ӳ����λ
       USART_ITConfig(UART4, USART_IT_TC, ENABLE);  //ʹ�ܷ�����λ�Ĵ������ж�
      }
     BLUETOOTH_TXNO += 1;
    }
  }
 //������λ�Ĵ������жϣ�������Ӳ����ȫ��������ɣ�
 if(USART_GetITStatus(UART4, USART_IT_TC) != RESET)
  {
   USART_ITConfig(UART4, USART_IT_TC, DISABLE); //��ֹ������λ�Ĵ������ж�
   BLUETOOTH_TXNO  = 0;
   BLUETOOTH_TXEND = 0;
  }  
}

/********************************************************************************************************************************
����ģ�鴮��ATָ����ӳ���
********************************************************************************************************************************/
void Bluetooth_ATcommand_Send(char *command)
{
 BLUETOOTH_TXEND = 0;
 while(*command > 0)//�ж��ַ�����β��־0x00
  {
   BLUETOOTH_TXBUF[BLUETOOTH_TXEND++] = *command++;
  }
 BLUETOOTH_TXEND -= 1;
 BLUETOOTH_TXNO = 1; 
 USART2->DR = BLUETOOTH_TXBUF[0];             //��������ͨѶ
 USART_ITConfig(USART2, USART_IT_TXE, ENABLE);//ʹ�ܷ��ͻ��������ж�
 BLUETOOTH_RXNO = 0;                          //����������ݼ�����
}

/********************************************************************************************************************************
����ģ�鴮��ATָ����ӳ���
����ģ���ڽ��յ�ָ��󣬴����27.5ms�󷵻�����
********************************************************************************************************************************/
void Bluetooth_Model_Init(void)
{
 unsigned int i;
 char buf[25];
 Delay_ms(500); //��ʱ500ms���ȴ�����ģ���ϵ����(����ģ���ϵ�ʱ��Ƚϳ������������KEY����Ϊ�ߵ�ƽ������HC05ģ���ϵ�ǰKEY���ž��Ǹߵ�ƽ������ͨѶ����38400��ATģʽ)
 BT_MODE_H;     //����ģ�����ATģʽ
 Delay_ms(50);  //��ʱ50ms���ȴ�����ģ����ȫ����ATģʽ
 Bluetooth_ATcommand_Send("AT\r\n");
 while(BLUETOOTH_TXEND != 0);//�ȴ��������
 //�ȴ��������ݽ������,50ms��û�н��յ��������ж�Ϊ����ģ�����
 for(i=0;i<155000;i++) if(BLUETOOTH_RXNO >= 4 && BLUETOOTH_RXTIME_END == YES) break;//���վ�ֹʱ�䵽ʱ���ҽ������ݳ��ȴ��ڵ���4��
 BLUETOOTH_RXTIME_END = NO;//��ʼ�����վ�ֹʱ��
 //���յ���ȷ�ķ�����Ϣ��OK\r\n��
 if(BLUETOOTH_RXBUF[0] == 'O'  && 
    BLUETOOTH_RXBUF[1] == 'K'  && 
    BLUETOOTH_RXBUF[2] == '\r' && 
    BLUETOOTH_RXBUF[3] == '\n')
  {
   Bluetooth_ATcommand_Send("AT+NAME?\r\n");//��ѯ����ģ������
   while(BLUETOOTH_TXEND != 0);             //�ȴ��������
   while(BLUETOOTH_RXNO < 6 || BLUETOOTH_RXTIME_END == NO);//���վ�ֹʱ�䵽ʱ���ҽ������ݳ��ȴ��ڵ���6��
   BLUETOOTH_RXTIME_END = NO;//��ʼ�����վ�ֹʱ��
   //�����ж�����֡��β�ؼ����Ƿ���ȷ
   if(BLUETOOTH_RXBUF[0]  == '+' && 
      BLUETOOTH_RXBUF[1]  == 'N' && 
      BLUETOOTH_RXBUF[2]  == 'A' && 
      BLUETOOTH_RXBUF[3]  == 'M' && 
      BLUETOOTH_RXBUF[4]  == 'E' && 
      BLUETOOTH_RXBUF[5]  == ':' &&
      BLUETOOTH_RXBUF[BLUETOOTH_RXNO - 6] == '\r' &&
      BLUETOOTH_RXBUF[BLUETOOTH_RXNO - 5] == '\n' &&
      BLUETOOTH_RXBUF[BLUETOOTH_RXNO - 4] == 'O'  &&
      BLUETOOTH_RXBUF[BLUETOOTH_RXNO - 3] == 'K'  &&
      BLUETOOTH_RXBUF[BLUETOOTH_RXNO - 2] == '\r' &&
      BLUETOOTH_RXBUF[BLUETOOTH_RXNO - 1] == '\n')
    {
#if FACTORY == LIXING //���˵���
     if(BLUETOOTH_RXBUF[6]  != 'L' ||
        BLUETOOTH_RXBUF[7]  != 'X' ||     
        BLUETOOTH_RXBUF[8]  != '3' || 
        BLUETOOTH_RXBUF[9]  != '6' || 
        BLUETOOTH_RXBUF[10] != '0' || 
        BLUETOOTH_RXBUF[11] != '0' || 
        BLUETOOTH_RXBUF[12] != '-' || 
        BLUETOOTH_RXBUF[13] != (DEVICE_NUMBER[0]+0x30) || 
        BLUETOOTH_RXBUF[14] != (DEVICE_NUMBER[1]+0x30) ||
        BLUETOOTH_RXBUF[15] != (DEVICE_NUMBER[2]+0x30) ||
        BLUETOOTH_RXBUF[16] != (DEVICE_NUMBER[3]+0x30))//�������ģ�����Ʋ��������ͺ�+װ�ñ�ţ�������д��
#elif FACTORY == AGENT //������
     if(BLUETOOTH_RXBUF[6]  != 'G' || 
        BLUETOOTH_RXBUF[7]  != 'Y' || 
        BLUETOOTH_RXBUF[8]  != 'X' || 
        BLUETOOTH_RXBUF[9]  != 'C' ||        
        BLUETOOTH_RXBUF[10] != '-' || 
        BLUETOOTH_RXBUF[11] != (DEVICE_NUMBER[0]+0x30) || 
        BLUETOOTH_RXBUF[12] != (DEVICE_NUMBER[1]+0x30) ||
        BLUETOOTH_RXBUF[13] != (DEVICE_NUMBER[2]+0x30) ||
        BLUETOOTH_RXBUF[14] != (DEVICE_NUMBER[3]+0x30))//�������ģ�����Ʋ��������ͺ�+װ�ñ�ţ�������д��
#endif
      {
#if FACTORY == LIXING //���˵���
       //��װ�ñ�Ų��뵽"AT+NAME=LX3600-"����
       sprintf(buf,"AT+NAME=LX3600-%d%d%d%d\r\n",DEVICE_NUMBER[0],DEVICE_NUMBER[1],DEVICE_NUMBER[2],DEVICE_NUMBER[3]);
#elif FACTORY == AGENT //������
       //��װ�ñ�Ų��뵽"AT+NAME=GYXC-"����
       sprintf(buf,"AT+NAME=GYXC-%d%d%d%d\r\n",DEVICE_NUMBER[0],DEVICE_NUMBER[1],DEVICE_NUMBER[2],DEVICE_NUMBER[3]);
#endif  
       Bluetooth_ATcommand_Send(buf);//��������ģ������
       while(BLUETOOTH_TXEND != 0);  //�ȴ��������
       while(BLUETOOTH_RXNO < 4 || BLUETOOTH_RXTIME_END == NO);//���վ�ֹʱ�䵽ʱ���ҽ������ݳ��ȴ��ڵ���4��
       BLUETOOTH_RXTIME_END = NO;//��ʼ�����վ�ֹʱ��
       //���յ���ȷ�ķ�����Ϣ��OK\r\n��
       if(BLUETOOTH_RXBUF[0] == 'O'  && 
          BLUETOOTH_RXBUF[1] == 'K'  && 
          BLUETOOTH_RXBUF[2] == '\r' && 
          BLUETOOTH_RXBUF[3] == '\n')
        {
         BT_MODE_L;                 //����ģ�������������ģʽ
         BLUETOOTH_MODLE_INIT = YES;//����ģ���ʼ���ɹ�
         //USART2->CR1 &= 0xffffffdb;//��ֹ�������ݣ�����ֹ���ջ������ǿ��ж�
        }
#if BLUETOOTH == ON //�����������ϴ����ܣ���������ģ��Ӳ������
       else Set_Bit(HARDWARE_EEOR_FLAG,BLUETOOTH_ERR);//����ģ��Ӳ������
#endif
      }
     else
      {
       BT_MODE_L;                 //����ģ�������������ģʽ
       BLUETOOTH_MODLE_INIT = YES;//����ģ���ʼ���ɹ�
       //USART2->CR1 &= 0xffffffdb;//��ֹ�������ݣ�����ֹ���ջ������ǿ��ж�
      }
    }
#if BLUETOOTH == ON //�����������ϴ����ܣ���������ģ��Ӳ������
   else Set_Bit(HARDWARE_EEOR_FLAG,BLUETOOTH_ERR);//����ģ��Ӳ������
#endif
  }
#if BLUETOOTH == ON //�����������ϴ����ܣ���������ģ��Ӳ������
 else Set_Bit(HARDWARE_EEOR_FLAG,BLUETOOTH_ERR);//����ģ��Ӳ������
#endif
 BLUETOOTH_RXNO       = 0; //��ʼ������ָ���Ա������½�������
 BLUETOOTH_RXTIME_END = NO;//�����ֹʱ�䵽ʱ��־���ڴ˱�־�ٴ���λǰ�����ٽ��й�Լ����
}

/************************************************************************************************************/
//�����ϴ�������¼��������
/************************************************************************************************************/
void Bluetooth_Send_Test_Record(void)
{
 
}

/************************************************************************************************************/
//����ģ�����ݽ��մ�������
/************************************************************************************************************/
void Bluetooth_Model_Data_Receive(void)
{
 unsigned int status_tmp;
 status_tmp = UART4->SR;//��ȡUART4״̬�Ĵ���
 if(status_tmp & 0x0f)  //�����ش���OREλ����������NEλ��֡����FEλ��У�����PEλ�Ƿ���λ
  {
   //���ֲ����˴���ִ������������������־λ
   status_tmp = UART4->SR;//��ȡUART4״̬�Ĵ���
   status_tmp = UART4->DR;//��ȡUART4���ݼĴ���
  }
 //���վ�ֹʱ�䳬����4�ֽ�ʱ�䣬˵��һ֡���ݽ�����ϣ���ʼ���ݷ���
 if(BLUETOOTH_RXTIME_END == YES)
  {
   //����֡��С���ȴ��ڵ���4������ʼ���ݴ���
   if(BLUETOOTH_RXNO >= 4)
    {
     //���յ���ϸ������ʾ����ָ��
     if(BLUETOOTH_RXBUF[0]  == 'd' || BLUETOOTH_RXBUF[1]  == 'i' || BLUETOOTH_RXBUF[2]  == 's' || BLUETOOTH_RXBUF[3]  == 'p' || 
        BLUETOOTH_RXBUF[4]  == 'l' || BLUETOOTH_RXBUF[5]  == 'a' || BLUETOOTH_RXBUF[6]  == 'y' || BLUETOOTH_RXBUF[7]  == '3' || 
        BLUETOOTH_RXBUF[8]  == '2' || BLUETOOTH_RXBUF[9]  == '8' || BLUETOOTH_RXBUF[10] == '8' || BLUETOOTH_RXBUF[11] == '1' || 
        BLUETOOTH_RXBUF[12] == '1' || BLUETOOTH_RXBUF[13] == '7')
      {
       if(PARAMETER_DISPLAY_EN == NO)
        {
         PARAMETER_DISPLAY_EN = YES;//����ϸ������ʾ
         if(SCRFL == 2 || SCRFL == 3) 
          {
            
          }
        }
       else
        {
         PARAMETER_DISPLAY_EN = NO;//�ر���ϸ������ʾ
         if(TEST_PROCESS_EN == NO && POPUP_SCRFL == 2) POPUP_SCRFL = 0;//�رղ���������ʾ��Ļ
        }
       SCRAPPLY = YES;//���󴰿�ˢ��
      }     
    }
   BLUETOOTH_RXNO       = 0; //��ʼ������ָ���Ա������½�������
   BLUETOOTH_RXTIME_END = NO;//�����ֹʱ�䵽ʱ��־���ڴ˱�־�ٴ���λǰ�����ٽ��й�Լ����
  }
}