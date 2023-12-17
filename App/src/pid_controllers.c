/*
 * pid_controllers.c
 *
 *  Created on: Dec 8, 2023
 *      Author: roland
 */
#include "pid_controllers.h"
#include "config.h"

PID_Struct Ireg =
{
    .gains_s =
    {
    .Kp_r32 = I_REG_KR_dr32,
    .Ki_r32 = I_REG_KI_dr32
    },
    .limits_s =
    {
    .out_max_r32 = MAX_VOLTAGE__V__dr32,
    .out_min_r32 = -MAX_VOLTAGE__V__dr32
    },
    .reg_period__sec__r32 = 1.0f / 10000.0f,
    .prev_e_r32 = 0.0f,
    .integrator_r32 = 0.0f,
    .prev_integrator_r32 = 0.0f,
    .setpoint_r32 = 0.0f
};

PID_Struct Wreg =
{
    .gains_s =
    {
    .Kp_r32 = 0.05f,
    .Ki_r32 = 1.0f
    },
    .limits_s =
    {
    .out_max_r32 = MAX_CURRENT__A__dr32,
    .out_min_r32 = -MAX_CURRENT__A__dr32
    },
    .reg_period__sec__r32 = 1.0f / 100.0f,
    .prev_e_r32 = 0.0f,
    .integrator_r32 = 0.0f,
    .prev_integrator_r32 = 0.0f,
    .setpoint_r32 = 0.0f
};


