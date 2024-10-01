//
// Created by ubuntu on 24-10-1.
//

#include "Extended_PID.h"

#include "INS_task.h"
#include "cmsis_os.h"
#include "controller.h"
#include "dm4310_drv.h"

extern INS_t INS;
PID_t PID_Example;
Joint_Motor_t Motor;

void Extended_PID_task(void)
{
    while(INS.ins_flag==0)
    {//等待加速度收敛
        osDelay(1);
    }

    Extended_PID_Init(&Motor, &PID_Example);
    while(1)
    {

        osDelay(1);
    }
}

void Extended_PID_Init(Joint_Motor_t *_Motor, PID_t *_PID)
{
    PID_Init(_PID, 5.f, 2.f, 0.1f,
        1.f, 0.f, 0.f,
        100.f, 100.f, 0.f,
        0.f, 0.f, 0.f);

    joint_motor_init(_Motor, 0x01, SPEED_MODE);
}
