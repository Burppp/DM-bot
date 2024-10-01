//
// Created by ubuntu on 24-10-1.
//

#ifndef EXTENDED_PID_H
#define EXTENDED_PID_H

#include "main.h"
#include "dm4310_drv.h"
#include "controller.h"

void Extended_PID_task(void);
void Extended_PID_Init(Joint_Motor_t *_Motor, PID_t *_PID);

#endif //EXTENDED_PID_H
