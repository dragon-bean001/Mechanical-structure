#ifndef USART3_H_
#define USART3_H_
#include "sys.h"
#include "stdio.h"	
#define USART3_REC_LEN  200  	//�����������ֽ��� 200
extern u8  USART3_RX_BUF[USART3_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�
extern u16 USART3_RX_STA;         		//����״̬���	 0-14λΪ�����ֽ�����15λ������ɱ�־λ
void usart3_init(u32 bound);
#endif

