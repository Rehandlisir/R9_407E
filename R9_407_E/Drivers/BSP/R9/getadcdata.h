/*
 * @Author: lisir lisir@rehand.com
 * @Date: 2024-06-07 16:01:18
 * @LastEditors: lisir lisir@rehand.com
 * @LastEditTime: 2024-06-14 10:45:00
 * @FilePath: \MDK-ARMc:\Users\fu\Desktop\Code\CodeV1.1\R9_407_V1.1\R9_407_V1.1\Drivers\BSP\R9\getadcdata.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#ifndef __GETADCDATA_H
#define __GETADCDATA_H

#include "./SYSTEM/sys/sys.h"

/************************************** ҡ�����ݳ��� �� ADC ���ݽṹ�嶨�� ****************************************************/
#define yadc_max      1700
#define yadc_min      -1700
#define xadc_max      1700
#define xadc_min      -1700

#define yadc_Dim      200
#define xadc_Dim      200

/*define class of joystic*/
#define SMC35B2G 

#if defined SMC35B2G
#define X_BASEreturn	0

#elif defined SMC25B2G
#define X_BASEreturn	1

#endif  



typedef struct 
{
    int32_t l_brakcurrent;
    int32_t r_brakcurrent;

    int32_t LED24_CURRENT;
    int32_t LED12_CURRENT;

    uint32_t A1_ADC;
    uint32_t A2_ADC;
    uint32_t B1_ADC;
    uint32_t B2_ADC;

    double A1V;
    double A2V;
    double B1V;
    double B2V;

    double A1V_BACKEMF;
    double A2V_BACKEMF;
    double B1V_BACKEMF;
    double B2V_BACKEMF;

    int32_t adc_x;
    int32_t adc_y;
    int32_t adc_xbase;
    int32_t adc_ybase;
    int32_t l_current;
    double l_currentAct;
    double r_currentAct;
    int32_t r_current;

    int32_t lift_pos;
    int32_t pedestal_pos;
    int32_t backboard_pos;
    int32_t legangle_pos;
    int32_t leglength_pos;
    int32_t support_pos;
    int32_t bat_v;
	int32_t chargeI_adc;
    double chargeI_adcAct;
    
} ADCDATA;
extern ADCDATA adcdata;
void Datareset(void);
void getadcDataInit(void);
void getadcData(void);
void getadc1Data(void);
void getadc3Data(void);
void joysticData_calculateInit(void);
void joysticData_calculate(void);
#endif
