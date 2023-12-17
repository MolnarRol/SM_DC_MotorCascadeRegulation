/**
 * \file mcu.h
 * \brief Initialization routines for MCU clocks and flash
 * \author Viktor Slapak
 * \date 2023
 * \version 0.9
 * \copyright Only for education and research purposes of
 * Department of Electrical Engineering and Mechatronics
 * at Technical University of Kosice, Slovakia
 */

#ifndef MCU_H_
#define MCU_H_

#include <DSP2833x_Device.h>

// crystal settings
#define MCU_CRYSTAL_FREQ_MHZ 30 //!< Crystal frequency

// PLL settings
// final cpu freq = crystal * multiplier/divider
#define MCU_PLL_MULTIPLIER 10 //!< PLL multiplier
#define MCU_PLL_DIVIDER 2 //!< PLL divider

// clock dividers
#define MCU_HIGH_SPEED_PRESCALE 1 //!< High speed clock prescale, integer from 1 to 14
#define MCU_LOW_SPEED_PRESCALE 2 //!< Low speed clock prescale, integer from 1 to 14

// frequency constants
#define MCU_FREQ_MHZ (MCU_CRYSTAL_FREQ_MHZ*MCU_PLL_MULTIPLIER/MCU_PLL_DIVIDER) //!< SYSCLOCK frequency in MHz
#define MCU_FREQ_HSPCLK_MHZ (MCU_FREQ_MHZ/(2*MCU_HIGH_SPEED_PRESCALE)) //!< High speed clock frequency in MHz
#define MCU_FREQ_LSPCLK_MHZ (MCU_FREQ_MHZ/(2*MCU_LOW_SPEED_PRESCALE)) //!< Low speed clock frequency in MHz

// select memory
#define _RAM //!< _RAM for program in RAM, _FLASH for program in FLASH

#ifdef _FLASH

extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
extern Uint16 RamfuncsLoadSize;

#endif

// macro for simple delay in microseconds
#define MCU_CPU_RATE_NS (1000.0/MCU_FREQ_MHZ)
#define DELAY_US(A) DSP28x_usDelay(((((long double) A * 1000.0L) / (long double)MCU_CPU_RATE_NS) - 9.0L) / 5.0L) //!< macro for delay function, param A is the time in microseconds
void DSP28x_usDelay(unsigned long int Count);


/**
 * \brief Initialization of MCU system clocks
 *
 * The function initializes the MCU system clocks
 * by PLL setting. Function does not have parameters
 * to ensure, that the correct MCU clock will be set
 * in all functions.
 *
 * \note This function should be call as soon as possible
 * to ensure the proper clocking, before starting anything else
 *
 * \param u16PLL_Multiplier Requested PLL multiplier of crystal frequency
 * \param u16PLL_Divider Requested PLL divider
 * \param u16HighSpdPrescale Requested prescaler for high speed clocks
 * \param u16LowSpdPrescale Requested prescaler for low speed clocks
 *
 * \note Recommended usage is with macros defined above,
 * e.g. mcu_vInitMcuClocks(MCU_PLL_MULTIPLIER, MCU_PLL_DIVIDER, MCU_HIGH_SPEED_PRESCALE, MCU_LOW_SPEED_PRESCALE);
 */
void mcu_vInitMcuClocks(Uint16 u16PLL_Multiplier, Uint16 u16PLL_Divider, Uint16 u16HighSpdPrescale, Uint16 u16LowSpdPrescale);

/**
 * \brief Initialization of FLASH
 *
 * The function enables the pipeline and sets the correct value of wait-states.
 *
 * \warning Function must be called from RAM!
 *
 * \param u16MCUFreq CPU frequency in MHz
 *
 * \note Recommended usage is with macro defined above,
 * e.g. mcu_vInitFlash(MCU_FREQ_MHZ);
 */
void mcu_vInitFlash(Uint16 u16MCUFreq);

#endif /* MCU_H_ */
