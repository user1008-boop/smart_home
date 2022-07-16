/*
	连线表：
	STM32								ULN2003驱动
	PA4    	<-------> 	IN1
	PA5			<-------> 	IN2
	PA6			<-------> 	IN3
	PA7			<-------> 	IN4
	
	5V 28BYJ-48步进电机，内部带有减速装置 减速比：1/64   步距角：5.625°/64（64个脉冲转动5.625°）

正向转动：
	四相八拍控制顺序：(A-AB-B-BC-C-CD-D-DA)
				1		2		3		4		5		6		7		8
	A			-		-		+		+		+		+		+		-
	B			+		-		-		-		+		+		+		+
	C			+		+		+		-		-		-		+		+
	D			+		+		+		+		+		-		-		-
反向转动：(DA-D-CD-C-BC-B-AB-A)
*/

#include "motor.h"
#include "sys.h"
#include "delay.h"
void ULN2003_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_WriteBit(GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3, Bit_RESET);
}

//速度控制:
void stepper(unsigned int dir, unsigned int speed)
{
	if(dir == Pos)
	{
		//step1:
		IN1=1;
		IN2=0;
		IN3=0;
		IN4=0;
		delay_us(speed);
		
		//step2:	
		IN1=1;
		IN2=1;
		IN3=0;
		IN4=0;
		delay_us(speed);

		//step3:		
		IN1=0;
		IN2=1;
		IN3=0;
		IN4=0;
		delay_us(speed);

		//step4:
		IN1=0;
		IN2=1;
		IN3=1;
		IN4=0;
		delay_us(speed);

		//step5:
		IN1=0;
		IN2=0;
		IN3=1;
		IN4=0;
		delay_us(speed);

		//step6:
		IN1=0;
		IN2=0;
		IN3=1;
		IN4=1;
		delay_us(speed);

		//step7:
		IN1=0;
		IN2=0;
		IN3=0;
		IN4=1;
		delay_us(speed);

		//step8:
		IN1=1;
		IN2=0;
		IN3=0;
		IN4=1;
		delay_us(speed);
	}
	else
	{
		//step1:
		IN1=1;
		IN2=0;
		IN3=0;
		IN4=1;
		delay_us(speed);

		//step2:		
		IN1=0;
		IN2=0;
		IN3=0;
		IN4=1;
		delay_us(speed);

		//step3:		
		IN1=0;
		IN2=0;
		IN3=1;
		IN4=1;
		delay_us(speed);

		//step4:
		IN1=0;
		IN2=0;
		IN3=1;
		IN4=0;
		delay_us(speed);

		//step5:
		IN1=0;
		IN2=1;
		IN3=1;
		IN4=0;
		delay_us(speed);

		//step6:
		IN1=0;
		IN2=1;
		IN3=0;
		IN4=0;
		delay_us(speed);

		//step7:
		IN1=1;
		IN2=1;
		IN3=0;
		IN4=0;
		delay_us(speed);

		//step8:
		IN1=1;
		IN2=0;
		IN3=0;
		IN4=0;
		delay_us(speed);
	}
}

//角度控制：
void angle_control(unsigned int dir, double angle, unsigned int speed)
{
	int i;
	
	for(i = 0; i < angle / STEPPER_ANGLE; i++)
		stepper(dir, speed);
}
