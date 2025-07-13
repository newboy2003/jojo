#include "motor.h"



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
    DL_TimerA_setCaptureCompareValue(PWM_0_INST, rightspeed, DL_TIMER_CC_1_INDEX);
    DL_TimerG_setCaptureCompareValue(PWM_1_INST, leftspeed, DL_TIMER_CC_1_INDEX);
}

int err = 0;
// 循迹逻辑（核心：更新actual值并调用PID）
void track(void)
{
    // 1. 根据传感器状态更新实际偏差值actual
    if (DL_GPIO_readPins(GPIO_SENSOR_PIN_0_PORT, GPIO_SENSOR_PIN_0_PIN))  // 最左侧传感器（大幅左偏）
    {
        err = -4.0f;  // 偏差为正：需向右修正
    } 
    else if (DL_GPIO_readPins(GPIO_SENSOR_PIN_7_PORT, GPIO_SENSOR_PIN_7_PIN))  // 最右侧传感器（大幅右偏）
    {   
        err = 4.0f; // 偏差为负：需向左修正
    }
    else if (DL_GPIO_readPins(GPIO_SENSOR_PIN_1_PORT, GPIO_SENSOR_PIN_1_PIN))  // 左侧中间（中度左偏）
    {
        err = -3.0f;
    }
    else if (DL_GPIO_readPins(GPIO_SENSOR_PIN_6_PORT, GPIO_SENSOR_PIN_6_PIN))  // 右侧中间（中度右偏）
    {
        err = 3.0f;
    }
    else if (DL_GPIO_readPins(GPIO_SENSOR_PIN_2_PORT, GPIO_SENSOR_PIN_2_PIN))  // 左近中间（轻微左偏）
    {
        err = -2.0f;
    }
    else if (DL_GPIO_readPins(GPIO_SENSOR_PIN_5_PORT, GPIO_SENSOR_PIN_5_PIN))  // 右近中间（轻微右偏）
    {
        err = 2.0f;
    }
    else if (DL_GPIO_readPins(GPIO_SENSOR_PIN_3_PORT, GPIO_SENSOR_PIN_3_PIN))  // 中间偏左（微左偏）
    {
        err = -1.0f;
    }
    else if (DL_GPIO_readPins(GPIO_SENSOR_PIN_4_PORT, GPIO_SENSOR_PIN_4_PIN))  // 中间偏右（微右偏）
    {
        err = 1.0f;
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
    int pid_out = pid_calculate(err);
    
    // 4. 根据PID输出调整电机速度
    // - 当pid_out为正时：实际偏差为正（左偏），需右转（左电机加速，右电机减速）
    // - 当pid_out为负时：实际偏差为负（右偏），需左转（右电机加速，左电机减速）
    int left_speed = base_speed + pid_out;
    int right_speed = base_speed - pid_out;
    
    // 5. 设置电机速度
    motor_speed(left_speed, right_speed);
}