/*
 * pidreg.c
 *
 *  Created on: Nov 9, 2023
 *      @author Roland Molnar
 */
#include "pidreg.h"

/*
 * @brief   Regulator output update function.
 * @param   reg_s   Updated regulator.
 * @param   y_r32   System output variable.
 * @reval   Regulation action.
 * @warning Should be called periodically with period of PID_Struct - reg_period__sec__r32
 * */
float PIDREG_CalculateOutput(PID_Struct* reg_s, r32 y_r32)
{
    /* Error term:
     *  E[n] = U - Y[n]
     * */
    const r32 E_r32 = reg_s->setpoint_r32 - y_r32;

    /* Proportional term:
     *  P[n] = Kp * E[n] *
     * */
    const r32 P_r32 = E_r32 * reg_s->gains_s.Kp_r32;

    /* Integral term:
     *  I[n] = (Ki * T) * (E[n] + E[n-1] + I[n-1]) / 2
     * */
//    reg_s->integrator_r32 = E_r32 + reg_s->prev_e_r32;
    reg_s->integrator_r32 += E_r32;
    const r32 I_r32 = (reg_s->integrator_r32)
                            * reg_s->gains_s.Ki_r32
                            * (reg_s->reg_period__sec__r32 / (r32)2.0f);
    /*
     * Derivative term
     * D[n] = ...
     * */
    const r32 D_r32 = (r32)0.0f;

    /* Regulator output (without limiting). */
    r32 U_r32 = P_r32 + I_r32 + D_r32;

    /* Clamping regulator output. */
    if(U_r32 > reg_s->limits_s.out_max_r32)
    {
        U_r32 = reg_s->limits_s.out_max_r32;
        reg_s->integrator_r32 = reg_s->prev_integrator_r32;
    }
    else if(U_r32 < reg_s->limits_s.out_min_r32)
    {
        U_r32 = reg_s->limits_s.out_min_r32;
        reg_s->integrator_r32 = reg_s->prev_integrator_r32;
    }

    reg_s->prev_integrator_r32 = reg_s->integrator_r32;
    reg_s->prev_e_r32 = E_r32;
    return (U_r32);
};

/*
 * @brief   Regulation reset function.
 * @param   reg_s   Reseted regulator.
 * */
void PIDREG_Reset(PID_Struct* reg_s)
{
    reg_s->integrator_r32       = (r32)0.0f;
    reg_s->prev_integrator_r32  = (r32)0.0f;
    reg_s->prev_e_r32           = (r32)0.0f;
};

