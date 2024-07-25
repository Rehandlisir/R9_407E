#ifndef __UNDERPANCONTROL_H
#define __UNDERPANCONTROL_H

#include "./SYSTEM/sys/sys.h"
#include "math.h"
#include "stdio.h"
#include "./BSP/R9/brake.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/Common/common.h"
#include "./BSP/R9/Slavemodbus.h"
#include "./BSP/R9/getadcdata.h"
#include "./BSP/R9/moterdriver.h"
#include "./BSP/R9/mpu6050.h"
#include "./BSP/R9/inv_mpu.h"
#include "./BSP/R9/inv_mpu_dmp_motion_driver.h" 
#include "./BSP/R9/mlx90393.h"

/* ???? */
#define underpan_H 0.55 /*???*/

#define pi 3.1415926 /*????PI*/

#define GEAR_RATIO 29.5		 /* 减速比 */
#define Diameter 0.354		 /* 轮子直径354mm  */
#define MoterMaxrN 175.0	 /*输出轴额定转速 175PRM */
#define KMPH_TO_RPM    15.0  /*1km/h 约 15RPM*/
#define VelocityConst  7.3   /*电机速率常数 单位 RPM/V*/
#define KMPH_TO_Voltage 2.055 /*V/KMPH*/
#define KMPH_TO_Duty   0.087  /*1km/h 占空比约 8.56%*/

/*   */

typedef enum
{
	 idle = 0,
	 forward,
	 backward,
	 front_left,
	 front_right,
	 back_left,
	 back_right,
	 turnself_left,
	 turnself_right
} RunState;

typedef struct /**/
{
	int16_t adcx;
	int16_t adcy;
	double set_Maximum_Strspeed;
} VELOCITY_PIn;
extern VELOCITY_PIn  velPlanIn1;
typedef struct /*??????????*/
{
	double underpanVelocity;	  
	double presentation_velocity; 		  
	double acceleration_coeff;	  
	double steering_angle;		  
	double L_Velocity;			  
	double R_Velocity;			  		
	double L_Dutycycle;			  
	double R_Dutycycle;			 	   
	RunState runstate; 
} VELOCITY_POUT;
void velocity_mapingLocal(VELOCITY_PIn velPlanIn);
void velocity_mapingRemote(VELOCITY_PIn velPlanIn);
void brake_excute(void);
void VelocityLevelSet(void);
void underpanExcute(void);
void MPU6050Excute(void);

#endif
