/*
***********************************************************************
*文件名称：infrared.c infrared.h
*实现功能：红外对管用于检测是否有硬币通过
*          当有硬币通过的时候，红外对管输出高电平
*          通过用1ms中断进行扫描，连续扫描16次（16ms）
*          最后通过判断变量的值，就可以判断硬币是否通过
*          信号输出out - PC0，这个引脚可以在头文件中进行修改
*使用说明：当前文件需要和timer2一起使用，用于中断消抖
*作    者：zzw
*创建日期：2016.09.26
*修改内容：创建文件 
*修改原因：无
*修 改 者：无 
*修改日期：无
*代码版本：v1.0 
***********************************************************************
*全局变量：coinInFlag记录是否有硬币进入
*全局静态： 
*局部静态：infraredBackUp记录上一次的按键flag 
*		   infraredBuf 每隔1ms连续8次缓存红外对管的值
***********************************************************************
*/

#ifndef _INFRARED_H
#define _INFRARED_H

#include "sys.h"

#define INFRARED_RCC RCC_APB2Periph_GPIOC
#define INFRARED_OUT_PIN GPIO_Pin_0
#define INFRARED_PORT GPIOC
#define INFRARED_HIGH() GPIO_SetBits(INFRARED_PORT,INFRARED_OUT_PIN)

extern uint8_t time200msFlag;

void Infrared_Init(void);
void Infrared_Driver(void);
void Infrared_Scan(void);


#endif
