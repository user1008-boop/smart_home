#include "dht11.h"
#include "delay.h"
/**
 *�������ƣ�DHT11_Rst
 *�������ܣ�DHT1���и�λ
 *��ڲ�������
 *���ز�������
 *���ߣ�deng
 *ʱ�䣺2022.12.23
**/ 
void DHT11_Rst(void)	   
{                 
		DHT11_IO_OUT(); 							//�������ģʽ
    DHT11_DQ_OUT=0; 							//����DQ
    delay_ms(20);    							//��������18ms
    DHT11_DQ_OUT=1; 							//DQ=1 
		delay_us(30);     						//��������20~40us
}

/**
 *�������ƣ�DHT11_Check
 *�������ܣ����������
 *��ڲ�������
 *���ز�����1���������룬0��δʶ��
 *���ߣ�deng
 *ʱ�䣺2022.12.23
**/ 
u8 DHT11_Check(void) 	   
{   
	u8 retry=0;
	DHT11_IO_IN();                      //��������ģʽ	 
  while (DHT11_DQ_IN&&retry<100)      //DHT11������40~80us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)
		return 1;
	else 
		retry=0;
  while (!DHT11_DQ_IN&&retry<100)  //DHT11���ͺ���ٴα�����40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)
		return 1;	    
	return 0;
}
/**
 *�������ƣ�DHT11_Read_Bit
 *�������ܣ���DHT11��ȡһλ����
 *��ڲ�������
 *���ز�����0/1
 *���ߣ�deng
 *ʱ�䣺2022.12.23
**/ 
u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(DHT11_DQ_IN&&retry<100)					//�ȴ���Ϊ�͵�ƽ
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)			  //�ȴ���Ϊ�ߵ�ƽ
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
 *�������ƣ�DHT11_Read_Byte
 *�������ܣ���DHT11��ȡһ���ֽ�����
 *��ڲ�������
 *���ز�������ȡ��������
 *���ߣ�deng
 *ʱ�䣺2022.12.23
**/ 
u8 DHT11_Read_Byte(void)    
{        
    u8 i,dat;
    dat=0;
	for (i=0;i<8;i++)                   //ѭ����λ����
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
}
/**
 *�������ƣ�DHT11_Read_Data
 *�������ܣ���DHT11��ȡһ������
 *��ڲ�������ʪ�����ݵĸߵ�λ
 *���ز�����0����ȡ�ɹ���1����ȡʧ��
 *���ߣ�deng
 *ʱ�䣺2022.12.23
**/ 
u8 DHT11_Read_Data(u8 *humiH,u8 *humiL,u8 *tempH,u8 *tempL)    
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//��ȡ40λ����
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humiH=buf[0];			//�Ӱ�ԭ�Ӹ磬˵����������ʪ����ǰ�¶��ں�
			*humiL=buf[1];			
			*tempH=buf[2];
			*tempL=buf[3];
				
		}
	}else return 1;
	return 0;	    
}
/**
 *�������ƣ�DHT11����Init
 *�������ܣ�DHT11��ʼ��
 *��ڲ�������
 *���ز�����0/1
 *���ߣ�deng
 *ʱ�䣺2022.12.23
**/   	 
u8 DHT11_Init(void)
{	 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	    //ʹ��PA�˿�ʱ��
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 					//PA8�˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 			//�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;					//IO������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);				 						//��ʼ��IO��
 	GPIO_SetBits(GPIOA,GPIO_Pin_8);						 								//PA8�����
			    
	DHT11_Rst();  																						//��λ
	return DHT11_Check();																			//�ȴ���Ӧ
} 









