/*
 * MOTOR_DC.h
 *
 *  Created on: Oct 9, 2022
 *      Author: Sarah Emil
 */

#ifndef MOTOR_DC_H_
#define MOTOR_DC_H_
#include "gpio.h"
#include "PWM.h"

#define Motor_Direction_Port_ID   PORTB_ID
#define Motor_Direction_PIN1_ID   PIN0_ID
#define Motor_Direction_PIN2_ID   PIN1_ID
#define Motor_Enable_PIN_ID       PIN3_ID
#define PWM_NEEDED                1
typedef enum
{
	STOP,CLOCKWISE,ANTI_CLOCKWISE
}DcMotor_State;


/* intialize the DC motor:
 *1- Setup the input pins of motor L293D H-bridge directions using the GPIO driver.
 *2-Stop at the DC-Motor at the beginning through the GPIO driver
 */
void DcMotor_Init(void);
/*
 The function responsible for rotate the DC Motor CW/ or A-CW or
stop the motor based on the state input state value
call PWM_Timer0_Start() function to control of motor speed
 input : DC MOTOR state & speed of motor
 output :non
 */


void DcMotor_Rotate(DcMotor_State state,uint8 speed);


#endif /* MOTOR_DC_H_ */
