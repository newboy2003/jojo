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

// PID结构体定义（位置式PID）
typedef struct {
    float target;       // 目标值（轨道中心，设为0）
    float actual;       // 实际值（传感器检测到的偏差）
    float out;          // PID输出（电机速度调节量）
    
    float kp;           // 比例系数
    float ki;           // 积分系数
    float kd;           // 微分系数
    
    float error0;       // 当前误差
    float error1;       // 上一次误差
    float error_int;    // 误差积分
    
    float integral_limit; // 积分限幅
    float output_limit;   // 输出限幅
} PID_TypeDef;

// 全局PID变量
PID_TypeDef pid;

// 函数声明
void motor_speed(int leftspeed, int rightspeed);
void track(void);
void pid_init(void);
int pid_calculate(void);

// 电机基础速度
int base_speed = 4000;

int main(void)
{
    SYSCFG_DL_init();

    // 启动PWM定时器
    DL_TimerG_startCounter(PWM_0_INST);
    DL_TimerG_startCounter(PWM_1_INST);

    // 初始化PID参数
    pid_init();

    while (1) 
    {
        track(); // 循迹逻辑（包含PID计算）
    }
}

// PID初始化
void pid_init(void)
{
    pid.target = 0.0f;       // 目标值：轨道中心（偏差为0）
    pid.actual = 0.0f;       // 初始实际偏差
    
    // PID参数（需根据实际调试调整）
    pid.kp = 900.0f;         // 比例系数：快速响应偏差
    pid.ki = 0.0f;           // 积分系数：消除静态误差
    pid.kd = 0.0f;          // 微分系数：抑制超调
    
    pid.error0 = 0.0f;
    pid.error1 = 0.0f;
    pid.error_int = 0.0f;
    
    pid.integral_limit = 500.0f; // 积分限幅，防止饱和
    pid.output_limit = 20000.0f;  // 输出限幅，限制最大转向力度
}

// 位置式PID计算
int pid_calculate(void)
{
    // 1. 计算当前误差（目标值 - 实际值）
    pid.error0 = pid.target - pid.actual;
    
    // 2. 计算误差积分（累加误差，带限幅）
    pid.error_int += pid.error0;
    if (pid.error_int > pid.integral_limit)
        pid.error_int = pid.integral_limit;
    else if (pid.error_int < -pid.integral_limit)
        pid.error_int = -pid.integral_limit;
    
    // 3. 计算误差微分（当前误差 - 上一次误差）
    float error_diff = pid.error0 - pid.error1;
    
    // 4. 位置式PID输出公式
    pid.out = pid.kp * pid.error0 + pid.ki * pid.error_int + pid.kd * error_diff;
    
    // 5. 输出限幅
    if (pid.out > pid.output_limit)
        pid.out = pid.output_limit;
    else if (pid.out < -pid.output_limit)
        pid.out = -pid.output_limit;
    
    // 6. 保存当前误差为下一次历史误差
    pid.error1 = pid.error0;
    
    return (int)pid.out;
}

// 电机速度控制函数
void motor_speed(int leftspeed, int rightspeed)
{
    // 左电机方向控制
    if (leftspeed >= 0) 
    {
        DL_GPIO_clearPins(GPIO_LEFT_MOTOR_PORT, GPIO_LEFT_MOTOR_LEFT_PIN_0_PIN);
        DL_GPIO_setPins(GPIO_LEFT_MOTOR_PORT, GPIO_LEFT_MOTOR_LEFT_PIN_1_PIN);
    }
    else 
    {
        DL_GPIO_setPins(GPIO_LEFT_MOTOR_PORT, GPIO_LEFT_MOTOR_LEFT_PIN_0_PIN);
        DL_GPIO_clearPins(GPIO_LEFT_MOTOR_PORT, GPIO_LEFT_MOTOR_LEFT_PIN_1_PIN);
        leftspeed = -leftspeed;
    }
    
    // 右电机方向控制
    if (rightspeed >= 0) 
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
    
    // 设置PWM占空比
    DL_TimerG_setCaptureCompareValue(PWM_1_INST, leftspeed, DL_TIMER_CC_1_INDEX);
    DL_TimerG_setCaptureCompareValue(PWM_0_INST, rightspeed, DL_TIMER_CC_1_INDEX);
}

// 循迹逻辑（核心：更新actual值并调用PID）
void track(void)
{
    // 1. 根据传感器状态更新实际偏差值actual
    if (DL_GPIO_readPins(GPIO_SENSOR_PIN_0_PORT, GPIO_SENSOR_PIN_0_PIN))  // 最左侧传感器（大幅左偏）
    {
        pid.actual = -4.0f;  // 偏差为正：需向右修正
    } 
    else if (DL_GPIO_readPins(GPIO_SENSOR_PIN_7_PORT, GPIO_SENSOR_PIN_7_PIN))  // 最右侧传感器（大幅右偏）
    {   
        pid.actual = 4.0f; // 偏差为负：需向左修正
    }
    else if (DL_GPIO_readPins(GPIO_SENSOR_PIN_1_PORT, GPIO_SENSOR_PIN_1_PIN))  // 左侧中间（中度左偏）
    {
        pid.actual = -3.0f;
    }
    else if (DL_GPIO_readPins(GPIO_SENSOR_PIN_6_PORT, GPIO_SENSOR_PIN_6_PIN))  // 右侧中间（中度右偏）
    {
        pid.actual = 3.0f;
    }
    else if (DL_GPIO_readPins(GPIO_SENSOR_PIN_2_PORT, GPIO_SENSOR_PIN_2_PIN))  // 左近中间（轻微左偏）
    {
        pid.actual = -2.0f;
    }
    else if (DL_GPIO_readPins(GPIO_SENSOR_PIN_5_PORT, GPIO_SENSOR_PIN_5_PIN))  // 右近中间（轻微右偏）
    {
        pid.actual = 2.0f;
    }
    else if (DL_GPIO_readPins(GPIO_SENSOR_PIN_3_PORT, GPIO_SENSOR_PIN_3_PIN))  // 中间偏左（微左偏）
    {
        pid.actual = -1.0f;
    }
    else if (DL_GPIO_readPins(GPIO_SENSOR_PIN_4_PORT, GPIO_SENSOR_PIN_4_PIN))  // 中间偏右（微右偏）
    {
        pid.actual = 1.0f;
    }
    // 无传感器检测到信号（偏离轨道）
    else 
    {
        // 保持上一次的偏差，或根据需求设置搜索逻辑（如小幅摆动）
        // 此处简单处理：维持当前偏差，LED报警
        DL_GPIO_setPins(GPIO_LED_PORT, GPIO_LED_PIN_LED_PIN);
        return; // 可根据需要调整
    }
    
    // 2. 清除报警灯（有传感器检测到信号时）
    DL_GPIO_clearPins(GPIO_LED_PORT, GPIO_LED_PIN_LED_PIN);
    
    // 3. 调用PID计算输出（得到速度调节量）
    int pid_out = pid_calculate();
    
    // 4. 根据PID输出调整电机速度
    // - 当pid_out为正时：实际偏差为正（左偏），需右转（左电机加速，右电机减速）
    // - 当pid_out为负时：实际偏差为负（右偏），需左转（右电机加速，左电机减速）
    int left_speed = base_speed + pid_out;
    int right_speed = base_speed - pid_out;
    
    // 5. 设置电机速度
    motor_speed(left_speed, right_speed);
}