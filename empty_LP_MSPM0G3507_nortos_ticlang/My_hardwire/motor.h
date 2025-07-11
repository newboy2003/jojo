#ifndef MOTO_h
#define MOTO_h

#include "ti_msp_dl_config.h"
#include "PID.h"

#define  base_speed 4000

void motor_speed(int leftspeed, int rightspeed);
void track(void);

#endif