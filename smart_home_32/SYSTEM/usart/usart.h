#ifndef _USART_H_
#define _USART_H_


#include "stm32f10x.h"


#define USART_DEBUG		USART1											//���Դ�ӡ��ʹ�õĴ�����
#define USART3_MAX_RECV_LEN		400									//�����ջ����ֽ���
#define USART3_MAX_SEND_LEN		400									//����ͻ����ֽ���
#define USART3_RX_EN 			1												//0,������;1,����.

extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//���ջ���,���USART2_MAX_RECV_LEN�ֽ�
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//���ͻ���,���USART2_MAX_SEND_LEN�ֽ�
extern vu16 USART3_RX_STA;   											//��������״̬
void u3_printf(char* fmt,...);

void Usart1_Init(unsigned int baud);

void Usart2_Init(unsigned int baud);

void Usart3_Init(unsigned int baud);

void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);

void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...);

void DEBUG_LOG(char *fmt,...);

#endif
