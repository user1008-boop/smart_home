#include "key.h"
#include "delay.h"
/**
 *�������ƣ�KEY_Init
 *�������ܣ�������ʼ��
 *��ڲ�������
 *���ز�������
 *���ߣ�deng
 *ʱ�䣺2022.12.21
**/ 
void KEY_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTA,PORTCʱ��

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);								 //�ر�jtag��ʹ��SWD
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;															 //PA15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 													 //������������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);																	 //��ʼ��PA15
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;															 //PC5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 													 //������������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);																	 //��ʼ��PC5
 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;															 //PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 													 //PA0����Ϊ��������	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);																	 //��ʼ��PA0
	
} 
/**
 *�������ƣ�KEY_Scan
 *�������ܣ�����ɨ��
 *��ڲ���������ģʽ��0����֧����������1��֧��
 *���ز������������
 *���ߣ�deng
 *ʱ�䣺2022.12.21
 *ע����Ӧ���ȼ�,KEY0>KEY1>WK_UP
**/ 
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;												//�������ɿ���־
	if(mode)key_up=1;  												//֧������		  
	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
	{
		delay_ms(10);														//���� 
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
	return 0;// �ް�������
}
