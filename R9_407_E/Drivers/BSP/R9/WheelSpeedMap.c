/**
 * @FilePath     : /MDK-ARMc:/Users/fu/Desktop/Code/R9_407E/R9_407_E/Drivers/BSP/R9/WheelSpeedMap.c
 * @Description  :  
 * @Author       : lisir
 * @Version      : V1.1
 * @LastEditors  : lisir lisir@rehand.com
 * @LastEditTime : 2024-07-27 10:46:59
 * @Copyright (c) 2024 by Rehand Medical Technology Co., LTD, All Rights Reserved. 
**/
#include "./BSP/R9/underpanControl.h"
// 本地
VELOCITY_POUT velocity_localpout; 
VELOCITY_PIn  velPlanIn_local; //输入实参
//远程
VELOCITY_POUT velocity_remotepout; 
VELOCITY_PIn  velPlanIn_remote; //输入实参

uint16_t brakeflage;
RunState drivestate;
RunState e_lastdrivestate;
static float pitch,roll,yaw;
// #define JOYSTIC_AI 
#define JOYSTIC_DI 
#define REMOTE_DI

/**
 * @description: 本地摇杆数据映射左右轮速方法
 * @param  set_Maximum_Strspeed ：最大直行速度  
 * @return {*}
**/
void velocity_mapingLocal(VELOCITY_PIn velPlanIn)
{
    /*本地摇杆操控轮椅状态*/
	if (velPlanIn.adcx!=0 || velPlanIn.adcy!=0)
	{
		g_slaveReg[22] = 1;
	}
	else
	{
		g_slaveReg[22] = 0;
	}
	/*转向角计算*/
    velocity_localpout.steering_angle = atan2((double)velPlanIn.adcy, (double)velPlanIn.adcx);
    if (velPlanIn.adcy < 0)
		{
		velocity_localpout.steering_angle = velocity_localpout.steering_angle +2*pi;
		}
  /*速度模长*/
    velocity_localpout.underpanVelocity = sqrt(pow(velPlanIn.adcx,2.0)+pow(velPlanIn.adcy,2.0));
	
	#if defined JOYSTIC_DI
		/*速度模长约束*/
		velocity_localpout.underpanVelocity = Value_limitf(0,velocity_localpout.underpanVelocity,MAX_YDATA);//(MAX_YDATA - YADC_DIM));
		// printf("velocity_localpout.underpanVelocity: %f\n",velocity_localpout.underpanVelocity);
		/*左右轮目标线速度 Km/h*/
		velocity_localpout.L_Velocity = velPlanIn.set_Maximum_Strspeed * velocity_localpout.underpanVelocity/MAX_YDATA  * \
		(sin(velocity_localpout.steering_angle-pi/12.0) + cos(velocity_localpout.steering_angle-pi/12.0)) / 1.0 ;

		velocity_localpout.R_Velocity = velPlanIn.set_Maximum_Strspeed * velocity_localpout.underpanVelocity/MAX_YDATA * \
		(sin(velocity_localpout.steering_angle+pi/12.0) - cos(velocity_localpout.steering_angle+pi/12.0)) / 1.0 ;
	#endif 
	/*模拟型摇杆*/
	#if defined JOYSTIC_AI
		/*速度模长约束*/
		velocity_localpout.underpanVelocity = Value_limitf(0,velocity_localpout.underpanVelocity,(yadc_max - yadc_Dim));
		velocity_localpout.underpanVelocity = Value_limitf(0,velocity_localpout.underpanVelocity,(yadc_max - yadc_Dim));
		// printf("velocity_localpout.underpanVelocity: %f\n",velocity_localpout.underpanVelocity);
		/*左右轮目标线速度 Km/h*/
		velocity_localpout.L_Velocity = velPlanIn.set_Maximum_Strspeed * velocity_localpout.underpanVelocity/ (yadc_max - yadc_Dim) * \
		(sin(velocity_localpout.steering_angle-pi/6.0) + cos(velocity_localpout.steering_angle-pi/6.0)) / 1.0 ;

		velocity_localpout.R_Velocity = velPlanIn.set_Maximum_Strspeed * velocity_localpout.underpanVelocity/ (yadc_max - yadc_Dim) * \
		(sin(velocity_localpout.steering_angle+pi/6.0) - cos(velocity_localpout.steering_angle+pi/6.0)) / 1.0 ;
	#endif	
	
	velocity_localpout.presentation_velocity = (fabs(velocity_localpout.L_Velocity) + fabs(velocity_localpout.R_Velocity))/2.0/1.3;
	velocity_localpout.presentation_velocity = Value_limitf (0,velocity_localpout.presentation_velocity ,velPlanIn.set_Maximum_Strspeed);
	g_slaveReg[3] = (uint16_t)(velocity_localpout.presentation_velocity * 100); // RK3588 接受车速信息KM/H

	/*KM/h —— RPM—— Voltage - Duty cycle*/
    
	/*左右目标轮线速度 转换为 占空比根据实际情况进行修正，常数为修正系数*/
	if  (velPlanIn.set_Maximum_Strspeed == 1.0)
	{
		velocity_localpout.L_Dutycycle = fabs(velocity_localpout.L_Velocity) * KMPH_TO_Duty*1.172;
		velocity_localpout.R_Dutycycle = fabs(velocity_localpout.R_Velocity) * KMPH_TO_Duty*1.172;
	}
	else 
	{
		velocity_localpout.L_Dutycycle = fabs(velocity_localpout.L_Velocity) * KMPH_TO_Duty;
		velocity_localpout.R_Dutycycle = fabs(velocity_localpout.R_Velocity) * KMPH_TO_Duty;;
	}

	/*算术平均滤波占空比滤波处理*/
    velocity_localpout.L_Dutycycle = filterValue_float(&filter_L,velocity_localpout.L_Dutycycle);
	velocity_localpout.R_Dutycycle = filterValue_float(&filter_R,velocity_localpout.R_Dutycycle);

	/* 占空比约束*/
	velocity_localpout.L_Dutycycle = slopelimitLDuty(velocity_localpout.L_Dutycycle,0.08,0.1);
	velocity_localpout.R_Dutycycle = slopelimitRDuty(velocity_localpout.R_Dutycycle,0.08,0.1);
	
	velocity_localpout.L_Dutycycle = Value_limitf(0.0, velocity_localpout.L_Dutycycle, 0.99);
	velocity_localpout.R_Dutycycle = Value_limitf(0.0, velocity_localpout.R_Dutycycle, 0.99);	

	/* 静止  */
	if (velPlanIn.adcx == 0 && velPlanIn.adcy  == 0)
	{
		velocity_localpout.runstate = idle;
		drivestate = idle;
		g_slaveReg[5] = 1 ;

	}
	/*前直行 */
	if (velPlanIn.adcx == 0 && velPlanIn.adcy > 0)
	{

		velocity_localpout.runstate = forward;
		drivestate = forward;
		g_slaveReg[5] = 2 ;
	}
	/*向后直行 */
	if (velPlanIn.adcx == 0 && velPlanIn.adcy < 0)
	{
		velocity_localpout.runstate = backward;
		drivestate = backward;
		g_slaveReg[5] = 3 ;
	}
	
	/*向左前转向 */
//	if (velPlanIn.adcx < 0 && velPlanIn.adcy > 0)
	if (velocity_localpout.steering_angle > pi/2 && velocity_localpout.steering_angle <(5/6.0)*pi && velPlanIn.adcx!=0 )
	{
		velocity_localpout.runstate = front_left;
		drivestate = front_left;
		g_slaveReg[5] = 6;
	}
	/*向右前转向 */
	if (velocity_localpout.steering_angle > pi*1.0/6.0 && velocity_localpout.steering_angle <1/2.0 *pi && velPlanIn.adcx!=0)
	{
		velocity_localpout.runstate = front_right;
		drivestate = front_right;
		g_slaveReg[5] = 7;
	}
	/*向左后转向 */
	if (velocity_localpout.steering_angle > 1.5*pi  && velocity_localpout.steering_angle <11/6.0 *pi && velPlanIn.adcx!=0)
	{
		velocity_localpout.runstate = back_left;
		drivestate = back_left;
		g_slaveReg[5] = 8;
	}
	/*向右后转向 */
	if (velocity_localpout.steering_angle > 7/6.0 *pi  && velocity_localpout.steering_angle <1.5 *pi && velPlanIn.adcx!=0 )
	{
		velocity_localpout.runstate = back_right;
		drivestate = back_right;
		g_slaveReg[5] = 9;
	}
	
	/*原地右转 */
if ((velocity_localpout.steering_angle >=0 &&  velPlanIn.adcx>0 && velocity_localpout.steering_angle < 1/6.0 *pi) \
	|| (velocity_localpout.steering_angle >11/6.0*pi  && velocity_localpout.steering_angle <2 *pi))	
	{
		velocity_localpout.runstate = turnself_right;
		drivestate = turnself_right;
		g_slaveReg[5] = 5;
	}

	/*原地左转 */
if (velocity_localpout.steering_angle >5/6.0*pi && velocity_localpout.steering_angle < 7/6.0 *pi)	
	{
		velocity_localpout.runstate = turnself_left;
		drivestate = turnself_left;
		g_slaveReg[5] = 4;
	}
	// printf("L:%lf,R:%lf,ADCX:%d,ADCY:%d\r\n",velocity_localpout.L_Dutycycle,velocity_localpout.R_Dutycycle,velPlanIn.adcx,velPlanIn.adcy);	
	switch (drivestate)
	{
		case idle:
			LeftMoterStop();
			RightMoterStop();
			break;
		case forward:
			LeftMoterMove(0,velocity_localpout.L_Dutycycle);
			RightMoterMove(1,velocity_localpout.R_Dutycycle);
			break;
		case front_right:
			LeftMoterMove(0,velocity_localpout.L_Dutycycle);
			RightMoterMove(1,velocity_localpout.R_Dutycycle);
			break;
		case front_left:
			LeftMoterMove(0,velocity_localpout.L_Dutycycle);
			RightMoterMove(1,velocity_localpout.R_Dutycycle);
			break;
		case backward : /*backward velocity is half of set_Maximum_Strspeed*/
			LeftMoterMove(1,velocity_localpout.L_Dutycycle);
			RightMoterMove(0,velocity_localpout.R_Dutycycle);
			break;
		case back_left:
			LeftMoterMove(1,velocity_localpout.L_Dutycycle);
			RightMoterMove(0,velocity_localpout.R_Dutycycle);
			break;
		case back_right:
			LeftMoterMove(1,velocity_localpout.L_Dutycycle);
			RightMoterMove(0,velocity_localpout.R_Dutycycle);
			break;
		case turnself_right:
		    if (g_slaveReg[73] == 1 || g_slaveReg[73] == 2)
			{
				LeftMoterMove(0,velocity_localpout.L_Dutycycle);
				RightMoterMove(0,velocity_localpout.R_Dutycycle);
			}
			else /*上位机数据通讯OK后需更改*/
			{
				LeftMoterMove(0,velocity_localpout.L_Dutycycle);
				RightMoterMove(0,velocity_localpout.R_Dutycycle);

			}
			break;
		case turnself_left:
			if (g_slaveReg[73] == 1 || g_slaveReg[73] == 2 )
			{
				LeftMoterMove(1,velocity_localpout.L_Dutycycle);
				RightMoterMove(1,velocity_localpout.R_Dutycycle);
			}
			else/*上位机数据通讯OK后需更改*/
			{
				LeftMoterMove(1,velocity_localpout.L_Dutycycle);
				RightMoterMove(1,velocity_localpout.R_Dutycycle);
			}

			break;
		default:
			break;
	}
}
/**
 * @description: 远程端摇杆数据映射左右轮速方法
 * @param  set_Maximum_Strspeed ：最大直行速度  
 * @return {*}
**/
void velocity_mapingRemote(VELOCITY_PIn velPlanIn)
{
	/*转向角计算*/
    velocity_remotepout.steering_angle = atan2((double)velPlanIn.adcy, (double)velPlanIn.adcx);
    if (velPlanIn.adcy < 0)
		{
		velocity_remotepout.steering_angle = velocity_remotepout.steering_angle +2*pi;
		}
  /*速度模长*/
    velocity_remotepout.underpanVelocity = sqrt(pow(velPlanIn.adcx,2.0)+pow(velPlanIn.adcy,2.0));
	/*速度模长约束*/
	velocity_remotepout.underpanVelocity = Value_limitf(0,velocity_remotepout.underpanVelocity,MAX_YDATA);//(MAX_YDATA - YADC_DIM));

	velocity_remotepout.L_Velocity = velPlanIn.set_Maximum_Strspeed * velocity_remotepout.underpanVelocity/MAX_YDATA  * \
	(sin(velocity_remotepout.steering_angle-pi/6.0) + cos(velocity_remotepout.steering_angle-pi/6.0)) / 1.0 ;
	
	velocity_remotepout.R_Velocity = velPlanIn.set_Maximum_Strspeed * velocity_remotepout.underpanVelocity/MAX_YDATA * \
	(sin(velocity_remotepout.steering_angle+pi/6.0) - cos(velocity_remotepout.steering_angle+pi/6.0)) / 1.0 ;
	/*KM/h —— RPM—— Voltage - Duty cycle*/
	/*左右目标轮线速度 转换为 占空比根据实际情况进行修正，常数为修正系数*/
	if  (velPlanIn.set_Maximum_Strspeed == 1.0)
	{
		velocity_remotepout.L_Dutycycle = fabs(velocity_remotepout.L_Velocity) * KMPH_TO_Duty*1.172;
		velocity_remotepout.R_Dutycycle = fabs(velocity_remotepout.R_Velocity) * KMPH_TO_Duty*1.172;
	}
	else 
	{
		velocity_remotepout.L_Dutycycle = fabs(velocity_remotepout.L_Velocity) * KMPH_TO_Duty;
		velocity_remotepout.R_Dutycycle = fabs(velocity_remotepout.R_Velocity) * KMPH_TO_Duty;;
	}
	/*算术平均滤波占空比滤波处理*/
    // velocity_remotepout.L_Dutycycle = filterValue(&filter_L,velocity_remotepout.L_Dutycycle);
	// velocity_remotepout.R_Dutycycle = filterValue(&filter_R,velocity_remotepout.R_Dutycycle);

	/* 占空比约束*/
	velocity_remotepout.L_Dutycycle = slopelimitLDuty(velocity_remotepout.L_Dutycycle,0.08,0.1);
	velocity_remotepout.R_Dutycycle = slopelimitRDuty(velocity_remotepout.R_Dutycycle,0.08,0.1);
	
	velocity_remotepout.L_Dutycycle = Value_limitf(0.0, velocity_remotepout.L_Dutycycle, 0.99);
	velocity_remotepout.R_Dutycycle = Value_limitf(0.0, velocity_remotepout.R_Dutycycle, 0.99);	

	/* 静止  */
	if (velPlanIn.adcx == 0 && velPlanIn.adcy  == 0)
	{
		velocity_remotepout.runstate = idle;
	}
	/*前直行 */
	if (velPlanIn.adcx == 0 && velPlanIn.adcy > 0)
	{
		velocity_remotepout.runstate = forward;
	}
	/*向后直行 */
	if (velPlanIn.adcx == 0 && velPlanIn.adcy < 0)
	{
		velocity_remotepout.runstate = backward;
	}
	
	/*向左前转向 */
	if (velocity_remotepout.steering_angle > pi/2 && velocity_remotepout.steering_angle <(11/12.0)*pi && velPlanIn.adcx!=0 )
	{
		velocity_remotepout.runstate = front_left;
	}
	/*向右前转向 */
	if (velocity_remotepout.steering_angle > pi*1.0/12.0 && velocity_remotepout.steering_angle <1/2.0 *pi && velPlanIn.adcx!=0)
	{
		velocity_remotepout.runstate = 	front_right;	
	}
	/*向左后转向 */
	if (velocity_remotepout.steering_angle > 1.5*pi  && velocity_remotepout.steering_angle <23/12.0 *pi && velPlanIn.adcx!=0)
	{
		velocity_remotepout.runstate = back_left;
	}
	/*向右后转向 */
if (velocity_remotepout.steering_angle > 11/12.0 *pi  && velocity_remotepout.steering_angle <1.5 *pi && velPlanIn.adcx!=0 )
	{
		velocity_remotepout.runstate = back_right;
	}
	/*原地右转 */
if ((velocity_remotepout.steering_angle >=0 &&  velPlanIn.adcx>0 && velocity_remotepout.steering_angle < 1/12.0 *pi) \
	|| (velocity_remotepout.steering_angle >23/12.0*pi  && velocity_remotepout.steering_angle <2 *pi))	
	{
		velocity_remotepout.runstate = turnself_right;
	}
	/*原地左转 */
if (velocity_remotepout.steering_angle >11/12.0*pi && velocity_remotepout.steering_angle < 13/12.0 *pi)	
	{
		velocity_remotepout.runstate = turnself_left;
	}
	
	switch (velocity_remotepout.runstate)
	{
		case idle:
			LeftMoterStop();
			RightMoterStop();
			break;
		case forward:
			LeftMoterMove(0,velocity_remotepout.L_Dutycycle);
			RightMoterMove(1,velocity_remotepout.R_Dutycycle);
			break;
		case front_right:
			LeftMoterMove(0,velocity_remotepout.L_Dutycycle);
			RightMoterMove(1,velocity_remotepout.R_Dutycycle);
			break;
		case front_left:
			LeftMoterMove(0,velocity_remotepout.L_Dutycycle);
			RightMoterMove(1,velocity_remotepout.R_Dutycycle);
			break;
		case backward : /*backward velocity is half of set_Maximum_Strspeed*/
			LeftMoterMove(1,velocity_remotepout.L_Dutycycle);
			RightMoterMove(0,velocity_remotepout.R_Dutycycle);
			break;
		case back_left:
			LeftMoterMove(1,velocity_remotepout.L_Dutycycle);
			RightMoterMove(0,velocity_remotepout.R_Dutycycle);
			break;
		case back_right:
			LeftMoterMove(1,velocity_remotepout.L_Dutycycle);
			RightMoterMove(0,velocity_remotepout.R_Dutycycle);
			break;
		case turnself_right:
				LeftMoterMove(0,velocity_remotepout.L_Dutycycle*1.5);
				RightMoterMove(0,velocity_remotepout.R_Dutycycle*1.5);
			break;
		case turnself_left:
				LeftMoterMove(1,velocity_remotepout.L_Dutycycle*1.5);
				RightMoterMove(1,velocity_remotepout.R_Dutycycle*1.5);
			break;
		default:
			break;
	}
}


/**
 * @brief        : Brake Control Excute
 * @param        ： 摇杆归位后延时100 ms 抱住抱闸器
 * @return        {*} 
**/
void brake_excute(void)
{
/*松开抱闸*/
	if (velPlanIn_local.adcx < -200 || velPlanIn_local.adcx > 200 || velPlanIn_local.adcy > 200 || velPlanIn_local.adcy < -200||\
		velPlanIn_remote.adcx < -100 || velPlanIn_remote.adcx > 100 || velPlanIn_remote.adcy > 100 || velPlanIn_remote.adcy < -100)
	{
			brake(0);	
			brakeflage = 0;	
	}	
	else
/*锁住抱闸*/
	// {
	//   if(struc_brake.detect_falge)
	//   {
	{	brakeflage++;
		if (brakeflage > 100)
		{ 
			brake(1);
			brakeflage = 0;
		}
	}	
	// 	struc_brake.detect_falge = 0;

}

/**
 * @brief        : Velocity Level Set
 * @return        {*}None
**/
void VelocityLevelSet(void)
{ 
  /*上下位机通讯失败，默认Indoor模式 2 挡位*/
  if (comheartstate.com_state == Fail)
  {
		g_slaveReg[64] = 2; 
		g_slaveReg[73] = 2;
  }

  switch (g_slaveReg[64])
	{
		case 1: /*normal mode*/
			if (g_slaveReg[73] == 1)/*one level*/
			{
				velPlanIn_local.set_Maximum_Strspeed = 1.0 ;
			}
			else if (g_slaveReg[73] == 2)/*two level*/
	   	    {
				velPlanIn_local.set_Maximum_Strspeed = 4.0 ;
			}
			else if (g_slaveReg[73] == 3)/*three level*/
			{
				velPlanIn_local.set_Maximum_Strspeed = 6.6 ;

			}
			else if (g_slaveReg[73] == 4)/*four level*/
			{
				velPlanIn_local.set_Maximum_Strspeed = 9.3 ;

			}
			else if (g_slaveReg[73] == 5)/*five level*/
			{
				velPlanIn_local.set_Maximum_Strspeed = 12.0 ;

			}
			else
			{
				velPlanIn_local.set_Maximum_Strspeed = 1.0 ;

			}
			break;
		case 2 :/*indoor mode*/
			if (g_slaveReg[73] == 1)/*one level*/
			{
				velPlanIn_local.set_Maximum_Strspeed = 1.0 ;

			}
			else if (g_slaveReg[73] == 2)/*two level*/
	   	    {
				velPlanIn_local.set_Maximum_Strspeed = 2.0 ;

			}
			else if (g_slaveReg[73] == 3)/*three level*/
			{
				velPlanIn_local.set_Maximum_Strspeed = 3.0 ;

			}
			else if (g_slaveReg[73] == 4)/*four level*/
			{
				velPlanIn_local.set_Maximum_Strspeed = 4.0 ;

			}
			else if (g_slaveReg[73] == 5)/*five level*/
			{
				velPlanIn_local.set_Maximum_Strspeed = 5.0 ;

			}
			else
			{
				velPlanIn_local.set_Maximum_Strspeed = 1.0 ;

			}
			break;			
		case 3 :/*outdoor mode*/
			if (g_slaveReg[73] == 1)/*one level*/
			{
				velPlanIn_local.set_Maximum_Strspeed = 1.0 ;

			}
			else if (g_slaveReg[73] == 2)/*two level*/
	   	    {
				velPlanIn_local.set_Maximum_Strspeed = 4.0 ;

			}
			else if (g_slaveReg[73] == 3)/*three level*/
			{
				velPlanIn_local.set_Maximum_Strspeed = 7.0 ;

			}
			else if (g_slaveReg[73] == 4)/*four level*/
			{
				velPlanIn_local.set_Maximum_Strspeed = 10.0 ;

			}
			else if (g_slaveReg[73] == 5)/*five level*/
			{
				velPlanIn_local.set_Maximum_Strspeed = 12.0 ;

			}
			else
			{
				velPlanIn_local.set_Maximum_Strspeed = 1.0 ;
			}
			break;
		default:
			break;
	}

}

/**
 * @description: 底盘驱动程序代码 主要用到定时器 3 和 定时器 9 控制左右电机占空比
 * @return 无
 */
void underpanExcute(void)
{
	static uint8_t LocalOpflage=0,RemoteOpfalge=0;
	#if defined JOYSTIC_AI
		/* X 数据清偏 */
		if (adcdata.adc_x > 0)
		{
			velPlanIn_local.adcx = adcdata.adc_x - xadc_Dim;
		}
		else if (adcdata.adc_x < 0)
		{
			velPlanIn_local.adcx = adcdata.adc_x + xadc_Dim;
		}
		else
		{
			velPlanIn_local.adcx = 0;
		}
		
		velPlanIn_local.adcx = slopelimitx( velPlanIn_local.adcx,25);  
		
		/* Y 数据清偏移*/
		if (adcdata.adc_y > 0)	
		{
			velPlanIn_local.adcy = adcdata.adc_y - yadc_Dim;
		}
		 else if (adcdata.adc_y < 0)
		{
			velPlanIn_local.adcy = adcdata.adc_y+ yadc_Dim;
		}	
		else
		{
			velPlanIn_local.adcy = 0 ;
		}
		velPlanIn_local.adcy = slopelimity( velPlanIn_local.adcy,25); 	
	#endif
	#if defined JOYSTIC_DI	
			/*上位机显示 摇杆数据*/
		g_slaveReg[10] =  mlxdata.xdata;
    	g_slaveReg[11] =  mlxdata.ydata;
		// printf("mlxdataX:%d,mlxdataY:%d\n",g_slaveReg[10],g_slaveReg[11]);
		velPlanIn_local.adcx = local_slopelimitx(mlxdata.xdata,35,25);  
		velPlanIn_local.adcy = local_slopelimity(mlxdata.ydata,35,25);  	  
	#endif
	#ifdef REMOTE_DI
		velPlanIn_remote.adcx = remote_slopelimitx(g_slaveReg[79],35,25);  
		velPlanIn_remote.adcy = remote_slopelimity(g_slaveReg[80],35,25); 
		if(g_slaveReg[78] )
		{
			velPlanIn_remote.set_Maximum_Strspeed = 2.0 ; // 默认二档速度
		}
		else
		{
			velPlanIn_remote.set_Maximum_Strspeed = 0.0 ; 
		}

		
	#endif
/*本地 或 远程操作 标识判断*/
	if (velPlanIn_local.adcx || velPlanIn_local.adcy)
	{
		LocalOpflage =1;
		RemoteOpfalge =0;
		// 本地打断远程操作的同时需要将远程摇杆数据清除
		g_slaveReg[79] = 0;
		g_slaveReg[80]  =0;

	}	
	else
	{
		LocalOpflage =0;
	}
/*远程可操控条件同时满足:(1)本地未操作 (2)蓝牙处于配对状态 (3)触发了移动端摇杆 (4)摇杆未归位 (5)上下位机通讯OK*/
	if (LocalOpflage ==0 &&g_slaveReg[78] && (g_slaveReg[79] || g_slaveReg[80] ) && g_slaveReg[81]==0 && comheartstate.com_state)
	{
		RemoteOpfalge =1;
	}
	else
	{
		RemoteOpfalge =0;
	}
/*上下位机通讯失败需要复位远程端数据*/
    if(comheartstate.com_state == Fail)
	{
			g_slaveReg[79] = 0;
			g_slaveReg[80] = 0;
	}
	VelocityLevelSet();
	brake_excute();
	if(LocalOpflage)
	{
		velocity_mapingLocal(velPlanIn_local); /*本地速度规划 */
	}
	if (RemoteOpfalge)
	{
		velocity_mapingRemote(velPlanIn_remote); /*远程速度规划 */
	}
	
}

/**
 * @description: 获取陀螺仪 的pry欧拉角
 * @return 空 rpy欧拉角数值放置静态变量 pitch roll yaw 中
 */
void MPU6050Excute(void)
{
	mpu_dmp_get_data(&pitch,&roll,&yaw);       
	g_slaveReg[24] = (int16_t)(pitch*100);
	g_slaveReg[25] = (int16_t)(roll*100);
	g_slaveReg[26] = (int16_t)(yaw*100); 
}

