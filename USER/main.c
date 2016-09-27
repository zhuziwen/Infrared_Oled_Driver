/*
***********************************************************************
*文件名称：main.c主逻辑文件
*实现功能：该文件用于测试红外对管计数功能的实现
*使用说明：配合oled12864显示屏，infrared红外对管timer2定时器2
*作    者：zzw
*创建日期：2016.09.26
*修改内容：创建文件 
*修改原因：无
*修 改 者：无 
*修改日期：无
*代码版本：v1.0 
***********************************************************************
*全局变量money记录当前的钱的数量
*全局静态：无
*局部静态：无
***********************************************************************
*/

#include "sys.h"
#include "oled.h"
#include "infrared.h"
#include "timer2.h"
#include "delay.h"

uint8_t money = 0;

void Update_View(void);

int main(void)
{
    OLED_Init();			//初始化OLED  
    TIM2_Configuration1ms();
    Infrared_Init();
    OLED_ShowNum(0,2,55,2,16);
    
    while(1)
    {
        Infrared_Driver();
		Update_View();
    }
   
}

void Infrared_Action(void)
{
	money++;
}

/*
**********************************************************************
*函 数 名：Update_View()
*函数功能：用于更新数据
*入口参数：无
*返 回 值：无 
*作    者：zzw
*创建日期：2016.09.26
*修改日期：无
*修改原因：无 
*修 改 者：无 
*函数版本：v1.0 
**********************************************************************
*/
void Update_View(void)
{
	if(time1msFlag)
	{
		time1msFlag = 0;
		Infrared_Scan();
	}
	if(time200msFlag)
	{
		OLED_ShowNum(0,0,money,2,16);
	}
}

