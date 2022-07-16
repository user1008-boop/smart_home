#ifndef __LB301_H
#define __LB301_H
#include <stdio.h>
#include "sys.h" 

#define PS_Sta   PAin(6)//��ָ��ģ��״̬����
#define CharBuffer1 0x01
#define CharBuffer2 0x02
#define CharBuffer3 0x03
#define CharBuffer4 0x04



extern u32 LB301Addr;//ģ���ַ

typedef struct  
{
	u16 pageID;//ָ��ID
	u16 mathscore;//ƥ��÷�
}SearchResult;

typedef struct
{
	u16 PS_max;//ָ���������
	u8  PS_level;//��ȫ�ȼ�
	u32 PS_addr;
	u8  PS_size;//ͨѶ���ݰ���С
	u8  PS_N;//�����ʻ���N
}SysPara;

void PS_StaGPIO_Init(void);//��ʼ��PA6��״̬����
	
u8 PS_GetImage(void); //¼��ͼ�� 
 
u8 PS_GenChar(u8 BufferID);//�������� 

u8 PS_Search(u8 BufferID,u16 StartPage,u16 PageNum,SearchResult *p);//����ָ�� 
 
u8 PS_RegMB(void);//�ϲ�����������ģ�壩 
 
u8 PS_StoreMB(u8 BufferID,u16 PageID);//����ģ�� 

u8 PS_DeletChar(u16 PageID,u16 N);//ɾ��ģ�� 

u8 PS_Empty(void);//���ָ�ƿ� 

u8 PS_ReadSysPara(SysPara *p); //��ϵͳ�������� 

u8 PS_AutoIdentify(SearchResult *p);//�Զ���ָ֤��

u8 PS_ValidTempleteNum(u16 *ValidN);//����Чģ����� 

u8 PS_FingerMoudleSet(u8 N);//�޸Ĳ����� ע�⣺���ú�ģ��������Ч ���������ô��ڲ����ʲ���ͨѶ

const char *EnsureMessage(u8 ensure);//ȷ���������Ϣ����

#endif







