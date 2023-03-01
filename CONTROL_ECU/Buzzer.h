/*
 * Buzzer.h
 *
 *  Created on: Nov 4, 2022
 *      Author: Sarah Emil
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"
#include "gpio.h"

/*   Buzzer configuration  */
#define Buzzer_port PORTC_ID
#define Buzzer_pin  2
/*     function prototype declaration    */

/*function Buzzer_init
 * definition: function to declare the buzzer configuration
 */
void Buzzer_init();

/*function Buzzer_on
 * definition: function to turn on buzzer
 */
void Buzzer_on(void);

/*function Buzzer_off
 * definition: function to turn off buzzer
 */
void Buzzer_off(void);


#endif /* BUZZER_H_ */
