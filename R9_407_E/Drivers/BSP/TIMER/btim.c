/**
 ****************************************************************************************************
 * @file        btim.c
 * @author    lis
 * @version     V1.0
 * @date        2024
 * @brief       基本定时器 驱动代码
 * @license     复成医疗
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:rehand 探索者 F407开发板
 * 复成医疗
 * rehand
 *复成医疗
 * 复成医疗
 *
 * 修改说明
 * V1.0 20211015
 * 第一次发布
 *
 ****************************************************************************************************
 */


#include "./BSP/TIMER/btim.h"
#include "./BSP/R9/brake.h"
// STRUCT_BRAKE struc_brake;
TIM_HandleTypeDef g_timx_handler;         /* 定时器参数句柄 */

/**
 * @brief       基本定时器TIMX定时中断初始化函数
 * @note
 *              基本定时器的时钟来自APB1,当PPRE1 ≥ 2分频的时候
 *              基本定时器的时钟为APB1时钟的2倍, 而APB1为42M, 所以定时器时钟 = 84Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=定时器工作频率,单位:Mhz
 *
 * @param       arr : 自动重装值。
 * @param       psc : 时钟预分频数
 * @retval      无
 */
void btim_timx_int_init(uint16_t arr, uint16_t psc)
{
    g_timx_handler.Instance = BTIM_TIMX_INT;                      /* 定时器x */
    g_timx_handler.Init.Prescaler = psc;                          /* 分频 */
    g_timx_handler.Init.CounterMode = TIM_COUNTERMODE_UP;         /* 递增计数模式 */
    g_timx_handler.Init.Period = arr;                             /* 自动装载值 */
    HAL_TIM_Base_Init(&g_timx_handler);
    
    HAL_TIM_Base_Start_IT(&g_timx_handler);                       /* 使能定时器x和定时器更新中断 */
}

  


/**
 * @brief       定时器底层驱动，开启时钟，设置中断优先级
                此函数会被HAL_TIM_Base_Init()函数调用
 * @param       无
 * @retval      无
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == BTIM_TIMX_INT)
    {
        BTIM_TIMX_INT_CLK_ENABLE();                     /* 使能TIMx时钟 */
        HAL_NVIC_SetPriority(BTIM_TIMX_INT_IRQn, 1, 1); /* 抢占1，子优先级3 */
        HAL_NVIC_EnableIRQ(BTIM_TIMX_INT_IRQn);         /* 开启ITMx中断 */
    }
}

/**
 * @brief       基本定时器TIMX中断服务函数
 * @param       无
 * @retval      无
 */
void BTIM_TIMX_INT_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_timx_handler);  /* 定时器回调函数 */
}

/**
 * @brief       回调函数，定时器中断服务函数调用
 * @param       无
 * @retval      无
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == BTIM_TIMX_INT)
    {
		OS_IT_RUN();			
//		Modbus 从机被RK3588读写
		if(slavemodbus.timrun != 0)//运行时间！=0表明
			{
			slavemodbus.timout++;
			if(slavemodbus.timout >=8)
			{
				slavemodbus.timrun = 0;
				slavemodbus.reflag = 1;//接收数据完毕
			}
				
			}

		if(modbus_dap21.timrun != 0)//运行时间！=0表明
			{
			modbus_dap21.timout++;
			if(modbus_dap21.timout >=8)
				{
					modbus_dap21.timrun = 0;
					modbus_dap21.reflag = 1;//接收数据完毕
				}
			
			}
		
		comheartstate.detect_time++; // 检测完上一次通讯状态后计数
		if (comheartstate.detect_time > 500) // 距离上一次检测过去了500ms
		{
			comheartstate.detect_falge = 1;
			comheartstate.detect_time = 0; 
		}

		struc_brake.detect_time++;// 检测完上一次通讯状态后计数
		if (struc_brake.detect_time >100)
		{
			struc_brake.detect_falge = 1;
			struc_brake.detect_time = 0 ;
		}
    }

		
		
}
