/*
***********************************************************************
*文件名称：oled.c oled.h
*实现功能：oled iic协议的演示例程，在使用的时候需要进行初始化一下。
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
*全局变量：
*全局静态： 
*局部静态： 
***********************************************************************
*/
#include "oled.h"
#include "oled_font.h"
#include "delay.h"

/*
 *一些只读变量
 */
//SIZE代表选择的字体的大小
const uint8_t SIZE = 16;
//128列像素点
const uint8_t MAX_COLUMN = 128;
//64行像素点
const uint8_t MAX_ROW = 64;
//点亮一个像素点
const uint8_t BRIGHT_DOT  = 0XFF;
//代表写入的是数据还是命令
const uint8_t OLED_CMD = 0;
const uint8_t OLED_DATA = 1;

//OLED的显存
//存放格式如下.（显示8行）
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
*函 数 名：OLED_WrByte
*函数功能：向液晶显示屏中写入一字节 
*入口参数：uint8_t dat 要写入的数据/命令
*          uint8_t cmd 数据或者命令标志  0代表命令 1代表数据
*返 回 值：无 
*作    者：zzw
*创建日期：2016.09.25
*修改日期：无
*修改原因：无 
*修 改 者：无 
*函数版本：v1.0 
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
    //使能液晶显示屏
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
*函 数 名：OLED_SetPos(uint8_t x, uint8_t y) 
*函数功能：设置 液晶显示屏 的位置
*入口参数：uint8_t x x坐标点
*          uint8_t y y坐标点
*返 回 值：无 
*作    者：zzw
*创建日期：2016.09.25
*修改日期：无
*修改原因：无 
*修 改 者：无 
*函数版本：v1.0 
**********************************************************************
*/
void OLED_SetPos(uint8_t x, uint8_t y) 
{ 
	OLED_WrByte(0xb0+y,OLED_CMD);
	OLED_WrByte(((x & 0xf0) >> 4) | 0x10,OLED_CMD);
	OLED_WrByte((x & 0x0f) | 0x01,OLED_CMD); 
} 

//开启OLED显示    
void OLED_DisplayOn(void)
{
	OLED_WrByte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WrByte(0X14,OLED_CMD);  //DCDC ON
	OLED_WrByte(0XAF,OLED_CMD);  //DISPLAY ON
}

//关闭OLED显示     
void OLED_DisplayOff(void)
{
	OLED_WrByte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WrByte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WrByte(0XAE,OLED_CMD);  //DISPLAY OFF
}
/*
**********************************************************************
*函 数 名：OLED_Clear
*函数功能：清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样
*入口参数：无
*返 回 值：无 
*作    者：zzw
*创建日期：2016.09.25
*修改日期：无
*修改原因：无 
*修 改 者：无 
*函数版本：v1.0 
**********************************************************************
*/	  
void OLED_Clear(void)  
{  
	uint8_t i = 0,n = 0;	    
	for(i = 0;i < 8;i++)  
	{  
		OLED_WrByte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WrByte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WrByte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n = 0;n<128;n++)
        {
            OLED_WrByte(0,OLED_DATA); 
        }
	} //更新显示
}
/*
**********************************************************************
*函 数 名：OLED_ShowChar(uint8_t x,uint8_t y,uint8_t ch)
*函数功能：显示一字节数据
*入口参数：uint8_t x x坐标的位置,uint8_t y，y坐标的位置,uint8_t ch需要显示的字节数据
*返 回 值：无 
*作    者：zzw
*创建日期：2016.09.25
*修改日期：无
*修改原因：无 
*修 改 者：无 
*函数版本：v1.0 
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
//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	uint32_t result=1;	 
	while(n--)
    {
        result*=m; 
    }        
	return result;
}

//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
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
*函 数 名：OLED_ShowString(uint8_t x,uint8_t y,uint8_t *ch)
*函数功能：液晶显示字符串函数（一个字符占8列）
*入口参数：uint8_t x字符串的x轴起始坐标 uint8_t y 字符串的y轴起始坐标
*          uint8_t *ch 字符串数组
*返 回 值：无 
*作    者：zzw
*创建日期：2016.09.25
*修改日期：无
*修改原因：无 
*修 改 者：无 
*函数版本：v1.0 
**********************************************************************
*/
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *ch)
{
    uint8_t j = 0;
    while(ch[j] != '\0')
    {
        OLED_ShowChar(x,y,ch[j]);
		//一个字符占8列
        x += 8;
		if(x > 120)
        {
            x = 0;
            //换行
            y += 2;
        }
		j++;
    }
    
}
/*
**********************************************************************
*函 数 名：OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no)
*函数功能：初始化液晶显示
*入口参数：uint8_t x汉字起始的x位置坐标 
*          uint8_t y汉字起始的y轴坐标
*          uint8_t no需要显示的第几个汉字，汉字取模在一个头文件中
*返 回 值：无 
*作    者：zzw
*创建日期：2016.09.25
*修改日期：无
*修改原因：无 
*修 改 者：无 
*函数版本：v1.0 
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
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
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
*函 数 名：OLED_Init()
*函数功能：初始化液晶显示，参考液晶的数据手册
*入口参数：无
*返 回 值：无 
*作    者：zzw
*创建日期：2016.09.25
*修改日期：无
*修改原因：无 
*修 改 者：无 
*函数版本：v1.0 
**********************************************************************
*/
void OLED_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
    delay_init();
	
	RCC_APB2PeriphClockCmd(OLED_RCC, ENABLE);	
	GPIO_InitStructure.GPIO_Pin = OLED_D0_CLK_PIN|OLED_D1_DAT_PIN|OLED_RES_PIN|OLED_DC_PIN|OLED_CS_PIN;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		    //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           //速度50MHz
 	GPIO_Init(OLED_PORT, &GPIO_InitStructure);	                //初始化GPIOD3,6
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
	OLED_WrByte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WrByte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
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














