/*
***********************************************************************
*�ļ����ƣ�oled.h
*ʵ�ֹ��ܣ�oled iicЭ�����ʾ����
*          GND-�ӵ�
*          VCC-��5v����3.3v
*          D0 -��PB13(SCL) 
*          D1 -��PB15(SDA)
*          RES-��PB1
*          DC -��PB0
*          CS -��PB12
*��    �ߣ�zzw
*�������ڣ�2016.09.25
*�޸����ݣ������ļ� 
*�޸�ԭ����
*�� �� �ߣ��� 
*�޸����ڣ���
*����汾��v1.0 
***********************************************************************
*/
#ifndef _OLED_H
#define _OLED_H

#include "sys.h"

//�������д���ݻ���д���������ֻ������

/*
 * ��Ҫ�õ������ţ��궨��,��ôд������ֲ
 */
#define OLED_PORT GPIOB
#define OLED_RCC RCC_APB2Periph_GPIOB
#define OLED_DC_PIN GPIO_Pin_0
#define OLED_RES_PIN GPIO_Pin_1
#define OLED_CS_PIN GPIO_Pin_12
#define OLED_D0_CLK_PIN GPIO_Pin_13
#define OLED_D1_DAT_PIN GPIO_Pin_15

/*
 * �궨���������Ÿߵ͵�ƽ ������ֲ
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
 * �ļ���д�ĺ���������
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
