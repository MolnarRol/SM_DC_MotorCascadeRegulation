/*
 * qep.c
 *
 *  Created on: Nov 29, 2023
 *      Author: roland
 */

#include "qep.h"

void qep_vInitQEP(Uint16 u16EncPPR, Uint16 u16Swap)
{
    EALLOW;
    SysCtrlRegs.PCLKCR1.bit.EQEP1ENCLK = 1;
    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 1;        /* A */
    GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 1;        /* B */
    EDIS;

    if(u16Swap)
    {
        EQep1Regs.QDECCTL.bit.SWAP = 1;
    }

    EQep1Regs.QEPCTL.bit.FREE_SOFT = 3;
    EQep1Regs.QEPCTL.bit.PCRM = 1;              // reset on maximum
    EQep1Regs.QPOSMAX = u16EncPPR * 4 - 1;
    EQep1Regs.QEPCTL.bit.QCLM = 1;
    EQep1Regs.QEPCTL.bit.QPEN = 1;


    /**/
    EQep1Regs.QUPRD = 1500000;

    EQep1Regs.QCAPCTL.bit.CCPS = 5;
    EQep1Regs.QEPCTL.bit.UTE = 1;
};

float qep_r32GetPosition(void)
{
    float r32Position = 0.0f;
    r32Position = (float)EQep1Regs.QPOSCNT / EQep1Regs.QPOSMAX * 360.0f;
    return r32Position;
}

float deltaT_U32 = 0.0f;
float speed__rpm__r32 = 0.0f;

float qep_r32GetSpeed(void)
{
    float speed_r32 = 0.0f;

    if(EQep1Regs.QEPSTS.bit.COEF == 1)
    {
        EQep1Regs.QEPSTS.bit.COEF = 1;
        speed__rpm__r32 = 0.0f;
        return (0.0f);
    }

    deltaT_U32 = (float)EQep1Regs.QCPRDLAT * 1920.0f * 32.0f / 150.0e6f;
    speed_r32 = 60.0f / deltaT_U32;

    if(EQep1Regs.QEPSTS.bit.QDF == 0)
    {
        speed__rpm__r32 = -speed_r32;
    }
    else
    {
        speed__rpm__r32 = speed_r32;
    }

    return (speed_r32);
}
