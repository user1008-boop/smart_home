#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "dht11.h"
#include "stdio.h"
#include "timer.h"
#include "beep.h"
#include "esp8266.h"
#include "onenet.h"
#include "bh1750.h"
#include "key.h"
#include "exti.h"
#include "oled.h"
#include "LB301.h"
#include "pwm.h"
#include "motor.h"
u8 Sw_Flag=0;
u8 humidityH;                                           //ʪ�����ݸ�λ
u8 humidityL;																						//ʪ�����ݵ�λ
u8 temperatureH;																				//�¶����ݸ�λ
u8 temperatureL;																				//�¶����ݵ�λ
u8 alarmFlag=0;                                         //������־λ
char LD_BUF='0';																						//������������
float light;																						//����ǿ��
char Pub_Buff[256];																			//ESP8266�ϴ����ݰ�
u8 Led_status=0;	                                      //�ƹ��־λ
u8 Fs_status=0;																					//���ȱ�־λ
u8 key_num=0;                                           //�������
u8 Qt_status=1;																					//�������־λ(Ĭ��Ϊ��)
u8 Hw_status=0;																					//�������־λ
u8 Admin_Flag=0;																				//����Ա��־λ
SysPara LB301Para;																			//ָ��ģ��LB301����
u16 ValidN;																							//ģ������Чģ�����
u16 ID; 																								//ָ��ID��
u8 YY_flag=0;
u8 Zf_Flag=0;
u8 indexCount=0;
void Add_FR(void);																			//¼ָ�ƹ��ܺ�������
void Del_FR(void);																			//ɾ��ָ��
void press_FR(void);																		//ָ����֤���ܺ���

const char *devSubTopic[] = {"/mysmarthome/sub"};				//��������Topic
const char devPubTopic[] = {"/mysmarthome/pub"}; 				//��������Topic
/*
*********************************************************************************************************
*�������ƣ�ADD_FR
*�������ܣ���ATK301ָ��ģ���ڲ�д��ָ������
*��ڲ�������
*���ز�������
*********************************************************************************************************
*/
void Add_FR(void)
{
	u8 i=0,ensure ,processnum=1,pressCout=1, str_buffer[40];
	while(1)
	{
		switch (processnum)
		{
			case 1:                                           //������ѹ4��ָ�Ʒֱ���д洢 
				i++;
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					i=0;
					delay_ms(100);																//ģ���ڲ�����ͼ��ʱ�仺��
					ensure=PS_GenChar(CharBuffer1);               //����ָ������
					if(ensure==0x00)															
					{
						printf("ָ����ȷ");
						pressCout++;
						if(pressCout >=5)
						{
							pressCout = 0;
							processnum=2;	 														//��ת���ڶ�����
						}					
					}				
				}				
				break;

			case 2:
				ensure=PS_RegMB();
				if(ensure==0x00) 
				{
					printf("��ģ�����ɳɹ�");
					processnum=3;																	//��ת����������
				}
				delay_ms(1000);
				break;
				
			case 3:		
				do
					ID=ID+0x0001;																	//ID��Ĭ��ÿ������1
				while(!(ID<300));																//���ID����
				printf("ID is %d",ID);
				ensure=PS_StoreMB(CharBuffer1,ID);							//����ָ������ģ��
				if(ensure==0x00) 
				{			
					printf("���ָ�Ƴɹ�");
					BEEP=1;
					delay_ms(100);
					BEEP=0;
					PS_ValidTempleteNum(&ValidN);									//��ȡ��ָ�Ƹ���
					return ;
				}					
				break;				
		}
		delay_ms(200);
		if(i>=20)																						//ָ�Ƽ�ⳬʱ
		{
			break;	
		}					
	}
}
/*
*********************************************************************************************************
*�������ƣ�press_FR
*�������ܣ�����ǰ���ָ�����ָ�ƽ��бȶ���֤
*��ڲ�������
*���ز�������
*********************************************************************************************************
*/
void press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	ensure=PS_AutoIdentify(&seach);												//ָ���Զ���֤
	if(ensure==0x00)																			//�Զ���֤�ɹ�
	{
 
		printf("ID:%d  ƥ��÷�:%d\r\n",seach.pageID,seach.mathscore);
		Admin_Flag=1;																				//�����֤�ɹ��������ԱȨ��
		
		angle_control(Neg, 90, 5000);//ת�򡢽Ƕȡ��ٶ�
		/*
		angle_control(Neg, 60, 5000);
		if(seach.pageID==1)
		LED0=!LED0;
		else if(seach.pageID==2)
		LED1=!LED1;
		*/
		delay_ms(3000);
		angle_control(Pos,90,5000);
		delay_ms(3000);
	}
	else if(ensure != 0xff)																//�����֤������
	{
		printf("��֤����\r\n");
		//Admin_Flag=0;
		delay_ms(500);
	}
}
/*
*********************************************************************************************************
*�������ƣ�Del_FR
*�������ܣ���ATK301�ڿ�ָ�ƽ���ɾ��
*��ڲ�������
*���ز�������
*********************************************************************************************************
*/ 
void Del_FR(void)
{
	u8  ensure;
	ensure=PS_Empty();																			//���ָ�ƿ�
	if(ensure==0)
	{
		printf("ɾ���ɹ�");
		ID=0;
	}
	delay_ms(1500);
	PS_ValidTempleteNum(&ValidN);														//����ָ�Ƹ���
}
/*
*********************************************************************************************************
*�������ƣ�main
*�������ܣ�����ϵͳ�����߼�ʵ��
*��ڲ�������
*���ز�������
*********************************************************************************************************
*/ 
int main(void)
{ 
		unsigned short timeCount = 0;													//���ͼ������
		unsigned char *dataPtr = NULL;
		u8 ensure;																						//ָ�Ʋ���
		u16 count = 0;																				//������־λ���ڶ�ʱ����TOPIC��Ϣ
		delay_init();	    	 																	//��ʱ������ʼ��	  
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				//�����ж����ȼ�����2
		LED_Init();		  																			//LED�Ƴ�ʼ��
		BEEP_Init();																					//������������ʼ��
		FS_Init();																						//���ȳ�ʼ��
		KEY_Init();																						//������ʼ��
		EXTIX_Init();																					//�ⲿ�жϳ�ʼ��
		DHT11_Init();																					//��ʪ�ȴ�������ʼ��
		BH1750_Init();																				//���մ�������ʼ��
		ULN2003_Configuration();                              //���������ʼ��
		Usart1_Init(9600);																	  //����1��ʼ�������ڴ������ݴ�ӡ
		Usart2_Init(115200);																	//����2��ʼ����������8266ͨ��
		Usart3_Init(57600);																		//����3��ʼ����������ATK301ͨ��
		OLED_Init();																					//OLED����ʼ��������������ʾ
		OLED_ColorTurn(0);																		//0������ʾ��1 ��ɫ��ʾ
		OLED_DisplayTurn(0);																	//0������ʾ 1 ��Ļ��ת��ʾ
		TIM2_Int_Init(4999,7199); 														//��ʱ��2��ʼ����������Ļ���ݸ��¶�ʱ
		TIM3_Int_Init(2499,7199);															//��ʱ��3��ʼ�����������ݼ�ⶨʱ
		ESP8266_Init();																				//8266��ʼ�����������ƶ���ͨ��
		while(OneNet_DevLink())																//���������
			delay_ms(500);																			//��ʱ�ȴ��������
		OLED_Clear();	                                        //��ʾ������
		OneNet_Subscribe(devSubTopic, 1);											//����Topic
		ensure=PS_ValidTempleteNum(&ValidN);									//��ȡ��ָ�Ƹ���
		ensure=PS_ReadSysPara(&LB301Para);  									//��ȡָ��ģ����� 
		while(1) 
		{
			if(key_num)																					//�жϰ���
			{
				printf("KEY_NUM:%d",key_num);
				if(key_num==1)                                    //����KEY1����
				{
						Add_FR();																			//¼��ָ��
						key_num=0;																		//��־����
				}				
				else if(key_num==2)																//����KEY_UP����
				{
						Del_FR();																			//ɾ��ָ��
						key_num=0;																		//��־����
				}
			}
			
			if(++count > 5) 																		//ÿ������ν���һ��ָ�Ƽ��
			{
				count = 0;
				press_FR();																				//ָ��ʶ��
			}
			delay_ms(1);			
			
			if(timeCount%40==0)                                 //��ʱ�������ݲɼ����Ϊ1��
			{
				DHT11_Read_Data(&humidityH,&humidityL,&temperatureH,&temperatureL);//��ȡ��ʪ������
				//UsartPrintf(USART_DEBUG,"�¶ȣ�%d.%d  ʪ�ȣ�%d.%d ",humidityH,humidityL,temperatureH,temperatureL);
				if(!i2c_CheckDevice(BH1750_Addr))	                //�ж��豸�Ƿ��������벢���ж���
				{
						light=LIght_Intensity();
						//UsartPrintf(USART_DEBUG,"��ǰ���ն�Ϊ:%.1f lx\r\n",light);
				}
				delay_ms(1000);
			}
			if(Zf_Flag==1&&indexCount>=40){
					Zf_Flag=0;
					indexCount=0;
					BEEP=1;
					delay_ms(100);
					BEEP=0;
					angle_control(Pos,90,5000);
					delay_ms(3000);
			}
			if(++timeCount >= 200)															     //����PUBʱ����5s
			{
				Led_status = GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2);  //LED�����ŵ�ƽ���
				Fs_status = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13);  //�������ŵ�ƽ���
				Qt_status = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9);   //MQ���ŵ�ƽ���
				Hw_status = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8);	 //����ģ���ƽ���
				alarmFlag = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8);   //���������ŵ�ƽ���
				if(Sw_Flag==0&&YY_flag==0)                             //�����Զ�����ģʽ
				{
					if(temperatureH>23)
						Fs_status=1;
					else
						Fs_status=0;
				}
				else
				{
					Fs_status = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13);  //�������ŵ�ƽ���
				}
				if(((Hw_status==1&&Admin_Flag==0)||Qt_status==0)&&Zf_Flag==0)//����ϵͳ
				{
					alarmFlag=1;
					if(Qt_status==0&&Zf_Flag==0){
						FS=1;
						angle_control(Neg,90,5000);
						delay_ms(3000);
					}
				}
				else
					alarmFlag=0;
				//UsartPrintf(USART_DEBUG, "OneNet_Publish\r\n");
				/**
					*���ܣ���������JSON��ʽд��Pub_Buff��������TOPIC��Ϣ
				**/
				sprintf(Pub_Buff,"{\"Humi\":%d.%d,\"Temp\":%d.%d,\"Light\":%.1f,\"Led\":%d,\"Beep\":%d,\"Fs\":%d}",
				humidityH,humidityL,temperatureH,temperatureL,light,Led_status?0:1,alarmFlag,Fs_status);
				OneNet_Publish(devPubTopic,Pub_Buff);
				timeCount = 0;                                     //������λ����
				ESP8266_Clear();								                   //��ջ���
			}
			
			dataPtr = ESP8266_GetIPD(3);												 //����TOPIC���ݽ���						
			if(dataPtr != NULL)
				OneNet_RevPro(dataPtr);
			
			/*�������Ʋ����߼�*/
			
			if(LD_BUF!='0')
			{
				if(LD_BUF=='a')																			//��������Ϊ����
				{
					LED1=0;
					LD_BUF='0';																					//����λ��0�����ڷ�ֹ��������
				}
				if(LD_BUF=='b')																			//��������Ϊ�ص�
				{
					LED1=1;
					LD_BUF='0';																					//����λ��0�����ڷ�ֹ��������
				}
				if(LD_BUF=='c')
				{
					FS=1;
					LD_BUF='0';
					YY_flag=1;
				}
				if(LD_BUF=='d')
				{
					FS=0;
					LD_BUF='0';
					YY_flag=1;
				}
				if(LD_BUF=='e')
				{
					alarmFlag=1;
					LD_BUF='0';
				}
				if(LD_BUF=='f')
				{
					alarmFlag=0;
					LD_BUF='0';
				}
			}
			delay_ms(10);	
		}
}
	
