#ifndef PID_h
#define PID_h

#include "ti_msp_dl_config.h"

void pid_init(void);
int pid_calculate(float actual);

#endif