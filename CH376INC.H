/* C Define for CH376        */
/* Website:  http://wch.cn   */
/* Email:    tech@wch.cn     */
/* Author:   W.ch 2008.10    */
/* V1.0 for CH376            */

#ifndef __CH376INC_H__
#define __CH376INC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************************************************************* */
/* ³£ÓÃÀàÐÍºÍ³£Á¿¶¨Òå */

#ifndef	TRUE
#define	TRUE	1
#define	FALSE	0
#endif 
#ifndef	NULL
#define	NULL	0
#endif

/* ********************************************************************************************************************* */
/* Ó²¼þÌØÐÔ */

#define	CH376_DAT_BLOCK_LEN	0x40 /* USBµ¥¸öÊý¾Ý°ü, Êý¾Ý¿éµÄ×î´ó³¤¶È, Ä¬ÈÏ»º³åÇøµÄ³¤¶È */

/* ********************************************************************************************************************* */
/* ÃüÁî´úÂë */
/* ²¿·ÖÃüÁî¼æÈÝCH375Ð¾Æ¬, µ«ÊÇÊäÈëÊý¾Ý»òÕßÊä³öÊý¾ÝµÄ¿ÉÄÜ¾Ö²¿²»Í¬) */
/* Ò»¸öÃüÁî²Ù×÷Ë³Ðò°üº¬:
          Ò»¸öÃüÁîÂë(¶ÔÓÚ´®¿Ú·½Ê½,ÃüÁîÂëÖ®Ç°»¹ÐèÒªÁ½¸öÍ¬²½Âë),
          Èô¸É¸öÊäÈëÊý¾Ý(¿ÉÒÔÊÇ0¸ö),
          ²úÉúÖÐ¶ÏÍ¨Öª »òÕß Èô¸É¸öÊä³öÊý¾Ý(¿ÉÒÔÊÇ0¸ö), ¶þÑ¡Ò», ÓÐÖÐ¶ÏÍ¨ÖªÔòÒ»¶¨Ã»ÓÐÊä³öÊý¾Ý, ÓÐÊä³öÊý¾ÝÔòÒ»¶¨²»²úÉúÖÐ¶Ï
       ½öCMD01_WR_REQ_DATAÃüÁîÀýÍâ, Ë³Ðò°üº¬: Ò»¸öÃüÁîÂë, Ò»¸öÊä³öÊý¾Ý, Èô¸É¸öÊäÈëÊý¾Ý
   ÃüÁîÂëÆðÃû¹æÔò: CMDxy_NAME
       ÆäÖÐµÄxºÍy¶¼ÊÇÊý×Ö, xËµÃ÷×îÉÙÊäÈëÊý¾Ý¸öÊý(×Ö½ÚÊý), yËµÃ÷×îÉÙÊä³öÊý¾Ý¸öÊý(×Ö½ÚÊý), yÈç¹ûÊÇHÔòËµÃ÷²úÉúÖÐ¶ÏÍ¨Öª,
       ÓÐÐ©ÃüÁîÄÜ¹»ÊµÏÖ0µ½¶à¸ö×Ö½ÚµÄÊý¾Ý¿é¶ÁÐ´, Êý¾Ý¿é±¾ÉíµÄ×Ö½ÚÊýÎ´°üº¬ÔÚÉÏÊöx»òyÖ®ÄÚ */
/* ±¾ÎÄ¼þÄ¬ÈÏ»áÍ¬Ê±Ìá¹©ÓëCH375Ð¾Æ¬ÃüÁîÂë¼æÈÝµÄÃüÁîÂë¸ñÊ½£¨¼´È¥µôxºÍyÖ®ºó), Èç¹û²»ÐèÒª, ÄÇÃ´¿ÉÒÔ¶¨Òå_NO_CH375_COMPATIBLE_½ûÖ¹ */

/* ********************************************************************************************************************* */
/* Ö÷ÒªÃüÁî(ÊÖ²áÒ»), ³£ÓÃ */

#define	CMD01_GET_IC_VER    0x01			/* »ñÈ¡Ð¾Æ¬¼°¹Ì¼þ°æ±¾ */
//Êä³ö: °æ±¾ºÅ( Î»7Îª0, Î»6Îª1, Î»5~Î»0Îª°æ±¾ºÅ 
//           CH376·µ»Ø°æ±¾ºÅµÄÖµÎª041H¼´°æ±¾ºÅÎª01H

#define	CMD21_SET_BAUDRATE  0x02			/* ´®¿Ú·½Ê½: ÉèÖÃ´®¿ÚÍ¨Ñ¶²¨ÌØÂÊ(ÉÏµç»òÕß¸´Î»ºóµÄÄ¬ÈÏ²¨ÌØÂÊÎª9600bps,ÓÉD4/D5/D6Òý½ÅÑ¡Ôñ) */
// ÊäÈë: ²¨ÌØÂÊ·ÖÆµÏµÊý, ²¨ÌØÂÊ·ÖÆµ³£Êý */
// Êä³ö: ²Ù×÷×´Ì¬( CMD_RET_SUCCESS»òCMD_RET_ABORT, ÆäËüÖµËµÃ÷²Ù×÷Î´Íê³É ) */

#define	CMD00_ENTER_SLEEP	0x03			/* ½øÈëË¯Ãß×´Ì¬ */

#define	CMD00_RESET_ALL		0x05			/* Ö´ÐÐÓ²¼þ¸´Î» */

#define	CMD11_CHECK_EXIST	0x06			/* ²âÊÔÍ¨Ñ¶½Ó¿ÚºÍ¹¤×÷×´Ì¬ */
// ÊäÈë: ÈÎÒâÊý¾Ý */
// Êä³ö: ÊäÈëÊý¾ÝµÄ°´Î»È¡·´ */

#define	CMD20_CHK_SUSPEND	0x0B			/* Éè±¸·½Ê½: ÉèÖÃ¼ì²éUSB×ÜÏß¹ÒÆð×´Ì¬µÄ·½Ê½ */
// ÊäÈë: Êý¾Ý10H, ¼ì²é·½Ê½ */
//           00H=²»¼ì²éUSB¹ÒÆð, 04H=ÒÔ50mSÎª¼ä¸ô¼ì²éUSB¹ÒÆð, 05H=ÒÔ10mSÎª¼ä¸ô¼ì²éUSB¹ÒÆð */

#define	CMD20_SET_SDO_INT	0x0B			/* SPI½Ó¿Ú·½Ê½: ÉèÖÃSPIµÄSDOÒý½ÅµÄÖÐ¶Ï·½Ê½ */
// ÊäÈë: Êý¾Ý16H, ÖÐ¶Ï·½Ê½ */
//           10H=½ûÖ¹SDOÒý½ÅÓÃÓÚÖÐ¶ÏÊä³ö,ÔÚSCSÆ¬Ñ¡ÎÞÐ§Ê±ÈýÌ¬Êä³ö½ûÖ¹, 90H=SDOÒý½ÅÔÚSCSÆ¬Ñ¡ÎÞÐ§Ê±¼æ×öÖÐ¶ÏÇëÇóÊä³ö */

#define	CMD14_GET_FILE_SIZE	0x0C			/* Ö÷»úÎÄ¼þÄ£Ê½: »ñÈ¡µ±Ç°ÎÄ¼þ³¤¶È */
// ÊäÈë: Êý¾Ý68H */
// Êä³ö: µ±Ç°ÎÄ¼þ³¤¶È(×Ü³¤¶È32Î»,µÍ×Ö½ÚÔÚÇ°) */

#define	CMD50_SET_FILE_SIZE	0x0D			/* Ö÷»úÎÄ¼þÄ£Ê½: ÉèÖÃµ±Ç°ÎÄ¼þ³¤¶È */
// ÊäÈë: Êý¾Ý68H, µ±Ç°ÎÄ¼þ³¤¶È(×Ü³¤¶È32Î»,µÍ×Ö½ÚÔÚÇ°) */

#define	CMD11_SET_USB_MODE	0x15			/* ÉèÖÃUSB¹¤×÷Ä£Ê½ */
// ÊäÈë: Ä£Ê½´úÂë */
//       00H=Î´ÆôÓÃµÄÉè±¸·½Ê½, 01H=ÒÑÆôÓÃµÄÉè±¸·½Ê½²¢ÇÒÊ¹ÓÃÍâ²¿¹Ì¼þÄ£Ê½(´®¿Ú²»Ö§³Ö), 02H=ÒÑÆôÓÃµÄÉè±¸·½Ê½²¢ÇÒÊ¹ÓÃÄÚÖÃ¹Ì¼þÄ£Ê½ */
//       03H=SD¿¨Ö÷»úÄ£Ê½/Î´ÆôÓÃµÄÖ÷»úÄ£Ê½,ÓÃÓÚ¹ÜÀíºÍ´æÈ¡SD¿¨ÖÐµÄÎÄ¼þ */
//       04H=Î´ÆôÓÃµÄÖ÷»ú·½Ê½, 05H=ÒÑÆôÓÃµÄÖ÷»ú·½Ê½, 06H=ÒÑÆôÓÃµÄÖ÷»ú·½Ê½²¢ÇÒ×Ô¶¯²úÉúSOF°ü, 07H=ÒÑÆôÓÃµÄÖ÷»ú·½Ê½²¢ÇÒ¸´Î»USB×ÜÏß */
// Êä³ö: ²Ù×÷×´Ì¬( CMD_RET_SUCCESS»òCMD_RET_ABORT, ÆäËüÖµËµÃ÷²Ù×÷Î´Íê³É ) */

#define	CMD01_GET_STATUS	0x22			/* »ñÈ¡ÖÐ¶Ï×´Ì¬²¢È¡ÏûÖÐ¶ÏÇëÇó */
// Êä³ö: ÖÐ¶Ï×´Ì¬ */

#define	CMD00_UNLOCK_USB	0x23			/* Éè±¸·½Ê½: ÊÍ·Åµ±Ç°USB»º³åÇø */

#define	CMD01_RD_USB_DATA0	0x27			/* ´Óµ±Ç°USBÖÐ¶ÏµÄ¶Ëµã»º³åÇø»òÕßÖ÷»ú¶ËµãµÄ½ÓÊÕ»º³åÇø¶ÁÈ¡Êý¾Ý¿é */
// Êä³ö: ³¤¶È, Êý¾ÝÁ÷ */

#define	CMD01_RD_USB_DATA	0x28			/* Éè±¸·½Ê½: ´Óµ±Ç°USBÖÐ¶ÏµÄ¶Ëµã»º³åÇø¶ÁÈ¡Êý¾Ý¿é, ²¢ÊÍ·Å»º³åÇø, Ïàµ±ÓÚ CMD01_RD_USB_DATA0 + CMD00_UNLOCK_USB */
// Êä³ö: ³¤¶È, Êý¾ÝÁ÷ */

#define	CMD10_WR_USB_DATA7	0x2B			/* Éè±¸·½Ê½: ÏòUSB¶Ëµã2µÄ·¢ËÍ»º³åÇøÐ´ÈëÊý¾Ý¿é */
// ÊäÈë: ³¤¶È, Êý¾ÝÁ÷ */

#define	CMD10_WR_HOST_DATA	0x2C			/* ÏòUSBÖ÷»ú¶ËµãµÄ·¢ËÍ»º³åÇøÐ´ÈëÊý¾Ý¿é */
// ÊäÈë: ³¤¶È, Êý¾ÝÁ÷ */

#define	CMD01_WR_REQ_DATA	0x2D			/* ÏòÄÚ²¿Ö¸¶¨»º³åÇøÐ´ÈëÇëÇóµÄÊý¾Ý¿é */
// Êä³ö: ³¤¶È */
// ÊäÈë: Êý¾ÝÁ÷ */

#define	CMD20_WR_OFS_DATA	0x2E			/* ÏòÄÚ²¿»º³åÇøÖ¸¶¨Æ«ÒÆµØÖ·Ð´ÈëÊý¾Ý¿é */
// ÊäÈë: Æ«ÒÆ, ³¤¶È, Êý¾ÝÁ÷ */

#define	CMD10_SET_FILE_NAME	0x2F			/* Ö÷»úÎÄ¼þÄ£Ê½: ÉèÖÃ½«Òª²Ù×÷µÄÎÄ¼þµÄÎÄ¼þÃû */
// ÊäÈë: ÒÔ0½áÊøµÄ×Ö·û´®(º¬½áÊø·û0ÔÚÄÚ³¤¶È²»³¬¹ý14¸ö×Ö·û) */

/* ********************************************************************************************************************* */
/* Ö÷ÒªÃüÁî(ÊÖ²áÒ»), ³£ÓÃ, ÒÔÏÂÃüÁî×ÜÊÇÔÚ²Ù×÷½áÊøÊ±²úÉúÖÐ¶ÏÍ¨Öª, ²¢ÇÒ×ÜÊÇÃ»ÓÐÊä³öÊý¾Ý */

#define	CMD0H_DISK_CONNECT	0x30			/* Ö÷»úÎÄ¼þÄ£Ê½/²»Ö§³ÖSD¿¨: ¼ì²é´ÅÅÌÊÇ·ñÁ¬½Ó */
// Êä³öÖÐ¶Ï */

#define	CMD0H_DISK_MOUNT	0x31			/* Ö÷»úÎÄ¼þÄ£Ê½: ³õÊ¼»¯´ÅÅÌ²¢²âÊÔ´ÅÅÌÊÇ·ñ¾ÍÐ÷ */
// Êä³öÖÐ¶Ï */

#define	CMD0H_FILE_OPEN		0x32			/* Ö÷»úÎÄ¼þÄ£Ê½: ´ò¿ªÎÄ¼þ»òÕßÄ¿Â¼(ÎÄ¼þ¼Ð),»òÕßÃ¶¾ÙÎÄ¼þºÍÄ¿Â¼(ÎÄ¼þ¼Ð) */
// Êä³öÖÐ¶Ï */

#define	CMD0H_FILE_ENUM_GO	0x33			/* Ö÷»úÎÄ¼þÄ£Ê½: ¼ÌÐøÃ¶¾ÙÎÄ¼þºÍÄ¿Â¼(ÎÄ¼þ¼Ð) */
// Êä³öÖÐ¶Ï */

#define	CMD0H_FILE_CREATE	0x34			/* Ö÷»úÎÄ¼þÄ£Ê½: ÐÂ½¨ÎÄ¼þ,Èç¹ûÎÄ¼þÒÑ¾­´æÔÚÄÇÃ´ÏÈÉ¾³ý */
// Êä³öÖÐ¶Ï */

#define	CMD0H_FILE_ERASE	0x35			/* Ö÷»úÎÄ¼þÄ£Ê½: É¾³ýÎÄ¼þ,Èç¹ûÒÑ¾­´ò¿ªÔòÖ±½ÓÉ¾³ý,·ñÔò¶ÔÓÚÎÄ¼þ»áÏÈ´ò¿ªÔÙÉ¾³ý,×ÓÄ¿Â¼±ØÐëÏÈ´ò¿ª */
// Êä³öÖÐ¶Ï */

#define	CMD1H_FILE_CLOSE	0x36			/* Ö÷»úÎÄ¼þÄ£Ê½: ¹Ø±Õµ±Ç°ÒÑ¾­´ò¿ªµÄÎÄ¼þ»òÕßÄ¿Â¼(ÎÄ¼þ¼Ð) */
// ÊäÈë: ÊÇ·ñÔÊÐí¸üÐÂÎÄ¼þ³¤¶È */
//          00H=½ûÖ¹¸üÐÂ³¤¶È, 01H=ÔÊÐí¸üÐÂ³¤¶È */
// Êä³öÖÐ¶Ï */

#define	CMD1H_DIR_INFO_READ	0x37			/* Ö÷»úÎÄ¼þÄ£Ê½: ¶ÁÈ¡ÎÄ¼þµÄÄ¿Â¼ÐÅÏ¢ */
// ÊäÈë: Ö¸¶¨ÐèÒª¶ÁÈ¡µÄÄ¿Â¼ÐÅÏ¢½á¹¹ÔÚÉÈÇøÄÚµÄË÷ÒýºÅ */
//           Ë÷ÒýºÅ·¶Î§Îª00H~0FH, Ë÷ÒýºÅ0FFHÔòÎªµ±Ç°ÒÑ¾­´ò¿ªµÄÎÄ¼þ */
// Êä³öÖÐ¶Ï */

#define	CMD0H_DIR_INFO_SAVE	0x38			/* Ö÷»úÎÄ¼þÄ£Ê½: ±£´æÎÄ¼þµÄÄ¿Â¼ÐÅÏ¢ */
// Êä³öÖÐ¶Ï */

#define	CMD4H_BYTE_LOCATE	0x39			/* Ö÷»úÎÄ¼þÄ£Ê½: ÒÔ×Ö½ÚÎªµ¥Î»ÒÆ¶¯µ±Ç°ÎÄ¼þÖ¸Õë */
// ÊäÈë: Æ«ÒÆ×Ö½ÚÊý(×Ü³¤¶È32Î»,µÍ×Ö½ÚÔÚÇ°) */
// Êä³öÖÐ¶Ï */

#define	CMD2H_BYTE_READ		0x3A			/* Ö÷»úÎÄ¼þÄ£Ê½: ÒÔ×Ö½ÚÎªµ¥Î»´Óµ±Ç°Î»ÖÃ¶ÁÈ¡Êý¾Ý¿é */
// ÊäÈë: ÇëÇó¶ÁÈ¡µÄ×Ö½ÚÊý(×Ü³¤¶È16Î»,µÍ×Ö½ÚÔÚÇ°) */
// Êä³öÖÐ¶Ï */

#define	CMD0H_BYTE_RD_GO	0x3B			/* Ö÷»úÎÄ¼þÄ£Ê½: ¼ÌÐø×Ö½Ú¶Á */
// Êä³öÖÐ¶Ï */

#define	CMD2H_BYTE_WRITE	0x3C			/* Ö÷»úÎÄ¼þÄ£Ê½: ÒÔ×Ö½ÚÎªµ¥Î»Ïòµ±Ç°Î»ÖÃÐ´ÈëÊý¾Ý¿é */
// ÊäÈë: ÇëÇóÐ´ÈëµÄ×Ö½ÚÊý(×Ü³¤¶È16Î»,µÍ×Ö½ÚÔÚÇ°) */
// Êä³öÖÐ¶Ï */

#define	CMD0H_BYTE_WR_GO	0x3D			/* Ö÷»úÎÄ¼þÄ£Ê½: ¼ÌÐø×Ö½ÚÐ´ */
// Êä³öÖÐ¶Ï */

#define	CMD0H_DISK_CAPACITY	0x3E			/* Ö÷»úÎÄ¼þÄ£Ê½: ²éÑ¯´ÅÅÌÎïÀíÈÝÁ¿ */
// Êä³öÖÐ¶Ï */

#define	CMD0H_DISK_QUERY	0x3F			/* Ö÷»úÎÄ¼þÄ£Ê½: ²éÑ¯´ÅÅÌ¿Õ¼äÐÅÏ¢ */
// Êä³öÖÐ¶Ï */

#define	CMD0H_DIR_CREATE	0x40			/* Ö÷»úÎÄ¼þÄ£Ê½: ÐÂ½¨Ä¿Â¼(ÎÄ¼þ¼Ð)²¢´ò¿ª,Èç¹ûÄ¿Â¼ÒÑ¾­´æÔÚÄÇÃ´Ö±½Ó´ò¿ª */
// Êä³öÖÐ¶Ï */

#define	CMD4H_SEC_LOCATE	0x4A			/* Ö÷»úÎÄ¼þÄ£Ê½: ÒÔÉÈÇøÎªµ¥Î»ÒÆ¶¯µ±Ç°ÎÄ¼þÖ¸Õë */
// ÊäÈë: Æ«ÒÆÉÈÇøÊý(×Ü³¤¶È32Î»,µÍ×Ö½ÚÔÚÇ°) */
// Êä³öÖÐ¶Ï */

#define	CMD1H_SEC_READ		0x4B			/* Ö÷»úÎÄ¼þÄ£Ê½/²»Ö§³ÖSD¿¨: ÒÔÉÈÇøÎªµ¥Î»´Óµ±Ç°Î»ÖÃ¶ÁÈ¡Êý¾Ý¿é */
// ÊäÈë: ÇëÇó¶ÁÈ¡µÄÉÈÇøÊý */
// Êä³öÖÐ¶Ï */

#define	CMD1H_SEC_WRITE		0x4C			/* Ö÷»úÎÄ¼þÄ£Ê½/²»Ö§³ÖSD¿¨: ÒÔÉÈÇøÎªµ¥Î»ÔÚµ±Ç°Î»ÖÃÐ´ÈëÊý¾Ý¿é */
// ÊäÈë: ÇëÇóÐ´ÈëµÄÉÈÇøÊý */
// Êä³öÖÐ¶Ï */

#define	CMD0H_DISK_BOC_CMD	0x50			/* Ö÷»ú·½Ê½/²»Ö§³ÖSD¿¨: ¶ÔUSB´æ´¢Æ÷Ö´ÐÐBulkOnly´«ÊäÐ­ÒéµÄÃüÁî */
// Êä³öÖÐ¶Ï */

#define	CMD5H_DISK_READ		0x54			/* Ö÷»ú·½Ê½/²»Ö§³ÖSD¿¨: ´ÓUSB´æ´¢Æ÷¶ÁÎïÀíÉÈÇø */
// ÊäÈë: LBAÎïÀíÉÈÇøµØÖ·(×Ü³¤¶È32Î», µÍ×Ö½ÚÔÚÇ°), ÉÈÇøÊý(01H~FFH) */
// Êä³öÖÐ¶Ï */

#define	CMD0H_DISK_RD_GO	0x55			/* Ö÷»ú·½Ê½/²»Ö§³ÖSD¿¨: ¼ÌÐøÖ´ÐÐUSB´æ´¢Æ÷µÄÎïÀíÉÈÇø¶Á²Ù×÷ */
// Êä³öÖÐ¶Ï */

#define	CMD5H_DISK_WRITE	0x56			/* Ö÷»ú·½Ê½/²»Ö§³ÖSD¿¨: ÏòUSB´æ´¢Æ÷Ð´ÎïÀíÉÈÇø */
// ÊäÈë: LBAÎïÀíÉÈÇøµØÖ·(×Ü³¤¶È32Î», µÍ×Ö½ÚÔÚÇ°), ÉÈÇøÊý(01H~FFH) */
// Êä³öÖÐ¶Ï */

#define	CMD0H_DISK_WR_GO	0x57			/* Ö÷»ú·½Ê½/²»Ö§³ÖSD¿¨: ¼ÌÐøÖ´ÐÐUSB´æ´¢Æ÷µÄÎïÀíÉÈÇøÐ´²Ù×÷ */
// Êä³öÖÐ¶Ï */

/* ********************************************************************************************************************* */
/* ¸¨ÖúÃüÁî(ÊÖ²á¶þ), ²»Ì«³£ÓÃ»òÕßÊÇÎªÁËÓëCH375ºÍCH372¼æÈÝ */

#define	CMD10_SET_USB_SPEED	0x04			/* ÉèÖÃUSB×ÜÏßËÙ¶È, ÔÚÃ¿´ÎCMD11_SET_USB_MODEÉèÖÃUSB¹¤×÷Ä£Ê½Ê±»á×Ô¶¯»Ö¸´µ½12MbpsÈ«ËÙ */
// ÊäÈë: ×ÜÏßËÙ¶È´úÂë */
//           00H=12MbpsÈ«ËÙFullSpeed£¨Ä¬ÈÏÖµ£©, 01H=1.5Mbps£¨½öÐÞ¸ÄÆµÂÊ£©, 02H=1.5MbpsµÍËÙLowSpeed */

#define	CMD11_GET_DEV_RATE	0x0A			/* Ö÷»ú·½Ê½: »ñÈ¡µ±Ç°Á¬½ÓµÄUSBÉè±¸µÄÊý¾ÝËÙÂÊÀàÐÍ */
// ÊäÈë: Êý¾Ý07H */
// Êä³ö: Êý¾ÝËÙÂÊÀàÐÍ */
//           Î»4Îª1ÔòÊÇ1.5MbpsµÍËÙUSBÉè±¸, ·ñÔòÊÇ12MbpsÈ«ËÙUSBÉè±¸ */

#define	CMD11_GET_TOGGLE	0x0A			/* »ñÈ¡OUTÊÂÎñµÄÍ¬²½×´Ì¬ */
// ÊäÈë: Êý¾Ý1AH */
// Êä³ö: Í¬²½×´Ì¬ */
//           Î»4Îª1ÔòOUTÊÂÎñÍ¬²½, ·ñÔòOUTÊÂÎñ²»Í¬²½ */

#define	CMD11_READ_VAR8		0x0A			/* ¶ÁÈ¡Ö¸¶¨µÄ8Î»ÎÄ¼þÏµÍ³±äÁ¿ */
// ÊäÈë: ±äÁ¿µØÖ· */
// Êä³ö: Êý¾Ý */

/*#define	CMD11_GET_MAX_LUN	= CMD11_READ_VAR8( VAR_UDISK_LUN )*/	/* Ö÷»ú·½Ê½: »ñÈ¡USB´æ´¢Æ÷×î´óºÍµ±Ç°Âß¼­µ¥ÔªºÅ */

#define	CMD20_SET_RETRY		0x0B			/* Ö÷»ú·½Ê½: ÉèÖÃUSBÊÂÎñ²Ù×÷µÄÖØÊÔ´ÎÊý */
// ÊäÈë: Êý¾Ý25H, ÖØÊÔ´ÎÊý */
//           Î»7Îª0ÔòÊÕµ½NAKÊ±²»ÖØÊÔ, Î»7Îª1Î»6Îª0ÔòÊÕµ½NAKÊ±ÎÞÏÞÖØÊÔ, Î»7Îª1Î»6Îª1ÔòÊÕµ½NAKÊ±×î¶àÖØÊÔ3Ãë, Î»5~Î»0Îª³¬Ê±ºóµÄÖØÊÔ´ÎÊý */

#define	CMD20_WRITE_VAR8	0x0B			/* ÉèÖÃÖ¸¶¨µÄ8Î»ÎÄ¼þÏµÍ³±äÁ¿ */
// ÊäÈë: ±äÁ¿µØÖ·, Êý¾Ý */

/*#define	CMD20_SET_DISK_LUN	= CMD20_WRITE_VAR8( VAR_UDISK_LUN )*/	/* Ö÷»ú·½Ê½: ÉèÖÃUSB´æ´¢Æ÷µÄµ±Ç°Âß¼­µ¥ÔªºÅ */

#define	CMD14_READ_VAR32	0x0C			/* ¶ÁÈ¡Ö¸¶¨µÄ32Î»ÎÄ¼þÏµÍ³±äÁ¿ */
// ÊäÈë: ±äÁ¿µØÖ· */
// Êä³ö: Êý¾Ý(×Ü³¤¶È32Î»,µÍ×Ö½ÚÔÚÇ°) */

#define	CMD50_WRITE_VAR32	0x0D			/* ÉèÖÃÖ¸¶¨µÄ32Î»ÎÄ¼þÏµÍ³±äÁ¿ */
// ÊäÈë: ±äÁ¿µØÖ·, Êý¾Ý(×Ü³¤¶È32Î»,µÍ×Ö½ÚÔÚÇ°) */

#define	CMD01_DELAY_100US	0x0F			/* ÑÓÊ±100uS(´®¿Ú²»Ö§³Ö) */
// Êä³ö: ÑÓÊ±ÆÚ¼äÊä³ö0,ÑÓÊ±½áÊøÊä³ö·Ç0 */

#define	CMD40_SET_USB_ID	0x12			/* Éè±¸·½Ê½: ÉèÖÃUSB³§ÉÌVIDºÍ²úÆ·PID */
// ÊäÈë: ³§ÉÌIDµÍ×Ö½Ú, ³§ÉÌID¸ß×Ö½Ú, ²úÆ·IDµÍ×Ö½Ú, ²úÆ·ID¸ß×Ö½Ú */

#define	CMD10_SET_USB_ADDR	0x13			/* ÉèÖÃUSBµØÖ· */
// ÊäÈë: µØÖ·Öµ */

#define	CMD01_TEST_CONNECT	0x16			/* Ö÷»ú·½Ê½/²»Ö§³ÖSD¿¨: ¼ì²éUSBÉè±¸Á¬½Ó×´Ì¬ */
// Êä³ö: ×´Ì¬( USB_INT_CONNECT»òUSB_INT_DISCONNECT»òUSB_INT_USB_READY, ÆäËüÖµËµÃ÷²Ù×÷Î´Íê³É ) */

#define	CMD00_ABORT_NAK		0x17			/* Ö÷»ú·½Ê½: ·ÅÆúµ±Ç°NAKµÄÖØÊÔ */

#define	CMD10_SET_ENDP2		0x18			/* Éè±¸·½Ê½(´®¿Ú²»Ö§³Ö): ÉèÖÃUSB¶Ëµã0µÄ½ÓÊÕÆ÷ */
// ÊäÈë: ¹¤×÷·½Ê½ */
//           Î»7Îª1ÔòÎ»6ÎªÍ¬²½´¥·¢Î», ·ñÔòÍ¬²½´¥·¢Î»²»±ä */
//           Î»3~Î»0ÎªÊÂÎñÏìÓ¦·½Ê½:  0000-¾ÍÐ÷ACK, 1110-ÕýÃ¦NAK, 1111-´íÎóSTALL */

#define	CMD10_SET_ENDP3		0x19			/* Éè±¸·½Ê½(´®¿Ú²»Ö§³Ö): ÉèÖÃUSB¶Ëµã0µÄ·¢ËÍÆ÷ */
// ÊäÈë: ¹¤×÷·½Ê½ */
//           Î»7Îª1ÔòÎ»6ÎªÍ¬²½´¥·¢Î», ·ñÔòÍ¬²½´¥·¢Î»²»±ä */
//           Î»3~Î»0ÎªÊÂÎñÏìÓ¦·½Ê½:  0000~1000-¾ÍÐ÷ACK, 1110-ÕýÃ¦NAK, 1111-´íÎóSTALL */

#define	CMD10_SET_ENDP4		0x1A			/* Éè±¸·½Ê½(´®¿Ú²»Ö§³Ö): ÉèÖÃUSB¶Ëµã1µÄ½ÓÊÕÆ÷ */
// ÊäÈë: ¹¤×÷·½Ê½ */
//           Î»7Îª1ÔòÎ»6ÎªÍ¬²½´¥·¢Î», ·ñÔòÍ¬²½´¥·¢Î»²»±ä */
//           Î»3~Î»0ÎªÊÂÎñÏìÓ¦·½Ê½:  0000-¾ÍÐ÷ACK, 1110-ÕýÃ¦NAK, 1111-´íÎóSTALL */

#define	CMD10_SET_ENDP5		0x1B			/* Éè±¸·½Ê½(´®¿Ú²»Ö§³Ö): ÉèÖÃUSB¶Ëµã1µÄ·¢ËÍÆ÷ */
// ÊäÈë: ¹¤×÷·½Ê½ */
//           Î»7Îª1ÔòÎ»6ÎªÍ¬²½´¥·¢Î», ·ñÔòÍ¬²½´¥·¢Î»²»±ä */
//           Î»3~Î»0ÎªÊÂÎñÏìÓ¦·½Ê½:  0000~1000-¾ÍÐ÷ACK, 1110-ÕýÃ¦NAK, 1111-´íÎóSTALL */

#define	CMD10_SET_ENDP6		0x1C			/* ÉèÖÃUSB¶Ëµã2/Ö÷»ú¶ËµãµÄ½ÓÊÕÆ÷ */
// ÊäÈë: ¹¤×÷·½Ê½ */
//           Î»7Îª1ÔòÎ»6ÎªÍ¬²½´¥·¢Î», ·ñÔòÍ¬²½´¥·¢Î»²»±ä */
//           Î»3~Î»0ÎªÊÂÎñÏìÓ¦·½Ê½:  0000-¾ÍÐ÷ACK, 1101-¾ÍÐ÷µ«²»·µ»ØACK, 1110-ÕýÃ¦NAK, 1111-´íÎóSTALL */

#define	CMD10_SET_ENDP7		0x1D			/* ÉèÖÃUSB¶Ëµã2/Ö÷»ú¶ËµãµÄ·¢ËÍÆ÷ */
// ÊäÈë: ¹¤×÷·½Ê½ */
//           Î»7Îª1ÔòÎ»6ÎªÍ¬²½´¥·¢Î», ·ñÔòÍ¬²½´¥·¢Î»²»±ä */
//           Î»3~Î»0ÎªÊÂÎñÏìÓ¦·½Ê½:  0000-¾ÍÐ÷ACK, 1101-¾ÍÐ÷µ«ÎÞÐëÓ¦´ð, 1110-ÕýÃ¦NAK, 1111-´íÎóSTALL */

#define	CMD00_DIRTY_BUFFER	0x25			/* Ö÷»úÎÄ¼þÄ£Ê½: Çå³ýÄÚ²¿µÄ´ÅÅÌºÍÎÄ¼þ»º³åÇø */

#define	CMD10_WR_USB_DATA3	0x29			/* Éè±¸·½Ê½(´®¿Ú²»Ö§³Ö): ÏòUSB¶Ëµã0µÄ·¢ËÍ»º³åÇøÐ´ÈëÊý¾Ý¿é */
// ÊäÈë: ³¤¶È, Êý¾ÝÁ÷ */

#define	CMD10_WR_USB_DATA5	0x2A			/* Éè±¸·½Ê½(´®¿Ú²»Ö§³Ö): ÏòUSB¶Ëµã1µÄ·¢ËÍ»º³åÇøÐ´ÈëÊý¾Ý¿é */
// ÊäÈë: ³¤¶È, Êý¾ÝÁ÷ */

/* ********************************************************************************************************************* */
/* ¸¨ÖúÃüÁî(ÊÖ²á¶þ), ²»Ì«³£ÓÃ»òÕßÊÇÎªÁËÓëCH375ºÍCH372¼æÈÝ, ÒÔÏÂÃüÁî×ÜÊÇÔÚ²Ù×÷½áÊøÊ±²úÉúÖÐ¶ÏÍ¨Öª, ²¢ÇÒ×ÜÊÇÃ»ÓÐÊä³öÊý¾Ý */

#define	CMD1H_CLR_STALL		0x41			/* Ö÷»ú·½Ê½: ¿ØÖÆ´«Êä-Çå³ý¶Ëµã´íÎó */
// ÊäÈë: ¶ËµãºÅ */
// Êä³öÖÐ¶Ï */

#define	CMD1H_SET_ADDRESS	0x45			/* Ö÷»ú·½Ê½: ¿ØÖÆ´«Êä-ÉèÖÃUSBµØÖ· */
// ÊäÈë: µØÖ·Öµ */
// Êä³öÖÐ¶Ï */

#define	CMD1H_GET_DESCR		0x46			/* Ö÷»ú·½Ê½: ¿ØÖÆ´«Êä-»ñÈ¡ÃèÊö·û */
// ÊäÈë: ÃèÊö·ûÀàÐÍ */
// Êä³öÖÐ¶Ï */

#define	CMD1H_SET_CONFIG	0x49			/* Ö÷»ú·½Ê½: ¿ØÖÆ´«Êä-ÉèÖÃUSBÅäÖÃ */
// ÊäÈë: ÅäÖÃÖµ */
// Êä³öÖÐ¶Ï */

#define	CMD0H_AUTO_SETUP	0x4D			/* Ö÷»ú·½Ê½/²»Ö§³ÖSD¿¨: ×Ô¶¯ÅäÖÃUSBÉè±¸ */
// Êä³öÖÐ¶Ï */

#define	CMD2H_ISSUE_TKN_X	0x4E			/* Ö÷»ú·½Ê½: ·¢³öÍ¬²½ÁîÅÆ, Ö´ÐÐÊÂÎñ, ¸ÃÃüÁî¿É´úÌæ CMD10_SET_ENDP6/CMD10_SET_ENDP7 + CMD1H_ISSUE_TOKEN */
// ÊäÈë: Í¬²½±êÖ¾, ÊÂÎñÊôÐÔ */
//           Í¬²½±êÖ¾µÄÎ»7ÎªÖ÷»ú¶ËµãINµÄÍ¬²½´¥·¢Î», Î»6ÎªÖ÷»ú¶ËµãOUTµÄÍ¬²½´¥·¢Î», Î»5~Î»0±ØÐëÎª0 */
//           ÊÂÎñÊôÐÔµÄµÍ4Î»ÊÇÁîÅÆ, ¸ß4Î»ÊÇ¶ËµãºÅ */
// Êä³öÖÐ¶Ï */

#define	CMD1H_ISSUE_TOKEN	0x4F			/* Ö÷»ú·½Ê½: ·¢³öÁîÅÆ, Ö´ÐÐÊÂÎñ, ½¨ÒéÓÃCMD2H_ISSUE_TKN_XÃüÁî */
// ÊäÈë: ÊÂÎñÊôÐÔ */
//           µÍ4Î»ÊÇÁîÅÆ, ¸ß4Î»ÊÇ¶ËµãºÅ */
// Êä³öÖÐ¶Ï */

#define	CMD0H_DISK_INIT		0x51			/* Ö÷»ú·½Ê½/²»Ö§³ÖSD¿¨: ³õÊ¼»¯USB´æ´¢Æ÷ */
// Êä³öÖÐ¶Ï */

#define	CMD0H_DISK_RESET	0x52			/* Ö÷»ú·½Ê½/²»Ö§³ÖSD¿¨: ¿ØÖÆ´«Êä-¸´Î»USB´æ´¢Æ÷ */
// Êä³öÖÐ¶Ï */

#define	CMD0H_DISK_SIZE		0x53			/* Ö÷»ú·½Ê½/²»Ö§³ÖSD¿¨: »ñÈ¡USB´æ´¢Æ÷µÄÈÝÁ¿ */
// Êä³öÖÐ¶Ï */

#define	CMD0H_DISK_INQUIRY	0x58			/* Ö÷»ú·½Ê½/²»Ö§³ÖSD¿¨: ²éÑ¯USB´æ´¢Æ÷ÌØÐÔ */
// Êä³öÖÐ¶Ï */

#define	CMD0H_DISK_READY	0x59			/* Ö÷»ú·½Ê½/²»Ö§³ÖSD¿¨: ¼ì²éUSB´æ´¢Æ÷¾ÍÐ÷ */
// Êä³öÖÐ¶Ï */

#define	CMD0H_DISK_R_SENSE	0x5A			/* Ö÷»ú·½Ê½/²»Ö§³ÖSD¿¨: ¼ì²éUSB´æ´¢Æ÷´íÎó */
// Êä³öÖÐ¶Ï */

#define	CMD0H_RD_DISK_SEC	0x5B			/* Ö÷»úÎÄ¼þÄ£Ê½: ´Ó´ÅÅÌ¶ÁÈ¡Ò»¸öÉÈÇøµÄÊý¾Ýµ½ÄÚ²¿»º³åÇø */
// Êä³öÖÐ¶Ï */

#define	CMD0H_WR_DISK_SEC	0x5C			/* Ö÷»úÎÄ¼þÄ£Ê½: ½«ÄÚ²¿»º³åÇøµÄÒ»¸öÉÈÇøµÄÊý¾ÝÐ´Èë´ÅÅÌ */
// Êä³öÖÐ¶Ï */

#define	CMD0H_DISK_MAX_LUN	0x5D			/* Ö÷»ú·½Ê½: ¿ØÖÆ´«Êä-»ñÈ¡USB´æ´¢Æ÷×î´óÂß¼­µ¥ÔªºÅ */
// Êä³öÖÐ¶Ï */

/* ********************************************************************************************************************* */
/* ÒÔÏÂ¶¨ÒåÖ»ÊÇÎªÁË¼æÈÝCH375µÄINCLUDEÎÄ¼þÖÐµÄÃüÁîÃû³Æ¸ñÊ½ */

#ifndef	_NO_CH375_COMPATIBLE_
#define	CMD_GET_IC_VER		CMD01_GET_IC_VER
#define	CMD_SET_BAUDRATE	CMD21_SET_BAUDRATE
#define	CMD_ENTER_SLEEP		CMD00_ENTER_SLEEP
#define	CMD_RESET_ALL		CMD00_RESET_ALL
#define	CMD_CHECK_EXIST		CMD11_CHECK_EXIST
#define	CMD_CHK_SUSPEND		CMD20_CHK_SUSPEND
#define	CMD_SET_SDO_INT		CMD20_SET_SDO_INT
#define	CMD_GET_FILE_SIZE	CMD14_GET_FILE_SIZE
#define	CMD_SET_FILE_SIZE	CMD50_SET_FILE_SIZE
#define	CMD_SET_USB_MODE	CMD11_SET_USB_MODE
#define	CMD_GET_STATUS		CMD01_GET_STATUS
#define	CMD_UNLOCK_USB		CMD00_UNLOCK_USB
#define	CMD_RD_USB_DATA0	CMD01_RD_USB_DATA0
#define	CMD_RD_USB_DATA		CMD01_RD_USB_DATA
#define	CMD_WR_USB_DATA7	CMD10_WR_USB_DATA7
#define	CMD_WR_HOST_DATA	CMD10_WR_HOST_DATA
#define	CMD_WR_REQ_DATA		CMD01_WR_REQ_DATA
#define	CMD_WR_OFS_DATA		CMD20_WR_OFS_DATA
#define	CMD_SET_FILE_NAME	CMD10_SET_FILE_NAME
#define	CMD_DISK_CONNECT	CMD0H_DISK_CONNECT
#define	CMD_DISK_MOUNT		CMD0H_DISK_MOUNT
#define	CMD_FILE_OPEN		CMD0H_FILE_OPEN
#define	CMD_FILE_ENUM_GO	CMD0H_FILE_ENUM_GO
#define	CMD_FILE_CREATE		CMD0H_FILE_CREATE
#define	CMD_FILE_ERASE		CMD0H_FILE_ERASE
#define	CMD_FILE_CLOSE		CMD1H_FILE_CLOSE
#define	CMD_DIR_INFO_READ	CMD1H_DIR_INFO_READ
#define	CMD_DIR_INFO_SAVE	CMD0H_DIR_INFO_SAVE
#define	CMD_BYTE_LOCATE		CMD4H_BYTE_LOCATE
#define	CMD_BYTE_READ		CMD2H_BYTE_READ
#define	CMD_BYTE_RD_GO		CMD0H_BYTE_RD_GO
#define	CMD_BYTE_WRITE		CMD2H_BYTE_WRITE
#define	CMD_BYTE_WR_GO		CMD0H_BYTE_WR_GO
#define	CMD_DISK_CAPACITY	CMD0H_DISK_CAPACITY
#define	CMD_DISK_QUERY		CMD0H_DISK_QUERY
#define	CMD_DIR_CREATE		CMD0H_DIR_CREATE
#define	CMD_SEC_LOCATE		CMD4H_SEC_LOCATE
#define	CMD_SEC_READ		CMD1H_SEC_READ
#define	CMD_SEC_WRITE		CMD1H_SEC_WRITE
#define	CMD_DISK_BOC_CMD	CMD0H_DISK_BOC_CMD
#define	CMD_DISK_READ		CMD5H_DISK_READ
#define	CMD_DISK_RD_GO		CMD0H_DISK_RD_GO
#define	CMD_DISK_WRITE		CMD5H_DISK_WRITE
#define	CMD_DISK_WR_GO		CMD0H_DISK_WR_GO
#define	CMD_SET_USB_SPEED	CMD10_SET_USB_SPEED
#define	CMD_GET_DEV_RATE	CMD11_GET_DEV_RATE
#define	CMD_GET_TOGGLE		CMD11_GET_TOGGLE
#define	CMD_READ_VAR8		CMD11_READ_VAR8
#define	CMD_SET_RETRY		CMD20_SET_RETRY
#define	CMD_WRITE_VAR8		CMD20_WRITE_VAR8
#define	CMD_READ_VAR32		CMD14_READ_VAR32
#define	CMD_WRITE_VAR32		CMD50_WRITE_VAR32
#define	CMD_DELAY_100US		CMD01_DELAY_100US
#define	CMD_SET_USB_ID		CMD40_SET_USB_ID
#define	CMD_SET_USB_ADDR	CMD10_SET_USB_ADDR
#define	CMD_TEST_CONNECT	CMD01_TEST_CONNECT
#define	CMD_ABORT_NAK		CMD00_ABORT_NAK
#define	CMD_SET_ENDP2		CMD10_SET_ENDP2
#define	CMD_SET_ENDP3		CMD10_SET_ENDP3
#define	CMD_SET_ENDP4		CMD10_SET_ENDP4
#define	CMD_SET_ENDP5		CMD10_SET_ENDP5
#define	CMD_SET_ENDP6		CMD10_SET_ENDP6
#define	CMD_SET_ENDP7		CMD10_SET_ENDP7
#define	CMD_DIRTY_BUFFER	CMD00_DIRTY_BUFFER
#define	CMD_WR_USB_DATA3	CMD10_WR_USB_DATA3
#define	CMD_WR_USB_DATA5	CMD10_WR_USB_DATA5
#define	CMD_CLR_STALL		CMD1H_CLR_STALL
#define	CMD_SET_ADDRESS		CMD1H_SET_ADDRESS
#define	CMD_GET_DESCR		CMD1H_GET_DESCR
#define	CMD_SET_CONFIG		CMD1H_SET_CONFIG
#define	CMD_AUTO_SETUP		CMD0H_AUTO_SETUP
#define	CMD_ISSUE_TKN_X		CMD2H_ISSUE_TKN_X
#define	CMD_ISSUE_TOKEN		CMD1H_ISSUE_TOKEN
#define	CMD_DISK_INIT		CMD0H_DISK_INIT
#define	CMD_DISK_RESET		CMD0H_DISK_RESET
#define	CMD_DISK_SIZE		CMD0H_DISK_SIZE
#define	CMD_DISK_INQUIRY	CMD0H_DISK_INQUIRY
#define	CMD_DISK_READY		CMD0H_DISK_READY
#define	CMD_DISK_R_SENSE	CMD0H_DISK_R_SENSE
#define	CMD_RD_DISK_SEC		CMD0H_RD_DISK_SEC
#define	CMD_WR_DISK_SEC		CMD0H_WR_DISK_SEC
#define	CMD_DISK_MAX_LUN	CMD0H_DISK_MAX_LUN
#endif

/* ********************************************************************************************************************* */
/* ²¢¿Ú·½Ê½, ×´Ì¬¶Ë¿Ú(¶ÁÃüÁî¶Ë¿Ú)µÄÎ»¶¨Òå */
#ifndef	PARA_STATE_INTB
#define	PARA_STATE_INTB		0x80			/* ²¢¿Ú·½Ê½×´Ì¬¶Ë¿ÚµÄÎ»7: ÖÐ¶Ï±êÖ¾,µÍÓÐÐ§ */
#define	PARA_STATE_BUSY		0x10			/* ²¢¿Ú·½Ê½×´Ì¬¶Ë¿ÚµÄÎ»4: Ã¦±êÖ¾,¸ßÓÐÐ§ */
#endif

/* ********************************************************************************************************************* */
/* ´®¿Ú·½Ê½, ²Ù×÷ÃüÁîÇ°µÄÒýµ¼Í¬²½Âë */
#ifndef	SER_CMD_TIMEOUT
#define	SER_CMD_TIMEOUT		32				/* ´®¿ÚÃüÁî³¬Ê±Ê±¼ä, µ¥Î»ÎªmS, Í¬²½ÂëÖ®¼ä¼°Í¬²½ÂëÓëÃüÁîÂëÖ®¼äµÄ¼ä¸ôÓ¦¸Ã¾¡Á¿¶Ì, ³¬Ê±ºóµÄ´¦Àí·½Ê½Îª¶ªÆú */
#define	SER_SYNC_CODE1		0x57			/* Æô¶¯²Ù×÷µÄµÚ1¸ö´®¿ÚÍ¬²½Âë */
#define	SER_SYNC_CODE2		0xAB			/* Æô¶¯²Ù×÷µÄµÚ2¸ö´®¿ÚÍ¬²½Âë */
#endif

/* ********************************************************************************************************************* */
/* ²Ù×÷×´Ì¬ */

#ifndef	CMD_RET_SUCCESS
#define	CMD_RET_SUCCESS		0x51			/* ÃüÁî²Ù×÷³É¹¦ */
#define	CMD_RET_ABORT		0x5F			/* ÃüÁî²Ù×÷Ê§°Ü */
#endif

/* ********************************************************************************************************************* */
/* USBÖÐ¶Ï×´Ì¬ */

#ifndef	USB_INT_EP0_SETUP

/* ÒÔÏÂ×´Ì¬´úÂëÎªÌØÊâÊÂ¼þÖÐ¶Ï, Èç¹ûÍ¨¹ýCMD20_CHK_SUSPENDÆôÓÃUSB×ÜÏß¹ÒÆð¼ì²é, ÄÇÃ´±ØÐë´¦ÀíUSB×ÜÏß¹ÒÆðºÍË¯Ãß»½ÐÑµÄÖÐ¶Ï×´Ì¬ */
#define	USB_INT_USB_SUSPEND	0x05			/* USB×ÜÏß¹ÒÆðÊÂ¼þ */
#define	USB_INT_WAKE_UP		0x06			/* ´ÓË¯ÃßÖÐ±»»½ÐÑÊÂ¼þ */

/* ÒÔÏÂ×´Ì¬´úÂë0XHÓÃÓÚUSBÉè±¸·½Ê½ */
/*   ÄÚÖÃ¹Ì¼þÄ£Ê½ÏÂÖ»ÐèÒª´¦Àí: USB_INT_EP1_OUT, USB_INT_EP1_IN, USB_INT_EP2_OUT, USB_INT_EP2_IN */
/*   Î»7-Î»4Îª0000 */
/*   Î»3-Î»2Ö¸Ê¾µ±Ç°ÊÂÎñ, 00=OUT, 10=IN, 11=SETUP */
/*   Î»1-Î»0Ö¸Ê¾µ±Ç°¶Ëµã, 00=¶Ëµã0, 01=¶Ëµã1, 10=¶Ëµã2, 11=USB×ÜÏß¸´Î» */
#define	USB_INT_EP0_SETUP	0x0C			/* USB¶Ëµã0µÄSETUP */
#define	USB_INT_EP0_OUT		0x00			/* USB¶Ëµã0µÄOUT */
#define	USB_INT_EP0_IN		0x08			/* USB¶Ëµã0µÄIN */
#define	USB_INT_EP1_OUT		0x01			/* USB¶Ëµã1µÄOUT */
#define	USB_INT_EP1_IN		0x09			/* USB¶Ëµã1µÄIN */
#define	USB_INT_EP2_OUT		0x02			/* USB¶Ëµã2µÄOUT */
#define	USB_INT_EP2_IN		0x0A			/* USB¶Ëµã2µÄIN */
/* USB_INT_BUS_RESET	0x0000XX11B */		/* USB×ÜÏß¸´Î» */
#define	USB_INT_BUS_RESET1	0x03			/* USB×ÜÏß¸´Î» */
#define	USB_INT_BUS_RESET2	0x07			/* USB×ÜÏß¸´Î» */
#define	USB_INT_BUS_RESET3	0x0B			/* USB×ÜÏß¸´Î» */
#define	USB_INT_BUS_RESET4	0x0F			/* USB×ÜÏß¸´Î» */

#endif

/* ÒÔÏÂ×´Ì¬´úÂë2XH-3XHÓÃÓÚUSBÖ÷»ú·½Ê½µÄÍ¨Ñ¶Ê§°Ü´úÂë */
/*   Î»7-Î»6Îª00 */
/*   Î»5Îª1 */
/*   Î»4Ö¸Ê¾µ±Ç°½ÓÊÕµÄÊý¾Ý°üÊÇ·ñÍ¬²½ */
/*   Î»3-Î»0Ö¸Ê¾µ¼ÖÂÍ¨Ñ¶Ê§°ÜÊ±USBÉè±¸µÄÓ¦´ð: 0010=ACK, 1010=NAK, 1110=STALL, 0011=DATA0, 1011=DATA1, XX00=³¬Ê± */
/* USB_INT_RET_ACK	0x001X0010B */			/* ´íÎó:¶ÔÓÚINÊÂÎñ·µ»ØACK */
/* USB_INT_RET_NAK	0x001X1010B */			/* ´íÎó:·µ»ØNAK */
/* USB_INT_RET_STALL	0x001X1110B */		/* ´íÎó:·µ»ØSTALL */
/* USB_INT_RET_DATA0	0x001X0011B */		/* ´íÎó:¶ÔÓÚOUT/SETUPÊÂÎñ·µ»ØDATA0 */
/* USB_INT_RET_DATA1	0x001X1011B */		/* ´íÎó:¶ÔÓÚOUT/SETUPÊÂÎñ·µ»ØDATA1 */
/* USB_INT_RET_TOUT	0x001XXX00B */			/* ´íÎó:·µ»Ø³¬Ê± */
/* USB_INT_RET_TOGX	0x0010X011B */			/* ´íÎó:¶ÔÓÚINÊÂÎñ·µ»ØÊý¾Ý²»Í¬²½ */
/* USB_INT_RET_PID	0x001XXXXXB */			/* ´íÎó:Î´¶¨Òå */

/* ÒÔÏÂ×´Ì¬´úÂë1XHÓÃÓÚUSBÖ÷»ú·½Ê½µÄ²Ù×÷×´Ì¬´úÂë */
#ifndef	USB_INT_SUCCESS
#define	USB_INT_SUCCESS		0x14			/* USBÊÂÎñ»òÕß´«Êä²Ù×÷³É¹¦ */
#define	USB_INT_CONNECT		0x15			/* ¼ì²âµ½USBÉè±¸Á¬½ÓÊÂ¼þ, ¿ÉÄÜÊÇÐÂÁ¬½Ó»òÕß¶Ï¿ªºóÖØÐÂÁ¬½Ó */
#define	USB_INT_DISCONNECT	0x16			/* ¼ì²âµ½USBÉè±¸¶Ï¿ªÊÂ¼þ */
#define	USB_INT_BUF_OVER	0x17			/* USB´«ÊäµÄÊý¾ÝÓÐÎó»òÕßÊý¾ÝÌ«¶à»º³åÇøÒç³ö */
#define	USB_INT_USB_READY	0x18			/* USBÉè±¸ÒÑ¾­±»³õÊ¼»¯(ÒÑ¾­·ÖÅäUSBµØÖ·) */
#define	USB_INT_DISK_READ	0x1D			/* USB´æ´¢Æ÷ÇëÇóÊý¾Ý¶Á³ö */
#define	USB_INT_DISK_WRITE	0x1E			/* USB´æ´¢Æ÷ÇëÇóÊý¾ÝÐ´Èë */
#define	USB_INT_DISK_ERR	0x1F			/* USB´æ´¢Æ÷²Ù×÷Ê§°Ü */
#endif

/* ÒÔÏÂ×´Ì¬´úÂëÓÃÓÚÖ÷»úÎÄ¼þÄ£Ê½ÏÂµÄÎÄ¼þÏµÍ³´íÎóÂë */
#ifndef	ERR_DISK_DISCON
#define	ERR_DISK_DISCON		0x82			/* ´ÅÅÌÉÐÎ´Á¬½Ó,¿ÉÄÜ´ÅÅÌÒÑ¾­¶Ï¿ª */
#define	ERR_LARGE_SECTOR	0x84			/* ´ÅÅÌµÄÉÈÇøÌ«´ó,Ö»Ö§³ÖÃ¿ÉÈÇø512×Ö½Ú */
#define	ERR_TYPE_ERROR		0x92			/* ´ÅÅÌ·ÖÇøÀàÐÍ²»Ö§³Ö,Ö»Ö§³ÖFAT12/FAT16/BigDOS/FAT32,ÐèÒªÓÉ´ÅÅÌ¹ÜÀí¹¤¾ßÖØÐÂ·ÖÇø */
#define	ERR_BPB_ERROR		0xA1			/* ´ÅÅÌÉÐÎ´¸ñÊ½»¯,»òÕß²ÎÊý´íÎó,ÐèÒªÓÉWINDOWS²ÉÓÃÄ¬ÈÏ²ÎÊýÖØÐÂ¸ñÊ½»¯ */
#define	ERR_DISK_FULL		0xB1			/* ´ÅÅÌÎÄ¼þÌ«Âú,Ê£Óà¿Õ¼äÌ«ÉÙ»òÕßÒÑ¾­Ã»ÓÐ,ÐèÒª´ÅÅÌÕûÀí */
#define	ERR_FDT_OVER		0xB2			/* Ä¿Â¼(ÎÄ¼þ¼Ð)ÄÚÎÄ¼þÌ«¶à,Ã»ÓÐ¿ÕÏÐµÄÄ¿Â¼Ïî,FAT12/FAT16¸ùÄ¿Â¼ÏÂµÄÎÄ¼þÊýÓ¦¸ÃÉÙÓÚ512¸ö,ÐèÒª´ÅÅÌÕûÀí */
#define	ERR_FILE_CLOSE		0xB4			/* ÎÄ¼þÒÑ¾­¹Ø±Õ,Èç¹ûÐèÒªÊ¹ÓÃ,Ó¦¸ÃÖØÐÂ´ò¿ªÎÄ¼þ */
#define	ERR_OPEN_DIR		0x41			/* Ö¸¶¨Â·¾¶µÄÄ¿Â¼(ÎÄ¼þ¼Ð)±»´ò¿ª */
#define	ERR_MISS_FILE		0x42			/* Ö¸¶¨Â·¾¶µÄÎÄ¼þÃ»ÓÐÕÒµ½,¿ÉÄÜÊÇÎÄ¼þÃû³Æ´íÎó */
#define	ERR_FOUND_NAME		0x43			/* ËÑË÷µ½ÏàÆ¥ÅäµÄÎÄ¼þÃû,»òÕßÊÇÒªÇó´ò¿ªÄ¿Â¼(ÎÄ¼þ¼Ð)¶øÊµ¼Ê½á¹ûÈ´´ò¿ªÁËÎÄ¼þ */
/* ÒÔÏÂÎÄ¼þÏµÍ³´íÎóÂëÓÃÓÚÎÄ¼þÏµÍ³×Ó³ÌÐò */
#define	ERR_MISS_DIR		0xB3			/* Ö¸¶¨Â·¾¶µÄÄ³¸ö×ÓÄ¿Â¼(ÎÄ¼þ¼Ð)Ã»ÓÐÕÒµ½,¿ÉÄÜÊÇÄ¿Â¼Ãû³Æ´íÎó */
#define	ERR_LONG_BUF_OVER	0x48			/* ³¤ÎÄ¼þ»º³åÇøÒç³ö */
#define	ERR_LONG_NAME_ERR	0x49			/* ¶ÌÎÄ¼þÃûÃ»ÓÐ¶ÔÓ¦µÄ³¤ÎÄ¼þÃû»òÕß³¤ÎÄ¼þÃû´íÎó */
#define	ERR_NAME_EXIST		0x4A			/* Í¬ÃûµÄ¶ÌÎÄ¼þÒÑ¾­´æÔÚ,½¨ÒéÖØÐÂÉú³ÉÁíÍâÒ»¸ö¶ÌÎÄ¼þÃû */
#endif

/* ********************************************************************************************************************* */
/* ÒÔÏÂ×´Ì¬´úÂëÓÃÓÚÖ÷»úÎÄ¼þÄ£Ê½ÏÂµÄ´ÅÅÌ¼°ÎÄ¼þ×´Ì¬, VAR_DISK_STATUS */
#ifndef	DEF_DISK_UNKNOWN
#define	DEF_DISK_UNKNOWN	0x00			/* ÉÐÎ´³õÊ¼»¯,Î´Öª×´Ì¬ */
#define	DEF_DISK_DISCONN	0x01			/* ´ÅÅÌÃ»ÓÐÁ¬½Ó»òÕßÒÑ¾­¶Ï¿ª */
#define	DEF_DISK_CONNECT	0x02			/* ´ÅÅÌÒÑ¾­Á¬½Ó,µ«ÊÇÉÐÎ´³õÊ¼»¯»òÕßÎÞ·¨Ê¶±ð¸Ã´ÅÅÌ */
#define	DEF_DISK_MOUNTED	0x03			/* ´ÅÅÌÒÑ¾­³õÊ¼»¯³É¹¦,µ«ÊÇÉÐÎ´·ÖÎöÎÄ¼þÏµÍ³»òÕßÎÄ¼þÏµÍ³²»Ö§³Ö */
#define	DEF_DISK_READY		0x10			/* ÒÑ¾­·ÖÎö´ÅÅÌµÄÎÄ¼þÏµÍ³²¢ÇÒÄÜ¹»Ö§³Ö */
#define	DEF_DISK_OPEN_ROOT	0x12			/* ÒÑ¾­´ò¿ª¸ùÄ¿Â¼,Ê¹ÓÃºó±ØÐë¹Ø±Õ,×¢ÒâFAT12/FAT16¸ùÄ¿Â¼ÊÇ¹Ì¶¨³¤¶È */
#define	DEF_DISK_OPEN_DIR	0x13			/* ÒÑ¾­´ò¿ª×ÓÄ¿Â¼(ÎÄ¼þ¼Ð) */
#define	DEF_DISK_OPEN_FILE	0x14			/* ÒÑ¾­´ò¿ªÎÄ¼þ */
#endif

/* ********************************************************************************************************************* */
/* ÎÄ¼þÏµÍ³³£ÓÃ¶¨Òå */

#ifndef	DEF_SECTOR_SIZE
#define	DEF_SECTOR_SIZE		512				/* UÅÌ»òÕßSD¿¨Ä¬ÈÏµÄÎïÀíÉÈÇøµÄ´óÐ¡ */
#endif

#ifndef	DEF_WILDCARD_CHAR
#define	DEF_WILDCARD_CHAR	0x2A			/* Â·¾¶ÃûµÄÍ¨Åä·û '*' */
#define	DEF_SEPAR_CHAR1		0x5C			/* Â·¾¶ÃûµÄ·Ö¸ô·û '\' */
#define	DEF_SEPAR_CHAR2		0x2F			/* Â·¾¶ÃûµÄ·Ö¸ô·û '/' */
#define	DEF_FILE_YEAR		2004			/* Ä¬ÈÏÎÄ¼þÈÕÆÚ: 2004Äê */
#define	DEF_FILE_MONTH		1				/* Ä¬ÈÏÎÄ¼þÈÕÆÚ: 1ÔÂ */
#define	DEF_FILE_DATE		1				/* Ä¬ÈÏÎÄ¼þÈÕÆÚ: 1ÈÕ */
#endif

#ifndef	ATTR_DIRECTORY

/* FATÊý¾ÝÇøÖÐÎÄ¼þÄ¿Â¼ÐÅÏ¢ */
typedef struct _FAT_DIR_INFO {
	unsigned char	DIR_Name[11];					/* 00H,ÎÄ¼þÃû,¹²11×Ö½Ú,²»×ã´¦Ìî¿Õ¸ñ */
	unsigned char	DIR_Attr;						/* 0BH,ÎÄ¼þÊôÐÔ,²Î¿¼ºóÃæµÄËµÃ÷ */
	unsigned char	DIR_NTRes;						/* 0CH */
	unsigned char	DIR_CrtTimeTenth;				/* 0DH,ÎÄ¼þ´´½¨µÄÊ±¼ä,ÒÔ0.1Ãëµ¥Î»¼ÆÊý */
	unsigned short	DIR_CrtTime;					/* 0EH,ÎÄ¼þ´´½¨µÄÊ±¼ä */
	unsigned short	DIR_CrtDate;					/* 10H,ÎÄ¼þ´´½¨µÄÈÕÆÚ */
	unsigned short	DIR_LstAccDate;					/* 12H,×î½üÒ»´Î´æÈ¡²Ù×÷µÄÈÕÆÚ */
	unsigned short	DIR_FstClusHI;					/* 14H */
	unsigned short	DIR_WrtTime;					/* 16H,ÎÄ¼þÐÞ¸ÄÊ±¼ä,²Î¿¼Ç°ÃæµÄºêMAKE_FILE_TIME */
	unsigned short	DIR_WrtDate;					/* 18H,ÎÄ¼þÐÞ¸ÄÈÕÆÚ,²Î¿¼Ç°ÃæµÄºêMAKE_FILE_DATE */
	unsigned short	DIR_FstClusLO;					/* 1AH */
	unsigned int	DIR_FileSize;					/* 1CH,ÎÄ¼þ³¤¶È */
} FAT_DIR_INFO, *P_FAT_DIR_INFO;			/* 20H */

/* ÎÄ¼þÊôÐÔ */
#define ATTR_READ_ONLY			0x01		/* ÎÄ¼þÎªÖ»¶ÁÊôÐÔ */
#define ATTR_HIDDEN				0x02		/* ÎÄ¼þÎªÒþº¬ÊôÐÔ */
#define ATTR_SYSTEM				0x04		/* ÎÄ¼þÎªÏµÍ³ÊôÐÔ */
#define ATTR_VOLUME_ID			0x08		/* ¾í±ê */
#define ATTR_DIRECTORY			0x10		/* ×ÓÄ¿Â¼(ÎÄ¼þ¼Ð) */
#define ATTR_ARCHIVE			0x20		/* ÎÄ¼þÎª´æµµÊôÐÔ */
#define ATTR_LONG_NAME			(ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID)	/* ³¤ÎÄ¼þÃûÊôÐÔ */
#define ATTR_LONG_NAME_MASK		(ATTR_LONG_NAME | ATTR_DIRECTORY | ATTR_ARCHIVE)
/* ÎÄ¼þÊôÐÔ unsigned char */
/* bit0 bit1 bit2 bit3 bit4 bit5 bit6 bit7 */
/*  Ö»   Òþ   Ïµ   ¾í   Ä¿   ´æ   Î´¶¨Òå   */
/*  ¶Á   ²Ø   Í³   ±ê   Â¼   µµ            */
/* ÎÄ¼þÊ±¼ä unsigned short */
/* Time = (Hour<<11) + (Minute<<5) + (Second>>1) */
#define MAKE_FILE_TIME(h, m, s)	((h<<11) + (m<<5) + (s>>1))	/* Éú³ÉÖ¸¶¨Ê±·ÖÃëµÄÎÄ¼þÊ±¼äÊý¾Ý */
/* ÎÄ¼þÈÕÆÚ unsigned short */
/* Date = ((Year-1980)<<9) + (Month<<5) + Day */
#define MAKE_FILE_DATE(y, m, d)	(((y-1980)<<9) + (m<<5) + d)	/* Éú³ÉÖ¸¶¨ÄêÔÂÈÕµÄÎÄ¼þÈÕÆÚÊý¾Ý */

#define LONE_NAME_MAX_CHAR		(255*2)		/* ³¤ÎÄ¼þÃû×î¶à×Ö·ûÊý/×Ö½ÚÊý */
#define LONG_NAME_PER_DIR		(13*2)		/* ³¤ÎÄ¼þÃûÔÚÃ¿¸öÎÄ¼þÄ¿Â¼ÐÅÏ¢½á¹¹ÖÐµÄ×Ö·ûÊý/×Ö½ÚÊý */

#endif

/* ********************************************************************************************************************* */
/* SCSIÃüÁîºÍÊý¾ÝÊäÈëÊä³ö½á¹¹ */

#ifndef	SPC_CMD_INQUIRY

/* SCSIÃüÁîÂë */
#define SPC_CMD_INQUIRY			0x12
#define SPC_CMD_READ_CAPACITY	0x25
#define SPC_CMD_READ10			0x28
#define SPC_CMD_WRITE10			0x2A
#define SPC_CMD_TEST_READY		0x00
#define SPC_CMD_REQUEST_SENSE	0x03
#define SPC_CMD_MODESENSE6		0x1A
#define SPC_CMD_MODESENSE10		0x5A
#define SPC_CMD_START_STOP		0x1B

/* BulkOnlyÐ­ÒéµÄÃüÁî¿é */
typedef struct _BULK_ONLY_CBW {
	unsigned int	CBW_Sig;
	unsigned int	CBW_Tag;
	unsigned char	CBW_DataLen0;					/* 08H,ÊäÈë: Êý¾Ý´«Êä³¤¶È,¶ÔÓÚÊäÈëÊý¾ÝÆäÓÐÐ§ÖµÊÇ0µ½48,¶ÔÓÚÊä³öÊý¾ÝÆäÓÐÐ§ÖµÎª0µ½33 */
	unsigned char	CBW_DataLen1;
	unsigned short	CBW_DataLen2;
	unsigned char	CBW_Flag;						/* 0CH,ÊäÈë: ´«Êä·½ÏòµÈ±êÖ¾,Î»7Îª1ÔòÊäÈëÊý¾Ý,Î»Îª0ÔòÊä³öÊý¾Ý»òÕßÃ»ÓÐÊý¾Ý */
	unsigned char	CBW_LUN;
	unsigned char	CBW_CB_Len;						/* 0EH,ÊäÈë: ÃüÁî¿éµÄ³¤¶È,ÓÐÐ§ÖµÊÇ1µ½16 */
	unsigned char	CBW_CB_Buf[16];					/* 0FH,ÊäÈë: ÃüÁî¿é,¸Ã»º³åÇø×î¶àÎª16¸ö×Ö½Ú */
} BULK_ONLY_CBW, *P_BULK_ONLY_CBW;			/* BulkOnlyÐ­ÒéµÄÃüÁî¿é, ÊäÈëCBW½á¹¹ */

/* INQUIRYÃüÁîµÄ·µ»ØÊý¾Ý */
typedef struct _INQUIRY_DATA {
	unsigned char	DeviceType;					/* 00H, Éè±¸ÀàÐÍ */
	unsigned char	RemovableMedia;				/* 01H, Î»7Îª1ËµÃ÷ÊÇÒÆ¶¯´æ´¢ */
	unsigned char	Versions;					/* 02H, Ð­Òé°æ±¾ */
	unsigned char	DataFormatAndEtc;			/* 03H, Ö¸¶¨·µ»ØÊý¾Ý¸ñÊ½ */
	unsigned char	AdditionalLength;			/* 04H, ºóÐøÊý¾ÝµÄ³¤¶È */
	unsigned char	Reserved1;
	unsigned char	Reserved2;
	unsigned char	MiscFlag;					/* 07H, Ò»Ð©¿ØÖÆ±êÖ¾ */
	unsigned char	VendorIdStr[8];				/* 08H, ³§ÉÌÐÅÏ¢ */
	unsigned char	ProductIdStr[16];			/* 10H, ²úÆ·ÐÅÏ¢ */
	unsigned char	ProductRevStr[4];			/* 20H, ²úÆ·°æ±¾ */
} INQUIRY_DATA, *P_INQUIRY_DATA;		/* 24H */

/* REQUEST SENSEÃüÁîµÄ·µ»ØÊý¾Ý */
typedef struct _SENSE_DATA {
	unsigned char	ErrorCode;					/* 00H, ´íÎó´úÂë¼°ÓÐÐ§Î» */
	unsigned char	SegmentNumber;
	unsigned char	SenseKeyAndEtc;				/* 02H, Ö÷¼üÂë */
	unsigned char	Information0;
	unsigned char	Information1;
	unsigned char	Information2;
	unsigned char	Information3;
	unsigned char	AdditSenseLen;				/* 07H, ºóÐøÊý¾ÝµÄ³¤¶È */
	unsigned char	CmdSpecInfo[4];
	unsigned char	AdditSenseCode;				/* 0CH, ¸½¼Ó¼üÂë */
	unsigned char	AddSenCodeQual;				/* 0DH, ÏêÏ¸µÄ¸½¼Ó¼üÂë */
	unsigned char	FieldReplaUnit;
	unsigned char	SenseKeySpec[3];
} SENSE_DATA, *P_SENSE_DATA;			/* 12H */

#endif

/* ********************************************************************************************************************* */
/* Ö÷»úÎÄ¼þÄ£Ê½ÏÂµÄÊý¾ÝÊäÈëºÍÊä³ö½á¹¹ */

#ifndef	MAX_FILE_NAME_LEN

#define MAX_FILE_NAME_LEN		(13+1)		/* ÎÄ¼þÃû×î´ó³¤¶È,×î´ó³¤¶ÈÊÇ1¸ö¸ùÄ¿Â¼·û+8¸öÖ÷ÎÄ¼þÃû+1¸öÐ¡Êýµã+3¸öÀàÐÍÃû+½áÊø·û=14 */

/* ÃüÁîµÄÊäÈëÊý¾ÝºÍÊä³öÊý¾Ý */
typedef union _CH376_CMD_DATA {
	struct {
		unsigned char	mBuffer[ MAX_FILE_NAME_LEN ];
	} Default;

	INQUIRY_DATA	DiskMountInq;			/* ·µ»Ø: INQUIRYÃüÁîµÄ·µ»ØÊý¾Ý */
											/* CMD0H_DISK_MOUNT: ³õÊ¼»¯´ÅÅÌ²¢²âÊÔ´ÅÅÌÊÇ·ñ¾ÍÐ÷,Ê×´ÎÖ´ÐÐÊ± */

	FAT_DIR_INFO	OpenDirInfo;			/* ·µ»Ø: Ã¶¾Ùµ½µÄÎÄ¼þÄ¿Â¼ÐÅÏ¢ */
											/* CMD0H_FILE_OPEN: Ã¶¾ÙÎÄ¼þºÍÄ¿Â¼(ÎÄ¼þ¼Ð) */

	FAT_DIR_INFO	EnumDirInfo;			/* ·µ»Ø: Ã¶¾Ùµ½µÄÎÄ¼þÄ¿Â¼ÐÅÏ¢ */
											/* CMD0H_FILE_ENUM_GO: ¼ÌÐøÃ¶¾ÙÎÄ¼þºÍÄ¿Â¼(ÎÄ¼þ¼Ð) */

	struct {
		unsigned char	mUpdateFileSz;				/* ÊäÈë²ÎÊý: ÊÇ·ñÔÊÐí¸üÐÂÎÄ¼þ³¤¶È, 0Ôò½ûÖ¹¸üÐÂ³¤¶È */
	} FileCLose;							/* CMD1H_FILE_CLOSE: ¹Ø±Õµ±Ç°ÒÑ¾­´ò¿ªµÄÎÄ¼þ */

	struct {
		unsigned char	mDirInfoIndex;				/* ÊäÈë²ÎÊý: Ö¸¶¨ÐèÒª¶ÁÈ¡µÄÄ¿Â¼ÐÅÏ¢½á¹¹ÔÚÉÈÇøÄÚµÄË÷ÒýºÅ, 0FFHÔòÎªµ±Ç°ÒÑ¾­´ò¿ªµÄÎÄ¼þ */
	} DirInfoRead;							/* CMD1H_DIR_INFO_READ: ¶ÁÈ¡ÎÄ¼þµÄÄ¿Â¼ÐÅÏ¢ */

	union {
		unsigned int	mByteOffset;				/* ÊäÈë²ÎÊý: Æ«ÒÆ×Ö½ÚÊý,ÒÔ×Ö½ÚÎªµ¥Î»µÄÆ«ÒÆÁ¿(×Ü³¤¶È32Î»,µÍ×Ö½ÚÔÚÇ°) */
		unsigned int	mSectorLba;					/* ·µ»Ø: µ±Ç°ÎÄ¼þÖ¸Õë¶ÔÓ¦µÄ¾ø¶ÔÏßÐÔÉÈÇøºÅ,0FFFFFFFFHÔòÒÑµ½ÎÄ¼þÎ²(×Ü³¤¶È32Î»,µÍ×Ö½ÚÔÚÇ°) */
	} ByteLocate;							/* CMD4H_BYTE_LOCATE: ÒÔ×Ö½ÚÎªµ¥Î»ÒÆ¶¯µ±Ç°ÎÄ¼þÖ¸Õë */

	struct {
		unsigned short	mByteCount;					/* ÊäÈë²ÎÊý: ÇëÇó¶ÁÈ¡µÄ×Ö½ÚÊý(×Ü³¤¶È16Î»,µÍ×Ö½ÚÔÚÇ°) */
	} ByteRead;								/* CMD2H_BYTE_READ: ÒÔ×Ö½ÚÎªµ¥Î»´Óµ±Ç°Î»ÖÃ¶ÁÈ¡Êý¾Ý¿é */

	struct {
		unsigned short	mByteCount;					/* ÊäÈë²ÎÊý: ÇëÇóÐ´ÈëµÄ×Ö½ÚÊý(×Ü³¤¶È16Î»,µÍ×Ö½ÚÔÚÇ°) */
	} ByteWrite;							/* CMD2H_BYTE_WRITE: ÒÔ×Ö½ÚÎªµ¥Î»Ïòµ±Ç°Î»ÖÃÐ´ÈëÊý¾Ý¿é */

	union {
		unsigned int	mSectorOffset;				/* ÊäÈë²ÎÊý: Æ«ÒÆÉÈÇøÊý,ÒÔÉÈÇøÎªµ¥Î»µÄÆ«ÒÆÁ¿(×Ü³¤¶È32Î»,µÍ×Ö½ÚÔÚÇ°) */
		unsigned int	mSectorLba;					/* ·µ»Ø: µ±Ç°ÎÄ¼þÖ¸Õë¶ÔÓ¦µÄ¾ø¶ÔÏßÐÔÉÈÇøºÅ,0FFFFFFFFHÔòÒÑµ½ÎÄ¼þÎ²(×Ü³¤¶È32Î»,µÍ×Ö½ÚÔÚÇ°) */
	} SectorLocate;							/* CMD4H_SEC_LOCATE: ÒÔÉÈÇøÎªµ¥Î»ÒÆ¶¯µ±Ç°ÎÄ¼þÖ¸Õë */

	struct {
		unsigned char	mSectorCount;				/* ÊäÈë²ÎÊý: ÇëÇó¶ÁÈ¡µÄÉÈÇøÊý */
											/* ·µ»Ø: ÔÊÐí¶ÁÈ¡µÄÉÈÇøÊý */
		unsigned char	mReserved1;
		unsigned char	mReserved2;
		unsigned char	mReserved3;
		unsigned int	mStartSector;				/* ·µ»Ø: ÔÊÐí¶ÁÈ¡µÄÉÈÇø¿éµÄÆðÊ¼¾ø¶ÔÏßÐÔÉÈÇøºÅ(×Ü³¤¶È32Î»,µÍ×Ö½ÚÔÚÇ°) */
	} SectorRead;							/* CMD1H_SEC_READ: ÒÔÉÈÇøÎªµ¥Î»´Óµ±Ç°Î»ÖÃ¶ÁÈ¡Êý¾Ý¿é */

	struct {
		unsigned char	mSectorCount;				/* ÊäÈë²ÎÊý: ÇëÇóÐ´ÈëµÄÉÈÇøÊý */
											/* ·µ»Ø: ÔÊÐíÐ´ÈëµÄÉÈÇøÊý */
		unsigned char	mReserved1;
		unsigned char	mReserved2;
		unsigned char	mReserved3;
		unsigned int	mStartSector;				/* ·µ»Ø: ÔÊÐíÐ´ÈëµÄÉÈÇø¿éµÄÆðÊ¼¾ø¶ÔÏßÐÔÉÈÇøºÅ(×Ü³¤¶È32Î»,µÍ×Ö½ÚÔÚÇ°) */
	} SectorWrite;							/* CMD1H_SEC_WRITE: ÒÔÉÈÇøÎªµ¥Î»ÔÚµ±Ç°Î»ÖÃÐ´ÈëÊý¾Ý¿é */

	struct {
		unsigned int	mDiskSizeSec;				/* ·µ»Ø: Õû¸öÎïÀí´ÅÅÌµÄÜÉÈÇøÊý(×Ü³¤¶È32Î»,µÍ×Ö½ÚÔÚÇ°) */
	} DiskCapacity;							/* CMD0H_DISK_CAPACITY: ²éÑ¯´ÅÅÌÎïÀíÈÝÁ¿ */

	struct {
		unsigned int	mTotalSector;				/* ·µ»Ø: µ±Ç°Âß¼­ÅÌµÄ×ÜÉÈÇøÊý(×Ü³¤¶È32Î»,µÍ×Ö½ÚÔÚÇ°) */
		unsigned int	mFreeSector;				/* ·µ»Ø: µ±Ç°Âß¼­ÅÌµÄÊ£ÓàÉÈÇøÊý(×Ü³¤¶È32Î»,µÍ×Ö½ÚÔÚÇ°) */
		unsigned char	mDiskFat;					/* ·µ»Ø: µ±Ç°Âß¼­ÅÌµÄFATÀàÐÍ,1-FAT12,2-FAT16,3-FAT32 */
	} DiskQuery;							/* CMD_DiskQuery, ²éÑ¯´ÅÅÌÐÅÏ¢ */

	BULK_ONLY_CBW	DiskBocCbw;				/* ÊäÈë²ÎÊý: CBWÃüÁî½á¹¹ */
											/* CMD0H_DISK_BOC_CMD: ¶ÔUSB´æ´¢Æ÷Ö´ÐÐBulkOnly´«ÊäÐ­ÒéµÄÃüÁî */

	struct {
		unsigned char	mMaxLogicUnit;				/* ·µ»Ø: USB´æ´¢Æ÷µÄ×î´óÂß¼­µ¥ÔªºÅ */
	} DiskMaxLun;							/* CMD0H_DISK_MAX_LUN: ¿ØÖÆ´«Êä-»ñÈ¡USB´æ´¢Æ÷×î´óÂß¼­µ¥ÔªºÅ */

	INQUIRY_DATA	DiskInitInq;			/* ·µ»Ø: INQUIRYÃüÁîµÄ·µ»ØÊý¾Ý */
											/* CMD0H_DISK_INIT: ³õÊ¼»¯USB´æ´¢Æ÷ */

	INQUIRY_DATA	DiskInqData;			/* ·µ»Ø: INQUIRYÃüÁîµÄ·µ»ØÊý¾Ý */
											/* CMD0H_DISK_INQUIRY: ²éÑ¯USB´æ´¢Æ÷ÌØÐÔ */

	SENSE_DATA		ReqSenseData;			/* ·µ»Ø: REQUEST SENSEÃüÁîµÄ·µ»ØÊý¾Ý */
											/* CMD0H_DISK_R_SENSE: ¼ì²éUSB´æ´¢Æ÷´íÎó */

	struct {
		unsigned int	mDiskSizeSec;				/* ·µ»Ø: Õû¸öÎïÀí´ÅÅÌµÄ×ÜÉÈÇøÊý(×Ü³¤¶È32Î»,¸ß×Ö½ÚÔÚÇ°) */
	} DiskSize;								/* CMD0H_DISK_SIZE: »ñÈ¡USB´æ´¢Æ÷µÄÈÝÁ¿ */

	struct {
		unsigned int	mStartSector;				/* ÊäÈë²ÎÊý: LBAÉÈÇøµØÖ·(×Ü³¤¶È32Î»,µÍ×Ö½ÚÔÚÇ°) */
		unsigned char	mSectorCount;				/* ÊäÈë²ÎÊý: ÇëÇó¶ÁÈ¡µÄÉÈÇøÊý */
	} DiskRead;								/* CMD5H_DISK_READ: ´ÓUSB´æ´¢Æ÷¶ÁÊý¾Ý¿é(ÒÔÉÈÇøÎªµ¥Î») */

	struct {
		unsigned int	mStartSector;				/* ÊäÈë²ÎÊý: LBAÉÈÇøµØÖ·(×Ü³¤¶È32Î»,µÍ×Ö½ÚÔÚÇ°) */
		unsigned char	mSectorCount;				/* ÊäÈë²ÎÊý: ÇëÇóÐ´ÈëµÄÉÈÇøÊý */
	} DiskWrite;							/* CMD5H_DISK_WRITE: ÏòUSB´æ´¢Æ÷Ð´Êý¾Ý¿é(ÒÔÉÈÇøÎªµ¥Î») */
} CH376_CMD_DATA, *P_CH376_CMD_DATA;

#endif

/* ********************************************************************************************************************* */
/* Ö÷»úÎÄ¼þÄ£Ê½ÏÂµÄÎÄ¼þÏµÍ³±äÁ¿µÄµØÖ· */

#ifndef	VAR_FILE_SIZE

/* 8Î»/µ¥×Ö½Ú±äÁ¿ */
#define	VAR_SYS_BASE_INFO	0x20			/* µ±Ç°ÏµÍ³µÄ»ù±¾ÐÅÏ¢ */
/*           Î»6ÓÃÓÚÖ¸Ê¾USB´æ´¢Éè±¸µÄ×ÓÀà±ðSubClass-Code, Î»6Îª0ÔòËµÃ÷×ÓÀà±ðÎª6, Î»6Îª1ÔòËµÃ÷×ÓÀà±ðÊÇ·Ç6µÄÆäËüÖµ */
/*           Î»5ÓÃÓÚÖ¸Ê¾USBÉè±¸·½Ê½ÏÂµÄUSBÅäÖÃ×´Ì¬ºÍUSBÖ÷»ú·½Ê½ÏÂµÄUSBÉè±¸Á¬½Ó×´Ì¬ */
/*                USBÉè±¸·½Ê½ÏÂ, Î»5Îª1ÔòUSBÅäÖÃÍê³É, Î»5Î»0ÔòÉÐÎ´ÅäÖÃ */
/*                USBÖ÷»ú·½Ê½ÏÂ, Î»5Îª1ÔòUSB¶Ë¿Ú´æÔÚUSBÉè±¸, Î»5Î»0ÔòUSB¶Ë¿ÚÃ»ÓÐUSBÉè±¸ */
/*           Î»4ÓÃÓÚÖ¸Ê¾USBÉè±¸·½Ê½ÏÂµÄ»º³åÇøËø¶¨×´Ì¬, Î»4Îª1ÔòËµÃ÷USB»º³åÇø´¦ÓÚËø¶¨×´Ì¬, Î»6Îª1ÔòËµÃ÷ÒÑ¾­ÊÍ·Å */
/*           ÆäËüÎ», ±£Áô,ÇëÎðÐÞ¸Ä */
#define	VAR_RETRY_TIMES		0x25			/* USBÊÂÎñ²Ù×÷µÄÖØÊÔ´ÎÊý */
/*           Î»7Îª0ÔòÊÕµ½NAKÊ±²»ÖØÊÔ, Î»7Îª1Î»6Îª0ÔòÊÕµ½NAKÊ±ÎÞÏÞÖØÊÔ(¿ÉÒÔÓÃCMD_ABORT_NAKÃüÁî·ÅÆúÖØÊÔ), Î»7Îª1Î»6Îª1ÔòÊÕµ½NAKÊ±×î¶àÖØÊÔ3Ãë */
/*           Î»5~Î»0Îª³¬Ê±ºóµÄÖØÊÔ´ÎÊý */
#define	VAR_FILE_BIT_FLAG	0x26			/* Ö÷»úÎÄ¼þÄ£Ê½ÏÂµÄÎ»±êÖ¾ */
/*           Î»1ºÍÎ»0, Âß¼­ÅÌµÄFATÎÄ¼þÏµÍ³±êÖ¾, 00-FAT12, 01-FAT16, 10-FAT32, 11-·Ç·¨ */
/*           Î»2, µ±Ç°»º³åÇøÖÐµÄFAT±íÊý¾ÝÊÇ·ñ±»ÐÞ¸Ä±êÖ¾, 0-Î´ÐÞ¸Ä, 1-ÒÑÐÞ¸Ä */
/*           Î»3, ÎÄ¼þ³¤¶ÈÐèÒªÐÞ¸Ä±êÖ¾, µ±Ç°ÎÄ¼þ±»×·¼ÓÊý¾Ý, 0-Î´×·¼ÓÎÞÐèÐÞ¸Ä, 1-ÒÑ×·¼ÓÐèÒªÐÞ¸Ä */
/*           ÆäËüÎ», ±£Áô,ÇëÎðÐÞ¸Ä */
#define	VAR_DISK_STATUS		0x2B			/* Ö÷»úÎÄ¼þÄ£Ê½ÏÂµÄ´ÅÅÌ¼°ÎÄ¼þ×´Ì¬ */
#define	VAR_SD_BIT_FLAG		0x30			/* Ö÷»úÎÄ¼þÄ£Ê½ÏÂSD¿¨µÄÎ»±êÖ¾ */
/*           Î»0, SD¿¨°æ±¾, 0-Ö»Ö§³ÖSDµÚÒ»°æ,1-Ö§³ÖSDµÚ¶þ°æ */
/*           Î»1, ×Ô¶¯Ê¶±ð, 0-SD¿¨, 1-MMC¿¨ */
/*           Î»2, ×Ô¶¯Ê¶±ð, 0-±ê×¼ÈÝÁ¿SD¿¨, 1-´óÈÝÁ¿SD¿¨(HC-SD) */
/*           Î»4, ACMD41ÃüÁî³¬Ê± */
/*           Î»5, CMD1ÃüÁî³¬Ê± */
/*           Î»6, CMD58ÃüÁî³¬Ê± */
/*           ÆäËüÎ», ±£Áô,ÇëÎðÐÞ¸Ä */
#define	VAR_UDISK_TOGGLE	0x31			/* USB´æ´¢Éè±¸µÄBULK-IN/BULK-OUT¶ËµãµÄÍ¬²½±êÖ¾ */
/*           Î»7, Bulk-In¶ËµãµÄÍ¬²½±êÖ¾ */
/*           Î»6, Bulk-In¶ËµãµÄÍ¬²½±êÖ¾ */
/*           Î»5~Î»0, ±ØÐëÎª0 */
#define	VAR_UDISK_LUN		0x34			/* USB´æ´¢Éè±¸µÄÂß¼­µ¥ÔªºÅ */
/*           Î»7~Î»4, USB´æ´¢Éè±¸µÄµ±Ç°Âß¼­µ¥ÔªºÅ,CH376³õÊ¼»¯USB´æ´¢Éè±¸ºó,Ä¬ÈÏÊÇ·ÃÎÊ0#Âß¼­µ¥Ôª */
/*           Î»3~Î»0, USB´æ´¢Éè±¸µÄ×î´óÂß¼­µ¥ÔªºÅ,¼Ó1ºóµÈÓÚÂß¼­µ¥ÔªÊý */
#define	VAR_SEC_PER_CLUS	0x38			/* Âß¼­ÅÌµÄÃ¿´ØÉÈÇøÊý */
#define	VAR_FILE_DIR_INDEX	0x3B			/* µ±Ç°ÎÄ¼þÄ¿Â¼ÐÅÏ¢ÔÚÉÈÇøÄÚµÄË÷ÒýºÅ */
#define	VAR_CLUS_SEC_OFS	0x3C			/* µ±Ç°ÎÄ¼þÖ¸ÕëÔÚ´ØÄÚµÄÉÈÇøÆ«ÒÆ,Îª0xFFÔòÖ¸ÏòÎÄ¼þÄ©Î²,´Ø½áÊø */

/* 32Î»/4×Ö½Ú±äÁ¿ */
#define	VAR_DISK_ROOT		0x44			/* ¶ÔÓÚFAT16ÅÌÎª¸ùÄ¿Â¼Õ¼ÓÃÉÈÇøÊý,¶ÔÓÚFAT32ÅÌÎª¸ùÄ¿Â¼ÆðÊ¼´ØºÅ(×Ü³¤¶È32Î»,µÍ×Ö½ÚÔÚÇ°) */
#define	VAR_DSK_TOTAL_CLUS	0x48			/* Âß¼­ÅÌµÄ×Ü´ØÊý(×Ü³¤¶È32Î»,µÍ×Ö½ÚÔÚÇ°) */
#define	VAR_DSK_START_LBA	0x4C			/* Âß¼­ÅÌµÄÆðÊ¼¾ø¶ÔÉÈÇøºÅLBA(×Ü³¤¶È32Î»,µÍ×Ö½ÚÔÚÇ°) */
#define	VAR_DSK_DAT_START	0x50			/* Âß¼­ÅÌµÄÊý¾ÝÇøÓòµÄÆðÊ¼LBA(×Ü³¤¶È32Î»,µÍ×Ö½ÚÔÚÇ°) */
#define	VAR_LBA_BUFFER		0x54			/* µ±Ç°´ÅÅÌÊý¾Ý»º³åÇøµÄÊý¾Ý¶ÔÓ¦µÄLBA(×Ü³¤¶È32Î»,µÍ×Ö½ÚÔÚÇ°) */
#define	VAR_LBA_CURRENT		0x58			/* µ±Ç°¶ÁÐ´µÄ´ÅÅÌÆðÊ¼LBAµØÖ·(×Ü³¤¶È32Î»,µÍ×Ö½ÚÔÚÇ°) */
#define	VAR_FAT_DIR_LBA		0x5C			/* µ±Ç°ÎÄ¼þÄ¿Â¼ÐÅÏ¢ËùÔÚµÄÉÈÇøLBAµØÖ·(×Ü³¤¶È32Î»,µÍ×Ö½ÚÔÚÇ°) */
#define	VAR_START_CLUSTER	0x60			/* µ±Ç°ÎÄ¼þ»òÕßÄ¿Â¼(ÎÄ¼þ¼Ð)µÄÆðÊ¼´ØºÅ(×Ü³¤¶È32Î»,µÍ×Ö½ÚÔÚÇ°) */
#define	VAR_CURRENT_CLUST	0x64			/* µ±Ç°ÎÄ¼þµÄµ±Ç°´ØºÅ(×Ü³¤¶È32Î»,µÍ×Ö½ÚÔÚÇ°) */
#define	VAR_FILE_SIZE		0x68			/* µ±Ç°ÎÄ¼þµÄ³¤¶È(×Ü³¤¶È32Î»,µÍ×Ö½ÚÔÚÇ°) */
#define	VAR_CURRENT_OFFSET	0x6C			/* µ±Ç°ÎÄ¼þÖ¸Õë,µ±Ç°¶ÁÐ´Î»ÖÃµÄ×Ö½ÚÆ«ÒÆ(×Ü³¤¶È32Î»,µÍ×Ö½ÚÔÚÇ°) */

#endif

/* ********************************************************************************************************************* */
/* ³£ÓÃUSB¶¨Òå */

/* USBµÄ°ü±êÊ¶PID, Ö÷»ú·½Ê½¿ÉÄÜÓÃµ½ */
#ifndef	DEF_USB_PID_SETUP
#define	DEF_USB_PID_NULL	0x00			/* ±£ÁôPID, Î´¶¨Òå */
#define	DEF_USB_PID_SOF		0x05
#define	DEF_USB_PID_SETUP	0x0D
#define	DEF_USB_PID_IN		0x09
#define	DEF_USB_PID_OUT		0x01
#define	DEF_USB_PID_ACK		0x02
#define	DEF_USB_PID_NAK		0x0A
#define	DEF_USB_PID_STALL	0x0E
#define	DEF_USB_PID_DATA0	0x03
#define	DEF_USB_PID_DATA1	0x0B
#define	DEF_USB_PID_PRE		0x0C
#endif

/* USBÇëÇóÀàÐÍ, ÍâÖÃ¹Ì¼þÄ£Ê½¿ÉÄÜÓÃµ½ */
#ifndef	DEF_USB_REQ_TYPE
#define	DEF_USB_REQ_READ	0x80			/* ¿ØÖÆ¶Á²Ù×÷ */
#define	DEF_USB_REQ_WRITE	0x00			/* ¿ØÖÆÐ´²Ù×÷ */
#define	DEF_USB_REQ_TYPE	0x60			/* ¿ØÖÆÇëÇóÀàÐÍ */
#define	DEF_USB_REQ_STAND	0x00			/* ±ê×¼ÇëÇó */
#define	DEF_USB_REQ_CLASS	0x20			/* Éè±¸ÀàÇëÇó */
#define	DEF_USB_REQ_VENDOR	0x40			/* ³§ÉÌÇëÇó */
#define	DEF_USB_REQ_RESERVE	0x60			/* ±£ÁôÇëÇó */
#endif

/* USB±ê×¼Éè±¸ÇëÇó, RequestTypeµÄÎ»6Î»5=00(Standard), ÍâÖÃ¹Ì¼þÄ£Ê½¿ÉÄÜÓÃµ½ */
#ifndef	DEF_USB_GET_DESCR
#define	DEF_USB_CLR_FEATURE	0x01
#define	DEF_USB_SET_FEATURE	0x03
#define	DEF_USB_GET_STATUS	0x00
#define	DEF_USB_SET_ADDRESS	0x05
#define	DEF_USB_GET_DESCR	0x06
#define	DEF_USB_SET_DESCR	0x07
#define	DEF_USB_GET_CONFIG	0x08
#define	DEF_USB_SET_CONFIG	0x09
#define	DEF_USB_GET_INTERF	0x0A
#define	DEF_USB_SET_INTERF	0x0B
#define	DEF_USB_SYNC_FRAME	0x0C
#endif

/* ********************************************************************************************************************* */

#ifdef __cplusplus
}
#endif

#endif
