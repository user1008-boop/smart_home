#ifndef __LB301_H
#define __LB301_H
#include <stdio.h>
#include "sys.h" 

#define PS_Sta   PAin(6)//读指纹模块状态引脚
#define CharBuffer1 0x01
#define CharBuffer2 0x02
#define CharBuffer3 0x03
#define CharBuffer4 0x04



extern u32 LB301Addr;//模块地址

typedef struct  
{
	u16 pageID;//指纹ID
	u16 mathscore;//匹配得分
}SearchResult;

typedef struct
{
	u16 PS_max;//指纹最大容量
	u8  PS_level;//安全等级
	u32 PS_addr;
	u8  PS_size;//通讯数据包大小
	u8  PS_N;//波特率基数N
}SysPara;

void PS_StaGPIO_Init(void);//初始化PA6读状态引脚
	
u8 PS_GetImage(void); //录入图像 
 
u8 PS_GenChar(u8 BufferID);//生成特征 

u8 PS_Search(u8 BufferID,u16 StartPage,u16 PageNum,SearchResult *p);//搜索指纹 
 
u8 PS_RegMB(void);//合并特征（生成模板） 
 
u8 PS_StoreMB(u8 BufferID,u16 PageID);//储存模板 

u8 PS_DeletChar(u16 PageID,u16 N);//删除模板 

u8 PS_Empty(void);//清空指纹库 

u8 PS_ReadSysPara(SysPara *p); //读系统基本参数 

u8 PS_AutoIdentify(SearchResult *p);//自动验证指纹

u8 PS_ValidTempleteNum(u16 *ValidN);//读有效模板个数 

u8 PS_FingerMoudleSet(u8 N);//修改波特率 注意：设置后模块立即生效 需重新配置串口波特率才能通讯

const char *EnsureMessage(u8 ensure);//确认码错误信息解析

#endif







