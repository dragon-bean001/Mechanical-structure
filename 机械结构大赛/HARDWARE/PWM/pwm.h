#ifndef _PWM_H
#define _PWM_H	 
#include "sys.h"
//电机驱动的引脚
#define qian_AIN2 PCout(9)	
#define qian_AIN1 PCout(8)	
#define qian_BIN2 PCout(6)
#define qian_BIN1 PCout(7)	
#define qian_STBY PBout(3)

#define hou_AIN2 PBout(7)
#define hou_AIN1 PBout(6)
#define hou_BIN2 PBout(2)
#define hou_BIN1 PBout(5)
#define hou_STBY PBout(8)

void Set_qian_motor(int motor_01, int motor_02);
void Set_hou_motor(int motor_03, int motor_04);
void MOTOR_GPIO_init(void);
void TIM3_DIANJI_PWM_Init(u16 arr,u16 psc);
void TIM2_DUOJI_PWM_Init(u16 arr,u16 psc);
#endif
