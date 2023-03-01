/*
 * PWM.h
 *
 *  Created on: Oct 9, 2022
 *      Author: Sarah Emil
 */

#ifndef PWM_H_
#define PWM_H_
#include "std_types.h"


/* function that start PWM mode on timer0 on pin OC0(PB3)
 * INPUT :the duty_cycle value
 * OUTPUT : non
 */
void PWM_Timer0_Start(uint8 duty_cycle);


#endif /* PWM_H_ */
