/*
 * PWM.c
 *
 *  Created on: Oct 9, 2022
 *      Author: Sarah Emil
 */

#include "PWM.h"
#include <avr/io.h>



void PWM_Timer0_Start(uint8 duty_cycle)
{
	{
		/*inital value of timer0*/
		TCNT0=0;

		/* compare value of timer0*/
		OCR0 =(duty_cycle*255/100);

		/* set OC0 as output where PWM signal is generated */
		DDRB |=(1<<PB3);

		/*FOC0 =0 to use PWM mode  */
		/*WGM00=0 and WGM01=1 FAST PWM on */
		/*COM00 =0 &COM01 =1  non_inverting mode on*//*COM00 =0 &COM01 =1  non_inverting mode on */
		/* CS02=0 & CS01=1 & CS00 =0 to make prescaler clk/8 */
		TCCR0 = (1<<WGM01) | (1<<WGM00) | (1<<COM01) | (1<<CS01);

	}
}


