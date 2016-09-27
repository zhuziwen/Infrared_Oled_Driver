#include "timer2.h"

uint8_t time200msFlag = 0;
uint8_t time1msFlag = 0;
/*
**************************************************************************
*函数功能：定时器2中断向量初始化函数
*入口参数：无
返  回 值：无
*使用说明：在使用定时器中断之前，必须先调用这个函数进行初始化
*          本人之间在下面的定时器配置函数中进行了调用，所以别的地方不需要调用了
*作    者：朱子文
*代码版本：v1.0.0
*修改原因：无
**************************************************************************
*/
void TIM2_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
**************************************************************************
*函数功能：1ms定时器2相关配置函数
*入口参数：无
返  回 值：无
*使用说明：在使用定时器2之前，必须先调用这个函数进行初始化
*          注意：由于函数初始化完成之后，关闭了相关时钟，
*                所以在使用的地方需要开一下时钟使能！！
*作    者：朱子文
*代码版本：v1.0.0
*修改原因：无
*修改时间：2017.7.17
**************************************************************************
*/
/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--71 
 * 中断周期为 = 1/(72MHZ /72) * 1000 = 1ms
 
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> 中断 且TIMxCNT重置为0重新计数 
 * 产生1ms定时器
 */
void TIM2_Configuration1ms(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//调用了中断配置函数，进行中断配置初始化，必须在下面关闭时钟之前进行调用
    TIM2_NVIC_Configuration();
	/* 设置TIM2CLK 为 72MHZ */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
    //TIM_DeInit(TIM2);
	
	/* 自动重装载寄存器周期的值(计数值) */
    TIM_TimeBaseStructure.TIM_Period = 1000;
	
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
	/* 时钟预分频数为72 */
    TIM_TimeBaseStructure.TIM_Prescaler = 71;
	
	/* 对外部时钟进行采样的时钟分频,这里没有用到 */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	
    /*设置定时器为向上计数模式*/
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
    
    //进行相应初始化
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
		
    TIM_Cmd(TIM2, ENABLE);																		
    
}

/*
**************************************************************************
*函数功能：1ms定时器2中断服务函数
*入口参数：无
返  回 值：无
*使用说明：当定时器计数器溢出的时候，自动调用此服务函数。这个函数用于设置标志位
*作    者：朱子文
*代码版本：v1.0.0
*修改原因：无
**************************************************************************
*/
void TIM2_IRQHandler()
{
    static u16 time = 0;
    if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
    {
        time1msFlag = 1;
				if(time >= 200)
        {
            time = 0;
            time200msFlag = 1;
        }else{
            time++;
            time200msFlag = 0;
        }
        //清楚一下中断的溢出标志位，便于下一次使用
        TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);
    }
}
/*********************************************END OF FILE**********************/





