/**
 * @FilePath     : /MDK-ARMc:/Users/fu/Desktop/Code/R9_407E/R9_407_E/Drivers/BSP/R9/WheelSpeedMap.c
 * @Description  :  
 * @Author       : lisir
 * @Version      : V1.1
 * @LastEditors  : lisir lisir@rehand.com
 * @LastEditTime : 2024-07-19 09:36:29
 * @Copyright (c) 2024 by Rehand Medical Technology Co., LTD, All Rights Reserved. 
**/
#include "./BSP/R9/underpanControl.h"

VELOCITY_POUT velocity_pout; 
VELOCITY_PIn  velPlanIn1;
uint16_t brakeflage;
RunState drivestate;
RunState e_lastdrivestate;
static float pitch,roll,yaw;
// #define JOYSTIC_AI 
#define JOYSTIC_DI 

/**
 * @description: 摇杆数据映射左右轮速方法
 * @param  set_Maximum_Strspeed ：最大直行速度  
 * @param  set_Maximum_Steespeed ：最大转向速度  
 * @return {*}
 */


/**
 * @brief        : 
 * @param         {VELOCITY_PIn} velPlanIn:steering_angle: 0~2pi
 * @return        {*} 
**/
void velocity_maping(VELOCITY_PIn velPlanIn)
{
	/*上位机显示 摇杆数据*/
	  g_slaveReg[10] =  velPlanIn.adcx;
    g_slaveReg[11] =  velPlanIn.adcy;
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
    velocity_pout.steering_angle = atan2((double)velPlanIn.adcy, (double)velPlanIn.adcx);
    if (velPlanIn.adcy < 0)
		{
		velocity_pout.steering_angle = velocity_pout.steering_angle +2*pi;
		}
  /*速度模长*/
    velocity_pout.underpanVelocity = sqrt(pow(velPlanIn.adcx,2.0)+pow(velPlanIn.adcy,2.0));
	
	#if defined JOYSTIC_DI
		/*速度模长约束*/
		velocity_pout.underpanVelocity = Value_limitf(0,velocity_pout.underpanVelocity,MAX_YDATA);//(MAX_YDATA - YADC_DIM));
		// printf("velocity_pout.underpanVelocity: %f\n",velocity_pout.underpanVelocity);
		/*左右轮目标线速度 Km/h*/
		velocity_pout.L_Velocity = velPlanIn.set_Maximum_Strspeed * velocity_pout.underpanVelocity/MAX_YDATA  * \
		(sin(velocity_pout.steering_angle-pi/6.0) + cos(velocity_pout.steering_angle-pi/6.0)) / 1.365 ;

		velocity_pout.R_Velocity = velPlanIn.set_Maximum_Strspeed * velocity_pout.underpanVelocity/MAX_YDATA * \
		(sin(velocity_pout.steering_angle+pi/6.0) - cos(velocity_pout.steering_angle+pi/6.0)) / 1.365 ;
	#endif 
	/*模拟型摇杆*/
	#if defined JOYSTIC_AI
		/*速度模长约束*/
		velocity_pout.underpanVelocity = Value_limitf(0,velocity_pout.underpanVelocity,(yadc_max - yadc_Dim));
		velocity_pout.underpanVelocity = Value_limitf(0,velocity_pout.underpanVelocity,(yadc_max - yadc_Dim));
		// printf("velocity_pout.underpanVelocity: %f\n",velocity_pout.underpanVelocity);
		/*左右轮目标线速度 Km/h*/
		velocity_pout.L_Velocity = velPlanIn.set_Maximum_Strspeed * velocity_pout.underpanVelocity/ (yadc_max - yadc_Dim) * \
		(sin(velocity_pout.steering_angle-pi/6.0) + cos(velocity_pout.steering_angle-pi/6.0)) / 1.0 ;

		velocity_pout.R_Velocity = velPlanIn.set_Maximum_Strspeed * velocity_pout.underpanVelocity/ (yadc_max - yadc_Dim) * \
		(sin(velocity_pout.steering_angle+pi/6.0) - cos(velocity_pout.steering_angle+pi/6.0)) / 1.0 ;
	#endif	
	
	velocity_pout.presentation_velocity = (fabs(velocity_pout.L_Velocity) + fabs(velocity_pout.R_Velocity))/2.0;
	velocity_pout.presentation_velocity = Value_limitf (0,velocity_pout.presentation_velocity ,velPlanIn.set_Maximum_Strspeed);
	g_slaveReg[3] = (uint16_t)(velocity_pout.presentation_velocity * 100); // RK3588 接受车速信息KM/H

	/*KM/h —— RPM—— Voltage - Duty cycle*/
    
	/*左右目标轮线速度 转换为 占空比根据实际情况进行修正，常数为修正系数*/
	if  (velPlanIn.set_Maximum_Strspeed == 1.0)
	{
		velocity_pout.L_Dutycycle = fabs(velocity_pout.L_Velocity) * KMPH_TO_Duty*1.6;
		velocity_pout.R_Dutycycle = fabs(velocity_pout.R_Velocity) * KMPH_TO_Duty*1.6;
	}
	else if (velPlanIn.set_Maximum_Strspeed == 2.0)
	{
		velocity_pout.L_Dutycycle = fabs(velocity_pout.L_Velocity) * KMPH_TO_Duty*1.2;
		velocity_pout.R_Dutycycle = fabs(velocity_pout.R_Velocity) * KMPH_TO_Duty*1.2;
	}
	else if (velPlanIn.set_Maximum_Strspeed == 3.0)
	{
		velocity_pout.L_Dutycycle = fabs(velocity_pout.L_Velocity) * KMPH_TO_Duty*1.125;
		velocity_pout.R_Dutycycle = fabs(velocity_pout.R_Velocity) * KMPH_TO_Duty*1.125;
	}
	else if (velPlanIn.set_Maximum_Strspeed == 4.0)
	{

		velocity_pout.L_Dutycycle = fabs(velocity_pout.L_Velocity) * KMPH_TO_Duty*1.09;
		velocity_pout.R_Dutycycle = fabs(velocity_pout.R_Velocity) * KMPH_TO_Duty*1.09;		
	}
	else if (velPlanIn.set_Maximum_Strspeed == 5.0)
	{
		velocity_pout.L_Dutycycle = fabs(velocity_pout.L_Velocity) * KMPH_TO_Duty*1.07;
		velocity_pout.R_Dutycycle = fabs(velocity_pout.R_Velocity) * KMPH_TO_Duty*1.07;		
		
	}
	else 
	{
		velocity_pout.L_Dutycycle = fabs(velocity_pout.L_Velocity) * KMPH_TO_Duty;
		velocity_pout.R_Dutycycle = fabs(velocity_pout.R_Velocity) * KMPH_TO_Duty;;
	}

	/*算术平均滤波占空比滤波处理*/
    velocity_pout.L_Dutycycle = filterValue(&filter_L,velocity_pout.L_Dutycycle);
	velocity_pout.R_Dutycycle = filterValue(&filter_R,velocity_pout.R_Dutycycle);

	/* 占空比约束*/
	velocity_pout.L_Dutycycle = slopelimitLDuty(velocity_pout.L_Dutycycle,0.08,0.1);
	velocity_pout.R_Dutycycle = slopelimitRDuty(velocity_pout.R_Dutycycle,0.08,0.1);
	
	velocity_pout.L_Dutycycle = Value_limitf(0.0, velocity_pout.L_Dutycycle, 0.99);
	velocity_pout.R_Dutycycle = Value_limitf(0.0, velocity_pout.R_Dutycycle, 0.99);	

	/* 静止  */
	if (velPlanIn.adcx == 0 && velPlanIn.adcy  == 0)
	{
		velocity_pout.runstate = idle;
		drivestate = idle;
		g_slaveReg[5] = 1 ;

	}
	/*前直行 */
	if (velPlanIn.adcx == 0 && velPlanIn.adcy > 0)
	{

		velocity_pout.runstate = forward;
		drivestate = forward;
		g_slaveReg[5] = 2 ;
	}
	/*向后直行 */
	if (velPlanIn.adcx == 0 && velPlanIn.adcy < 0)
	{
		velocity_pout.runstate = backward;
		drivestate = backward;
		g_slaveReg[5] = 3 ;
	}
	
	/*向左前转向 */
//	if (velPlanIn.adcx < 0 && velPlanIn.adcy > 0)
	if (velocity_pout.steering_angle > pi/2 && velocity_pout.steering_angle <(11/12.0)*pi && velPlanIn.adcx!=0 )
	{
		velocity_pout.runstate = front_left;
		drivestate = front_left;
		g_slaveReg[5] = 6;
	}
	/*向右前转向 */
	if (velocity_pout.steering_angle > pi*1.0/12.0 && velocity_pout.steering_angle <1/2.0 *pi && velPlanIn.adcx!=0)
	{
		velocity_pout.runstate = front_right;
		drivestate = front_right;
		g_slaveReg[5] = 7;
	}
	/*向左后转向 */
	if (velocity_pout.steering_angle > 1.5*pi  && velocity_pout.steering_angle <23/12.0 *pi && velPlanIn.adcx!=0)
	{
		velocity_pout.runstate = back_left;
		drivestate = back_left;
		g_slaveReg[5] = 8;
	}
	/*向右后转向 */
if (velocity_pout.steering_angle > 11/12.0 *pi  && velocity_pout.steering_angle <1.5 *pi && velPlanIn.adcx!=0 )
	{
		velocity_pout.runstate = back_right;
		drivestate = back_right;
		g_slaveReg[5] = 9;
	}
	
	/*原地右转 */
if ((velocity_pout.steering_angle >=0 &&  velPlanIn.adcx>0 && velocity_pout.steering_angle < 1/12.0 *pi) \
	|| (velocity_pout.steering_angle >23/12.0*pi  && velocity_pout.steering_angle <2 *pi))	
	{
		velocity_pout.runstate = turnself_right;
		drivestate = turnself_right;
		g_slaveReg[5] = 5;
	}

	/*原地左转 */
if (velocity_pout.steering_angle >11/12.0*pi && velocity_pout.steering_angle < 13/12.0 *pi)	
	{
		velocity_pout.runstate = turnself_left;
		drivestate = turnself_left;
		g_slaveReg[5] = 4;
	}
	// printf("L:%lf,R:%lf,ADCX:%d,ADCY:%d\r\n",velocity_pout.L_Dutycycle,velocity_pout.R_Dutycycle,velPlanIn.adcx,velPlanIn.adcy);	
	switch (drivestate)
	{
		case idle:
			LeftMoterStop();
			RightMoterStop();
			break;
		case forward:
			LeftMoterMove(0,velocity_pout.L_Dutycycle);
			RightMoterMove(1,velocity_pout.R_Dutycycle);
			break;
		case front_right:
			LeftMoterMove(0,velocity_pout.L_Dutycycle);
			RightMoterMove(1,velocity_pout.R_Dutycycle);
			break;
		case front_left:
			LeftMoterMove(0,velocity_pout.L_Dutycycle);
			RightMoterMove(1,velocity_pout.R_Dutycycle);
			break;
		case backward : /*backward velocity is half of set_Maximum_Strspeed*/
			LeftMoterMove(1,velocity_pout.L_Dutycycle);
			RightMoterMove(0,velocity_pout.R_Dutycycle);
			break;
		case back_left:
			LeftMoterMove(1,velocity_pout.L_Dutycycle);
			RightMoterMove(0,velocity_pout.R_Dutycycle);
			break;
		case back_right:
			LeftMoterMove(1,velocity_pout.L_Dutycycle);
			RightMoterMove(0,velocity_pout.R_Dutycycle);
			break;
		case turnself_right:
		    if (g_slaveReg[73] == 1 || g_slaveReg[73] == 2)
			{
				LeftMoterMove(0,velocity_pout.L_Dutycycle*2.0);
				RightMoterMove(0,velocity_pout.R_Dutycycle*2.0);
			}
			else
			{
				LeftMoterMove(0,velocity_pout.L_Dutycycle);
				RightMoterMove(0,velocity_pout.R_Dutycycle);

			}

			break;
		case turnself_left:
			if (g_slaveReg[73] == 1 || g_slaveReg[73] == 2 )
			{
				LeftMoterMove(1,velocity_pout.L_Dutycycle*2.0);
				RightMoterMove(1,velocity_pout.R_Dutycycle*2.0);
			}
			else
			{
				LeftMoterMove(1,velocity_pout.L_Dutycycle);
				RightMoterMove(1,velocity_pout.R_Dutycycle);
			}

			break;
		default:
			break;
	}

	if (velocity_pout.L_Dutycycle <= 0 && velocity_pout.R_Dutycycle <= 0)
	{
		brakeflage++;
		if (brakeflage > 100)
		{ // 抱住
			brake(1);
			brakeflage = 0;
		}
	}
	else
	{
		brakeflage = 0;
	}

	if (velPlanIn.adcx < -50 || velPlanIn.adcx > 50 || velPlanIn.adcy > 50 || velPlanIn.adcy < -50)

	{
			brake(0);
	}
	// printf("adcx:%d,adcy:%d,Lduty:%f,Rduty:%f\r\n",velPlanIn.adcx,velPlanIn.adcy,velocity_pout.L_Dutycycle,velocity_pout.R_Dutycycle);
}
  
 

/**
 * @description: 底盘驱动程序代码 主要用到定时器 3 和 定时器 9 控制左右电机占空比
 * @return 无
 */
void underpanExcute(void)
{
	// printf("Modle:%d,Level:%d\n",g_slaveReg[64],g_slaveReg[73]);
  switch (g_slaveReg[64])
	{
		case 1: /*normal mode*/
			if (g_slaveReg[73] == 1)/*one level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 1.0 ;
			}
			else if (g_slaveReg[73] == 2)/*two level*/
	   	    {
				velPlanIn1.set_Maximum_Strspeed = 4.0 ;
			}
			else if (g_slaveReg[73] == 3)/*three level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 6.6 ;

			}
			else if (g_slaveReg[73] == 4)/*four level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 9.3 ;

			}
			else if (g_slaveReg[73] == 5)/*five level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 12.0 ;

			}
			else
			{
				velPlanIn1.set_Maximum_Strspeed = 1.0 ;

			}
			break;
		case 2 :/*indoor mode*/
			if (g_slaveReg[73] == 1)/*one level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 1.0 ;

			}
			else if (g_slaveReg[73] == 2)/*two level*/
	   	    {
				velPlanIn1.set_Maximum_Strspeed = 2.0 ;

			}
			else if (g_slaveReg[73] == 3)/*three level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 3.0 ;

			}
			else if (g_slaveReg[73] == 4)/*four level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 4.0 ;

			}
			else if (g_slaveReg[73] == 5)/*five level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 5.0 ;

			}
			else
			{
				velPlanIn1.set_Maximum_Strspeed = 1.0 ;

			}
			break;			
		case 3 :/*outdoor mode*/
			if (g_slaveReg[73] == 1)/*one level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 1.0 ;

			}
			else if (g_slaveReg[73] == 2)/*two level*/
	   	    {
				velPlanIn1.set_Maximum_Strspeed = 4.0 ;

			}
			else if (g_slaveReg[73] == 3)/*three level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 7.0 ;

			}
			else if (g_slaveReg[73] == 4)/*four level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 10.0 ;

			}
			else if (g_slaveReg[73] == 5)/*five level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 12.0 ;

			}
			else
			{
				velPlanIn1.set_Maximum_Strspeed = 1.0 ;

			}
			break;
		default:
			velPlanIn1.set_Maximum_Strspeed = 3.0 ;
			break;
	}

	#if defined JOYSTIC_AI
		/* X 数据清偏 */
		if (adcdata.adc_x > 0)
		{
			velPlanIn1.adcx = adcdata.adc_x - xadc_Dim;
		}
		else if (adcdata.adc_x < 0)
		{
			velPlanIn1.adcx = adcdata.adc_x + xadc_Dim;
		}
		else
		{
			velPlanIn1.adcx = 0;
		}
		
		velPlanIn1.adcx = slopelimitx( velPlanIn1.adcx,25);  
		
		/* Y 数据清偏移*/
		if (adcdata.adc_y > 0)	
		{
			velPlanIn1.adcy = adcdata.adc_y - yadc_Dim;
		}
		 else if (adcdata.adc_y < 0)
		{
			velPlanIn1.adcy = adcdata.adc_y+ yadc_Dim;
		}	
		else
		{
			velPlanIn1.adcy = 0 ;
		}
		velPlanIn1.adcy = slopelimity( velPlanIn1.adcy,25); 	
	#endif


	#if defined JOYSTIC_DI
		
		/* X 数据清偏 */
		// if (mlxdata.xdata > 0)
		// {
		// 	velPlanIn1.adcx = mlxdata.xdata - XADC_DIM;
		// }
		// else if (mlxdata.xdata < 0)
		// {
		// 	velPlanIn1.adcx = mlxdata.xdata + XADC_DIM;
		// }
		// else
		// {
		// 	velPlanIn1.adcx = 0;
		// }
		
		velPlanIn1.adcx = slopelimitx(mlxdata.xdata,25,25);  
		
		/* Y 数据清偏移*/
		// if (mlxdata.ydata > 0)	
		// {
		// 	velPlanIn1.adcy = mlxdata.ydata - YADC_DIM;
		// }
		//  else if (mlxdata.ydata < 0)
		// {
		// 	velPlanIn1.adcy = mlxdata.ydata + YADC_DIM;
		// }	
		// else
		// {
		// 	velPlanIn1.adcy = 0 ;
		// }
		velPlanIn1.adcy = slopelimity(mlxdata.ydata,25,25);  	  
	#endif
	velocity_maping(velPlanIn1); /*速度规划 */
	// printf("adcx:%d,adcy:%d\n",velPlanIn1.adcx,velPlanIn1.adcy);
}
/**
 * @description: 获取陀螺仪 的pry欧拉角
 * @return 空 rpy欧拉角数值放置静态变量 pitch roll yaw 中
 */
void MPU6050Excute(void)
{

	//	uint8_t res;
    //	Temperature =  MPU_Get_Temperature();
	//	MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);
	//	MPU_Get_Accelerometer(&aacx,&aacy,&aacz);
	//    printf("%d,%d,%d,%d,%d,%d,%d\n",Temperature,gyrox,gyroy,gyroz,aacx,aacy,aacz);
	//	res=MPU_Read_Byte(MPU_DEVICE_ID_REG);
	//	printf("%d\n",res);
	mpu_dmp_get_data(&pitch,&roll,&yaw);       
    // printf("connected:%d\t\n",g_slaveReg[86]); 
	g_slaveReg[24] = (int16_t)(pitch*100);
	g_slaveReg[25] = (int16_t)(roll*100);
	g_slaveReg[26] = (int16_t)(yaw*100);
//	printf("pitch:%d,roll:%d,yaw:%d\t\n",g_slaveReg[24],g_slaveReg[25],g_slaveReg[26]); 

}

/**
 * @description: 电机曲线加减速操作
 * @param {CurveObjectType} *curve
 * @return {*}
 */

void (*pCalCurve[])(CurveObjectType *curve)={CalCurveTRAP,CalCurveSPTA};

void MotorVelocityCurve(CurveObjectType *curve)
{
	 float temp=0;
	 
	 if(curve->targetSpeed>curve->speedMax)
	{
	   curve->targetSpeed=curve->speedMax;
	}
	 
	 if(curve->targetSpeed<curve->speedMin)
	{
	   curve->targetSpeed=curve->speedMin;
	}

	 if((fabs(curve->currentSpeed-curve->startSpeed)<=curve->stepSpeed)&&(curve->maxTimes==0))
	{
	   if(curve->startSpeed<curve->speedMin)
	  {
		 curve->startSpeed=curve->speedMin;
	  }
	   
	   temp=fabs(curve->targetSpeed-curve->startSpeed);
	   temp=temp/curve->stepSpeed;
	   curve->maxTimes=(uint32_t)(temp)+1;
	   curve->aTimes=0;
	}
	 
	 if(curve->aTimes<curve->maxTimes)
	{
	   pCalCurve[curve->curveMode](curve);
	   curve->aTimes++;
	}
	 else
	{
	   curve->currentSpeed=curve->targetSpeed;
	   curve->maxTimes=0;
	   curve->aTimes=0;
	}
}


/**
 * @description: 电机曲线加减速操作
 * @param {CurveObjectType} *curve
 * @return {*}
 */
static void CalCurveTRAP(CurveObjectType *trap)
{
	 float slope=0.0;
	 
	 slope=(trap->targetSpeed-trap->startSpeed)/trap->maxTimes;
	   
	 trap->currentSpeed=trap->startSpeed+slope*trap->aTimes;

	 if(trap->currentSpeed>trap->speedMax)
	{
	   trap->currentSpeed=trap->speedMax;
	}
	 
	 if(trap->currentSpeed<trap->speedMin)
	{
	   trap->currentSpeed=trap->speedMin;
	}
}


static void CalCurveSPTA(CurveObjectType *spta)
{
	 float power=0.0;
	 float speed=0.0;
	 
	 power=(2*((float)spta->aTimes)-((float)spta->maxTimes))/((float)spta->maxTimes);
	 power=(0.0-spta->flexible)*power;
	 
	 speed=1+expf(power);
	 speed=(spta->targetSpeed-spta->startSpeed)/speed;
	 spta->currentSpeed=speed+spta->startSpeed;
	 
	 if(spta->currentSpeed>spta->speedMax)
	{
		spta->currentSpeed=spta->speedMax;
	}
	 
	 if(spta->currentSpeed<spta->speedMin)
	{
		spta->currentSpeed=spta->speedMin;
	}
}
