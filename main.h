#include <types.h>
#include <Modules/inc/adc.h>
#include <Modules/inc/mcu.h>
#include <Modules/inc/gpio.h>
#include <Modules/inc/pwm.h>
#include <Modules/inc/interrupts.h>
#include <Modules/inc/qep.h>
#include "pidreg.h"
#include "config.h"
#include "pid_controllers.h"

#define POT_CENTERRED_POSITION_dMf32    ( 2 * ((                                                \
                                         (MAX_ADC_POT_VAL_dr32 - (float)AdcMirror.ADCRESULT1)   \
                                         / MAX_ADC_POT_VAL_dr32)                               \
                                        - 0.5f) )
#define TARGET_RPM_dMf32                ( MAX_SPEED__rpm__dr32 * (POT_CENTERRED_POSITION_dMf32) )

void APP_StartMotorControl(void);
void APP_StopMotorControl(void);
Uint16 CalZeroAmpOffset_U16(void);

float r32CalcMotorCurrent(const Uint16 u16AdcValue);
float r32CalcDutyFromVoltage(const float r32Voltage);
float lowPassFilter(float input, float previousOutput, float timeConstant, float deltaTime);
