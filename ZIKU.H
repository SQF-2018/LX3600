#ifndef __ZIKU_H
#define __ZIKU_H

/*******************************************************************************************************************************
8*8点阵图形、ASCII字符库
取模方式：横向8点，左高位；上到下，左到右
字模实际为5*8点阵，右边三点无用，实际按照6*8扫描，这样每个字符之间会有一个像素的空隙
*******************************************************************************************************************************/
extern const unsigned char DOT_ARRAY_68[][8];

/*******************************************************************************************************************************
6*12点阵图形、ASCII字符库
取模方式：横向8点，左高位；上到下，左到右
*******************************************************************************************************************************/
extern const unsigned char DOT_ARRAY_612[][12];

/*******************************************************************************************************************************
8*16点阵图形ASCII字符库
取模方式：横向8点，左高位；上到下，左到右
*******************************************************************************************************************************/
extern const unsigned char DOT_ARRAY_816[][16];

/*******************************************************************************************************************************
12*24点阵图形、ASCII字符库
取模方式：横向8点，右高位；左到右，上到下
*******************************************************************************************************************************/
extern const unsigned char DOT_ARRAY_1224[][48];

/*******************************************************************************************************************************
16*32点阵图形、ASCII字符库
取模方式：横向8点，右高位；左到右，上到下
*******************************************************************************************************************************/
extern const unsigned char DOT_ARRAY_1632[][64];

/*******************************************************************************************************************************
24*48点阵图形、ASCII字符库
取模方式：横向8点，左高位；左到右，上到下
*******************************************************************************************************************************/
extern const unsigned char DOT_ARRAY_2448[][144];

/*******************************************************************************************************************************
32*64点阵图形、ASCII字符库
取模方式：横向8点，左高位；左到右，上到下
每个字符点阵的第一个字节是索引字节，用于快速查找字符
*******************************************************************************************************************************/
extern const unsigned char DOT_ARRAY_3264[][257];

/*******************************************************************************************************************************
48*24点阵字库
取模方式：横向8点，右高位；左到右，上到下
*******************************************************************************************************************************/
extern const unsigned char DOT_ARRAY_4824[][144];

/*******************************************************************************************************************************
24*24点阵图标、汉字字库，带区位码索引字节
取模方式：横向8点，右高位；左到右，上到下
*******************************************************************************************************************************/
extern const unsigned char DOT_ARRAY_2424[][74];

/*******************************************************************************************************************************
32*32点阵图标、汉字字库，带区位码索引字节
取模方式：横向8点，右高位；左到右，上到下
*******************************************************************************************************************************/
extern const unsigned char DOT_ARRAY_3232[][130];

/*******************************************************************************************************************************
48*48点阵图标、汉字字库，带区位码索引字节
取模方式：横向8点，左高位；左到右，上到下
*******************************************************************************************************************************/
extern const unsigned char DOT_ARRAY_4848[][290];

/*******************************************************************************************************************************
力兴、伏安图标400×275点阵，单色
取模方式：横向8点，右高位；左到右，上到下
*******************************************************************************************************************************/
extern const unsigned char COMPANY_LOGO[13750];

/*****************************************************************************************************************************
PT接线图224×175点阵，单色
*****************************************************************************************************************************/
extern const unsigned char IMAGE_PT[][12056];

/*************************************************************************************************************************************
取32×64点阵字符字库字符数量
*************************************************************************************************************************************/
extern unsigned int Character3264_Number(void);

/*************************************************************************************************************************************
取48×48点阵汉字字库汉字数量
*************************************************************************************************************************************/
extern unsigned int HZDot48_Number(void);

#endif
