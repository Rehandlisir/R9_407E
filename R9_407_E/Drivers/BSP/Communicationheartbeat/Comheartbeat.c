#include "./BSP/Communicationheartbeat/Comheartbeat.h"
STRUCT_COMHEART comheartstate;

void  ComheartReset(void)
{
        g_slaveReg[87] = 0;
  
}

/**
 * @brief        : 该函数 需在大于 200ms 定时器下执行一次，
 * @param         {uint16_t} overtcont:
 * @return        1：Success： 通讯成功   0:Fail通讯失败（连续 overtcont 次都为0 则判定上位机通讯失败）
**/
uint8_t  ComheartDetect(uint16_t overtcont)
{
	static uint16_t comfali_cont;
	if (g_slaveReg[87] == 0) /*读取寄存器g_slaveReg[87]是否被上位机置1了，连续overtcont次未被置1 则判定上位机关机或断线通讯失败*/
    {
        comfali_cont++;
        if (comfali_cont > overtcont)
        {
            comfali_cont = 0;
            comheartstate.com_state = Fail;
        }
    }
    else  /*上位机有心跳 通讯是OK的将g_slaveReg[87] 置0*/
    {
        comheartstate.com_state = Success;
        comfali_cont = 0;
    }
    return comheartstate.com_state;
}
