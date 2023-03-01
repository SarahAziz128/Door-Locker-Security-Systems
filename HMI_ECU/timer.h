/*
 * timer.h
 *
 *  Created on: Nov 3, 2022
 *      Author: Sarah Emil
 */

#ifndef TIMER_H_
#define TIMER_H_


#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"
/*                   Preprocessor Macros declaration           */

/* Timer configurations for timer number(0,1,2) and  channel selection (A,B)*/
#define Timer_number 1
#define Channel_number 0
#define NULL_PTR    ((void*)0)



typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}Timer1_Clock;

typedef enum
{
	Overflow=0,COMPARE=2
	/* the compare =2 to fit the insertion equation of the TIMSK register */
}Timer1_Mode;
typedef enum
{
	DISABLE,ENABLE
}Timer_Interrupt_Mode;

typedef struct
{Timer1_Mode Timer1_OpMode;
Timer1_Clock Timer1prescaler;
uint16 initial_value;
uint16 compare_value; // it will be used in compare mode only/


}Timer1_ConfigType;



/*             Functions Prototypes               */

void Timer1_init(const Timer1_ConfigType * Config_Ptr);
void Timer1_DeInit();
void Timer1_setCallBack(void(*aTimer_ptr)(void));
#endif /* TIMER_H_ */
