/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ti_msp_dl_config.h"

// 延时函数：大约延时ms毫秒（基于32MHz时钟）
/*void delay_ms(uint32_t ms) 
{
    uint32_t i, j;
    for (i = 0; i < ms; i++) 
    {
        for (j = 0; j < 8000; j++); // 调整此值可校准延时
    }
}*/


//int value = 10000;


void motor_speed(int leftspeed,int rightspeed);
void track(void);

int main(void)
{
    SYSCFG_DL_init();

    // 启动PWM定时器
    DL_TimerG_startCounter(PWM_0_INST);
    DL_TimerG_startCounter(PWM_1_INST);

    while (1) 
    {
        //motor_speed(100,100);
        track();
    }
}

int base_speed = 4000;
int dif_speed_1 = 1600;
int dif_speed_2 = 2200;
int dif_speed_3 = 2800;


void motor_speed(int leftspeed,int rightspeed)
{
    // 限制速度范围
    if(leftspeed>=0)
    {
        DL_GPIO_clearPins(GPIO_LEFT_MOTOR_PORT, GPIO_LEFT_MOTOR_LEFT_PIN_0_PIN);
        DL_GPIO_setPins(GPIO_LEFT_MOTOR_PORT, GPIO_LEFT_MOTOR_LEFT_PIN_1_PIN);
    }
    else
    {
        DL_GPIO_setPins(GPIO_LEFT_MOTOR_PORT, GPIO_LEFT_MOTOR_LEFT_PIN_0_PIN);
        DL_GPIO_clearPins(GPIO_LEFT_MOTOR_PORT, GPIO_LEFT_MOTOR_LEFT_PIN_1_PIN);
    }
    if(rightspeed>=0)
    {
        DL_GPIO_setPins(GPIO_RIGHT_MOTOR_PORT, GPIO_RIGHT_MOTOR_RIGHT_PIN_0_PIN);
        DL_GPIO_clearPins(GPIO_RIGHT_MOTOR_PORT, GPIO_RIGHT_MOTOR_RIGHT_PIN_1_PIN);

    }
    else
    {
        DL_GPIO_clearPins(GPIO_RIGHT_MOTOR_PORT, GPIO_RIGHT_MOTOR_RIGHT_PIN_0_PIN);
        DL_GPIO_setPins(GPIO_RIGHT_MOTOR_PORT, GPIO_RIGHT_MOTOR_RIGHT_PIN_1_PIN);
        rightspeed = -rightspeed;
    }

    //DL_TimerG_setCaptureCompareValue(PWM_0_INST, leftspeed, GPIO_PWM_0_C0_PIN);  // 左电机PWM（通道0）
    //DL_TimerG_setCaptureCompareValue(PWM_0_INST, rightspeed,GPIO_PWM_0_C1_PIN); // 右电机PWM（通道1

    DL_TimerG_setCaptureCompareValue(PWM_1_INST, leftspeed, DL_TIMER_CC_1_INDEX); 
    DL_TimerG_setCaptureCompareValue(PWM_0_INST, rightspeed, DL_TIMER_CC_1_INDEX);
    
}

void track(void) 
{
    // 优先处理极端偏离情况
    if(DL_GPIO_readPins(GPIO_SENSOR_PIN_0_PORT, GPIO_SENSOR_PIN_0_PIN))
    {
        motor_speed(base_speed + dif_speed_3, base_speed - dif_speed_3);
    } 
    else if(DL_GPIO_readPins(GPIO_SENSOR_PIN_7_PORT, GPIO_SENSOR_PIN_7_PIN)) 
    {
        motor_speed(base_speed - dif_speed_3, base_speed + dif_speed_3);
    }
    // 增加中间传感器的判断优先级
    else if(DL_GPIO_readPins(GPIO_SENSOR_PIN_3_PORT, GPIO_SENSOR_PIN_3_PIN) && DL_GPIO_readPins(GPIO_SENSOR_PIN_4_PORT, GPIO_SENSOR_PIN_4_PIN)) 
    {
        motor_speed(base_speed, base_speed); // 双中间传感器检测时直行
    }
    // 再处理其他传感器
    else if(DL_GPIO_readPins(GPIO_SENSOR_PIN_1_PORT, GPIO_SENSOR_PIN_1_PIN)) {
        motor_speed(base_speed + dif_speed_2, base_speed - dif_speed_2);
    }
    else if(DL_GPIO_readPins(GPIO_SENSOR_PIN_6_PORT, GPIO_SENSOR_PIN_6_PIN)) {
        motor_speed(base_speed - dif_speed_2, base_speed + dif_speed_2);
    }
    else if(DL_GPIO_readPins(GPIO_SENSOR_PIN_2_PORT, GPIO_SENSOR_PIN_2_PIN)) {
        motor_speed(base_speed + dif_speed_1, base_speed - dif_speed_1);
    }
    else if(DL_GPIO_readPins(GPIO_SENSOR_PIN_5_PORT, GPIO_SENSOR_PIN_5_PIN)) {
        motor_speed(base_speed - dif_speed_1, base_speed + dif_speed_1);
    }
    // 单中间传感器检测视为轻微偏离
    else if(DL_GPIO_readPins(GPIO_SENSOR_PIN_3_PORT, GPIO_SENSOR_PIN_3_PIN)) {
        motor_speed(base_speed + dif_speed_1, base_speed - dif_speed_1);
    }
    else if(DL_GPIO_readPins(GPIO_SENSOR_PIN_4_PORT, GPIO_SENSOR_PIN_4_PIN)) {
        motor_speed(base_speed - dif_speed_1, base_speed + dif_speed_1);
    }
    // 无传感器检测时缓行搜索
    else {
        motor_speed(base_speed, base_speed);
    }
}