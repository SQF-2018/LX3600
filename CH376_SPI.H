/*************************************************************************************************/
//CH376芯片 硬件抽象层 V1.0
//提供I/O接口子程序
//修改：程元
//修改日期：2014.10.29
/*************************************************************************************************/

#include "ioconfig.h"
#include "CH376INC.H"

#ifndef	__CH376_SPI_H__
#define __CH376_SPI_H__


/* 附加的USB操作状态定义 */
#define	ERR_USB_UNKNOWN	0xFA	   //未知错误,不应该发生的情况,需检查硬件或者程序错误

#define	xEndCH376Cmd()	CH376_CS_H //SPI片选无效,结束CH376命令,仅用于SPI接口方式

/************************************************************************************************************/
//设置SPI1，用于与CH376通讯
/************************************************************************************************************/
extern void SPI1_Configuration(void);

/*************************************************************************************************/
//向CH376写命令
/*************************************************************************************************/
extern void	xWriteCH376Cmd(unsigned char mCmd);
	 
/*************************************************************************************************/
//向CH376写数据
/*************************************************************************************************/
extern void	xWriteCH376Data(unsigned char mData);

/*************************************************************************************************/
// 从CH376读数据
/*************************************************************************************************/
extern unsigned char xReadCH376Data(void);

/*************************************************************************************************/
//查询CH376中断(INT#引脚为低电平)
/*************************************************************************************************/
extern unsigned char Query376Interrupt(void);

/*************************************************************************************************/
//上电初始化CH376
//初始化成主机方式
/*************************************************************************************************/
extern unsigned char CH376_Init_Host(void); 

#endif
