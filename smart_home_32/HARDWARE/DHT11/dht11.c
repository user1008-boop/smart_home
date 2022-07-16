#include "dht11.h"
#include "delay.h"
/**
 *函数名称：DHT11_Rst
 *函数功能：DHT1进行复位
 *入口参数：无
 *返回参数：无
 *作者：deng
 *时间：2022.12.23
**/ 
void DHT11_Rst(void)	   
{                 
		DHT11_IO_OUT(); 							//设置输出模式
    DHT11_DQ_OUT=0; 							//拉低DQ
    delay_ms(20);    							//拉低至少18ms
    DHT11_DQ_OUT=1; 							//DQ=1 
		delay_us(30);     						//主机拉高20~40us
}

/**
 *函数名称：DHT11_Check
 *函数功能：传感器检测
 *入口参数：无
 *返回参数：1：正常接入，0：未识别
 *作者：deng
 *时间：2022.12.23
**/ 
u8 DHT11_Check(void) 	   
{   
	u8 retry=0;
	DHT11_IO_IN();                      //设置输入模式	 
  while (DHT11_DQ_IN&&retry<100)      //DHT11会拉低40~80us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)
		return 1;
	else 
		retry=0;
  while (!DHT11_DQ_IN&&retry<100)  //DHT11拉低后会再次被拉高40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)
		return 1;	    
	return 0;
}
/**
 *函数名称：DHT11_Read_Bit
 *函数功能：从DHT11读取一位数据
 *入口参数：无
 *返回参数：0/1
 *作者：deng
 *时间：2022.12.23
**/ 
u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(DHT11_DQ_IN&&retry<100)					//等待变为低电平
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)			  //等待变为高电平
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);
	if(DHT11_DQ_IN)
		return 1;
	else 
		return 0;		   
}
/**
 *函数名称：DHT11_Read_Byte
 *函数功能：从DHT11读取一个字节数据
 *入口参数：无
 *返回参数：读取到的数据
 *作者：deng
 *时间：2022.12.23
**/ 
u8 DHT11_Read_Byte(void)    
{        
    u8 i,dat;
    dat=0;
	for (i=0;i<8;i++)                   //循环移位读数
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
}
/**
 *函数名称：DHT11_Read_Data
 *函数功能：从DHT11读取一次数据
 *入口参数：温湿度数据的高低位
 *返回参数：0：读取成功，1：读取失败
 *作者：deng
 *时间：2022.12.23
**/ 
u8 DHT11_Read_Data(u8 *humiH,u8 *humiL,u8 *tempH,u8 *tempL)    
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//读取40位数据
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humiH=buf[0];			//坑啊原子哥，说明书明明是湿度在前温度在后
			*humiL=buf[1];			
			*tempH=buf[2];
			*tempL=buf[3];
				
		}
	}else return 1;
	return 0;	    
}
/**
 *函数名称：DHT11——Init
 *函数功能：DHT11初始化
 *入口参数：无
 *返回参数：0/1
 *作者：deng
 *时间：2022.12.23
**/   	 
u8 DHT11_Init(void)
{	 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	    //使能PA端口时钟
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 					//PA8端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 			//推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;					//IO口速率
 	GPIO_Init(GPIOA, &GPIO_InitStructure);				 						//初始化IO口
 	GPIO_SetBits(GPIOA,GPIO_Pin_8);						 								//PA8输出高
			    
	DHT11_Rst();  																						//复位
	return DHT11_Check();																			//等待回应
} 









