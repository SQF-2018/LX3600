#include "stm32f10x.h"
#include "QJBL.H"
#include "IOCONFIG.H"
#include "RTC.H"
#include "stdio.h"

/********************************************************************************************************************************
HC-05蓝牙转串口模块控制及通讯处理程序
使用UART4串口,TTL232接口
********************************************************************************************************************************/

/********************************************************************************************************************************
蓝牙模块使用的UART口初始化程序
********************************************************************************************************************************/
void BlueTooth_UART_Configuration(unsigned int baudrate)
{
 USART_InitTypeDef USART_InitStructure;
 USART_InitStructure.USART_BaudRate            = baudrate;           //设置通讯速率
 USART_InitStructure.USART_WordLength          = USART_WordLength_8b;//8位数据位
 USART_InitStructure.USART_StopBits            = USART_StopBits_1;   //1位停止位
 USART_InitStructure.USART_Parity              = USART_Parity_No ;   //无奇偶校验
 USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//禁止硬件流控制
 USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx; //发送、接收双向传输模式
 USART_Init(UART4, &USART_InitStructure);
 USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//使能接收中断
 USART_Cmd(UART4, ENABLE);                    //使能UART4
 BLUETOOTH_RXNO  = 0;
 BLUETOOTH_TXNO  = 0;
 BLUETOOTH_TXEND = 0;    
}

/********************************************************************************************************************************
UART4中断子程序
********************************************************************************************************************************/
void USART2_IRQHandler(void) 
{
 //接收寄存器非空则为接收中断
 if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
  {     
   if(BLUETOOTH_RXNO >= 50) BLUETOOTH_RXNO = 0;
   BLUETOOTH_RXBUF[BLUETOOTH_RXNO++] = UART4->DR;
   BLUETOOTH_RXTIME                  = 0;  //静止时间计时器清零
   BLUETOOTH_RXTIME_END              = NO; //静止时间到时标志清零
   BLUETOOTH_RXTIME_EN               = YES;//允许静止时间计时
  }
 //发送寄存器空中断（发送寄存器的数据已经转移到发送移位寄存器中）
 if(USART_GetITStatus(UART4, USART_IT_TXE) != RESET)
  {
   if(BLUETOOTH_TXNO <= BLUETOOTH_TXEND)
    {
     UART4->DR = BLUETOOTH_TXBUF[BLUETOOTH_TXNO];//启动串行通讯;
     if(BLUETOOTH_TXNO == BLUETOOTH_TXEND)
      {
       USART_ITConfig(UART4, USART_IT_TXE, DISABLE);//禁止发送缓冲器空中断
       USART_ClearFlag(UART4, USART_FLAG_TC);       //清除TC标志，等待移位寄存器空后硬件置位
       USART_ITConfig(UART4, USART_IT_TC, ENABLE);  //使能发送移位寄存器空中断
      }
     BLUETOOTH_TXNO += 1;
    }
  }
 //发送移位寄存器空中断（数据在硬件中全部发送完成）
 if(USART_GetITStatus(UART4, USART_IT_TC) != RESET)
  {
   USART_ITConfig(UART4, USART_IT_TC, DISABLE); //禁止发送移位寄存器空中断
   BLUETOOTH_TXNO  = 0;
   BLUETOOTH_TXEND = 0;
  }  
}

/********************************************************************************************************************************
蓝牙模块串口AT指令发送子程序
********************************************************************************************************************************/
void Bluetooth_ATcommand_Send(char *command)
{
 BLUETOOTH_TXEND = 0;
 while(*command > 0)//判断字符串结尾标志0x00
  {
   BLUETOOTH_TXBUF[BLUETOOTH_TXEND++] = *command++;
  }
 BLUETOOTH_TXEND -= 1;
 BLUETOOTH_TXNO = 1; 
 USART2->DR = BLUETOOTH_TXBUF[0];             //启动串行通讯
 USART_ITConfig(USART2, USART_IT_TXE, ENABLE);//使能发送缓冲器空中断
 BLUETOOTH_RXNO = 0;                          //清零接收数据计数器
}

/********************************************************************************************************************************
蓝牙模块串口AT指令发送子程序
蓝牙模块在接收到指令后，大概在27.5ms后返回数据
********************************************************************************************************************************/
void Bluetooth_Model_Init(void)
{
 unsigned int i;
 char buf[25];
 Delay_ms(500); //延时500ms，等待蓝牙模块上电完成(蓝牙模块上电时间比较长，如果过早置KEY引脚为高电平将导致HC05模块上电前KEY引脚就是高电平而进入通讯速率38400的AT模式)
 BT_MODE_H;     //蓝牙模块进入AT模式
 Delay_ms(50);  //延时50ms，等待蓝牙模块完全进入AT模式
 Bluetooth_ATcommand_Send("AT\r\n");
 while(BLUETOOTH_TXEND != 0);//等待发送完成
 //等待返回数据接收完成,50ms内没有接收到数据则判断为蓝牙模块故障
 for(i=0;i<155000;i++) if(BLUETOOTH_RXNO >= 4 && BLUETOOTH_RXTIME_END == YES) break;//接收静止时间到时，且接收数据长度大于等于4个
 BLUETOOTH_RXTIME_END = NO;//初始化接收静止时间
 //接收到正确的返回信息“OK\r\n”
 if(BLUETOOTH_RXBUF[0] == 'O'  && 
    BLUETOOTH_RXBUF[1] == 'K'  && 
    BLUETOOTH_RXBUF[2] == '\r' && 
    BLUETOOTH_RXBUF[3] == '\n')
  {
   Bluetooth_ATcommand_Send("AT+NAME?\r\n");//查询蓝牙模块名称
   while(BLUETOOTH_TXEND != 0);             //等待发送完成
   while(BLUETOOTH_RXNO < 6 || BLUETOOTH_RXTIME_END == NO);//接收静止时间到时，且接收数据长度大于等于6个
   BLUETOOTH_RXTIME_END = NO;//初始化接收静止时间
   //首先判断数据帧首尾关键字是否正确
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
#if FACTORY == LIXING //力兴电子
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
        BLUETOOTH_RXBUF[16] != (DEVICE_NUMBER[3]+0x30))//如果蓝牙模块名称不是仪器型号+装置编号，则重新写入
#elif FACTORY == AGENT //代理商
     if(BLUETOOTH_RXBUF[6]  != 'G' || 
        BLUETOOTH_RXBUF[7]  != 'Y' || 
        BLUETOOTH_RXBUF[8]  != 'X' || 
        BLUETOOTH_RXBUF[9]  != 'C' ||        
        BLUETOOTH_RXBUF[10] != '-' || 
        BLUETOOTH_RXBUF[11] != (DEVICE_NUMBER[0]+0x30) || 
        BLUETOOTH_RXBUF[12] != (DEVICE_NUMBER[1]+0x30) ||
        BLUETOOTH_RXBUF[13] != (DEVICE_NUMBER[2]+0x30) ||
        BLUETOOTH_RXBUF[14] != (DEVICE_NUMBER[3]+0x30))//如果蓝牙模块名称不是仪器型号+装置编号，则重新写入
#endif
      {
#if FACTORY == LIXING //力兴电子
       //将装置编号插入到"AT+NAME=LX3600-"后面
       sprintf(buf,"AT+NAME=LX3600-%d%d%d%d\r\n",DEVICE_NUMBER[0],DEVICE_NUMBER[1],DEVICE_NUMBER[2],DEVICE_NUMBER[3]);
#elif FACTORY == AGENT //代理商
       //将装置编号插入到"AT+NAME=GYXC-"后面
       sprintf(buf,"AT+NAME=GYXC-%d%d%d%d\r\n",DEVICE_NUMBER[0],DEVICE_NUMBER[1],DEVICE_NUMBER[2],DEVICE_NUMBER[3]);
#endif  
       Bluetooth_ATcommand_Send(buf);//设置蓝牙模块名称
       while(BLUETOOTH_TXEND != 0);  //等待发送完成
       while(BLUETOOTH_RXNO < 4 || BLUETOOTH_RXTIME_END == NO);//接收静止时间到时，且接收数据长度大于等于4个
       BLUETOOTH_RXTIME_END = NO;//初始化接收静止时间
       //接收到正确的返回信息“OK\r\n”
       if(BLUETOOTH_RXBUF[0] == 'O'  && 
          BLUETOOTH_RXBUF[1] == 'K'  && 
          BLUETOOTH_RXBUF[2] == '\r' && 
          BLUETOOTH_RXBUF[3] == '\n')
        {
         BT_MODE_L;                 //蓝牙模块进入正常工作模式
         BLUETOOTH_MODLE_INIT = YES;//蓝牙模块初始化成功
         //USART2->CR1 &= 0xffffffdb;//禁止接收数据，并禁止接收缓冲区非空中断
        }
#if BLUETOOTH == ON //开启了蓝牙上传功能，则检测蓝牙模块硬件错误
       else Set_Bit(HARDWARE_EEOR_FLAG,BLUETOOTH_ERR);//蓝牙模块硬件错误
#endif
      }
     else
      {
       BT_MODE_L;                 //蓝牙模块进入正常工作模式
       BLUETOOTH_MODLE_INIT = YES;//蓝牙模块初始化成功
       //USART2->CR1 &= 0xffffffdb;//禁止接收数据，并禁止接收缓冲区非空中断
      }
    }
#if BLUETOOTH == ON //开启了蓝牙上传功能，则检测蓝牙模块硬件错误
   else Set_Bit(HARDWARE_EEOR_FLAG,BLUETOOTH_ERR);//蓝牙模块硬件错误
#endif
  }
#if BLUETOOTH == ON //开启了蓝牙上传功能，则检测蓝牙模块硬件错误
 else Set_Bit(HARDWARE_EEOR_FLAG,BLUETOOTH_ERR);//蓝牙模块硬件错误
#endif
 BLUETOOTH_RXNO       = 0; //初始化接收指针以便于重新接收数据
 BLUETOOTH_RXTIME_END = NO;//清除静止时间到时标志，在此标志再次置位前，不再进行规约处理
}

/************************************************************************************************************/
//蓝牙上传测量记录处理程序
/************************************************************************************************************/
void Bluetooth_Send_Test_Record(void)
{
 
}

/************************************************************************************************************/
//蓝牙模块数据接收处理程序
/************************************************************************************************************/
void Bluetooth_Model_Data_Receive(void)
{
 unsigned int status_tmp;
 status_tmp = UART4->SR;//读取UART4状态寄存器
 if(status_tmp & 0x0f)  //检测过载错误ORE位、噪声错误NE位、帧错误FE位、校验错误PE位是否被置位
  {
   //发现产生了错误，执行软件序列清零错误标志位
   status_tmp = UART4->SR;//读取UART4状态寄存器
   status_tmp = UART4->DR;//读取UART4数据寄存器
  }
 //接收静止时间超过了4字节时间，说明一帧数据接收完毕，开始数据分析
 if(BLUETOOTH_RXTIME_END == YES)
  {
   //数据帧最小长度大于等于4个，开始数据处理
   if(BLUETOOTH_RXNO >= 4)
    {
     //接收到详细参数显示控制指令
     if(BLUETOOTH_RXBUF[0]  == 'd' || BLUETOOTH_RXBUF[1]  == 'i' || BLUETOOTH_RXBUF[2]  == 's' || BLUETOOTH_RXBUF[3]  == 'p' || 
        BLUETOOTH_RXBUF[4]  == 'l' || BLUETOOTH_RXBUF[5]  == 'a' || BLUETOOTH_RXBUF[6]  == 'y' || BLUETOOTH_RXBUF[7]  == '3' || 
        BLUETOOTH_RXBUF[8]  == '2' || BLUETOOTH_RXBUF[9]  == '8' || BLUETOOTH_RXBUF[10] == '8' || BLUETOOTH_RXBUF[11] == '1' || 
        BLUETOOTH_RXBUF[12] == '1' || BLUETOOTH_RXBUF[13] == '7')
      {
       if(PARAMETER_DISPLAY_EN == NO)
        {
         PARAMETER_DISPLAY_EN = YES;//打开详细参数显示
         if(SCRFL == 2 || SCRFL == 3) 
          {
            
          }
        }
       else
        {
         PARAMETER_DISPLAY_EN = NO;//关闭详细参数显示
         if(TEST_PROCESS_EN == NO && POPUP_SCRFL == 2) POPUP_SCRFL = 0;//关闭测量过程显示屏幕
        }
       SCRAPPLY = YES;//请求窗口刷新
      }     
    }
   BLUETOOTH_RXNO       = 0; //初始化接收指针以便于重新接收数据
   BLUETOOTH_RXTIME_END = NO;//清除静止时间到时标志，在此标志再次置位前，不再进行规约处理
  }
}
