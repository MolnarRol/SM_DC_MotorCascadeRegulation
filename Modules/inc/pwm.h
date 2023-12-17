/*
 * pwm.h
 *
 *  Created on: Nov 18, 2023
 *      Author: roland
 */

#ifndef MODULES_INC_PWM_H_
#define MODULES_INC_PWM_H_

#define PWM_ZERO_VOLTS__V__dr32         ( (float)0.5f )

void vPwmInit(void);
void vSetPwmDuty(float r32Duty);


#endif /* MODULES_INC_PWM_H_ */
