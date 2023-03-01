/*
 * timer.c
 *
 *  Created on: Nov 3 , 2022
 *      Author: Sarah Emil
 */

#include "timer.h"

/*             declaration of variables    */

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackTimerPtr)(void) = NULL_PTR;






ISR(TIMER1_OVF_vect)
{
	if(g_callBackTimerPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackTimerPtr)();
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(g_callBackTimerPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackTimerPtr)();
	}
}

void Timer1_init(const Timer1_ConfigType *Config_Ptr) {

	TCNT1=Config_Ptr->initial_value;
		switch (Config_Ptr -> Timer1_OpMode){
			case Overflow:
				SREG  |= (1<<7);           // Enable global interrupts in MC.
				/*Overflow Interrupt Enable*/
				TIMSK |=(1<<TOIE1);
				/*Enable Force Output Compare*/
				TCCR1A=(1<<FOC1A);
				TCCR1A=(1<<FOC1B);
				TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->Timer1prescaler);

				break;

			case COMPARE:
				SREG  |= (1<<7);           // Enable global interrupts in MC.
				/*Compare Interrupt Enable*/
				TIMSK|=(1<<OCIE1A);
				/*Enable Force Output Compare*/
				TCCR1A|=(1<<FOC1A);
				TCCR1A&=~(1<<FOC1B);
				/*Set Compare Value*/
				OCR1A = Config_Ptr ->compare_value;
				TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->Timer1prescaler);
				TCCR1B = (1<<WGM12);
				break;

			}
	}



/*
 * Description: Function to disable the Timer1
 */
void Timer1_DeInit(void)
{
	/* Clear All Timer1 Registers */
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	OCR1A = 0 ;
	OCR1B = 0 ;


}
/*
 * Description: Function to set the Call Back function address.
 */
void Timer1_setCallBack( void(*aTimer_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackTimerPtr = aTimer_ptr;
}
