#include "./BSP/Exception_handling/exception_handling.h"
void ex_handl_Init(void)
{
	linearActuatordec_init();
}
void ex_handl_brake(void)
{
	// if (LEFT_BREAK_STATE && (!RIGHT_BRAKE_STATE )) //LEFT push
	// {
	// 	g_slaveReg[4] = 1;
	// 	g_slaveReg[27] = 1;
	// 	// printf("push\n");
	// }
    // else if ((!LEFT_BREAK_STATE) && RIGHT_BRAKE_STATE ) //RIGHT push
	// {
	// 	g_slaveReg[4] = 1;
	// 	g_slaveReg[27] = 1;

	// 	// printf("push\n");

	// }
	// else if ((LEFT_BREAK_STATE) && RIGHT_BRAKE_STATE ) // LEFT && RIGHT push
	// {
	// 	g_slaveReg[4] = 1;
	// 	g_slaveReg[27] = 1;
	// 	// printf("push\n");
	// }
	// else //LEFT && RIGHT  DRIVE
	// {
	// 	g_slaveReg[4] = 1;
	// 	g_slaveReg[27] = 1;
	// 	// printf("drive\n");  
	// }
		g_slaveReg[4] = 1;
		g_slaveReg[27] = 1;
}

void ex_handl_LRmoter(void)
{
	
	
	
	;
	
	
}
void linearActuatordec_init(void)
{
    // GPIO_InitTypeDef gpio_init_struct;                         
    // BACK_DEC_GPIO_CLK_ENABLE();
    // PEDESTAL_DEC_GPIO_CLK_ENABLE();
	// LIFT_DEC_GPIO_CLK_ENABLE();
	// LEGANGLE_DEC_GPIO_CLK_ENABLE();
	// LEGLENGTH_DEC_GPIO_CLK_ENABLE();
	// SUPPORT_DEC_GPIO_CLK_ENABLE();

    // gpio_init_struct.Pin = BACK_DEC_DEC_PIN|PEDESTAL_DEC_PIN|LIFT_DEC_PIN|LEGLENGTH_DEC_DEC_PIN|SUPPORT_DEC_DEC_PIN;                       
    // gpio_init_struct.Mode = GPIO_MODE_INPUT;                    
    // gpio_init_struct.Pull = GPIO_PULLDOWN;                       
    // gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              
    // HAL_GPIO_Init(GPIOG, &gpio_init_struct); 
	
    // gpio_init_struct.Pin = LEGANGLE_DEC_PIN; 
    // HAL_GPIO_Init(LEGANGLE_DEC_POR, &gpio_init_struct); 
}


void ex_handl_linearActuator(void)
{	
	CanCmdled(LED_YELLOW,LED_GREEN,LED_GREEN,LED_GREEN,LED_GREEN);
	if (BACK_OVERCURRENT_STATE)
	{

		;
	}
	if (PEDESTAL_OVERCURRENT_STATE)
	{
		;

	}

	if (LIFT_OVERCURRENT_STATE)
	{

		;
	}

	if (LEGANGLE_OVERCURRENT_STATE)
	{

		;
	}

	if (LEGLENGTH_OVERCURRENT_STATE)
	{

		;
	}
   if (SUPPORT_OVERCURRENT_STATE)
   {
		;


   }
}

void ex_handl_battary(void)
{
	
	;
	
}

void ex_handl_joystic(void)
{
	
	
	;
	
}

void ex_handl_indicatorlight(void)
{
	
	;
}

void ex_handl_excute(void)
{
	
	ex_handl_brake();
	
	ex_handl_LRmoter();
	
	ex_handl_linearActuator();
	
	ex_handl_battary();
	
	ex_handl_joystic();
	
}
