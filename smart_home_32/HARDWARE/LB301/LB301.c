#include <string.h>
#include "delay.h" 	
#include "usart.h"
#include "LB301.h"

u32 LB301Addr = 0XFFFFFFFF; //Ĭ��

//��ʼ��PA6Ϊ��������		    
//��������Ӧ״̬(������Ӧʱ����ߵ�ƽ�ź�)
void PS_StaGPIO_Init(void)
{   
	RCC->APB1ENR |= 1<<2;//ʹ��PORTAʱ��
	GPIOA->CRL &=0XF0FFFFFF;//��λPA6
	GPIOA->CRL |=0X08000000;//����ģʽ��Ĭ������
	GPIOA->ODR &=~(1<<6);//����
}

//���ڷ���һ���ֽ�
static void MYUSART_SendData(u8 data)
{
	while((USART3->SR&0X40)==0); 
	USART3->DR = data;
}
//���Ͱ�ͷ
static void SendHead(void)
{
	MYUSART_SendData(0xEF);
	MYUSART_SendData(0x01);
}
//���͵�ַ
static void SendAddr(void)
{
	MYUSART_SendData(LB301Addr>>24);
	MYUSART_SendData(LB301Addr>>16);
	MYUSART_SendData(LB301Addr>>8);
	MYUSART_SendData(LB301Addr);
}
//���Ͱ���ʶ,
static void SendFlag(u8 flag)
{
	MYUSART_SendData(flag);
}
//���Ͱ�����
static void SendLength(int length)
{
	MYUSART_SendData(length>>8);
	MYUSART_SendData(length);
}
//����ָ����
static void Sendcmd(u8 cmd)
{
	MYUSART_SendData(cmd);
}
//����У���
static void SendCheck(u16 check)
{
	MYUSART_SendData(check>>8);
	MYUSART_SendData(check);
}
//�ȴ�Ӧ��
//waittime���ȴ���ʱʱ�䣨��λ1ms��
//���ؽ���������׵�ַ
static u8 *WaitBack(u16 waittime)
{
	char *data;
	u8 str[8];
	str[0]=0xef;					str[1]=0x01;
	str[2]=LB301Addr>>24;	str[3]=LB301Addr>>16;		
	str[4]=LB301Addr>>8;	str[5]=LB301Addr;				
	str[6]=0x07;					str[7]='\0';
	USART3_RX_STA=0;
	while(--waittime)
	{
		delay_ms(1);
		if(USART3_RX_STA&0X8000)//���յ�һ������
		{
			data=strstr((const char*)USART3_RX_BUF,(const char*)str);
			if(data)
				return (u8*)data;	
		}
	}
	return 0;
}
//¼��ͼ�� PS_GetImage
//����:̽����ָ��̽�⵽��¼��ָ��ͼ�����ImageBuffer�� 
//ģ�鷵��ȷ����
u8 PS_GetImage(void)
{
	u16 temp;
	u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x03);
	Sendcmd(0x01);
	temp =  0x01+0x03+0x01;
	SendCheck(temp);
	data=WaitBack(500);//�ȴ����أ�ms��ʱ���ƣ�
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	memset(USART3_RX_BUF,0,USART3_RX_STA&0x7fff); //������0
	USART3_RX_STA = 0;
	return ensure;
}
//�������� PS_GenChar
//����:��ImageBuffer�е�ԭʼͼ������ָ�������ļ�����CharBuffer1\2\3\4		 
//����:BufferID --> charBuffer1:0x01 0x02 0x03 0x04	  											
//ģ�鷵��ȷ����
u8 PS_GenChar(u8 BufferID)
{
	u16 temp;
	u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x04);
	Sendcmd(0x02);
	MYUSART_SendData(BufferID);
	temp = 0x01+0x04+0x02+BufferID;
	SendCheck(temp);
	data=WaitBack(500); //����ָ��ظ�ʱ���е��� ���ٴ���100ms
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	memset(USART3_RX_BUF,0,USART3_RX_STA&0x7fff); //������0
	USART3_RX_STA = 0;
	return ensure;
}
//����ָ�� PS_Search
//����:��CharBuffer1��CharBuffer2�е������ļ����������򲿷�ָ�ƿ�.�����������򷵻�ҳ�롣			
//����:  BufferID @ref CharBuffer1	CharBuffer2
//˵��:  ģ�鷵��ȷ���֣�ҳ�루����ָ��ģ�壩
u8 PS_Search(u8 BufferID,u16 StartPage,u16 PageNum,SearchResult *p)
{
	u16 temp;
	u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x08);
	Sendcmd(0x04);
	MYUSART_SendData(BufferID);
	MYUSART_SendData(StartPage>>8);
	MYUSART_SendData(StartPage);
	MYUSART_SendData(PageNum>>8);
	MYUSART_SendData(PageNum);
	temp = 0x01+0x08+0x04+BufferID
	+(StartPage>>8)+(u8)StartPage
	+(PageNum>>8)+(u8)PageNum;
	SendCheck(temp);
	data=WaitBack(500);
	if(data)
	{
		ensure = data[9];
		p->pageID   =(data[10]<<8)+data[11];
		p->mathscore=(data[12]<<8)+data[13];	
	}
	else
		ensure = 0xff;
	memset(USART3_RX_BUF,0,USART3_RX_STA&0x7fff); //������0
	USART3_RX_STA = 0;
	return ensure;	
}
//�ϲ�����������ģ�壩PS_RegMB
//����:��CharBuffer1��CharBuffer2�е������ļ��ϲ����� ģ��,�������CharBuffer1��CharBuffer2	
//˵��:  ģ�鷵��ȷ����
u8 PS_RegMB(void)
{
	u16 temp;
	u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x03);
	Sendcmd(0x05);
	temp = 0x01+0x03+0x05;
	SendCheck(temp);
	data=WaitBack(500);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	memset(USART3_RX_BUF,0,USART3_RX_STA&0x7fff); //������0
	USART3_RX_STA = 0;
	return ensure;		
}
//����ģ�� PS_StoreMB
//����:�� CharBuffer1 �� CharBuffer2 �е�ģ���ļ��浽 PageID ��flash���ݿ�λ�á�			
//����:  BufferID @ref charBuffer1:0x01	charBuffer1:0x02
//       PageID��ָ�ƿ�λ�úţ�
//˵��:  ģ�鷵��ȷ����
u8 PS_StoreMB(u8 BufferID,u16 PageID)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x06);
	Sendcmd(0x06);
	MYUSART_SendData(BufferID);
	MYUSART_SendData(PageID>>8);
	MYUSART_SendData(PageID);
	temp = 0x01+0x06+0x06+BufferID
	+(PageID>>8)+(u8)PageID;
	SendCheck(temp);
	data=WaitBack(500);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	memset(USART3_RX_BUF,0,USART3_RX_STA&0x7fff); //������0
	USART3_RX_STA = 0;
	return ensure;	
}
//ɾ��ģ�� PS_DeletChar
//����:  ɾ��flash���ݿ���ָ��ID�ſ�ʼ��N��ָ��ģ��
//����:  PageID(ָ�ƿ�ģ���)��Nɾ����ģ�������
//˵��:  ģ�鷵��ȷ����
u8 PS_DeletChar(u16 PageID,u16 N)
{
	u16 temp;
	u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x07);
	Sendcmd(0x0C);
	MYUSART_SendData(PageID>>8);
	MYUSART_SendData(PageID);
	MYUSART_SendData(N>>8);
	MYUSART_SendData(N);
	temp = 0x01+0x07+0x0C
	+(PageID>>8)+(u8)PageID
	+(N>>8)+(u8)N;
	SendCheck(temp);
	data=WaitBack(500);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	memset(USART3_RX_BUF,0,USART3_RX_STA&0x7fff); //������0
	USART3_RX_STA = 0;
	return ensure;
}
//���ָ�ƿ� PS_Empty
//����:  ɾ��flash���ݿ�������ָ��ģ��
//����:  ��
//˵��:  ģ�鷵��ȷ����
u8 PS_Empty(void)
{
	u16 temp;
	u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x03);
	Sendcmd(0x0D);
	temp = 0x01+0x03+0x0D;
	SendCheck(temp);
	data=WaitBack(500);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	memset(USART3_RX_BUF,0,USART3_RX_STA&0x7fff); //������0
	USART3_RX_STA = 0;
	return ensure;
}
//��ϵͳ�������� PS_ReadSysPara
//����:  ��ȡģ��Ļ��������������ʣ�����С��)
//����:  ��
//˵��:  ģ�鷵��ȷ���� + ����������16bytes��
u8 PS_ReadSysPara(SysPara *p)
{
	u16 temp;
	u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x03);
	Sendcmd(0x0F);
	temp = 0x01+0x03+0x0F;
	SendCheck(temp);
	data=WaitBack(500);
	if(data)
	{
		ensure = data[9];
		p->PS_max = (data[15]<<8)+data[14];
		p->PS_level = (data[17]<<8)+data[16];
		p->PS_addr=(data[18]<<24)+(data[19]<<16)+(data[20]<<8)+data[21];
		p->PS_size = (data[23]<<8)+data[22];
		p->PS_N = (data[25]<<8)+data[24];
	}		
	else
		ensure=0xff;
	if(ensure==0x00)
	{
		printf("\r\nģ�����ָ������=%d",p->PS_max);
		printf("\r\n�Աȵȼ�=%d",p->PS_level);
		printf("\r\n��ַ=%x",p->PS_addr);
		printf("\r\n������=%d",p->PS_N*9600);
	}
	else 
		printf("\r\n%s",EnsureMessage(ensure));
	memset(USART3_RX_BUF,0,USART3_RX_STA&0x7fff); //������0
	USART3_RX_STA = 0;
	return ensure;
}
//�Զ���ָ֤�� PS_AutoIdentify
//����:�Զ��ɼ�ָ�ƣ���ָ�ƿ�����ƥ���ģ�壬�������������			 												
//ģ�鷵��ȷ����
u8 PS_AutoIdentify(SearchResult *p)
{
#define  SEC_LEVEL		0X03	//��ȫ�ȼ�
#define  S_ID			0XFFFF  //ID�ţ�0XFFFF��ʾ����������
#define  S_PARAM		0X0007  //������bit0 LED����  bit1 ͼ��Ԥ���� bit2 ��;�Ƿ�Ӧ��
	
	u16 temp;
	u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x08);
	Sendcmd(0x32);
	MYUSART_SendData(SEC_LEVEL);//��ȫ�ȼ�
	MYUSART_SendData(S_ID>>8);  //ID��
	MYUSART_SendData((u8)S_ID);
	MYUSART_SendData(S_PARAM>>8);//����
	MYUSART_SendData((u8)S_PARAM);
	temp = 0x01+0x08+0x32+SEC_LEVEL \
	+(S_ID>>8)+(u8)S_ID+(S_PARAM>>8)+(u8)S_PARAM;
	SendCheck(temp);
	data=WaitBack(40);//����ָ����֤���ɹ�ʱû�з��أ��������ﳬʱʱ�価��С
	if(data)
	{
		ensure=data[9];
		p->pageID 	=(data[11]<<8) +data[12];
		p->mathscore=(data[13]<<8) +data[14];
	}
	else
		ensure=0xff;
	memset(USART3_RX_BUF,0,USART3_RX_STA&0x7fff); //������0
	USART3_RX_STA = 0;
	return ensure;
}
//����Чģ����� PS_ValidTempleteNum
//���ܣ�����Чģ�����
//����: ��
//˵��: ģ�鷵��ȷ����+��Чģ�����ValidN
u8 PS_ValidTempleteNum(u16 *ValidN)
{
	u16 temp;
	u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x03);
	Sendcmd(0x1d);
	temp = 0x01+0x03+0x1d;
	SendCheck(temp);
	data=WaitBack(500);
	if(data)
	{
		ensure=data[9];
		*ValidN = (data[10]<<8) +data[11];
	}		
	else
		ensure=0xff;
	
	if(ensure==0x00)
	{
		printf("\r\n��Чָ�Ƹ���=%d",(data[10]<<8)+data[11]);
	}
	else
		printf("\r\n%s",EnsureMessage(ensure));
	memset(USART3_RX_BUF,0,USART3_RX_STA&0x7fff); //������0
	USART3_RX_STA = 0;
	return ensure;
}

//�޸Ĳ����� PS_FingerMoudleSet  ע�⣺���ú�ģ��������Ч ���������ô��ڲ����ʲ���ͨѶ
//����:  дģ�����ã��޸Ĳ����ʣ�
//����:  �������RegNum:4
//˵��:  ģ�鷵��ȷ����
u8 PS_FingerMoudleSet(u8 N)
{
	u16 temp;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x05);
	Sendcmd(0x0E);
	MYUSART_SendData(0x04);
	MYUSART_SendData(N);
	temp = 0x04+N+0x01+0x05+0x0E;
	SendCheck(temp);//���ú�ģ��������Ч 
	return 0;
}
//ģ��Ӧ���ȷ������Ϣ����
//���ܣ�����ȷ���������Ϣ������Ϣ
//����: ensure
const char *EnsureMessage(u8 ensure) 
{
	const char *p;
	switch(ensure)
	{
		case  0x00:
			p="OK";break;		
		case  0x01:
			p="���ݰ����մ���";break;
		case  0x02:
			p="No finger on the sensor!";break;
		case  0x03:
			p="¼��ָ��ͼ��ʧ��";break;
		case  0x04:
			p="ָ��ͼ��̫�ɡ�̫��������������";break;
		case  0x05:
			p="ָ��ͼ��̫ʪ��̫��������������";break;
		case  0x06:
			p="ָ��ͼ��̫�Ҷ�����������";break;
		case  0x07:
			p="ָ��ͼ����������������̫�٣������̫С��������������";break;
		case  0x08:
			p="ָ�Ʋ�ƥ��";break;
		case  0x09:
			p="No fingerprint found!";break;
		case  0x0a:
			p="�����ϲ�ʧ��";break;
		case  0x0b:
			p="ID����ָ�ƿⷶΧ";
		case  0x0c:
			p="��ָ�ƿ��ģ��������Ч";
		case  0x0d:
			p="�ϴ�����ʧ��";
		case  0x0e:
			p="ģ�鲻�ܽ��ܺ������ݰ�";
		case  0x0f:
			p="�ϴ�ͼ��ʧ��";
		case  0x10:
			p="ɾ��ģ��ʧ��";break;
		case  0x11:
			p="���ָ�ƿ�ʧ��";break;
		case  0x12:
			p="���ܽ���͹���״̬";break;
		case  0x13:
			p="�����ȷ";break;
		case  0x14:
			p="ϵͳ��λʧ��";break;			
		case  0x15:
			p="��������û����Чԭʼͼ��������ͼ��";break;
		case  0x16:
			p="��������ʧ��";break;
		case  0x17:
			p="����ָ�ƻ����βɼ�֮����ָû���ƶ���";break;
		case  0x18:
			p="��д FLASH ����";break;
		case  0x19:
			p="δ�������";break;
		case  0x1a:
			p="��Ч�Ĵ�����";break;
		case  0x1b:
			p="�Ĵ����趨���ݴ���";break;
		case  0x1c:
			p="���±�ҳ��ָ������";break;
		case  0x1e:
			p="�Զ�ע��ʧ��";break;
		case  0x1f:
			p="ָ�ƿ���";break;
		case  0x20:
			p="��ַ����";break;
		case  0x23:
			p="ָ��ģ��Ϊ��";break;
		case  0x24:
			p="Fingerprint library is empty";break;
		case  0x26:
			p="�Զ�ע�ᳬʱ";break;
		case  0x27:
			p="�Զ�ע��ָ���Ѵ���";break;
		default :
			p="ģ�鷵��ȷ��������";break;
	}
 return p;	
}





