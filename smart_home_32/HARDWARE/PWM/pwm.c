#include "pwm.h"
/**
 *�������ƣ�FS_Init
 *�������ܣ����ȳ�ʼ��
 *��ڲ�������
 *���ز�������
 *���ߣ�deng
 *ʱ�䣺2022.12.27
**/ 
void FS_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��GPIOC�˿�ʱ��
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 				//PC13�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 		//�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 			//�ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);	 									//��ʼ��PC13
 
 GPIO_ResetBits(GPIOC,GPIO_Pin_13);												//��ʼ��Ϊ�͵�ƽ
 
}
