/**
 * main.c
 */
#include "main.h"

float    ia__A__r32;

Uint16  zeroAmpOffset_U16;
float    currentConst_r32;
float    requestedSpeed__rpm__r32 = 0.0f;

void main(void)
{
    Uint16 motorControlState_U16 = 0;

    mcu_vInitMcuClocks(MCU_PLL_MULTIPLIER, MCU_PLL_DIVIDER,
                       MCU_HIGH_SPEED_PRESCALE, MCU_LOW_SPEED_PRESCALE);
    vInterruptsSetup();
    vPwmInit();
    vADC_Init();
    vGpioInit();
    qep_vInitQEP(QEP_ENC_PPR * QEP_RATIO - 1, QEP_NOSWAP);

    /* Current sensor calibration */
    zeroAmpOffset_U16 = CalZeroAmpOffset_U16();
    currentConst_r32 = I_SENSOR_MAX__A__dr32 / (float)zeroAmpOffset_U16;

    vSetPwmDuty(PWM_ZERO_VOLTS__V__dr32);

    while(1)
    {
        if(STOP_BTN_STATE_dU16)
        {
            APP_StopMotorControl();
            motorControlState_U16 = 0;
        }
        else if(START_BTN_STATE_dU16)
        {
            APP_StartMotorControl();
            motorControlState_U16 = 1;
        }

        if(motorControlState_U16)
        {
            requestedSpeed__rpm__r32 = TARGET_RPM_dMf32;
            if( requestedSpeed__rpm__r32 < MIN_SPEED__rpm__dr32
                && requestedSpeed__rpm__r32 > (-MIN_SPEED__rpm__dr32) )
            {
                APP_StopMotorControl();
            }
            else
            {
                Wreg.setpoint_r32 = requestedSpeed__rpm__r32;
                APP_StartMotorControl();
            }
        }

        DELAY_US(10);
    }
}

void APP_StartMotorControl(void)
{
    EQep1Regs.QEINT.bit.UTO = 1;
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;
    EQep1Regs.QCAPCTL.bit.CEN = 1;
}

void APP_StopMotorControl(void)
{
    EQep1Regs.QEINT.bit.UTO = 0;
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 0;
    EQep1Regs.QCAPCTL.bit.CEN = 0;
    EQep1Regs.QCTMR = 0;
    vSetPwmDuty(PWM_ZERO_VOLTS__V__dr32);
    PIDREG_Reset(&Ireg);
    PIDREG_Reset(&Wreg);
    Ireg.setpoint_r32 = 0.05;
}

Uint16 CalZeroAmpOffset_U16(void)
{
    Uint32 OffsetSum = 0;
    Uint32 index_U32;
    const Uint32 sampleSize = 1000;
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;

    for(index_U32 = 0; index_U32 < sampleSize; index_U32++)
    {
        AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;
        while(AdcRegs.ADCST.bit.INT_SEQ1 == 0) {};
        AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
        OffsetSum += (Uint32)AdcMirror.ADCRESULT0;
        DELAY_US(1000);
    }

    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
    return (Uint16)(OffsetSum / sampleSize);
};

float r32CalcMotorCurrent(const Uint16 u16AdcValue)
{
    return currentConst_r32 * (float)((int16)u16AdcValue - (int16)zeroAmpOffset_U16);
};

float r32CalcDutyFromVoltage(const float r32Voltage)
{
    return ((r32Voltage / 24.0f) + 0.5f);
};

float lowPassFilter(float input, float previousOutput, float timeConstant, float deltaTime)
{
    // Calculate the smoothing factor (alpha) based on the time constant and sampling period
    float alpha = deltaTime / (timeConstant + deltaTime);

    // Calculate the output of the low-pass filter using the exponential smoothing formula
    float output = alpha * input + (1 - alpha) * previousOutput;

    return output;
}
