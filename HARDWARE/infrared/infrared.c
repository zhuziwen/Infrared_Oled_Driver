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

#include "infrared.h"
#include "timer2.h"

extern void Infrared_Action(void);
//��¼�Ƿ���Ӳ�ҽ����־λ
uint8_t coinInFlag = 0;
/*
**********************************************************************
*�� �� ����Infrared_Init(void)
*�������ܣ���ʼ������Թܵ����ţ�ֻ������Ϊ�ߵ�ƽ��ʱ����ܶ�ȡ����źţ�
*��ڲ�������
*�� �� ֵ���� 
*��    �ߣ�zzw
*�������ڣ�2016.09.26
*�޸����ڣ���
*�޸�ԭ���� 
*�� �� �ߣ��� 
*�����汾��v1.0 
**********************************************************************
*/
void Infrared_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(INFRARED_RCC,ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = INFRARED_OUT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		        //����io������Ϊ��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           //�ٶ�50MHz
 	GPIO_Init(INFRARED_PORT, &GPIO_InitStructure);	            //��ʼ��GPIOC0
    //��pc0io�ڳ�ʼ��Ϊ�ߵ�ƽ
    INFRARED_HIGH();
    
}
/*
**********************************************************************
*�� �� ����Infrared_Driver(void)
*�������ܣ�����Թ�������������������ѭ���е���
*��ڲ�������
*�� �� ֵ���� 
*��    �ߣ�zzw
*�������ڣ�2016.09.26
*�޸����ڣ���
*�޸�ԭ���� 
*�� �� �ߣ��� 
*�����汾��v1.0 
**********************************************************************
*/
void Infrared_Driver(void)
{
    static uint8_t infraredBackUp = 0;	//�����̬�������ڼ�¼��һ�ε�Ӳ�ҽ���flag

    if(infraredBackUp != coinInFlag)	
    {
        if(infraredBackUp != 1)			//˵����Ӳ�ҽ���
        {
            Infrared_Action();
        }
    }
    infraredBackUp = coinInFlag;
}
/*
**********************************************************************
*�� �� ����Infrared_Scan(void)
*�������ܣ�����Թ�ɨ�躯����ÿ1ms����һ�Σ�������8��Ҳ����8ms����������
*		   ���õ����ж������㷨
*��ڲ�������
*�� �� ֵ���� 
*��    �ߣ�zzw
*�������ڣ�2016.09.26
*�޸����ڣ���
*�޸�ԭ���� 
*�� �� �ߣ��� 
*�����汾��v1.0 
**********************************************************************
*/
void Infrared_Scan(void)
{
    static uint16_t infraredBuf = 0x00;
    infraredBuf = ((infraredBuf << 1) | GPIO_ReadInputDataBit(INFRARED_PORT,INFRARED_OUT_PIN));
    //����8�ζ�ȡΪ�ߵ�ƽ������Ӳ��ȷʵͶ��
    if(infraredBuf == 0xff)
    {
        coinInFlag = 1;
    }else if(infraredBuf == 0x00){	//���8��֮��infraredBufΪ0x00��ô����Ϊû��Ӳ�ҽ���
        coinInFlag = 0;
    }
    INFRARED_HIGH();				//���ű���Ϊ�ߵ�ƽ�����ܶ�ȡ����ƽ״̬
    
}






