#include "./BSP/Communicationheartbeat/Comheartbeat.h"
STRUCT_COMHEART comheartstate;

void  ComheartReset(void)
{
	
	if (g_slaveReg[87] == 1)/*接收到心跳信号就置 0*/
    {
        g_slaveReg[87] = 0;
    }
	
}

/**
 * @brief        : 该函数 需在大于 200ms 周期执行一次，
 * @param         {uint16_t} overtcont:
 * @return        1： 通讯成功   0:通讯失败（连续 overtcont 次都为0 则判定上位机通讯失败）
**/
void  ComheartDetect(uint16_t overtcont)
{
	static uint16_t comfali_cont;
	if (g_slaveReg[87] == 0) /*通讯正常情况下该值会上位机200ms 置一次 1 不会为 0，一旦为 0 且超过一定次数判定通讯失败*/
    {
        comfali_cont++;
        if (comfali_cont > overtcont)
        {
            comfali_cont = 0;
            comheartstate.com_state = Fail;
        }

    }
    else  /*上位机有心跳 通讯是OK的*/
    {
        comfali_cont = 0;
        comheartstate.com_state = Success;
    }
	
}
