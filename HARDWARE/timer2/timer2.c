#include "timer2.h"

uint8_t time200msFlag = 0;
uint8_t time1msFlag = 0;
/*
**************************************************************************
*�������ܣ���ʱ��2�ж�������ʼ������
*��ڲ�������
��  �� ֵ����
*ʹ��˵������ʹ�ö�ʱ���ж�֮ǰ�������ȵ�������������г�ʼ��
*          ����֮��������Ķ�ʱ�����ú����н����˵��ã����Ա�ĵط�����Ҫ������
*��    �ߣ�������
*����汾��v1.0.0
*�޸�ԭ����
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
*�������ܣ�1ms��ʱ��2������ú���
*��ڲ�������
��  �� ֵ����
*ʹ��˵������ʹ�ö�ʱ��2֮ǰ�������ȵ�������������г�ʼ��
*          ע�⣺���ں�����ʼ�����֮�󣬹ر������ʱ�ӣ�
*                ������ʹ�õĵط���Ҫ��һ��ʱ��ʹ�ܣ���
*��    �ߣ�������
*����汾��v1.0.0
*�޸�ԭ����
*�޸�ʱ�䣺2017.7.17
**************************************************************************
*/
/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--71 
 * �ж�����Ϊ = 1/(72MHZ /72) * 1000 = 1ms
 
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> �ж� ��TIMxCNT����Ϊ0���¼��� 
 * ����1ms��ʱ��
 */
void TIM2_Configuration1ms(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//�������ж����ú����������ж����ó�ʼ��������������ر�ʱ��֮ǰ���е���
    TIM2_NVIC_Configuration();
	/* ����TIM2CLK Ϊ 72MHZ */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
    //TIM_DeInit(TIM2);
	
	/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    TIM_TimeBaseStructure.TIM_Period = 1000;
	
    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
	/* ʱ��Ԥ��Ƶ��Ϊ72 */
    TIM_TimeBaseStructure.TIM_Prescaler = 71;
	
	/* ���ⲿʱ�ӽ��в�����ʱ�ӷ�Ƶ,����û���õ� */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	
    /*���ö�ʱ��Ϊ���ϼ���ģʽ*/
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
    
    //������Ӧ��ʼ��
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
		
    TIM_Cmd(TIM2, ENABLE);																		
    
}

/*
**************************************************************************
*�������ܣ�1ms��ʱ��2�жϷ�����
*��ڲ�������
��  �� ֵ����
*ʹ��˵��������ʱ�������������ʱ���Զ����ô˷���������������������ñ�־λ
*��    �ߣ�������
*����汾��v1.0.0
*�޸�ԭ����
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
        //���һ���жϵ������־λ��������һ��ʹ��
        TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);
    }
}
/*********************************************END OF FILE**********************/





