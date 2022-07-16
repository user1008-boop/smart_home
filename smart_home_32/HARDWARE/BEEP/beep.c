#include "beep.h"
/**
 *函数名称：BEEP_Init
 *函数功能：蜂鸣器初始化
 *入口参数：无
 *返回参数：无
 *作者：deng
 *时间：2022.12.21
**/ 
void BEEP_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能GPIOB端口时钟
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 			 //BEEP：PB8端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		   //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	     //速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);	                 //初始化PB8
 
 GPIO_ResetBits(GPIOB,GPIO_Pin_8);												//输出低电平
 
}
