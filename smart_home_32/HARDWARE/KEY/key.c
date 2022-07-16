#include "key.h"
#include "delay.h"
/**
 *函数名称：KEY_Init
 *函数功能：按键初始化
 *入口参数：无
 *返回参数：无
 *作者：deng
 *时间：2022.12.21
**/ 
void KEY_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//使能PORTA,PORTC时钟

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);								 //关闭jtag，使能SWD
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;															 //PA15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 													 //设置上拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);																	 //初始化PA15
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;															 //PC5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 													 //设置上拉输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);																	 //初始化PC5
 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;															 //PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 													 //PA0设置为下拉输入	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);																	 //初始化PA0
	
} 
/**
 *函数名称：KEY_Scan
 *函数功能：按键扫描
 *入口参数：按键模式，0：不支持连续按，1：支持
 *返回参数：按键标号
 *作者：deng
 *时间：2022.12.21
 *注意响应优先级,KEY0>KEY1>WK_UP
**/ 
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;												//按键按松开标志
	if(mode)key_up=1;  												//支持连按		  
	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
	{
		delay_ms(10);														//消抖 
		key_up=0;
		if(KEY0==0)
			return KEY0_PRES;
		else if(KEY1==0)
			return KEY1_PRES;
		else if(WK_UP==1)
			return WKUP_PRES; 
	}
	else if(KEY0==1&&KEY1==1&&WK_UP==0)
		key_up=1; 	     
	return 0;// 无按键按下
}
