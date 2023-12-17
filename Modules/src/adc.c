/*
 * HAL_DSP_2833x_ADC.c
 *
 *  Created on: Nov 10, 2023
 *      Author: Roland Molnar
 */
#include <adc.h>
#include "mcu.h"

void vADC_Init(void)
{
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;       /* Enable ADC clocks. */
    SysCtrlRegs.HISPCP.bit.HSPCLK = 3;          /* Set clock prescaler -> ADC_CLK = 25 Mhz*/
    EDIS;

    AdcRegs.ADCTRL1.bit.ACQ_PS = 15;            /* Set acquisition window. */

    AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = 0;       /* Number of conversions. */
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0;        /* A0(current sensor) + B0(potentiometer) */
    AdcRegs.ADCTRL3.bit.SMODE_SEL = 1;          /* Simultaneous conversion mode. */

    AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;     /* Set StartOfConvetion pulse on TBCNT = TBPRD. */
    ADC_ENABLE_POWER;                           /* Enable power to ADC converter. */
};
