#include "pwm.h"
#include "led.h"
//PWM 输出初始化
//arr：自动重装值
//psc：时钟预分频数

void MOTOR_GPIO_init(void)
{
	GPIO_InitTypeDef  GPIOB_InitStructure;
	GPIO_InitTypeDef  GPIOC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PC端口时钟

	GPIOB_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;				 // 端口配置
	GPIOB_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIOB_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	
	GPIOC_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;				 //端口配置
	GPIOC_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIOC_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	
	GPIO_Init(GPIOB, &GPIOB_InitStructure);					 //根据设定参数初始化
	GPIO_Init(GPIOC, &GPIOC_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8);				//输出高电平		 
	GPIO_SetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);	//初始化都为高电平
	
}

void TIM3_DIANJI_PWM_Init(u16 arr,u16 psc)
{
		GPIO_InitTypeDef GPIOA_InitStructure;
	  GPIO_InitTypeDef GPIOB_InitStructure;
	
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_OCInitTypeDef TIM_OCInitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);// ①使能 TIM3 时钟
	
	  //①使能 GPIO 外设时钟使能
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
   	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	
		
		//设置该引脚为复用输出功能,输出 TIM3 CH2 的 PWM 脉冲波形
		GPIOA_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; //TIM_CH1、ch2
		GPIOA_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
		GPIOA_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
		GPIOB_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //TIM_CH3、ch4
		GPIOB_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
		GPIOB_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	  GPIO_Init(GPIOA, &GPIOA_InitStructure);
		GPIO_Init(GPIOB, &GPIOB_InitStructure);
		
		TIM_TimeBaseStructure.TIM_Period = arr;
		//设置在下一个更新事件装入活动的自动重装载寄存器周期的值 80K
		TIM_TimeBaseStructure.TIM_Prescaler =psc;
		//设置用来作为 TIMx 时钟频率除数的预分频值 不分频
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
		
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //②初始化 TIMx
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //脉宽调制模式 2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
		TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性高
		
		TIM_OC1Init(TIM3, &TIM_OCInitStructure); //③初始化外设 TIMx通道1
		TIM_OC2Init(TIM3, &TIM_OCInitStructure); //③初始化外设 TIMx通道2
		TIM_OC3Init(TIM3, &TIM_OCInitStructure); //③初始化外设 TIMx通道3
		TIM_OC4Init(TIM3, &TIM_OCInitStructure); //③初始化外设 TIMx通道4
		
		TIM_CtrlPWMOutputs(TIM3,ENABLE); //⑤MOE 主输出使能
		
		TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); //CH1 预装载使能
		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); //CH2 预装载使能
		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); //CH3 预装载使能
		TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable); //CH4 预装载使能
		
		TIM_ARRPreloadConfig(TIM3, ENABLE); //使能 TIMx 在 ARR 上的预装载寄存器
		TIM_Cmd(TIM3, ENABLE); //④使能 TIM3
	
}

//取绝对值函数
int GFP_abs(int p)
{
  int q;
	
	if(p>0) q=p;
	else    q=-p;
	
	return q;
}

//设置电机的PWM以及转向
void Set_qian_motor(int motor_01, int motor_02)
{
  if(motor_01>0)
	{
	  qian_AIN1=1;//正转
		qian_AIN2=0;
	}
	else
	{
	  qian_AIN1=0;//反转
		qian_AIN2=1;
	}
	//研究pwm值
	TIM_SetCompare1(TIM3,GFP_abs(motor_01));//PA6
	
	
	if(motor_02>0)
	{
	  qian_BIN1=1;//正转
		qian_BIN2=0;
	}
	else
	{
	  qian_BIN1=0;//反转
		qian_BIN2=1;
	}
	//研究pwm值
	TIM_SetCompare2(TIM3,GFP_abs(motor_02));//PA7
	
}

void Set_hou_motor(int motor_03, int motor_04)
{
  if(motor_03>0)
	{
	  hou_AIN1=1;//正转
		hou_AIN2=0;
	}
	else
	{
	  hou_AIN1=0;//反转
		hou_AIN2=1;
	}
	//研究pwm值
	TIM_SetCompare3(TIM3,GFP_abs(motor_03));//PB0
	
	
	if(motor_04>0)
	{
	  hou_BIN1=1;//正转
		hou_BIN2=0;
	}
	else
	{
	  hou_BIN1=0;//反转
		hou_BIN2=1;
	}
	//研究pwm值
	TIM_SetCompare4(TIM3,GFP_abs(motor_04));//PB1
	
}

//舵机的pwm,PA0,A1,A2,A3->ch1,ch2,ch3,ch4
void TIM2_DUOJI_PWM_Init(u16 arr,u16 psc)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_OCInitTypeDef TIM_OCInitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);// ①使能 TIM2 时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
		//①使能 GPIO 外设时钟使能
		//设置该引脚为复用输出功能,输出 TIM2 CH2 的 PWM 脉冲波形
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; //TIM_CH1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		
		TIM_TimeBaseStructure.TIM_Period = arr;
		//设置在下一个更新事件装入活动的自动重装载寄存器周期的值 80K
		TIM_TimeBaseStructure.TIM_Prescaler =psc;
		//设置用来作为 TIMx 时钟频率除数的预分频值 不分频
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //②初始化 TIMx
	
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //脉宽调制模式 2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
		TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性高
		
		
		TIM_OC2Init(TIM2, &TIM_OCInitStructure); //③初始化外设 TIMx
		TIM_OC3Init(TIM2, &TIM_OCInitStructure); //③初始化外设 TIMx
		TIM_OC4Init(TIM2, &TIM_OCInitStructure); //③初始化外设 TIMx
		
		TIM_CtrlPWMOutputs(TIM2,ENABLE); //⑤MOE 主输出使能
		
		
		TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable); //CH2 预装载使能
		TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable); //CH3 预装载使能
		TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable); //CH4 预装载使能
		
		TIM_ARRPreloadConfig(TIM2, ENABLE); //使能 TIMx 在 ARR 上的预装载寄存器
		TIM_Cmd(TIM2, ENABLE); //④使能 TIM2
	
	
	
}



