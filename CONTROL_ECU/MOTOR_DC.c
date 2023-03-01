/*
 * MOTOR_DC.c
 *
 *  Created on: Oct 9, 2022
 *      Author: Sarah Emil
 */
#include "common_macros.h"
#include "MOTOR_DC.h"
#include "std_types.h"

/* intialize the DC motor:
 *1- Setup the input pins of motor L293D H-bridge directions using the GPIO driver.
 *2-Stop at the DC-Motor at the beginning through the GPIO driver
 */
void DcMotor_Init(void)
{
	GPIO_setupPinDirection(Motor_Direction_Port_ID,Motor_Direction_PIN1_ID ,PIN_OUTPUT);
	GPIO_setupPinDirection(Motor_Direction_Port_ID, Motor_Direction_PIN2_ID,PIN_OUTPUT);
	GPIO_writePin(Motor_Direction_Port_ID,Motor_Direction_PIN1_ID , LOGIC_LOW);
	GPIO_writePin(Motor_Direction_Port_ID,Motor_Direction_PIN2_ID , LOGIC_LOW);
#if (PWM_NEEDED == 0)
	GPIO_setupPinDirection(Motor_Direction_Port_ID,Motor_Enable_PIN_ID ,PIN_OUTPUT);
	GPIO_writePin(Motor_Direction_Port_ID,Motor_Enable_PIN_ID , LOGIC_LOW);
#endif
}

/*
 The function responsible for rotate the DC Motor CW/ or A-CW or
stop the motor based on the state input state value
call PWM_Timer0_Start() function to control of motor speed
 input : DC MOTOR state & speed of motor
 output :non
 */
void DcMotor_Rotate(DcMotor_State state,uint8 speed)
{
	switch(state)
	{
	default:
	case STOP :
		GPIO_writePin(Motor_Direction_Port_ID,Motor_Direction_PIN1_ID , LOGIC_LOW);
		GPIO_writePin(Motor_Direction_Port_ID,Motor_Direction_PIN2_ID , LOGIC_LOW);
		GPIO_writePin(Motor_Direction_Port_ID,Motor_Enable_PIN_ID , LOGIC_LOW);
		break;
	case CLOCKWISE:
		GPIO_writePin(Motor_Direction_Port_ID,Motor_Direction_PIN1_ID , LOGIC_LOW);
		GPIO_writePin(Motor_Direction_Port_ID,Motor_Direction_PIN2_ID , LOGIC_LOW);
#if (PWM_NEEDED == 0)
		GPIO_writePin(Motor_Direction_PORT_ID, Motor_Enable_PIN_ID	, LOGIC_HIGH);
#else
		PWM_Timer0_Start(speed);
#endif
		break;
	case ANTI_CLOCKWISE:
		GPIO_writePin(Motor_Direction_Port_ID,Motor_Direction_PIN1_ID , LOGIC_LOW);
		GPIO_writePin(Motor_Direction_Port_ID,Motor_Direction_PIN2_ID , LOGIC_LOW);
#if (PWM_NEEDED == 0)
		GPIO_writePin(Motor_Direction_PORT_ID, Motor_Enable_PIN_ID	, LOGIC_HIGH);
#else
		PWM_Timer0_Start(speed);
#endif
		break;
	}
}


