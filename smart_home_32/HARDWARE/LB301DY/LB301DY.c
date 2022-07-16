#include "lb301dy.h"
#include "LB301.h"
#include "delay.h"
SysPara LB301Para;//ָ��ģ��LB301����
u16 ValidN;//ģ������Чģ�����
u16 ID; 
void Add_FR(void);	//¼ָ��
void Del_FR(void);	//ɾ��ָ��
void press_FR(void);//ˢָ��
//¼ָ��
void Add_FR(void)
{
	u8 i=0,ensure ,processnum=1,pressCout=1, str_buffer[40];
	while(1)
	{
		switch (processnum)
		{
			case 1: //������ѹ4��ָ�Ʒֱ�浽4��charBuffer�� 
				i++;
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					i=0;
					delay_ms(100);//������Ҫ��ʱһ�£�ģ���ڲ�����ͼ����Ҫʱ��
					ensure=PS_GenChar(CharBuffer1);//��������
					if(ensure==0x00)
					{
						printf("ָ����ȷ");
						pressCout++;
						if(pressCout >=5)
						{
							pressCout = 0;
							processnum=2;//�����ڶ���
						}					
					}				
				}				
				break;

			case 2:
				ensure=PS_RegMB();
				if(ensure==0x00) 
				{
					printf("��ģ�����ɳɹ�");
					processnum=3;//����������
				}
				delay_ms(1000);
				break;
				
			case 3:		
				do
					ID=ID+0x0001;
				while(!(ID<300));//����ID����С��300
				printf("ID is %d",ID);
				ensure=PS_StoreMB(CharBuffer1,ID);//����ģ��
				if(ensure==0x00) 
				{			
					printf("���ָ�Ƴɹ�");
					PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
					return ;
				}					
				break;				
		}
		delay_ms(200);
		if(i>=20)//����20��û�а���ָ���˳�
		{
			break;	
		}					
	}
}

#define USE_AUTO_IDETIFY  1 // 1ʹ���Զ���֤��ʽ  0ʹ�÷ֲ�ʽ��֤��ʽ

//ˢָ��
void press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	char *str;
#if (USE_AUTO_IDETIFY == 1) //�Զ���ָ֤��	
	ensure=PS_AutoIdentify(&seach);
	if(ensure==0x00)//�Զ���֤�ɹ�
	{
	printf("�Զ���֤�ɹ�");
		printf("Match ID:%d  Match score:%d",seach.pageID,seach.mathscore);
	}
#else  //�ֲ�ʽ��ָ֤��	
	ensure=PS_GetImage();
	if(ensure==0x00)//��ȡͼ��ɹ� 
	{	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //���������ɹ�
		{		
			ensure=PS_HighSpeedSearch(CharBuffer1,0,300,&seach);
			if(ensure==0x00)//�����ɹ�
			{				
				printf("Match ID:%d  Match score:%d",seach.pageID,seach.mathscore);
			}			
	  }
	 delay_ms(1000);//��ʱ�������ʾ
	}
#endif	
}

//ɾ��ָ��

void Del_FR(void)
{
	u8  ensure;
	//printf("ID is %d",ID);
	//if(ID==0xFF00)
		ensure=PS_Empty();//���ָ�ƿ�
	/*else 
	{
		ensure=PS_DeletChar(ID,1);//ɾ������ָ��
		ID++;
		printf("ID is %d",ID);
	}*/
		
	if(ensure==0)
	{
		printf("ɾ���ɹ�");
	}
	delay_ms(1500);//��ʱ�������ʾ
	PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
}