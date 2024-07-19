/**
 * @FilePath     : /MDK-ARMc:/Users/fu/Desktop/Code/R9_407E/R9_407_E/Drivers/BSP/LED/led.c
 * @Description  :  LED Control
 * @Author       : lisir lisir@rehand.com
 * @Version      : 0.0.1
 * @LastEditors  : lisir lisir@rehand.com
 * @LastEditTime : 2024-07-17 14:44:52
 * @2024 by Rehand Medical Technology Co., LTD, All Rights Reserved.
**/


#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/R9/Slavemodbus.h"
TIM_HandleTypeDef g_time2_pwm_chy_handle;  /* LED PWM */

void LEDmain_pwm_chy_init(uint16_t arr, uint16_t psc) /*1KHz 3%*/
{
	GPIO_InitTypeDef gpio_init_struct;
	FRONT_MAIN_GPIO_CLK_ENABLE();
	BACK_MAIN_GPIO_CLK_ENABLE();                                                /* 开启通道y的GPIO时钟 */
	__HAL_RCC_TIM2_CLK_ENABLE();                                                      /* 使能定时器时钟 */
	gpio_init_struct.Pin = GPIO_PIN_10 | GPIO_PIN_11; /* 通道1 2的CPIO口 */
	gpio_init_struct.Mode = GPIO_MODE_AF_PP;                                            /* 复用推完输出 */
	gpio_init_struct.Pull = GPIO_PULLUP;                                                /* 上拉 */
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;                                      /* 高速 */
	gpio_init_struct.Alternate = GPIO_AF1_TIM2;                             /* IO口REMAP设置, 是否必要查看头文件配置的说明! */
	HAL_GPIO_Init(GPIOB, &gpio_init_struct);
    TIM_OC_InitTypeDef tim2_oc_pwm_chy = {0};                                                /* 定时器9输出句柄 */
    g_time2_pwm_chy_handle.Instance = TIM2;                                         /* 定时器x */
    g_time2_pwm_chy_handle.Init.Prescaler = psc;                                             /* 预分频系数 */
    g_time2_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;                            /* 递增计数模式 */
    g_time2_pwm_chy_handle.Init.Period = arr;                                                /* 自动重装载值 */
    HAL_TIM_PWM_Init(&g_time2_pwm_chy_handle);                                               /* 初始化PWM */
    tim2_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                                                /* 模式选择PWM1 */
    tim2_oc_pwm_chy.Pulse = 0;                                                               /* 设置比较值,此值用来确定占空比 */
    tim2_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_HIGH;                                         /* 输出比较极性为低 */
    HAL_TIM_PWM_ConfigChannel(&g_time2_pwm_chy_handle, &tim2_oc_pwm_chy, TIM_CHANNEL_3); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time2_pwm_chy_handle, TIM_CHANNEL_3);                           /* 开启对应PWM通道 */
    HAL_TIM_PWM_ConfigChannel(&g_time2_pwm_chy_handle, &tim2_oc_pwm_chy, TIM_CHANNEL_4); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time2_pwm_chy_handle, TIM_CHANNEL_4);                           /* 开启对应PWM通道 */
}

void LED_pwm_Set(uint16_t ccr3,uint16_t ccr4)
{
	TIM2->CCR3 = ccr3;
	TIM2->CCR4 = ccr4;	
}

void Main_PwmSet(uint8_t flage)
{
	if (flage)
	{
		LED_pwm_Set(2000,2000);
	}
	else
	{
		LED_pwm_Set(20,20);
	}
	
}
void led_init(void)
{
	GPIO_InitTypeDef gpio_init_struct;

	LED0_GPIO_CLK_ENABLE(); /* //LED0?????? */
	LED1_GPIO_CLK_ENABLE(); /* //LED1?????? */

	gpio_init_struct.Pin = LED0_GPIO_PIN;			  /* //LED0???? */
	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;	  /* ??????? */
	gpio_init_struct.Pull = GPIO_PULLUP;			  /* ???? */
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;	  /* ???? */
	HAL_GPIO_Init(LED0_GPIO_PORT, &gpio_init_struct); /* ?????//LED0???? */

	gpio_init_struct.Pin = LED1_GPIO_PIN;			  /* //LED1???? */
	HAL_GPIO_Init(LED1_GPIO_PORT, &gpio_init_struct); /* ?????//LED1???? */



	LEFT_FRONT_TURE_GPIO_CLK_ENABLE();
	RIGHT_FRONT_TURE_GPIO_CLK_ENABLE();


	gpio_init_struct.Pin = LEFT_FRONT_TURE_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;   /* ??????? */
	gpio_init_struct.Pull = GPIO_PULLUP;		   /* ???? */
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH; /* ???? */
	HAL_GPIO_Init(LEFT_FRONT_TURE_GPIO_PORT, &gpio_init_struct);

	gpio_init_struct.Pin = RIGHT_FRONT_TURE_GPIO_PIN;
	HAL_GPIO_Init(RIGHT_FRONT_TURE_GPIO_PORT, &gpio_init_struct);

	LEFT_BACK_TURE_GPIO_CLK_ENABLE();
	RIGHT_BACK_TURE_GPIO_CLK_ENABLE();

	gpio_init_struct.Pin = LEFT_BACK_TURE_GPIO_PIN | RIGHT_BACK_TURE_GPIO_PIN;
	HAL_GPIO_Init(LEFT_BACK_TURE_GPIO_PORT, &gpio_init_struct);

	LED0(1); /* ??? //LED0 */
	LED1(1); /* ??? //LED1 */
    LEDmain_pwm_chy_init(2000, 42); /*84000000/2000*42 - 1KHz*/
	LEFT_FRONT_TURE(0);
	g_slaveReg[14] = LeftBulbState;
	RIGHT_FRONT_TURE(0);
	g_slaveReg[15] = RightBulbState;
	FRONT_MAIN(0);
	g_slaveReg[13] = MainBulbState;
	LEFT_BACK_TURE(0);
	RIGHT_BACK_TURE(0);
	BACK_MAIN(0);
}

/**
 * @param
 *   @arg
 *
 *   @arg
 * @retval
 *   
 */

Led_State led_state;
Led_State lastled_state;
uint8_t doubleflingflage = 1;


void led_beepControl(void)
{  
	#if defined KEYCONTRONL
		static uint8_t cmdBulb_contes = 1; // 移动端主灯指令计数器
		static uint8_t KEY2_PRES_contes = 1; // 左转向灯指令计数器
		static uint8_t KEY3_PRES_contes = 1; // 右转向灯指令计数器
		static uint8_t KEY4_PRES_contes = 1; // 360view全景指令计数器
		static uint8_t KEYDouble_PRES_contes = 1; // 双闪指令计数器
		lastled_state = None;
		if (led_state != lastled_state)
		{
			lastled_state = led_state;
		}
	// beepcontrol 
		if (key_scan1() == 1 ) 
		{
			g_slaveReg[35] = 1;
		}
		else
		{
			g_slaveReg[35] = 0;
		}

	/* left   right and  maibulb control*/
		if (cmdBulb_contes==1  && KEY2_PRES_contes==1 && KEY3_PRES_contes==1 && KEYDouble_PRES_contes==1)
		{
			led_state = idle_state;
		}

	// maibulb control
		if (keycmdbulb() == 1 ) 
		{

			cmdBulb_contes++;

			if (cmdBulb_contes > 2)
				cmdBulb_contes = 1;
			if (cmdBulb_contes % 2 == 0 )
			{
				led_state = open_mainbulb;
				cmdBulb_contes = 1;
			}
			else
			{
				led_state = close_mainbulb;
			}
		}

	// left control
		if (key_scan2() == 1 || keycmdleftbulb()==1)
		{

			KEY2_PRES_contes++;

			if (KEY2_PRES_contes > 2)
				KEY2_PRES_contes = 1;
			if (KEY2_PRES_contes % 2 == 0)
			{
				led_state = open_leftbling;
				KEY3_PRES_contes = 1;           // close right
				KEYDouble_PRES_contes = 1;     // close double
			}
			else
			{
				led_state = close_leftbling;
			}
		}

		/*right */
		if (key_scan3() == 1 || keycmdrightbulb()==1 )
		{

			KEY3_PRES_contes++;

			if (KEY3_PRES_contes > 2)
				KEY3_PRES_contes = 1;
			if (KEY3_PRES_contes % 2 == 0)
			{
				led_state = open_rightbling;
				KEY2_PRES_contes = 1;       // close left
				KEYDouble_PRES_contes = 1;     // close doubles
			}
			else
			{
				led_state = close_rightbling;
			}
		}
		/*double*/
		if (key_scandouble() == 1)
		{

			KEYDouble_PRES_contes++;

			if (KEYDouble_PRES_contes > 2)
				KEYDouble_PRES_contes = 1;
			if (KEYDouble_PRES_contes % 2 == 0)
			{
				led_state = open_doublebling;
				
				KEY3_PRES_contes = 1; // close right
				KEY2_PRES_contes = 1;// close left
				cmdBulb_contes =1 ; //  close mainbulb
			}
			else
			{
				led_state = close_doublebling;
			}
		}

		switch (led_state)
		{
		case idle_state:
			LEFT_FRONT_TURE(0);
			g_slaveReg[14] = LeftBulbState;
			RIGHT_FRONT_TURE(0);
			g_slaveReg[15] = RightBulbState;
			FRONT_MAIN(0);
			g_slaveReg[13] = MainBulbState;
			LEFT_BACK_TURE(0);		
			RIGHT_BACK_TURE(0);	
			BACK_MAIN(0);
			break;

		case open_leftbling:
			LEFT_FRONT_TURE_TOGGLE();
			g_slaveReg[14] = LeftBulbState;
			RIGHT_FRONT_TURE(0);
			g_slaveReg[15] = RightBulbState;
			LEFT_BACK_TURE(1);	
			RIGHT_BACK_TURE(0);
		
			break;

		case close_leftbling:

			LEFT_FRONT_TURE(0);
			g_slaveReg[14] = LeftBulbState;
			LEFT_BACK_TURE(0);
			
			break;

		case open_rightbling:
			RIGHT_FRONT_TURE_TOGGLE();
			g_slaveReg[15] = RightBulbState;
			LEFT_FRONT_TURE(0);
			g_slaveReg[14] = LeftBulbState;

			LEFT_BACK_TURE(0);
			
			RIGHT_BACK_TURE(1);
			

			break;

		case close_rightbling:

			RIGHT_FRONT_TURE(0);
			g_slaveReg[15] = RightBulbState;
			RIGHT_BACK_TURE(0);
			

			break;

		case open_doublebling:
			FRONT_MAIN(0);
			g_slaveReg[13] = MainBulbState;
			BACK_MAIN(0);
			LEFT_BACK_TURE(1);
			
			RIGHT_BACK_TURE(1);
			
			if (doubleflingflage)
			{
				LEFT_FRONT_TURE(0);
				g_slaveReg[14] = LeftBulbState;
				RIGHT_FRONT_TURE(0);
				g_slaveReg[15] = RightBulbState;
				FRONT_MAIN(0);
				g_slaveReg[13] = MainBulbState;
				doubleflingflage = 0;
			}
			else
			{
				doubleflingflage = 1;
				FRONT_MAIN(0);
				g_slaveReg[13] = MainBulbState;
				LEFT_FRONT_TURE(1);
				g_slaveReg[14] = LeftBulbState;
				RIGHT_FRONT_TURE(1);
				g_slaveReg[15] = RightBulbState;
			}

			if (lastled_state == open_leftbling || lastled_state == open_rightbling) 
			{
				LEFT_BACK_TURE(0);
				
				RIGHT_BACK_TURE(0);
				
				delay_ms(10);
			}

			break;

		case close_doublebling:
			LEFT_FRONT_TURE(0);
			g_slaveReg[14] = LeftBulbState;
			RIGHT_FRONT_TURE(0);
			g_slaveReg[15] = RightBulbState;
			LEFT_BACK_TURE(0);
			
			RIGHT_BACK_TURE(0);
			

			break;

		case open_mainbulb:
			BACK_MAIN(1);

			FRONT_MAIN(1);
			g_slaveReg[13] = MainBulbState;

			if (lastled_state == open_leftbling) 
			{
				led_state = open_leftbling;
			}
			if (lastled_state == open_rightbling) 
			{
				led_state = open_rightbling;
			}
			if (lastled_state == open_doublebling) 
			{
				LEFT_FRONT_TURE(0);
				g_slaveReg[14] = LeftBulbState;
				RIGHT_FRONT_TURE(0);
				g_slaveReg[15] = RightBulbState;
				LEFT_BACK_TURE(0);
				
				RIGHT_BACK_TURE(0);
				
				FRONT_MAIN(0);
				g_slaveReg[13] = MainBulbState;
				led_state = open_mainbulb;
			}

			break;

		case close_mainbulb:
			FRONT_MAIN(0);
			g_slaveReg[13] = MainBulbState;
			BACK_MAIN(0);
			if (lastled_state == open_leftbling) 
			{
				led_state = open_leftbling;
			}
			if (lastled_state == open_rightbling) 
			{
				led_state = open_rightbling;
			}
			break;
		default:
			break;
		}
	
	#endif
}

/**
 * @description: 上位机控制 LED灯
 * @return {*}
 */
void led_beepControlRK3588(void)
{
	lastled_state = None;
	if (led_state != lastled_state)
	{
		lastled_state = led_state;
	}
/* left   right and  maibulb control
82 : 左转向指令
83 ：右转向指令
84 ：照明指令
85： 双闪指令
*/
	if (g_slaveReg[82] == 0  && g_slaveReg[83] == 0 && g_slaveReg[84] == 0 && g_slaveReg[85]== 0 )
	{
		led_state = idle_state;
	}
// maibulb control
	if (g_slaveReg[84] ) 
	{
		led_state = open_mainbulb;
		BACK_MAIN(1);
		FRONT_MAIN(1);
		g_slaveReg[13] = MainBulbState;
	}
	else
	{		
		led_state = close_mainbulb;	
		FRONT_MAIN(0);
		g_slaveReg[13] = MainBulbState;
		BACK_MAIN(0);

	}

// left && right control
	if (g_slaveReg[82]== 1 && g_slaveReg[83]== 0 && g_slaveReg[85] == 0)
	{
		led_state = open_leftbling;
	}
	if (g_slaveReg[82]==0 && g_slaveReg[83]== 1 && g_slaveReg[85] == 0)
	{
		led_state = open_rightbling;
	}
/*double*/
	if (g_slaveReg[85] )
	{
		led_state = open_doublebling;
	}		
	else if (g_slaveReg[85] ==0 && g_slaveReg[82]== 0 && g_slaveReg[83]== 0 )
	{
		led_state = close_doublebling;
	}

/*bulb driving*/
	switch (led_state)
	{
	case idle_state:
		LEFT_FRONT_TURE(0);
		g_slaveReg[14] = LeftBulbState;
		RIGHT_FRONT_TURE(0);
		g_slaveReg[15] = RightBulbState;
		FRONT_MAIN(0);
		g_slaveReg[13] = MainBulbState;
		LEFT_BACK_TURE(0);		
		RIGHT_BACK_TURE(0);	
		BACK_MAIN(0);
		break;
	case open_leftbling:
		LEFT_FRONT_TURE_TOGGLE();
		g_slaveReg[14] = LeftBulbState;
		RIGHT_FRONT_TURE(0);
		g_slaveReg[15] = RightBulbState;
		LEFT_BACK_TURE(1);	
		RIGHT_BACK_TURE(0);
		break;
	case close_leftbling:
		LEFT_FRONT_TURE(0);
		g_slaveReg[14] = LeftBulbState;
		LEFT_BACK_TURE(0);	
		break;
	case open_rightbling:
		RIGHT_FRONT_TURE_TOGGLE();
		g_slaveReg[15] = RightBulbState;
		LEFT_FRONT_TURE(0);
		g_slaveReg[14] = LeftBulbState;
		LEFT_BACK_TURE(0);	
		RIGHT_BACK_TURE(1);
		break;
	case close_rightbling:
		RIGHT_FRONT_TURE(0);
		g_slaveReg[15] = RightBulbState;
		RIGHT_BACK_TURE(0);
		break;
	case open_doublebling:
		// g_slaveReg[13] = MainBulbState;
		// BACK_MAIN(0);
		LEFT_BACK_TURE(1);	
		RIGHT_BACK_TURE(1);	
		if (doubleflingflage)
		{
			LEFT_FRONT_TURE(0);
			g_slaveReg[14] = LeftBulbState;
			RIGHT_FRONT_TURE(0);
			g_slaveReg[15] = RightBulbState;
			// FRONT_MAIN(0);
			// g_slaveReg[13] = MainBulbState;
			doubleflingflage = 0;
		}
		else
		{
			doubleflingflage = 1;
			// FRONT_MAIN(0);
			// g_slaveReg[13] = MainBulbState;
			LEFT_FRONT_TURE(1);
			g_slaveReg[14] = LeftBulbState;
			RIGHT_FRONT_TURE(1);
			g_slaveReg[15] = RightBulbState;
		}
		if (lastled_state == open_leftbling || lastled_state == open_rightbling) 
		{
			LEFT_BACK_TURE(0);	
			RIGHT_BACK_TURE(0);	
			delay_ms(10);
		}
		break;
	case close_doublebling:
		LEFT_FRONT_TURE(0);
		g_slaveReg[14] = LeftBulbState;
		RIGHT_FRONT_TURE(0);
		g_slaveReg[15] = RightBulbState;
		LEFT_BACK_TURE(0);	
		RIGHT_BACK_TURE(0);
		break;
	default:
		break;
	}
}
