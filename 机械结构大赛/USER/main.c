#include "led.h"
#include "delay.h"
#include "sys.h"
#include "time.h"
#include "pwm.h"
#include "usart3.h"
//ALIENTEK miniSTM32开发板实验1
//跑马灯实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司
 int main(void)
 {	
		delay_init(); //延时函数初始化
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置 NVIC 中断分组 2:2 位
		MOTOR_GPIO_init();
		TIM3_DIANJI_PWM_Init(1999,719);//不分频。  //PWM频率=72000000/(719+1)/(1999+1)=50hz=20mS
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

