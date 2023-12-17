/*
 * pwm.c
 *
 *  Created on: Nov 18, 2023
 *      Author: roland
 */
#include "pwm.h"
#include "mcu.h"

void vPwmInit(void)
{
    EALLOW;
    SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1;         /* Enable clocks for ePWM1 module. */
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;             /* EPWM1A(0). */
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;             /* EPWM1B(0). */
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;

    EPwm1Regs.TBCTL.bit.FREE_SOFT = 3;              /* Free-run. */
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0;
    EPwm1Regs.TBCTL.bit.CLKDIV = 2;
    EPwm1Regs.TBPRD = (Uint16)1874;
    EPwm1Regs.TBCTL.bit.CTRMODE = 2;                /* Up-down count. */

    EPwm1Regs.CMPA.half.CMPA = (Uint16)0;
    EPwm1Regs.CMPB = (Uint16)0;

    /* Action qualifier. */
    EPwm1Regs.AQCTLA.bit.CAU = 2;
    EPwm1Regs.AQCTLA.bit.CAD = 1;

    EPwm1Regs.AQCTLB.bit.CBU = 2;
    EPwm1Regs.AQCTLB.bit.CBD = 1;

    EPwm1Regs.ETSEL.bit.SOCASEL = 2;
    EPwm1Regs.ETPS.bit.SOCAPRD = 1;
    EPwm1Regs.ETSEL.bit.SOCAEN = 1;

    EPwm1Regs.TBCTL.bit.SWFSYNC = 1;
}

void vSetPwmDuty(float r32Duty)
{
    if(r32Duty < 0.0f || r32Duty > 1.0f)
    {
        return;
    }

    if(r32Duty < 0.5f)
    {
        EPwm1Regs.CMPA.half.CMPA = (Uint16)0;
        EPwm1Regs.CMPB = (Uint16)((1.0f - 2.0f * r32Duty) * (float)EPwm1Regs.TBPRD );
    }
    else
    {
        EPwm1Regs.CMPA.half.CMPA = (Uint16)((2.0f * (r32Duty - 0.5f)) * (float)EPwm1Regs.TBPRD );;
        EPwm1Regs.CMPB = (Uint16)0;
    }
}
