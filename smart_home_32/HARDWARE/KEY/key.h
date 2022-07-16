#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
	 
#define KEY0 PCin(5)   	
#define KEY1 PAin(15)	 
#define WK_UP  PAin(0)	 

#define KEY0_PRES	1									//KEY0  
#define KEY1_PRES	2									//KEY1 
#define WKUP_PRES	3									//WK_UP  

void KEY_Init(void);								//按键初始化
u8 KEY_Scan(u8 mode);  							//按键扫描函数声明					    
#endif
