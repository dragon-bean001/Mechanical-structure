#include "sys.h"
#include "usart3.h"
u8 USART3_RX_BUF[USART3_REC_LEN]; //���������洢���յ������ݣ���USART3_REC_LENΪ����ܽ��յ��ֽ��޶�
u16 USART3_RX_STA=0;         		//����״̬���	 0-14λΪ�����ֽ�����15λ������ɱ�־λ
void usart3_init(u32 bound)
{ 
	  GPIO_InitTypeDef GPIO_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructurea;
	  USART_InitTypeDef USART_InitStruct;
	 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ�ܴ���3
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //ʹ��GPIOB
	  USART_DeInit(USART3);//����3��λ ���Ǳ�Ҫ��һ��
	
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	  GPIO_Init(GPIOB,&GPIO_InitStructure);    //PB10����Ϊ�����������
	
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	  GPIO_Init(GPIOB,&GPIO_InitStructure);  //PB11����Ϊ��������	 
  
	  NVIC_InitStructurea.NVIC_IRQChannel=USART3_IRQn;
	  NVIC_InitStructurea.NVIC_IRQChannelCmd=ENABLE;
	  NVIC_InitStructurea.NVIC_IRQChannelPreemptionPriority=2;
	  NVIC_InitStructurea.NVIC_IRQChannelSubPriority=2;
	  NVIC_Init(&NVIC_InitStructurea);          //����3�жϵ�����
		
		USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//����3�ж�ʹ�ܺ��ж���������
	
	  USART_InitStruct.USART_BaudRate=bound;//���ڲ�����
		USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Rx;//�շ�ģʽ
		USART_InitStruct.USART_Parity=USART_Parity_No;//����żУ��λ
		USART_InitStruct.USART_StopBits=USART_StopBits_1;//һ��ֹͣλ
		USART_InitStruct.USART_WordLength=USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	  USART_Init(USART3,&USART_InitStruct);                 //����3��һЩ��������
		
		USART_Cmd(USART3,ENABLE);//����3ʹ��
	
	  
}
void USART3_IRQHandler()//����3���ж�
{ 
	 u8 res;//��ʱ������յ�����
   if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)//�ж��Ƿ�Ϊ����3�����ж�
	 {
		      
	     res=USART_ReceiveData(USART3);//���յ����ݷŽ�res
//		   USART3_RX_BUF[USART3_RX_STA&0x7FFF]=res;//���ݷŽ������У�������õ�main��������
//		   USART3_RX_STA++;                         //�ֽڳ���++
//		 if(USART3_RX_STA>(USART3_REC_LEN-1))USART3_RX_STA=0;//�������ݴ���,���¿�ʼ����	+; 
//		  USART3_RX_STA|=0x8000;//����3�������
//		 USART_ClearITPendingBit(USART3,USART_IT_RXNE);//��������жϱ�־
//	 
//	 
	 

		if((USART3_RX_STA&0x8000)==0)//����δ���
			{
			if(USART3_RX_STA&0x4000)//���յ���0x0d
				{
				if(res!=0x0a)USART3_RX_STA=0;//���մ���,���¿�ʼ
				else USART3_RX_STA|=0x8000;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(res==0x0d)USART3_RX_STA|=0x4000;
				else
					{
					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=res ;
					USART3_RX_STA++;
					if(USART3_RX_STA>(USART3_REC_LEN-1))USART3_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
//						 USART3_RX_STA|=0x8000;//����3�������
//							USART_ClearITPendingBit(USART3,USART_IT_RXNE);//��������жϱ�־
					}		 
				}
			}    
	  
		}
	}
