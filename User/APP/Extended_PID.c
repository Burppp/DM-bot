//
// Created by ubuntu on 24-10-1.
//

#include "Extended_PID.h"

#include "INS_task.h"
#include "cmsis_os.h"
#include "controller.h"
#include "dm4310_drv.h"

extern INS_t INS;
Joint_Motor_t Motor;

void Extended_PID_task(void)
{
    while(INS.ins_flag==0)
    {//等待加速度收敛
        osDelay(1);
    }

    while(1)
    {

        osDelay(1);
    }
}

void Extended_PID_Init(Joint_Motor_t *_Motor)
{

}
