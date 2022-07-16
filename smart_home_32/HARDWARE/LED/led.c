#include "led.h"
/**
 *�������ƣ�LED_Init
 *�������ܣ���LED0,LED1���г�ʼ��
 *��ڲ�������
 *���ز�������
 *���ߣ�deng
 *ʱ�䣺2022.12.21
**/ 
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PA,PD�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 													 //LED0��PA8�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 											 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 											 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					  													 //��ʼ��PA8
 GPIO_SetBits(GPIOA,GPIO_Pin_8);						 																 //PA8����ߵ�ƽ

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 												 //LED1��PD2�˿�����
 GPIO_Init(GPIOD, &GPIO_InitStructure);	  				 													
 GPIO_SetBits(GPIOD,GPIO_Pin_2); 						 																 //PD2����ߵ�ƽ 
}
 
