#ifndef __IOFONFIG_H
#define __IOFONFIG_H

#include "stm32f10x_gpio.h"
/*指示灯IO定义************************************************************************************/
//运行指示灯 PE5
#define RUNLED_ON    GPIOE->BRR  = 0x0020//亮
#define RUNLED_OFF   GPIOE->BSRR = 0x0020//灭

/*继电器控制IO定义********************************************************************************/
//交流电源输入控制继电器 PE4
#define JDQ_POWER_CTRL_ON   GPIOE->BSRR = 0x0010//吸合
#define JDQ_POWER_CTRL_OFF  GPIOE->BRR  = 0x0010//释放
//主控制继电器 PD14
#define JDQ_MASTER_CTRL_ON  GPIOD->BSRR = 0x4000//吸合,功率电源输出
#define JDQ_MASTER_CTRL_OFF GPIOD->BRR  = 0x4000//释放,功率电源断开
//恒流源输出控制继电器1 PA12
#define JDQ1_ON   GPIOA->BSRR = 0x1000//吸合
#define JDQ1_OFF  GPIOA->BRR  = 0x1000//释放
//恒流源输出控制继电器2 PA11
#define JDQ2_ON   GPIOA->BSRR = 0x0800//吸合
#define JDQ2_OFF  GPIOA->BRR  = 0x0800//释放
//恒流源输出控制继电器3 PA10
#define JDQ3_ON   GPIOA->BSRR = 0x0400//吸合
#define JDQ3_OFF  GPIOA->BRR  = 0x0400//释放
//恒流源输出控制继电器4 PA9
#define JDQ4_ON   GPIOA->BSRR = 0x0200//吸合
#define JDQ4_OFF  GPIOA->BRR  = 0x0200//释放
//恒流源输出控制继电器5 PA8
#define JDQ5_ON   GPIOA->BSRR = 0x0100//吸合
#define JDQ5_OFF  GPIOA->BRR  = 0x0100//释放
//恒流源输出控制继电器6 PC9
#define JDQ6_ON   GPIOC->BSRR = 0x0200//吸合
#define JDQ6_OFF  GPIOC->BRR  = 0x0200//释放
//恒流源输出控制继电器7 PC8
#define JDQ7_ON   GPIOC->BSRR = 0x0100//吸合
#define JDQ7_OFF  GPIOC->BRR  = 0x0100//释放
//恒流源输出控制继电器8 PC7
#define JDQ8_ON   GPIOC->BSRR = 0x0080//吸合
#define JDQ8_OFF  GPIOC->BRR  = 0x0080//释放
//电压输入控制继电器1 PD9
#define AD_JDQ1_ON   GPIOD->BSRR = 0x0200//吸合
#define AD_JDQ1_OFF  GPIOD->BRR  = 0x0200//释放
//电压输入控制继电器2 PD10
#define AD_JDQ2_ON   GPIOD->BSRR = 0x0400//吸合
#define AD_JDQ2_OFF  GPIOD->BRR  = 0x0400//释放
//电压输入控制继电器3 PD11
#define AD_JDQ3_ON   GPIOD->BSRR = 0x0800//吸合
#define AD_JDQ3_OFF  GPIOD->BRR  = 0x0800//释放
//电压输入控制继电器4 PD12
#define AD_JDQ4_ON   GPIOD->BSRR = 0x1000//吸合
#define AD_JDQ4_OFF  GPIOD->BRR  = 0x1000//释放

/*按键输入IO定义**********************************************************************************/
//KESC按键   PE9
#define KESC_STAT   (GPIOE->IDR & 0x0200)
//KENTER按键 PE11
#define KENTER_STAT (GPIOE->IDR & 0x0800)
//KRIGHT按键 PE14
#define KRIGHT_STAT (GPIOE->IDR & 0x4000)
//KLEFT按键  PE12
#define KLEFT_STAT  (GPIOE->IDR & 0x1000)
//KDOWN按键  PE10
#define KDOWN_STAT  (GPIOE->IDR & 0x0400)
//KUP按键    PE13
#define KUP_STAT    (GPIOE->IDR & 0x2000)
/*旋转编码器IO定义*******************************************************************************/
//A相输入  PB1
#define ENCODE_A   GPIOB->IDR & 0x0002
//B相输入  PE7
#define ENCODE_B   GPIOE->IDR & 0x0080
//按键输入 PE8
#define KEY_SET_IN GPIOE->IDR & 0x0100

/*风扇IO定义**************************************************************************************/
//COOL-FAN控制信号 PD13
#define COOL_FAN_ON   GPIOD->BSRR = 0x2000//风扇启动
#define COOL_FAN_OFF  GPIOD->BRR  = 0x2000//风扇停止

/*X9C103数字电位器IO定义**************************************************************************/
//INC控制信号 PD15
#define X9C103_INC_H  GPIOD->BRR  = 0x8000//高电平
#define X9C103_INC_L  GPIOD->BSRR = 0x8000//低电平
//U/D控制信号 PC6
#define X9C103_UD_H   GPIOC->BRR  = 0x0040//高电平
#define X9C103_UD_L   GPIOC->BSRR = 0x0040//低电平

/*液晶屏控制IO定义********************************************************************************/
//片选信号CS       PA4
#define LCD_CS_H   GPIOA->BSRR = 0x0010//高电平
#define LCD_CS_L   GPIOA->BRR  = 0x0010//低电平
//复位信号RST      PC4
#define LCD_RST_H  GPIOC->BSRR = 0x0010//高电平
#define LCD_RST_L  GPIOC->BRR  = 0x0010//低电平
//忙信号BUSY       PB0
#define LCD_BUSY_STAT (GPIOB->IDR  & 0x0001)//取BUSY脚当前状态
//中断信号INT      PC5
#define LCD_INT_STAT  (GPIOC->IDR  & 0x0020)//取INT脚当前状态

/*铁电存储器IIC接口IO定义*************************************************************************/
//FM_SCL时钟线 PC0
#define FM_SCL_H    GPIOC->BSRR = 0x0001//置高
#define FM_SCL_L    GPIOC->BRR  = 0x0001//置低
//FM_SDA数据线 PC1
#define FM_SDA_1    GPIOC->BSRR = 0x0002//置高
#define FM_SDA_0    GPIOC->BRR  = 0x0002//置低
#define FM_SDA_STAT (GPIOC->IDR  & 0x0002)//取SDA引脚当前状态 

/*RTC实时时钟IIC接口IO定义************************************************************************/
//RTC_SDA数据线 PE1
#define RTC_SDA_1    GPIOE->BSRR = 0x0002//置高
#define RTC_SDA_0    GPIOE->BRR  = 0x0002//置低
#define RTC_SDA_STAT GPIOE->IDR  & 0x0002//取SDA引脚当前状态
//RTC_SCL时钟线 PE0
#define RTC_SCL_H    GPIOE->BSRR = 0x0001//置高
#define RTC_SCL_L    GPIOE->BRR  = 0x0001//置低
//RTC_IRQ数据线 PB9
#define RTC_IRQ_STAT (GPIOB->IDR  & 0x0200)//取IRQ引脚当前状态

/*蜂鸣器控制IO定义********************************************************************************/
//BELL控制信号 PE2
#define BELL_ON    GPIOE->BSRR = 0x0004//开
#define BELL_OFF   GPIOE->BRR  = 0x0004//关

/*打印机忙信号IO定义******************************************************************************/
//PRINT-BUSY信号 PB7
#define PRINT_BUSY_STAT (GPIOB->IDR & 0x0080)//取PRINT-BUSY引脚当前状态

/*485通讯发送控制IO定义***************************************************************************/
//发送控制信号 PA1
#define RS485TXD_EN_ON    GPIOA->BRR  = 0x0002//发送
#define RS485TXD_EN_OFF   GPIOA->BSRR = 0x0002//接收

/*U盘控制芯片信号IO定义***************************************************************************/
//应答信号ACT PD7
#define CH376_ACT_STAT     (GPIOD->IDR  & 0x0080)//取USB-ACT引脚当前状态
//中断信号INT PD4
#define CH376_INT_STAT     (GPIOD->IDR  & 0x0010)//取USB-INT引脚当前状态
//忙信号BUSY  PD5
#define CH376_BUSY_STAT    (GPIOD->IDR  & 0x0020)//取USB-BUSY引脚当前状态
//片选信号CS  PD6
#define CH376_CS_H         GPIOD->BSRR = 0x0040//高电平
#define CH376_CS_L         GPIOD->BRR  = 0x0040//低电平
//SPI-MISO引脚状态 PB4
#define CH376_SPIMISO_STAT (GPIOB->IDR  & 0x0010)//取USB-SPIMISO引脚当前状态

/*AD采样芯片接口IO定义****************************************************************************/
//忙信号BUSY     PD8
#define AD_BUSY_STAT   (GPIOD->IDR  & 0x0100)//取AD-BUSY引脚当前状态
//复位信号RESET  PB15
#define AD_RESET_H     GPIOB->BSRR = 0x8000//高电平
#define AD_RESET_L     GPIOB->BRR  = 0x8000//低电平
//片选信号CS     PB12
#define AD_CS_H        GPIOB->BSRR = 0x1000//高电平
#define AD_CS_L        GPIOB->BRR  = 0x1000//低电平

/*蓝牙模块控制接口IO定义**************************************************************************/
//工作模式控制信号MODE  PC12
#define BT_MODE_H     GPIOC->BSRR = 0x1000//高电平
#define BT_MODE_L     GPIOC->BRR  = 0x1000//低电平
//连接状态信号PIO9      PA15
#define BT_LINK_STAT  (GPIOA->IDR  & 0x8000)//取PIO9引脚当前状态



#endif
