/*
***********************************************************************
*�ļ����ƣ�infrared.c infrared.h
*ʵ�ֹ��ܣ�����Թ����ڼ���Ƿ���Ӳ��ͨ��
*          ����Ӳ��ͨ����ʱ�򣬺���Թ�����ߵ�ƽ
*          ͨ����1ms�жϽ���ɨ�裬����ɨ��16�Σ�16ms��
*          ���ͨ���жϱ�����ֵ���Ϳ����ж�Ӳ���Ƿ�ͨ��
*          �ź����out - PC0��������ſ�����ͷ�ļ��н����޸�
*ʹ��˵������ǰ�ļ���Ҫ��timer2һ��ʹ�ã������ж�����
*��    �ߣ�zzw
*�������ڣ�2016.09.26
*�޸����ݣ������ļ� 
*�޸�ԭ����
*�� �� �ߣ��� 
*�޸����ڣ���
*����汾��v1.0 
***********************************************************************
*ȫ�ֱ�����coinInFlag��¼�Ƿ���Ӳ�ҽ���
*ȫ�־�̬�� 
*�ֲ���̬��infraredBackUp��¼��һ�εİ���flag 
*		   infraredBuf ÿ��1ms����8�λ������Թܵ�ֵ
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
