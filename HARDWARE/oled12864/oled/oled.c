/*
***********************************************************************
*�ļ����ƣ�oled.c oled.h
*ʵ�ֹ��ܣ�oled iicЭ�����ʾ���̣���ʹ�õ�ʱ����Ҫ���г�ʼ��һ�¡�
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
*ȫ�ֱ�����
*ȫ�־�̬�� 
*�ֲ���̬�� 
***********************************************************************
*/
#include "oled.h"
#include "oled_font.h"
#include "delay.h"

/*
 *һЩֻ������
 */
//SIZE����ѡ�������Ĵ�С
const uint8_t SIZE = 16;
//128�����ص�
const uint8_t MAX_COLUMN = 128;
//64�����ص�
const uint8_t MAX_ROW = 64;
//����һ�����ص�
const uint8_t BRIGHT_DOT  = 0XFF;
//����д��������ݻ�������
const uint8_t OLED_CMD = 0;
const uint8_t OLED_DATA = 1;

//OLED���Դ�
//��Ÿ�ʽ����.����ʾ8�У�
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 

/*
**********************************************************************
*�� �� ����OLED_WrByte
*�������ܣ���Һ����ʾ����д��һ�ֽ� 
*��ڲ�����uint8_t dat Ҫд�������/����
*          uint8_t cmd ���ݻ��������־  0�������� 1��������
*�� �� ֵ���� 
*��    �ߣ�zzw
*�������ڣ�2016.09.25
*�޸����ڣ���
*�޸�ԭ���� 
*�� �� �ߣ��� 
*�����汾��v1.0 
**********************************************************************
*/
void OLED_WrByte(uint8_t dat,uint8_t cmd)
{
    uint8_t i = 0;
    if(cmd)
    {
        OLED_DC_HIGH();
    }else
    {
        OLED_DC_LOW();
    }
    //ʹ��Һ����ʾ��
    OLED_CS_LOW();
    for(i = 0;i < 8;i++)
    {
        OLED_CLK_LOW();
        if(dat & 0x80)
        {
            OLED_DAT_HIGH();
        }else 
        {
            OLED_DAT_LOW();
        }
        OLED_CLK_HIGH();
        dat <<= 1;
    }
    OLED_CS_HIGH();
    OLED_DC_HIGH();
    
}

/*
**********************************************************************
*�� �� ����OLED_SetPos(uint8_t x, uint8_t y) 
*�������ܣ����� Һ����ʾ�� ��λ��
*��ڲ�����uint8_t x x�����
*          uint8_t y y�����
*�� �� ֵ���� 
*��    �ߣ�zzw
*�������ڣ�2016.09.25
*�޸����ڣ���
*�޸�ԭ���� 
*�� �� �ߣ��� 
*�����汾��v1.0 
**********************************************************************
*/
void OLED_SetPos(uint8_t x, uint8_t y) 
{ 
	OLED_WrByte(0xb0+y,OLED_CMD);
	OLED_WrByte(((x & 0xf0) >> 4) | 0x10,OLED_CMD);
	OLED_WrByte((x & 0x0f) | 0x01,OLED_CMD); 
} 

//����OLED��ʾ    
void OLED_DisplayOn(void)
{
	OLED_WrByte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WrByte(0X14,OLED_CMD);  //DCDC ON
	OLED_WrByte(0XAF,OLED_CMD);  //DISPLAY ON
}

//�ر�OLED��ʾ     
void OLED_DisplayOff(void)
{
	OLED_WrByte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WrByte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WrByte(0XAE,OLED_CMD);  //DISPLAY OFF
}
/*
**********************************************************************
*�� �� ����OLED_Clear
*�������ܣ���������,������,������Ļ�Ǻ�ɫ��!��û����һ��
*��ڲ�������
*�� �� ֵ���� 
*��    �ߣ�zzw
*�������ڣ�2016.09.25
*�޸����ڣ���
*�޸�ԭ���� 
*�� �� �ߣ��� 
*�����汾��v1.0 
**********************************************************************
*/	  
void OLED_Clear(void)  
{  
	uint8_t i = 0,n = 0;	    
	for(i = 0;i < 8;i++)  
	{  
		OLED_WrByte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WrByte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WrByte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n = 0;n<128;n++)
        {
            OLED_WrByte(0,OLED_DATA); 
        }
	} //������ʾ
}
/*
**********************************************************************
*�� �� ����OLED_ShowChar(uint8_t x,uint8_t y,uint8_t ch)
*�������ܣ���ʾһ�ֽ�����
*��ڲ�����uint8_t x x�����λ��,uint8_t y��y�����λ��,uint8_t ch��Ҫ��ʾ���ֽ�����
*�� �� ֵ���� 
*��    �ߣ�zzw
*�������ڣ�2016.09.25
*�޸����ڣ���
*�޸�ԭ���� 
*�� �� �ߣ��� 
*�����汾��v1.0 
**********************************************************************
*/	
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t ch)
{
    uint8_t c = 0,i = 0;
    c = ch - ' ';
    if(x > MAX_COLUMN - 1)
    {
        x = 0;
        y += 2;
    }
    if(SIZE == 16)
	{
		OLED_SetPos(x,y);	
		for(i = 0;i < 8;i++)
        {
            OLED_WrByte(F8X16[c*16+i],OLED_DATA);
        }
        OLED_SetPos(x,y+1);
		for(i = 0;i < 8;i++)
        {
            OLED_WrByte(F8X16[c*16+i+8],OLED_DATA);
        }
    }else 
    {	
		OLED_SetPos(x,y+1);
		for(i = 0;i < 6;i++)
		OLED_WrByte(F6x8[c][i],OLED_DATA);
				
	}
    
}
//m^n����
u32 oled_pow(u8 m,u8 n)
{
	uint32_t result=1;	 
	while(n--)
    {
        result*=m; 
    }        
	return result;
}

//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t i,temp;
	uint8_t enshow=0;						   
	for(i = 0;i < len;i++)
	{
		temp  = (num / oled_pow(10,len - i - 1)) % 10;
		if(enshow == 0 && i < (len-1))
		{
			if(temp == 0)
			{
				OLED_ShowChar(x+(size/2)*i,y,' ');
				continue;
			}else 
            {
                enshow=1; 
            }
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*i,y,temp+'0'); 
	}
}

/*
**********************************************************************
*�� �� ����OLED_ShowString(uint8_t x,uint8_t y,uint8_t *ch)
*�������ܣ�Һ����ʾ�ַ���������һ���ַ�ռ8�У�
*��ڲ�����uint8_t x�ַ�����x����ʼ���� uint8_t y �ַ�����y����ʼ����
*          uint8_t *ch �ַ�������
*�� �� ֵ���� 
*��    �ߣ�zzw
*�������ڣ�2016.09.25
*�޸����ڣ���
*�޸�ԭ���� 
*�� �� �ߣ��� 
*�����汾��v1.0 
**********************************************************************
*/
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *ch)
{
    uint8_t j = 0;
    while(ch[j] != '\0')
    {
        OLED_ShowChar(x,y,ch[j]);
		//һ���ַ�ռ8��
        x += 8;
		if(x > 120)
        {
            x = 0;
            //����
            y += 2;
        }
		j++;
    }
    
}
/*
**********************************************************************
*�� �� ����OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no)
*�������ܣ���ʼ��Һ����ʾ
*��ڲ�����uint8_t x������ʼ��xλ������ 
*          uint8_t y������ʼ��y������
*          uint8_t no��Ҫ��ʾ�ĵڼ������֣�����ȡģ��һ��ͷ�ļ���
*�� �� ֵ���� 
*��    �ߣ�zzw
*�������ڣ�2016.09.25
*�޸����ڣ���
*�޸�ԭ���� 
*�� �� �ߣ��� 
*�����汾��v1.0 
**********************************************************************
*/
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no)
{      			    
	uint8_t t = 0;
	OLED_SetPos(x,y);	
    for(t = 0;t < 16;t++)
	{
		OLED_WrByte(Hzk[2*no][t],OLED_DATA);
     }	
	OLED_SetPos(x,y+1);	
    for(t = 0; t < 16; t++)
	{	
		OLED_WrByte(Hzk[2*no+1][t],OLED_DATA);
    }					
}
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void OLED_DrawBMP(uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1,uint8_t BMP[])
{ 	
    uint8_t j = 0;
    uint8_t x,y = 0;
    for(y = y0;y < y1;y++)
    {
		OLED_SetPos(x0,y);
		for(x=x0;x<x1;x++)
		{      
			OLED_WrByte(BMP[j++],OLED_DATA);	    	
		}
    }
}  

/*
**********************************************************************
*�� �� ����OLED_Init()
*�������ܣ���ʼ��Һ����ʾ���ο�Һ���������ֲ�
*��ڲ�������
*�� �� ֵ���� 
*��    �ߣ�zzw
*�������ڣ�2016.09.25
*�޸����ڣ���
*�޸�ԭ���� 
*�� �� �ߣ��� 
*�����汾��v1.0 
**********************************************************************
*/
void OLED_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
    delay_init();
	
	RCC_APB2PeriphClockCmd(OLED_RCC, ENABLE);	
	GPIO_InitStructure.GPIO_Pin = OLED_D0_CLK_PIN|OLED_D1_DAT_PIN|OLED_RES_PIN|OLED_DC_PIN|OLED_CS_PIN;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		    //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           //�ٶ�50MHz
 	GPIO_Init(OLED_PORT, &GPIO_InitStructure);	                //��ʼ��GPIOD3,6
 	GPIO_SetBits(OLED_PORT,OLED_D0_CLK_PIN|OLED_D1_DAT_PIN|OLED_RES_PIN|OLED_DC_PIN|OLED_CS_PIN);

    
    OLED_RES_HIGH();
    delay_ms(20);
    OLED_RES_LOW();
    delay_ms(20);
    OLED_RES_HIGH();
    

    OLED_WrByte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WrByte(0x00,OLED_CMD);//---set low column address
	OLED_WrByte(0x10,OLED_CMD);//---set high column address
	OLED_WrByte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WrByte(0x81,OLED_CMD);//--set contrast control register
	OLED_WrByte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WrByte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_WrByte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	OLED_WrByte(0xA6,OLED_CMD);//--set normal display
	OLED_WrByte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WrByte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WrByte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WrByte(0x00,OLED_CMD);//-not offset
	OLED_WrByte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WrByte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WrByte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WrByte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WrByte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WrByte(0x12,OLED_CMD);
	OLED_WrByte(0xDB,OLED_CMD);//--set vcomh
	OLED_WrByte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WrByte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WrByte(0x02,OLED_CMD);//
	OLED_WrByte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WrByte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WrByte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WrByte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WrByte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_WrByte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_Clear();
	OLED_SetPos(0,0);
}














