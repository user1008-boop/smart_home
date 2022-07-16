#include "lb301dy.h"
#include "LB301.h"
#include "delay.h"
SysPara LB301Para;//指纹模块LB301参数
u16 ValidN;//模块内有效模板个数
u16 ID; 
void Add_FR(void);	//录指纹
void Del_FR(void);	//删除指纹
void press_FR(void);//刷指纹
//录指纹
void Add_FR(void)
{
	u8 i=0,ensure ,processnum=1,pressCout=1, str_buffer[40];
	while(1)
	{
		switch (processnum)
		{
			case 1: //连续按压4次指纹分别存到4个charBuffer里 
				i++;
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					i=0;
					delay_ms(100);//这里需要延时一下，模块内部处理图像需要时间
					ensure=PS_GenChar(CharBuffer1);//生成特征
					if(ensure==0x00)
					{
						printf("指纹正确");
						pressCout++;
						if(pressCout >=5)
						{
							pressCout = 0;
							processnum=2;//跳到第二步
						}					
					}				
				}				
				break;

			case 2:
				ensure=PS_RegMB();
				if(ensure==0x00) 
				{
					printf("新模板生成成功");
					processnum=3;//跳到第三步
				}
				delay_ms(1000);
				break;
				
			case 3:		
				do
					ID=ID+0x0001;
				while(!(ID<300));//输入ID必须小于300
				printf("ID is %d",ID);
				ensure=PS_StoreMB(CharBuffer1,ID);//储存模板
				if(ensure==0x00) 
				{			
					printf("添加指纹成功");
					PS_ValidTempleteNum(&ValidN);//读库指纹个数
					return ;
				}					
				break;				
		}
		delay_ms(200);
		if(i>=20)//超过20次没有按手指则退出
		{
			break;	
		}					
	}
}

#define USE_AUTO_IDETIFY  1 // 1使用自动验证方式  0使用分布式验证方式

//刷指纹
void press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	char *str;
#if (USE_AUTO_IDETIFY == 1) //自动验证指纹	
	ensure=PS_AutoIdentify(&seach);
	if(ensure==0x00)//自动验证成功
	{
	printf("自动验证成功");
		printf("Match ID:%d  Match score:%d",seach.pageID,seach.mathscore);
	}
#else  //分布式验证指纹	
	ensure=PS_GetImage();
	if(ensure==0x00)//获取图像成功 
	{	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //生成特征成功
		{		
			ensure=PS_HighSpeedSearch(CharBuffer1,0,300,&seach);
			if(ensure==0x00)//搜索成功
			{				
				printf("Match ID:%d  Match score:%d",seach.pageID,seach.mathscore);
			}			
	  }
	 delay_ms(1000);//延时后清除显示
	}
#endif	
}

//删除指纹

void Del_FR(void)
{
	u8  ensure;
	//printf("ID is %d",ID);
	//if(ID==0xFF00)
		ensure=PS_Empty();//清空指纹库
	/*else 
	{
		ensure=PS_DeletChar(ID,1);//删除单个指纹
		ID++;
		printf("ID is %d",ID);
	}*/
		
	if(ensure==0)
	{
		printf("删除成功");
	}
	delay_ms(1500);//延时后清除显示
	PS_ValidTempleteNum(&ValidN);//读库指纹个数
}