#include "sys.h"
#include "usart3.h"
u8 USART3_RX_BUF[USART3_REC_LEN]; //数组用来存储接收到的数据，而USART3_REC_LEN为最多能接收的字节限度
u16 USART3_RX_STA=0;         		//接收状态标记	 0-14位为接收字节数，15位接收完成标志位
void usart3_init(u32 bound)
{ 
	  GPIO_InitTypeDef GPIO_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructurea;
	  USART_InitTypeDef USART_InitStruct;
	 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能串口3
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //使能GPIOB
	  USART_DeInit(USART3);//串口3复位 不是必要的一步
	
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	  GPIO_Init(GPIOB,&GPIO_InitStructure);    //PB10设置为复用推挽输出
	
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	  GPIO_Init(GPIOB,&GPIO_InitStructure);  //PB11设置为浮空输入	 
  
	  NVIC_InitStructurea.NVIC_IRQChannel=USART3_IRQn;
	  NVIC_InitStructurea.NVIC_IRQChannelCmd=ENABLE;
	  NVIC_InitStructurea.NVIC_IRQChannelPreemptionPriority=2;
	  NVIC_InitStructurea.NVIC_IRQChannelSubPriority=2;
	  NVIC_Init(&NVIC_InitStructurea);          //串口3中断的设置
		
		USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//串口3中断使能和中断类型设置
	
	  USART_InitStruct.USART_BaudRate=bound;//串口波特率
		USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//无硬件数据流控制
		USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Rx;//收发模式
		USART_InitStruct.USART_Parity=USART_Parity_No;//无奇偶校验位
		USART_InitStruct.USART_StopBits=USART_StopBits_1;//一个停止位
		USART_InitStruct.USART_WordLength=USART_WordLength_8b;//字长为8位数据格式
	  USART_Init(USART3,&USART_InitStruct);                 //串口3的一些参数设置
		
		USART_Cmd(USART3,ENABLE);//串口3使能
	
	  
}
void USART3_IRQHandler()//串口3的中断
{ 
	 u8 res;//暂时缓存接收的数据
   if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)//判断是否为串口3接收中断
	 {
		      
	     res=USART_ReceiveData(USART3);//接收到数据放进res
//		   USART3_RX_BUF[USART3_RX_STA&0x7FFF]=res;//数据放进数组中，则可以用到main函数中了
//		   USART3_RX_STA++;                         //字节长度++
//		 if(USART3_RX_STA>(USART3_REC_LEN-1))USART3_RX_STA=0;//接收数据错误,重新开始接收	+; 
//		  USART3_RX_STA|=0x8000;//串口3接收完成
//		 USART_ClearITPendingBit(USART3,USART_IT_RXNE);//清除接收中断标志
//	 
//	 
	 

		if((USART3_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART3_RX_STA&0x4000)//接收到了0x0d
				{
				if(res!=0x0a)USART3_RX_STA=0;//接收错误,重新开始
				else USART3_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(res==0x0d)USART3_RX_STA|=0x4000;
				else
					{
					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=res ;
					USART3_RX_STA++;
					if(USART3_RX_STA>(USART3_REC_LEN-1))USART3_RX_STA=0;//接收数据错误,重新开始接收	  
//						 USART3_RX_STA|=0x8000;//串口3接收完成
//							USART_ClearITPendingBit(USART3,USART_IT_RXNE);//清除接收中断标志
					}		 
				}
			}    
	  
		}
	}
