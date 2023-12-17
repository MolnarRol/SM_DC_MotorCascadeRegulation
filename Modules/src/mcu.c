/**
 * \file mcu.c
 * \brief Initialization routines for MCU clocks and flash - source file
 * \author Viktor Slapak
 * \date 2023
 * \version 0.9
 * \copyright Only for education and research purposes of
 * Department of Electrical Engineering and Mechatronics
 * at Technical University of Kosice, Slovakia
 */

#include "mcu.h"

// place mcu_vInitFlash into .TI.ramfunc section
#pragma CODE_SECTION(mcu_vInitFlash, ".TI.ramfunc");

extern void ADC_cal(void);

void mcu_vInitMcuClocks(Uint16 u16PLL_Multiplier, Uint16 u16PLL_Divider, Uint16 u16HighSpdPrescale, Uint16 u16LowSpdPrescale) {

    // recalculate divider to proper register value according to datasheet
    if (u16PLL_Divider == 1){
        u16PLL_Divider = 3;
    }
    else if (u16PLL_Divider == 2){
        u16PLL_Divider = 2;
    }
    else {
        u16PLL_Divider = 0;
    }

    // recalculate prescaler to proper register values
    u16HighSpdPrescale /= 2;
    u16LowSpdPrescale /= 2;

    // Disable watchdog
    EALLOW;
    SysCtrlRegs.WDCR= 0x0068;
    EDIS;

/*--------------- PLL Settings -----------------------------*/
/* TMS320x2833x Technical reference manual, Texas Instruments
 * Lit. No.: SPRUI07 from March 2020
 * page: 71
 */

    // Make sure the PLL is not running in limp mode
   if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 0)
   {
      asm("        ESTOP0");
   }

   // set PLLSTS[DIVSEL] to 0
   if (SysCtrlRegs.PLLSTS.bit.DIVSEL != 0)
   {
       EALLOW;
       SysCtrlRegs.PLLSTS.bit.DIVSEL = 0;
       EDIS;
   }

   EALLOW;

   // Disable failed oscillator detect logic
   SysCtrlRegs.PLLSTS.bit.MCLKOFF = 1;

   // Set new PLLCR value
   SysCtrlRegs.PLLCR.bit.DIV = u16PLL_Multiplier;

   // Wait for the PLL to lock.
   while(SysCtrlRegs.PLLSTS.bit.PLLLOCKS != 1);

   // Enable failed oscillator detect logic
   SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;

   EDIS;


   if (SysCtrlRegs.PLLSTS.bit.DIVSEL != u16PLL_Divider){
       EALLOW;
       SysCtrlRegs.PLLSTS.bit.DIVSEL = u16PLL_Divider;
       EDIS;
   }


/* ------------------- ADC calibration -----------------*/
/*   Only for debug purposes, otherwise it's automatic  */

    EALLOW;
    // turn on ADC clocks
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;
    // call TI calibration routine
    ADC_cal();
    // turn off ADC clocks
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 0;
    EDIS;


/*------------- Peripheral clock prescales -------------*/

   EALLOW;
   if (u16HighSpdPrescale > 7) u16HighSpdPrescale = 7; // limit to max value
   SysCtrlRegs.HISPCP.bit.HSPCLK = u16HighSpdPrescale;
   if (u16LowSpdPrescale > 7) u16LowSpdPrescale = 7; // limit to max value
   SysCtrlRegs.LOSPCP.bit.LSPCLK = u16LowSpdPrescale;
   EDIS;

}

void mcu_vInitFlash(Uint16 u16MCUFreq) {
    EALLOW;
    // enable Flash Pipeline Mode
    // from now, wait states must be greater than zero
    FlashRegs.FOPT.bit.ENPIPE = 1;

    /* set wait-states according to tms320F2833x datasheet
    * Lit. No.: SPRS439P from June 2007, rev. February 2021
    * page 95
    */
    if (u16MCUFreq <= 100){ // for 100 MHz
       FlashRegs.FBANKWAIT.bit.PAGEWAIT = 3;
       FlashRegs.FBANKWAIT.bit.RANDWAIT = 3;
       FlashRegs.FOTPWAIT.bit.OTPWAIT = 5;
    }
    else{ // for > 100 MHz
       FlashRegs.FBANKWAIT.bit.PAGEWAIT = 5;
       FlashRegs.FBANKWAIT.bit.RANDWAIT = 5;
       FlashRegs.FOTPWAIT.bit.OTPWAIT = 8;
    }


   EDIS;

   // wait at least 8  NOPs
   asm(" RPT #7 || NOP");
}
