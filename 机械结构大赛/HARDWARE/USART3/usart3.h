#ifndef USART3_H_
#define USART3_H_
#include "sys.h"
#include "stdio.h"	
#define USART3_REC_LEN  200  	//定义最大接收字节数 200
extern u8  USART3_RX_BUF[USART3_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节
extern u16 USART3_RX_STA;         		//接收状态标记	 0-14位为接收字节数，15位接收完成标志位
void usart3_init(u32 bound);
#endif

