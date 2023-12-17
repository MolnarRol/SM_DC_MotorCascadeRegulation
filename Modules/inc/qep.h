/*
 * qep.h
 *
 *  Created on: Nov 29, 2023
 *      Author: roland
 */

#ifndef MODULES_INC_QEP_H_
#define MODULES_INC_QEP_H_
#include "mcu.h"

#define QEP_ENC_PPR 16
#define QEP_RATIO   30

#define QEP_NOSWAP  0

void qep_vInitQEP(Uint16 u15EncPPR, Uint16 u16Swap);
float qep_r32GetPosition(void);
float qep_r32GetSpeed(void);

#endif /* MODULES_INC_QEP_H_ */
