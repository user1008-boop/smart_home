#include "pwm.h"
/**
 *函数名称：FS_Init
 *函数功能：风扇初始化
 *入口参数：无
 *返回参数：无
 *作者：deng
 *时间：2022.12.27
**/ 
void FS_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能GPIOC端口时钟
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 				//PC13端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 		//推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 			//速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);	 									//初始化PC13
 
 GPIO_ResetBits(GPIOC,GPIO_Pin_13);												//初始化为低电平
 
}
