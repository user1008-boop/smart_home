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
u8 humidityH;                                           //湿度数据高位
u8 humidityL;																						//湿度数据低位
u8 temperatureH;																				//温度数据高位
u8 temperatureL;																				//温度数据低位
u8 alarmFlag=0;                                         //报警标志位
char LD_BUF='0';																						//语音串口数据
float light;																						//光照强度
char Pub_Buff[256];																			//ESP8266上传数据包
u8 Led_status=0;	                                      //灯光标志位
u8 Fs_status=0;																					//风扇标志位
u8 key_num=0;                                           //按键标号
u8 Qt_status=1;																					//气体检测标志位(默认为高)
u8 Hw_status=0;																					//人体检测标志位
u8 Admin_Flag=0;																				//管理员标志位
SysPara LB301Para;																			//指纹模块LB301参数
u16 ValidN;																							//模块内有效模板个数
u16 ID; 																								//指纹ID号
u8 YY_flag=0;
u8 Zf_Flag=0;
u8 indexCount=0;
void Add_FR(void);																			//录指纹功能函数声明
void Del_FR(void);																			//删除指纹
void press_FR(void);																		//指纹验证功能函数

const char *devSubTopic[] = {"/mysmarthome/sub"};				//数据上行Topic
const char devPubTopic[] = {"/mysmarthome/pub"}; 				//数据下行Topic
/*
*********************************************************************************************************
*函数名称：ADD_FR
*函数功能：向ATK301指纹模块内部写入指纹数据
*入口参数：无
*返回参数：无
*********************************************************************************************************
*/
void Add_FR(void)
{
	u8 i=0,ensure ,processnum=1,pressCout=1, str_buffer[40];
	while(1)
	{
		switch (processnum)
		{
			case 1:                                           //连续按压4次指纹分别进行存储 
				i++;
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					i=0;
					delay_ms(100);																//模块内部处理图像时间缓冲
					ensure=PS_GenChar(CharBuffer1);               //生成指纹特征
					if(ensure==0x00)															
					{
						printf("指纹正确");
						pressCout++;
						if(pressCout >=5)
						{
							pressCout = 0;
							processnum=2;	 														//跳转至第二过程
						}					
					}				
				}				
				break;

			case 2:
				ensure=PS_RegMB();
				if(ensure==0x00) 
				{
					printf("新模板生成成功");
					processnum=3;																	//跳转至第三过程
				}
				delay_ms(1000);
				break;
				
			case 3:		
				do
					ID=ID+0x0001;																	//ID号默认每次自增1
				while(!(ID<300));																//添加ID限制
				printf("ID is %d",ID);
				ensure=PS_StoreMB(CharBuffer1,ID);							//储存指纹数据模板
				if(ensure==0x00) 
				{			
					printf("添加指纹成功");
					BEEP=1;
					delay_ms(100);
					BEEP=0;
					PS_ValidTempleteNum(&ValidN);									//读取库指纹个数
					return ;
				}					
				break;				
		}
		delay_ms(200);
		if(i>=20)																						//指纹检测超时
		{
			break;	
		}					
	}
}
/*
*********************************************************************************************************
*函数名称：press_FR
*函数功能：将当前检测指纹与库指纹进行比对验证
*入口参数：无
*返回参数：无
*********************************************************************************************************
*/
void press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	ensure=PS_AutoIdentify(&seach);												//指纹自动验证
	if(ensure==0x00)																			//自动验证成功
	{
 
		printf("ID:%d  匹配得分:%d\r\n",seach.pageID,seach.mathscore);
		Admin_Flag=1;																				//身份验证成功给与管理员权限
		
		angle_control(Neg, 90, 5000);//转向、角度、速度
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
	else if(ensure != 0xff)																//身份验证错误处理
	{
		printf("验证错误\r\n");
		//Admin_Flag=0;
		delay_ms(500);
	}
}
/*
*********************************************************************************************************
*函数名称：Del_FR
*函数功能：对ATK301内库指纹进行删除
*入口参数：无
*返回参数：无
*********************************************************************************************************
*/ 
void Del_FR(void)
{
	u8  ensure;
	ensure=PS_Empty();																			//清空指纹库
	if(ensure==0)
	{
		printf("删除成功");
		ID=0;
	}
	delay_ms(1500);
	PS_ValidTempleteNum(&ValidN);														//读库指纹个数
}
/*
*********************************************************************************************************
*函数名称：main
*函数功能：整个系统功能逻辑实现
*入口参数：无
*返回参数：无
*********************************************************************************************************
*/ 
int main(void)
{ 
		unsigned short timeCount = 0;													//发送间隔变量
		unsigned char *dataPtr = NULL;
		u8 ensure;																						//指纹参数
		u16 count = 0;																				//计数标志位用于定时发送TOPIC消息
		delay_init();	    	 																	//延时函数初始化	  
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				//设置中断优先级分组2
		LED_Init();		  																			//LED灯初始化
		BEEP_Init();																					//蜂鸣报警器初始化
		FS_Init();																						//风扇初始化
		KEY_Init();																						//按键初始化
		EXTIX_Init();																					//外部中断初始化
		DHT11_Init();																					//温湿度传感器初始化
		BH1750_Init();																				//光照传感器初始化
		ULN2003_Configuration();                              //步进电机初始化
		Usart1_Init(9600);																	  //串口1初始化，用于串口数据打印
		Usart2_Init(115200);																	//串口2初始化，用于与8266通信
		Usart3_Init(57600);																		//串口3初始化，用于与ATK301通信
		OLED_Init();																					//OLED屏初始化，用于数据显示
		OLED_ColorTurn(0);																		//0正常显示，1 反色显示
		OLED_DisplayTurn(0);																	//0正常显示 1 屏幕翻转显示
		TIM2_Int_Init(4999,7199); 														//定时器2初始化，用于屏幕数据更新定时
		TIM3_Int_Init(2499,7199);															//定时器3初始化，用于数据检测定时
		ESP8266_Init();																				//8266初始化，用于与移动端通信
		while(OneNet_DevLink())																//接入服务器
			delay_ms(500);																			//延时等待接入完成
		OLED_Clear();	                                        //显示屏清屏
		OneNet_Subscribe(devSubTopic, 1);											//订阅Topic
		ensure=PS_ValidTempleteNum(&ValidN);									//读取库指纹个数
		ensure=PS_ReadSysPara(&LB301Para);  									//读取指纹模块参数 
		while(1) 
		{
			if(key_num)																					//判断按键
			{
				printf("KEY_NUM:%d",key_num);
				if(key_num==1)                                    //按键KEY1按下
				{
						Add_FR();																			//录入指纹
						key_num=0;																		//标志清零
				}				
				else if(key_num==2)																//按键KEY_UP按下
				{
						Del_FR();																			//删除指纹
						key_num=0;																		//标志清零
				}
			}
			
			if(++count > 5) 																		//每运行五次进行一次指纹检测
			{
				count = 0;
				press_FR();																				//指纹识别
			}
			delay_ms(1);			
			
			if(timeCount%40==0)                                 //定时进行数据采集间隔为1秒
			{
				DHT11_Read_Data(&humidityH,&humidityL,&temperatureH,&temperatureL);//读取温湿度数据
				//UsartPrintf(USART_DEBUG,"温度：%d.%d  湿度：%d.%d ",humidityH,humidityL,temperatureH,temperatureL);
				if(!i2c_CheckDevice(BH1750_Addr))	                //判断设备是否正常接入并进行读数
				{
						light=LIght_Intensity();
						//UsartPrintf(USART_DEBUG,"当前光照度为:%.1f lx\r\n",light);
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
			if(++timeCount >= 200)															     //数据PUB时间间隔5s
			{
				Led_status = GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2);  //LED灯引脚电平检测
				Fs_status = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13);  //风扇引脚电平检测
				Qt_status = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9);   //MQ引脚电平检测
				Hw_status = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8);	 //红外模块电平检测
				alarmFlag = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8);   //蜂鸣器引脚电平检测
				if(Sw_Flag==0&&YY_flag==0)                             //风扇自动控制模式
				{
					if(temperatureH>23)
						Fs_status=1;
					else
						Fs_status=0;
				}
				else
				{
					Fs_status = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13);  //风扇引脚电平检测
				}
				if(((Hw_status==1&&Admin_Flag==0)||Qt_status==0)&&Zf_Flag==0)//报警系统
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
					*功能：将数据以JSON格式写入Pub_Buff，并发布TOPIC消息
				**/
				sprintf(Pub_Buff,"{\"Humi\":%d.%d,\"Temp\":%d.%d,\"Light\":%.1f,\"Led\":%d,\"Beep\":%d,\"Fs\":%d}",
				humidityH,humidityL,temperatureH,temperatureL,light,Led_status?0:1,alarmFlag,Fs_status);
				OneNet_Publish(devPubTopic,Pub_Buff);
				timeCount = 0;                                     //计数单位清零
				ESP8266_Clear();								                   //清空缓存
			}
			
			dataPtr = ESP8266_GetIPD(3);												 //下行TOPIC数据接收						
			if(dataPtr != NULL)
				OneNet_RevPro(dataPtr);
			
			/*语音控制部分逻辑*/
			
			if(LD_BUF!='0')
			{
				if(LD_BUF=='a')																			//接收命令为开灯
				{
					LED1=0;
					LD_BUF='0';																					//数据位归0，用于防止程序阻塞
				}
				if(LD_BUF=='b')																			//接收命令为关灯
				{
					LED1=1;
					LD_BUF='0';																					//数据位归0，用于防止程序阻塞
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
	
