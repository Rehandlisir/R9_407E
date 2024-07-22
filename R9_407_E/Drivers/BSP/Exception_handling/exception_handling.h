#ifndef __EXCEPT_H
#define __EXCEPT_H
#include "./SYSTEM/sys/sys.h"
#include "./BSP/R9/brake.h"
#include "./BSP/R9/Slavemodbus.h"
#include "./BSP/R9/getadcdata.h"
#include "./BSP/CAN/can.h"

#define BACK_DEC_DEC_POR GPIOG
#define BACK_DEC_DEC_PIN GPIO_PIN_15
#define BACK_DEC_GPIO_CLK_ENABLE()        \
    do                                \
    {                                 \
        __HAL_RCC_GPIOG_CLK_ENABLE(); \
    } while (0) /*  */

#define PEDESTAL_DEC_POR GPIOG
#define PEDESTAL_DEC_PIN GPIO_PIN_10
#define PEDESTAL_DEC_GPIO_CLK_ENABLE()        \
    do                                \
    {                                 \
        __HAL_RCC_GPIOG_CLK_ENABLE(); \
    } while (0) /*  */

#define LIFT_DEC_POR GPIOG
#define LIFT_DEC_PIN GPIO_PIN_11
#define LIFT_DEC_GPIO_CLK_ENABLE()        \
    do                                \
    {                                 \
        __HAL_RCC_GPIOG_CLK_ENABLE(); \
    } while (0) /*  */

#define LEGANGLE_DEC_POR  GPIOD
#define LEGANGLE_DEC_PIN GPIO_PIN_7
#define LEGANGLE_DEC_GPIO_CLK_ENABLE()        \
    do                                \
    {                                 \
        __HAL_RCC_GPIOD_CLK_ENABLE(); \
    } while (0) /*  */

#define LEGLENGTH_DEC_DEC_POR GPIOG
#define LEGLENGTH_DEC_DEC_PIN GPIO_PIN_12
#define LEGLENGTH_DEC_GPIO_CLK_ENABLE()        \
    do                                \
    {                                 \
        __HAL_RCC_GPIOG_CLK_ENABLE(); \
    } while (0) /*  */

#define SUPPORT_DEC_DEC_POR GPIOG
#define SUPPORT_DEC_DEC_PIN GPIO_PIN_13
#define SUPPORT_DEC_GPIO_CLK_ENABLE()        \
    do                                \
    {                                 \
        __HAL_RCC_GPIOG_CLK_ENABLE(); \
    } while (0) /*  */


#define BACK_OVERCURRENT_STATE         HAL_GPIO_ReadPin(BACK_DEC_DEC_POR,BACK_DEC_DEC_PIN)  
#define PEDESTAL_OVERCURRENT_STATE     HAL_GPIO_ReadPin(PEDESTAL_DEC_POR,PEDESTAL_DEC_PIN) 
#define LIFT_OVERCURRENT_STATE         HAL_GPIO_ReadPin(LIFT_DEC_POR,LIFT_DEC_PIN) 
#define LEGANGLE_OVERCURRENT_STATE     HAL_GPIO_ReadPin(LEGANGLE_DEC_POR,LEGANGLE_DEC_PIN) 
#define LEGLENGTH_OVERCURRENT_STATE    HAL_GPIO_ReadPin(LEGLENGTH_DEC_DEC_POR,LEGLENGTH_DEC_DEC_PIN) 
#define SUPPORT_OVERCURRENT_STATE      HAL_GPIO_ReadPin(SUPPORT_DEC_DEC_POR,SUPPORT_DEC_DEC_PIN) 

void linearActuatordec_init(void);
void ex_handl_Init(void);
void ex_handl_brake(void);
void ex_handl_LRmoter(void);
void ex_handl_linearActuator(void);
void ex_handl_battary(void);
void ex_handl_joystic(void);
void ex_handl_indicatorlight(void);
void ex_handl_excute(void);
#endif
