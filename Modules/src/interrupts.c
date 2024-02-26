/*
 * interrupts.c
 *
 *  Created on: Nov 21, 2023
 *      Author: roland
 */
#include "interrupts.h"
#include "main.h"

int x;

extern float ia__A__r32;
extern PID_Struct Ireg;
extern PID_Struct Wreg;
extern float speed__rpm__r32;

float u;
float zu;
float i_filtered = 0.0f;
float T = 100.0e-6f;


float rpm_filtered = 0.0f;
float prev_rpm_filtered = 0.0f;
float T_rpm = 10.0e-3f;
float prev_ia = 0.0f;

interrupt void vAdcSeq1IntHandler(void)
{
    ia__A__r32 = -r32CalcMotorCurrent(AdcMirror.ADCRESULT0);
    i_filtered = lowPassFilter(ia__A__r32, prev_ia, T, (1.0f/10000.0f));
    prev_ia = ia__A__r32;

    u = PIDREG_CalculateOutput(&Ireg, i_filtered);
    zu = r32CalcDutyFromVoltage(u);
    vSetPwmDuty(zu);

    /* Clear interrupt flags. */
    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
    PieCtrlRegs.PIEACK.bit.ACK1 = 1;
}

interrupt void vQepTimerOverflowIntHandler(void)
{
    qep_r32GetSpeed();
    Ireg.setpoint_r32 = PIDREG_CalculateOutput(&Wreg, speed__rpm__r32);
    /* Clear interrupt flags. */
    EQep1Regs.QCLR.bit.UTO = 1;
    EQep1Regs.QCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.bit.ACK5 = 1;
}

void vInterruptsSetup(void)
{
    DINT;
    IFR = 0x0000;
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;
    PieCtrlRegs.PIEIER5.bit.INTx1 = 1;
    IER |= (1 << 0) | (1 << 4);

    EALLOW;
    PieVectTable.SEQ1INT = &vAdcSeq1IntHandler;
    PieVectTable.EQEP1_INT = &vQepTimerOverflowIntHandler;
    EDIS;
    EINT;
}
