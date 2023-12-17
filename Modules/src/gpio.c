/*
 * gpio.c
 *
 *  Created on: Nov 18, 2023
 *      Author: roland
 */
#include "gpio.h"
#include "mcu.h"

void vGpioInit(void)
{
    EALLOW;
    SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO2 = 1;


    GpioCtrlRegs.GPBDIR.bit.GPIO32 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO32 = 0;
    EDIS;
}

