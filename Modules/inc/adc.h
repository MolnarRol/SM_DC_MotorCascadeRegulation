/*
 * adc.h
 *
 *  ADC module for DC motor control
 *
 *  Created on: Nov 10, 2023
 *      Author: Roland Molnar
 */

#ifndef ADC_H_
#define ADC_H_

#include "types.h"

#define ADC_RESOLUTION                      0x0FFFUL

#define ADC_RESULT_MIRROR_BASE              0x0B00UL
#define ADC_GET_CHANNEL_RESULT(channel)     ( (u16)*((u16*)ADC_RESULT_MIRROR_BASE + channel) )

/* Enable power to ADC module(ADCPWDN = 0x1) and bandgap (ADCBGRFDN = 0b11) */
#define ADC_ENABLE_POWER                    {\
                                            AdcRegs.ADCTRL3.all |= 0x00E0;\
                                            DELAY_US(5000);\
                                           }

void vADC_Init(void);

#define ADC_StartConversion_SEQ1            ( AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1 )
#define ADC_ResetSequence_SEQ1              ( AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1 )

#define ADC_StartConversion_SEQ2            ( AdcRegs.ADCTRL2.bit.SOC_SEQ2 = 1 )
#define ADC_ResetSequence_SEQ2              ( AdcRegs.ADCTRL2.bit.RST_SEQ2 = 1 )

#endif /* ADC_H_ */
