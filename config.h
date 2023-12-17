/*
 * config.h
 *
 *  Created on: Dec 8, 2023
 *      Author: roland
 */

#ifndef CONFIG_H_
#define CONFIG_H_

/* I regulation parameters */
#define I_REG_KR_dr32           ( (float)8.85f )
#define I_REG_KI_dr32           ( (float)11000.0f )
#define I_SENSOR_MAX__A__dr32   ( (float)13.5f )

/* Speed regulation parameters */

/* Application limits */
#define MAX_VOLTAGE__V__dr32    ( (float)12.0f )
#define MAX_CURRENT__A__dr32    ( (float)2.5f )
#define MIN_SPEED__rpm__dr32    ( (float)5.0f )
#define MAX_SPEED__rpm__dr32    ( (float)300.0f )

/* Speed potenciometer max value */
#define MAX_ADC_POT_VAL_dr32    ( (float)3600.0f )

#endif /* CONFIG_H_ */
