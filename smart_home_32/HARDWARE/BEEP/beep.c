#include "beep.h"
/**
 *�������ƣ�BEEP_Init
 *�������ܣ���������ʼ��
 *��ڲ�������
 *���ز�������
 *���ߣ�deng
 *ʱ�䣺2022.12.21
**/ 
void BEEP_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��GPIOB�˿�ʱ��
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 			 //BEEP��PB8�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		   //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	     //�ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);	                 //��ʼ��PB8
 
 GPIO_ResetBits(GPIOB,GPIO_Pin_8);												//����͵�ƽ
 
}
