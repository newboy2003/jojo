#include "PID.h"

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
int pid_calculate(float actual)
{
    pid.actual = actual;
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

