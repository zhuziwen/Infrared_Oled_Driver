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

#include "infrared.h"
#include "timer2.h"

extern void Infrared_Action(void);
//记录是否有硬币进入标志位
uint8_t coinInFlag = 0;
/*
**********************************************************************
*函 数 名：Infrared_Init(void)
*函数功能：初始化红外对管的引脚（只有引脚为高电平的时候才能读取外接信号）
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
void Infrared_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(INFRARED_RCC,ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = INFRARED_OUT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		        //设置io口引脚为下拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           //速度50MHz
 	GPIO_Init(INFRARED_PORT, &GPIO_InitStructure);	            //初始化GPIOC0
    //将pc0io口初始化为高电平
    INFRARED_HIGH();
    
}
/*
**********************************************************************
*函 数 名：Infrared_Driver(void)
*函数功能：红外对管驱动函数，在主函数循环中调用
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
void Infrared_Driver(void)
{
    static uint8_t infraredBackUp = 0;	//这个静态变量用于记录上一次的硬币进入flag

    if(infraredBackUp != coinInFlag)	
    {
        if(infraredBackUp != 1)			//说明有硬币进入
        {
            Infrared_Action();
        }
    }
    infraredBackUp = coinInFlag;
}
/*
**********************************************************************
*函 数 名：Infrared_Scan(void)
*函数功能：红外对管扫描函数，每1ms进入一次，连续进8次也就是8ms，用于消抖
*		   利用的是中断消抖算法
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
void Infrared_Scan(void)
{
    static uint16_t infraredBuf = 0x00;
    infraredBuf = ((infraredBuf << 1) | GPIO_ReadInputDataBit(INFRARED_PORT,INFRARED_OUT_PIN));
    //连续8次读取为高电平，代表硬币确实投下
    if(infraredBuf == 0xff)
    {
        coinInFlag = 1;
    }else if(infraredBuf == 0x00){	//如果8次之后，infraredBuf为0x00那么久认为没有硬币进入
        coinInFlag = 0;
    }
    INFRARED_HIGH();				//引脚必须为高电平，才能读取外界电平状态
    
}






