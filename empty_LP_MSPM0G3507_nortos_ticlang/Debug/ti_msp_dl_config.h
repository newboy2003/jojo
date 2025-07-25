/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
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

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X
#define CONFIG_MSPM0G3507

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


#define CPUCLK_FREQ                                                     32000000



/* Defines for PWM_0 */
#define PWM_0_INST                                                         TIMG0
#define PWM_0_INST_IRQHandler                                   TIMG0_IRQHandler
#define PWM_0_INST_INT_IRQN                                     (TIMG0_INT_IRQn)
#define PWM_0_INST_CLK_FREQ                                             32000000
/* GPIO defines for channel 1 */
#define GPIO_PWM_0_C1_PORT                                                 GPIOA
#define GPIO_PWM_0_C1_PIN                                         DL_GPIO_PIN_13
#define GPIO_PWM_0_C1_IOMUX                                      (IOMUX_PINCM35)
#define GPIO_PWM_0_C1_IOMUX_FUNC                     IOMUX_PINCM35_PF_TIMG0_CCP1
#define GPIO_PWM_0_C1_IDX                                    DL_TIMER_CC_1_INDEX

/* Defines for PWM_1 */
#define PWM_1_INST                                                         TIMG6
#define PWM_1_INST_IRQHandler                                   TIMG6_IRQHandler
#define PWM_1_INST_INT_IRQN                                     (TIMG6_INT_IRQn)
#define PWM_1_INST_CLK_FREQ                                             32000000
/* GPIO defines for channel 1 */
#define GPIO_PWM_1_C1_PORT                                                 GPIOB
#define GPIO_PWM_1_C1_PIN                                         DL_GPIO_PIN_11
#define GPIO_PWM_1_C1_IOMUX                                      (IOMUX_PINCM28)
#define GPIO_PWM_1_C1_IOMUX_FUNC                     IOMUX_PINCM28_PF_TIMG6_CCP1
#define GPIO_PWM_1_C1_IDX                                    DL_TIMER_CC_1_INDEX




/* Port definition for Pin Group GPIO_LED */
#define GPIO_LED_PORT                                                    (GPIOA)

/* Defines for PIN_LED: GPIOA.0 with pinCMx 1 on package pin 33 */
#define GPIO_LED_PIN_LED_PIN                                     (DL_GPIO_PIN_0)
#define GPIO_LED_PIN_LED_IOMUX                                    (IOMUX_PINCM1)
/* Port definition for Pin Group GPIO_LEFT_MOTOR */
#define GPIO_LEFT_MOTOR_PORT                                             (GPIOA)

/* Defines for LEFT_PIN_0: GPIOA.28 with pinCMx 3 on package pin 35 */
#define GPIO_LEFT_MOTOR_LEFT_PIN_0_PIN                          (DL_GPIO_PIN_28)
#define GPIO_LEFT_MOTOR_LEFT_PIN_0_IOMUX                          (IOMUX_PINCM3)
/* Defines for LEFT_PIN_1: GPIOA.31 with pinCMx 6 on package pin 39 */
#define GPIO_LEFT_MOTOR_LEFT_PIN_1_PIN                          (DL_GPIO_PIN_31)
#define GPIO_LEFT_MOTOR_LEFT_PIN_1_IOMUX                          (IOMUX_PINCM6)
/* Port definition for Pin Group GPIO_RIGHT_MOTOR */
#define GPIO_RIGHT_MOTOR_PORT                                            (GPIOB)

/* Defines for RIGHT_PIN_0: GPIOB.13 with pinCMx 30 on package pin 1 */
#define GPIO_RIGHT_MOTOR_RIGHT_PIN_0_PIN                        (DL_GPIO_PIN_13)
#define GPIO_RIGHT_MOTOR_RIGHT_PIN_0_IOMUX                       (IOMUX_PINCM30)
/* Defines for RIGHT_PIN_1: GPIOB.20 with pinCMx 48 on package pin 19 */
#define GPIO_RIGHT_MOTOR_RIGHT_PIN_1_PIN                        (DL_GPIO_PIN_20)
#define GPIO_RIGHT_MOTOR_RIGHT_PIN_1_IOMUX                       (IOMUX_PINCM48)
/* Defines for PIN_0: GPIOB.3 with pinCMx 16 on package pin 51 */
#define GPIO_SENSOR_PIN_0_PORT                                           (GPIOB)
#define GPIO_SENSOR_PIN_0_PIN                                    (DL_GPIO_PIN_3)
#define GPIO_SENSOR_PIN_0_IOMUX                                  (IOMUX_PINCM16)
/* Defines for PIN_1: GPIOB.2 with pinCMx 15 on package pin 50 */
#define GPIO_SENSOR_PIN_1_PORT                                           (GPIOB)
#define GPIO_SENSOR_PIN_1_PIN                                    (DL_GPIO_PIN_2)
#define GPIO_SENSOR_PIN_1_IOMUX                                  (IOMUX_PINCM15)
/* Defines for PIN_2: GPIOA.27 with pinCMx 60 on package pin 31 */
#define GPIO_SENSOR_PIN_2_PORT                                           (GPIOA)
#define GPIO_SENSOR_PIN_2_PIN                                   (DL_GPIO_PIN_27)
#define GPIO_SENSOR_PIN_2_IOMUX                                  (IOMUX_PINCM60)
/* Defines for PIN_3: GPIOB.9 with pinCMx 26 on package pin 61 */
#define GPIO_SENSOR_PIN_3_PORT                                           (GPIOB)
#define GPIO_SENSOR_PIN_3_PIN                                    (DL_GPIO_PIN_9)
#define GPIO_SENSOR_PIN_3_IOMUX                                  (IOMUX_PINCM26)
/* Defines for PIN_4: GPIOB.24 with pinCMx 52 on package pin 23 */
#define GPIO_SENSOR_PIN_4_PORT                                           (GPIOB)
#define GPIO_SENSOR_PIN_4_PIN                                   (DL_GPIO_PIN_24)
#define GPIO_SENSOR_PIN_4_IOMUX                                  (IOMUX_PINCM52)
/* Defines for PIN_5: GPIOA.26 with pinCMx 59 on package pin 30 */
#define GPIO_SENSOR_PIN_5_PORT                                           (GPIOA)
#define GPIO_SENSOR_PIN_5_PIN                                   (DL_GPIO_PIN_26)
#define GPIO_SENSOR_PIN_5_IOMUX                                  (IOMUX_PINCM59)
/* Defines for PIN_6: GPIOA.8 with pinCMx 19 on package pin 54 */
#define GPIO_SENSOR_PIN_6_PORT                                           (GPIOA)
#define GPIO_SENSOR_PIN_6_PIN                                    (DL_GPIO_PIN_8)
#define GPIO_SENSOR_PIN_6_IOMUX                                  (IOMUX_PINCM19)
/* Defines for PIN_7: GPIOA.25 with pinCMx 55 on package pin 26 */
#define GPIO_SENSOR_PIN_7_PORT                                           (GPIOA)
#define GPIO_SENSOR_PIN_7_PIN                                   (DL_GPIO_PIN_25)
#define GPIO_SENSOR_PIN_7_IOMUX                                  (IOMUX_PINCM55)

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_PWM_0_init(void);
void SYSCFG_DL_PWM_1_init(void);


bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
