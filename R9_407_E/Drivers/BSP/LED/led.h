/**
 * @FilePath     : /MDK-ARMc:/Users/fu/Desktop/Code/R9_407E/R9_407_E/Drivers/BSP/LED/led.h
 * @Description  :  LED Control
 * @Author       : lisir lisir@rehand.com
 * @Version      : 0.0.1
 * @LastEditors  : lisir lisir@rehand.com
 * @LastEditTime : 2024-07-03 10:16:15
 * @2024 by Rehand Medical Technology Co., LTD, All Rights Reserved.
**/

#ifndef __LED_H
#define __LED_H

#include "./SYSTEM/sys/sys.h"

#define LED0_GPIO_PORT GPIOA
#define LED0_GPIO_PIN GPIO_PIN_4
#define LED0_GPIO_CLK_ENABLE()        \
    do                                \
    {                                 \
        __HAL_RCC_GPIOA_CLK_ENABLE(); \
    } while (0) /* PF��ʱ��ʹ�� */

#define LED1_GPIO_PORT GPIOF
#define LED1_GPIO_PIN GPIO_PIN_11
#define LED1_GPIO_CLK_ENABLE()        \
    do                                \
    {                                 \
        __HAL_RCC_GPIOF_CLK_ENABLE(); \
    } while (0) /* PF��ʱ��ʹ�� */

/*前 - 左转向*/
#define LEFT_FRONT_TURE_GPIO_PORT GPIOC
#define LEFT_FRONT_TURE_GPIO_PIN GPIO_PIN_13
#define LEFT_FRONT_TURE_GPIO_CLK_ENABLE() \
    do                                    \
    {                                     \
        __HAL_RCC_GPIOC_CLK_ENABLE();     \
    } while (0) /* PC13 ????? */
/*前 - 右转向*/
#define RIGHT_FRONT_TURE_GPIO_PORT GPIOD
#define RIGHT_FRONT_TURE_GPIO_PIN GPIO_PIN_4
#define RIGHT_FRONT_TURE_GPIO_CLK_ENABLE() \
    do                                     \
    {                                      \
        __HAL_RCC_GPIOD_CLK_ENABLE();      \
    } while (0)
/*前 - 主灯*/
#define FRONT_MAIN_GPIO_PORT GPIOB
#define FRONT_MAIN_GPIO_PIN GPIO_PIN_11
#define FRONT_MAIN_GPIO_CLK_ENABLE()  \
    do                                \
    {                                 \
        __HAL_RCC_GPIOB_CLK_ENABLE(); \
    } while (0)


/*后 -左转向*/
#define LEFT_BACK_TURE_GPIO_PORT GPIOE
#define LEFT_BACK_TURE_GPIO_PIN GPIO_PIN_3
#define LEFT_BACK_TURE_GPIO_CLK_ENABLE() \
    do                                   \
    {                                    \
        __HAL_RCC_GPIOE_CLK_ENABLE();    \
    } while (0) /* PC13 ????? */
/*后 -右转向*/
#define RIGHT_BACK_TURE_GPIO_PORT GPIOE
#define RIGHT_BACK_TURE_GPIO_PIN GPIO_PIN_2
#define RIGHT_BACK_TURE_GPIO_CLK_ENABLE() \
    do                                    \
    {                                     \
        __HAL_RCC_GPIOE_CLK_ENABLE();     \
    } while (0)
/*后 - 主灯*/
#define BACK_MAIN_GPIO_PORT GPIOB
#define BACK_MAIN_GPIO_PIN GPIO_PIN_10
#define BACK_MAIN_GPIO_CLK_ENABLE()   \
    do                                \
    {                                 \
        __HAL_RCC_GPIOB_CLK_ENABLE(); \
    } while (0)

/******************************************************************************************/

/* 指示LED */
#define LED0(x)                                                                                                                                \
    do                                                                                                                                         \
    {                                                                                                                                          \
        x ? HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_GPIO_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_GPIO_PIN, GPIO_PIN_RESET); \
    } while (0) /* LED0 = RED */

#define LED1(x)                                                                                                                                \
    do                                                                                                                                         \
    {                                                                                                                                          \
        x ? HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN, GPIO_PIN_RESET); \
    } while (0) /* LED1 = GREEN */

#define LED0_TOGGLE()                                      \
    do                                                     \
    {                                                      \
        HAL_GPIO_TogglePin(LED0_GPIO_PORT, LED0_GPIO_PIN); \
    } while (0) /* LED0 = !LED0 */
#define LED1_TOGGLE()                                      \
    do                                                     \
    {                                                      \
        HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_GPIO_PIN); \
    } while (0) /* LED1 = !LED1 */

/*前 -左转向控制*/
#define LEFT_FRONT_TURE(x)                                                                                                                                                                 \
    do                                                                                                                                                                                     \
    {                                                                                                                                                                                      \
        x ? HAL_GPIO_WritePin(LEFT_FRONT_TURE_GPIO_PORT, LEFT_FRONT_TURE_GPIO_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(LEFT_FRONT_TURE_GPIO_PORT, LEFT_FRONT_TURE_GPIO_PIN, GPIO_PIN_RESET); \
    } while (0) /* ????? */
/*前-右转向控制*/
#define RIGHT_FRONT_TURE(x)                                                                                                                                                                    \
    do                                                                                                                                                                                         \
    {                                                                                                                                                                                          \
        x ? HAL_GPIO_WritePin(RIGHT_FRONT_TURE_GPIO_PORT, RIGHT_FRONT_TURE_GPIO_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(RIGHT_FRONT_TURE_GPIO_PORT, RIGHT_FRONT_TURE_GPIO_PIN, GPIO_PIN_RESET); \
    } while (0)
/*前 -主灯控制*/
#define FRONT_MAIN(x)                                                                                                                                                  \
    do                                                                                                                                                                 \
    {                                                                                                                                                                  \
        x ? Main_PwmSet(x) :\
            Main_PwmSet(x); \
    } while (0)
/*前 -左转向控制*/
#define LEFT_FRONT_TURE_TOGGLE()                                                 \
    do                                                                           \
    {                                                                            \
        HAL_GPIO_TogglePin(LEFT_FRONT_TURE_GPIO_PORT, LEFT_FRONT_TURE_GPIO_PIN); \
    } while (0) /*????? ?? */
/*前-右转向控制*/   
#define RIGHT_FRONT_TURE_TOGGLE()                                                  \
    do                                                                             \
    {                                                                              \
        HAL_GPIO_TogglePin(RIGHT_FRONT_TURE_GPIO_PORT, RIGHT_FRONT_TURE_GPIO_PIN); \
    } while (0) /*????? ?? */


/*后-左转向控制*/
#define LEFT_BACK_TURE(x)                                                                                                                                                              \
    do                                                                                                                                                                                 \
    {                                                                                                                                                                                  \
        x ? HAL_GPIO_WritePin(LEFT_BACK_TURE_GPIO_PORT, LEFT_BACK_TURE_GPIO_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(LEFT_BACK_TURE_GPIO_PORT, LEFT_BACK_TURE_GPIO_PIN, GPIO_PIN_RESET); \
    } while (0) /* ????? */
/*后-右转向控制*/
#define RIGHT_BACK_TURE(x)                                                                                                                                                                 \
    do                                                                                                                                                                                     \
    {                                                                                                                                                                                      \
        x ? HAL_GPIO_WritePin(RIGHT_BACK_TURE_GPIO_PORT, RIGHT_BACK_TURE_GPIO_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(RIGHT_BACK_TURE_GPIO_PORT, RIGHT_BACK_TURE_GPIO_PIN, GPIO_PIN_RESET); \
    } while (0)
/*后-主灯控制*/
#define BACK_MAIN(x)                                                                                                                                               \
    do                                                                                                                                                             \
    {                                                                                                                                                              \
        x ? Main_PwmSet(x) : Main_PwmSet(x); \
    } while (0)

#define MainBulbState HAL_GPIO_ReadPin(FRONT_MAIN_GPIO_PORT,FRONT_MAIN_GPIO_PIN);
#define LeftBulbState HAL_GPIO_ReadPin(LEFT_FRONT_TURE_GPIO_PORT,LEFT_FRONT_TURE_GPIO_PIN);
#define RightBulbState HAL_GPIO_ReadPin(RIGHT_FRONT_TURE_GPIO_PORT,RIGHT_FRONT_TURE_GPIO_PIN);

// #define KEYCONTRONL 0

/******************************************************************************************/

typedef enum
{
    None,
    idle_state,
    open_leftbling,
    open_rightbling,
    open_doublebling,
    open_mainbulb,

    close_leftbling,
    close_rightbling,
    close_doublebling,
    close_mainbulb
} Led_State;

/* */
void led_init(void);
void led_beepControl(void); /*  */
void led_beepControlRK3588(void);
void LEDmain_pwm_chy_init(uint16_t arr, uint16_t psc);
void LED_pwm_Set(uint16_t ccr3,uint16_t ccr4);
void Main_PwmSet(uint8_t flage);
#endif /**/
