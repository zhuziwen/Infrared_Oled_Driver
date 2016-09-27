/*
***********************************************************************
*文件名称：oled.h
*实现功能：oled iic协议的演示例程
*          GND-接地
*          VCC-接5v或者3.3v
*          D0 -接PB13(SCL) 
*          D1 -接PB15(SDA)
*          RES-接PB1
*          DC -接PB0
*          CS -接PB12
*作    者：zzw
*创建日期：2016.09.25
*修改内容：创建文件 
*修改原因：无
*修 改 者：无 
*修改日期：无
*代码版本：v1.0 
***********************************************************************
*/
#ifndef _OLED_H
#define _OLED_H

#include "sys.h"

//定义的是写数据还是写命令“常量”只读变量

/*
 * 需要用到的引脚，宏定义,这么写方便移植
 */
#define OLED_PORT GPIOB
#define OLED_RCC RCC_APB2Periph_GPIOB
#define OLED_DC_PIN GPIO_Pin_0
#define OLED_RES_PIN GPIO_Pin_1
#define OLED_CS_PIN GPIO_Pin_12
#define OLED_D0_CLK_PIN GPIO_Pin_13
#define OLED_D1_DAT_PIN GPIO_Pin_15

/*
 * 宏定义上述引脚高低电平 方便移植
 */
#define OLED_CLK_LOW() GPIO_ResetBits(OLED_PORT,OLED_D0_CLK_PIN)
#define OLED_CLK_HIGH() GPIO_SetBits(OLED_PORT,OLED_D0_CLK_PIN)

#define OLED_DAT_LOW() GPIO_ResetBits(OLED_PORT,OLED_D1_DAT_PIN)
#define OLED_DAT_HIGH() GPIO_SetBits(OLED_PORT,OLED_D1_DAT_PIN)

#define OLED_RES_LOW()  GPIO_ResetBits(OLED_PORT,OLED_RES_PIN)//
#define OLED_RES_HIGH() GPIO_SetBits(OLED_PORT,OLED_RES_PIN)

#define OLED_DC_LOW() GPIO_ResetBits(OLED_PORT,OLED_DC_PIN)
#define OLED_DC_HIGH() GPIO_SetBits(OLED_PORT,OLED_DC_PIN)

#define OLED_CS_LOW() GPIO_ResetBits(OLED_PORT,OLED_CS_PIN)
#define OLED_CS_HIGH() GPIO_SetBits(OLED_PORT,OLED_CS_PIN)

/*
 * 文件中写的函数的声明
 */
void OLED_WrByte(u8 dat,u8 cmd);	    
void OLED_DisplayOn(void);
void OLED_DisplayOff(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p);	 
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);


#endif
