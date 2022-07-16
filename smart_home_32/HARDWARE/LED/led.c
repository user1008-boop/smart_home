#include "led.h"
/**
 *函数名称：LED_Init
 *函数功能：对LED0,LED1进行初始化
 *入口参数：无
 *返回参数：无
 *作者：deng
 *时间：2022.12.21
**/ 
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD, ENABLE);	 //使能PA,PD端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 													 //LED0：PA8端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 											 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 											 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					  													 //初始化PA8
 GPIO_SetBits(GPIOA,GPIO_Pin_8);						 																 //PA8输出高电平

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 												 //LED1：PD2端口配置
 GPIO_Init(GPIOD, &GPIO_InitStructure);	  				 													
 GPIO_SetBits(GPIOD,GPIO_Pin_2); 						 																 //PD2输出高电平 
}
 
