#include "led.h"
#include "delay.h"
#include "sys.h"
#include "time.h"
#include "pwm.h"
#include "usart3.h"
//ALIENTEK miniSTM32������ʵ��1
//�����ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
 int main(void)
 {	
		delay_init(); //��ʱ������ʼ��
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //���� NVIC �жϷ��� 2:2 λ
		MOTOR_GPIO_init();
		TIM3_DIANJI_PWM_Init(1999,719);//����Ƶ��  //PWMƵ��=72000000/(719+1)/(1999+1)=50hz=20mS
		TIM2_DUOJI_PWM_Init(1999,719);
	  usart3_init(9600);
							
			
		
		delay_ms(1000);
		while(1)
		{
			
			Set_qian_motor(1800, 1800);
			delay_ms(1000);
			//Set_qian_motor(-1000, -1000);
			delay_ms(1000);
		}
	 
	 
	 
 }

