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
FuzzyRule_t FuzzyRule_Example;
Joint_Motor_t Motor;
float input = 0;

float current_time = 0;

static float FuzzyRuleKpRAW[7][7] = {
        PB, PB, PM, PM, PS, ZE, ZE,
        PB, PB, PM, PS, PS, ZE, PS,
        PM, PM, PM, PS, ZE, PS, PS,
        PM, PM, PS, ZE, PS, PM, PM,
        PS, PS, ZE, PS, PS, PM, PM,
        PS, ZE, PS, PM, PM, PM, PB,
        ZE, ZE, PM, PM, PM, PB, PB};

static float FuzzyRuleKiRAW[7][7] = {
        PB, PB, PM, PM, PS, ZE, ZE,
        PB, PB, PM, PS, PS, ZE, ZE,
        PB, PM, PM, PS, ZE, PS, PS,
        PM, PM, PS, ZE, PS, PM, PM,
        PS, PS, ZE, PS, PS, PM, PB,
        ZE, ZE, PS, PS, PM, PB, PB,
        ZE, ZE, PS, PM, PM, PB, PB};

static float FuzzyRuleKdRAW[7][7] = {
        PS, PS, PB, PB, PB, PM, PS,
        PS, PS, PB, PM, PM, PS, ZE,
        ZE, PS, PM, PM, PS, PS, ZE,
        ZE, PS, PS, PS, PS, PS, ZE,
        ZE, ZE, ZE, ZE, ZE, ZE, ZE,
        PB, PS, PS, PS, PS, PS, PB,
        PB, PM, PM, PM, PS, PS, PB};

void Extended_PID_task(void)
{
    while(INS.ins_flag==0)
    {//等待加速度收敛
        osDelay(1);
    }

    Extended_PID_Init(&Motor, &PID_Example, &FuzzyRule_Example);
    while(1)
    {
        DWT_SysTimeUpdate();
        current_time = DWT_GetTimeline_s();
        Fuzzy_Rule_Implementation(PID_Example.FuzzyRule, Motor.para.vel, input);
        PID_Calculate(&PID_Example, Motor.para.vel, input);
        speed_ctrl(&hfdcan1, Motor.para.id, PID_Example.Output);
        osDelay(1);
    }
}

void Extended_PID_Init(Joint_Motor_t *_Motor, PID_t *_PID, FuzzyRule_t *_Rule)
{
    Fuzzy_Rule_Init(_Rule, FuzzyRuleKpRAW, FuzzyRuleKiRAW, FuzzyRuleKdRAW, 1.f, 1.f, 0.1f, 0.f, 0.f);

    PID_Init(_PID, 15.f, 12.f, 0.1f,
        1.f, 30.f, 0.f,
        100.f, 100.f, 0.f,
        0.f, 3, Derivative_On_Measurement | Integral_Limit);

    _PID->FuzzyRule = _Rule;

    joint_motor_init(_Motor, 0x01, SPEED_MODE);
    for(int i = 0;i < 10;i++)
    {
        enable_motor_mode(&hfdcan1, _Motor->para.id, SPEED_MODE);
        osDelay(1);
    }
}
