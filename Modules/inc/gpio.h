/*
 * gpio.h
 *
 *  Created on: Nov 18, 2023
 *      Author: roland
 */

#ifndef MODULES_INC_GPIO_H_
#define MODULES_INC_GPIO_H_

#define START_BTN_STATE_dU16    ( GpioDataRegs.GPBDAT.bit.GPIO32 ^ 1 )
#define STOP_BTN_STATE_dU16     ( GpioDataRegs.GPBDAT.bit.GPIO33 ^ 1)

void vGpioInit(void);

#endif /* MODULES_INC_GPIO_H_ */
