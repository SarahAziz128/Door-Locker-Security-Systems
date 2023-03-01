/*
 * Buzzer.c
 *
 *  Created on: Nov 4, 2022
 *      Author: Sarah Emil
 */


#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"
#include "gpio.h"
#include "Buzzer.h"



void Buzzer_init()
{
	GPIO_setupPinDirection(Buzzer_port,Buzzer_pin,PIN_OUTPUT);    // Configure pin 2 in PORTD as OUTPUT
	void Buzzer_off(void);

}
void Buzzer_on(void){
	GPIO_writePin(Buzzer_port,Buzzer_pin,HIGH); 	 // turn on buzzer
}
void Buzzer_off(void){
	GPIO_writePin(Buzzer_port,Buzzer_pin,LOW);      // turn off buzzer
}


