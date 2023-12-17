/*
 * pidreg.h
 *
 *  Created on: Nov 9, 2023
 *      Author: Roland Molnar
 */

#ifndef APP_INC_PIDREG_H_
#define APP_INC_PIDREG_H_

#include "types.h"

/* PID regulator structure. */
typedef struct {
    struct {
        r32     Kp_r32;                     /**< Proportional gain. */
        r32     Ki_r32;                     /**< Integral gain. */
        r32     Kd_r32;                     /**< Derivative gain. */
    } gains_s;

    struct {
        r32     out_max_r32;                /**< Output upper limit. */
        r32     out_min_r32;                /**< Upper lower limit. */
    } limits_s;

    r32     reg_period__sec__r32;           /**< Regulation period. [s] */
    r32     setpoint_r32;

    r32     prev_e_r32;
    r32     integrator_r32;
    r32     prev_integrator_r32;

} PID_Struct;

float PIDREG_CalculateOutput(PID_Struct* reg_s, r32 y_r32);
void PIDREG_Reset(PID_Struct* reg_s);

#endif /* APP_INC_PIDREG_H_ */
