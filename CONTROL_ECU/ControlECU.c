/*

 * File Name: controlECU.c
 *
 * Created on: Nov 4, 2022
 *
 * Description: Source file for the control ECU that is responsible for all the processing and decisions in the system like password
checking, open the door and activate the system alarm.
 *
 * Author: Sarah Emil
 */
#include <util/delay.h>

/* Include hardware abstraction layer drivers */
#include "MOTOR_DC.h"
#include "buzzer.h"
#include "uart.h"
#include "external_eeprom.h"
#include "twi.h"
#include "timer.h"
#include "std_types.h"
#include "common_macros.h"

#define MC1_READY 6
#define MC2_READY 7

/* MC1 functions */
#define SetNewPasswordFn	9
#define GetOptionsFn		1
#define EnterPasswordFn		2
#define SetFirstPasswordFn	5

/* MC2 functions */
#define ChangePasswordFn	3
#define OpenDoorFn			4


/* Defined for timer number of compares */
#define NUMBER_OF_COMPARE_MTACHES_PER_SECOND 31

/*******************************************************************************
 *                               Functions' prototypes                         *
 *******************************************************************************/

/* Description:
 * Function used for the main menu decisions made by the user.
 *
 * INPUTS:
 * 		uint8 * PassPtr1: pointer to the string where the first entered password is/will be saved
 * 		uint8 * PassPtr2: pointer to the string where the second entered password is/will be saved
 *
 * OUTPUTS:N/A
 */
void UserChoice(uint8 * PassPtr1, uint8 * PassPtr2);

/* Description:
 * Function used for:
 *  Opening the door for 15 seconds
 *  waiting 3 seconds
 *  closing the door for 15 seconds
 *
 * INPUTS:	N/A
 *
 * OUTPUTS:	N/A
 */
void OpenDoor(void);

/* Description:
 * Function used to read the entered password from MC1 and save it in a string.
 *
 * INPUTS:
 * 		uint8 * PassPtr: pointer to the string where the entered password will be saved
 *
 * OUTPUTS:N/A
 */
void ReadEnteredPassword(uint8 * PassPtr);

/* Description:
 * Function used to check if the two entered passwords are equal and if yes save them in the EEPROM
 *
 * INPUTS:
 * 		uint8 * PassPtr1: pointer to the string where the first entered password is saved
 * 		uint8 * PassPtr2: pointer to the string where the second entered password is saved
 *
 * OUTPUTS:N/A
 */

void SavePassword(uint8 * PassPtr1, uint8 * PassPtr2);

/* Description:
 * Function used to read and store the two entered passwords and save them.
 *
 * INPUTS:
 * 		uint8 * PassPtr1: pointer to the string where the first entered password is saved
 * 		uint8 * PassPtr2: pointer to the string where the second entered password is saved
 *
 * OUTPUTS:N/A
 */
void ChangePassword(uint8 * PassPtr1, uint8 * PassPtr2);

/* Description:
 * Function used to compare the two passwords and set the g_PasswordCorrectFlag accordingly and inform MC1 of the decision.
 *
 * INPUTS:
 * 		uint8 * PassPtr1: pointer to the string where the first entered password is saved
 * 		uint8 * PassPtr2: pointer to the string where the second entered password is saved
 *
 * OUTPUTS:N/A
 */
void CheckPassword(uint8 * PassPtr1, uint8 * PassPtr2);

/* Description:
 * Function used for checking the flag in the EEPROM to determine if previous password is saved at first use
 * Used after power cuts to prevent creating a new password

 * INPUTS:
 * 		uint8 * PassPtr1: pointer to the string where the first entered password will be saved
 * 		uint8 * PassPtr2: pointer to the string where the second entered password will be saved
 *
 * OUTPUTS:	N/A
 */
void CheckForPreviouslySavedPassword(uint8 * PassPtr1, uint8 * PassPtr2);

/* Description:
 * Function used for saving the new password in the EEPROM
 *
 * INPUTS:
 * 		uint8 * PassPtr: pointer to the string where the password is saved
 *
 * OUTPUTS:	N/A
 */
void InformMC1(uint8 SelectedFn);

void EEPROMStorePassword(uint8 * PassPtr);

/* Description:
 * Function used for reading the saved password in the EEPROM
 *
 * INPUTS:
 * 		uint8 * PassPtr: pointer to the string where the password will be saved
 *
 * OUTPUTS:	N/A
 */
void EEPROMRetrivePassword(uint8 * PassPtr);

/* Description:
 * Function used for:
 *  Interrupt Service Routine for timer0 compare mode
 *  Set g_FinshedCounting flag to 1 every 1 second
 *
 * INPUTS:	N/A
 *
 * OUTPUTS:	N/A
 */
void TickCounter(void);

/* Description:
 * Function used for:
 *  Initializing Timer0 with the desired values
 *  Loop until reaching the desired number of delayed seconds
 *
 * INPUTS:
 * 		uint8 Seconds: Number of desired seconds to delay
 *
 * OUTPUTS:	N/A
 */
void CountByTimer1(uint8 Seconds);




/*******************************************************************************
 *                               Global Variables                              *
 *******************************************************************************/

//uint8 First_Password_Flag=0;
//EEPROM_readByte( 0x0311 , &First_Password_Flag );

/* global variable contain the ticks count of the timer */
uint8 g_tick = 0;

/* global variable flag to indicate finish of counting desired number of seconds */
uint8 g_FinshedCounting = 0;

/* global variable flag to indicate the state of the password comparison */
uint8 g_PasswordCorrectFlag = 0;

int main(void)
{


	/* Initialize the TWI/I2C Driver with dynamic configuration */
	/*TWI_ConfigType TWI_Structure={P_1,FAST_MODE,0b00000010};*/
	/*TWI_init(&TWI_Structure);*/

	TWI_init();				/* Initialize the TWI/I2C Driver */
	DcMotor_Init();			/* Initialize DC motor driver*/
	Buzzer_init();			/* Initialize buzzer driver*/

	/* Initialize the UART driver with Baud-rate = 9600 bits/sec, 1 stop bit, disabled parity and 8 bit character */
	UART_ConfigType UART_Structure={EIGHT_BIT,DISABLED,ONE_BIT,9600};
	UART_init(&UART_Structure);

	/*Array of characters to store the two received passwords, size is 16 due to LCD limit and 1 place for the null */
	uint8 Password_1[17];
	uint8 Password_2[17];


	while(1)
	{

		/*Receive the chosen function from MC1 */
		uint8 ChosenFunction;
		ChosenFunction = UART_recieveByte();

		switch(ChosenFunction)
		{
		case SetFirstPasswordFn:
			CheckForPreviouslySavedPassword(Password_1, Password_2);
			_delay_ms(500);
			break;

		case SetNewPasswordFn:
			ChangePassword(Password_1, Password_2);
			break;

		case GetOptionsFn:
			UserChoice(Password_1, Password_2);
			break;

		case EnterPasswordFn:
			CheckPassword(Password_1, Password_2);
			break;

		}
	}
}
/********************************************************************************************************/

/* Description:
 * Function used for the main menu decisions made by the user.
 *
 * INPUTS:
 * 		uint8 * PassPtr1: pointer to the string where the first entered password is/will be saved
 * 		uint8 * PassPtr2: pointer to the string where the second entered password is/will be saved
 *
 * OUTPUTS:N/A
 */

void UserChoice(uint8 * PassPtr1, uint8 * PassPtr2)
{
	uint8 choice;
	static uint8 FailureCounter=0;
	choice = UART_recieveByte();
	switch(choice){
	case '+':
		UART_sendByte(OpenDoorFn);	/* Inform MC1 about the selected choice*/
		UART_recieveByte(); //dummy
		CheckPassword(PassPtr1, PassPtr2);
		if(g_PasswordCorrectFlag)
		{
			FailureCounter = 0;
			OpenDoor();
		}
		else
		{
			FailureCounter++;
			if (FailureCounter==3)
			{
				Buzzer_on();
				CountByTimer1(60);
				Buzzer_off();
				FailureCounter = 0;
			}
		}
		break;
	case '-':
		InformMC1(ChangePasswordFn);
		CheckPassword(PassPtr1, PassPtr2);
		if(g_PasswordCorrectFlag)
		{
			FailureCounter = 0;
			UART_recieveByte(); //dummy
			ChangePassword(PassPtr1, PassPtr2);
		}
		else
		{
			FailureCounter++;
			if (FailureCounter==3)
			{
				Buzzer_on();
				CountByTimer1(60);
				Buzzer_off();
				FailureCounter = 0;
			}
		}
		break;
	}
}
/********************************************************************************************************/

/* Description:
 * Function used for:
 *  Opening the door for 15 seconds
 *  waiting 3 seconds
 *  closing the door for 15 seconds
 *
 * INPUTS:	N/A
 *
 * OUTPUTS:	N/A
 */

void OpenDoor(void)
{
	DcMotor_Rotate (CLOCKWISE,100);
	CountByTimer1(15);
	DcMotor_Rotate (STOP,0);
	CountByTimer1(10);
	DcMotor_Rotate (ANTI_CLOCKWISE,100);
	CountByTimer1(15);
	DcMotor_Rotate (STOP,0);
}
/********************************************************************************************************/

/* Description:
 * Function used to read the entered password from MC1 and save it in a string.
 *
 * INPUTS:
 * 		uint8 * PassPtr: pointer to the string where the entered password will be saved
 *
 * OUTPUTS:N/A
 */

void ReadEnteredPassword(uint8 * PassPtr)
{
	uint8 RecievedCharacter, counter=0;

	UART_sendByte(MC2_READY);	/* Send MC2_READY byte to MC1 to ask it to send the string */
	UART_receiveString(PassPtr);
	_delay_ms(1000);

}
/********************************************************************************************************/

/* Description:
 * Function used to check if the two entered passwords are equal and if yes save them in the EEPROM
 *
 * INPUTS:
 * 		uint8 * PassPtr1: pointer to the string where the first entered password is saved
 * 		uint8 * PassPtr2: pointer to the string where the second entered password is saved
 *
 * OUTPUTS:N/A
 */


void SavePassword(uint8 * PassPtr1, uint8 * PassPtr2)
{
	if (!(strcmp(PassPtr1,PassPtr2))){

		UART_sendByte(TRUE);
		EEPROMStorePassword(PassPtr2);

	}
	else{
		UART_sendByte(FALSE);
	}
}
/********************************************************************************************************/

/* Description:
 * Function used to read and store the two entered passwords and save them.
 *
 * INPUTS:
 * 		uint8 * PassPtr1: pointer to the string where the first entered password is saved
 * 		uint8 * PassPtr2: pointer to the string where the second entered password is saved
 *
 * OUTPUTS:N/A
 */

void ChangePassword(uint8 * PassPtr1, uint8 * PassPtr2)
{
	//UART_sendByte(ChangePasswordFn);	/* Inform MC1 about the selected choice*/
	// error function decision byte read as password character ====================================
	ReadEnteredPassword(PassPtr1);
	ReadEnteredPassword(PassPtr2);

	SavePassword(PassPtr1, PassPtr2);

}
/********************************************************************************************************/

/* Description:
 * Function used to compare the two passwords and set the g_PasswordCorrectFlag accordingly and inform MC1 of the decision.
 *
 * INPUTS:
 * 		uint8 * PassPtr1: pointer to the string where the first entered password is saved
 * 		uint8 * PassPtr2: pointer to the string where the second entered password is saved
 *
 * OUTPUTS:N/A
 */

void CheckPassword(uint8 * PassPtr1, uint8 * PassPtr2)
{
	EEPROMRetrivePassword(PassPtr1);
	ReadEnteredPassword(PassPtr2);
	if (!(strcmp(PassPtr1,PassPtr2))){
		UART_sendByte(TRUE);
		g_PasswordCorrectFlag=1;
	}
	else{
		UART_sendByte(FALSE);
		g_PasswordCorrectFlag=0;
	}
}
/********************************************************************************************************/

/* Description:
 * Function used for checking the flag in the EEPROM to determine if previous password is saved at first use
 * Used after power cuts to prevent creating a new password

 * INPUTS:
 * 		uint8 * PassPtr1: pointer to the string where the first entered password will be saved
 * 		uint8 * PassPtr2: pointer to the string where the second entered password will be saved
 *
 * OUTPUTS:	N/A
 */

void CheckForPreviouslySavedPassword(uint8 * PassPtr1, uint8 * PassPtr2)
{
	uint8 FirstSystemPassword_flag;
	EEPROM_readByte( 0x0311, &FirstSystemPassword_flag ); /* Read current character in the external EEPROM*/
	if (FirstSystemPassword_flag==1)
	{
		EEPROMRetrivePassword(PassPtr1);
		UART_sendByte(TRUE);
	}
	else
	{
		EEPROM_writeByte( 0x0311 , 1); /* Write current character in the external EEPROM */
		_delay_ms(10);
		UART_sendByte(FALSE);
		UART_recieveByte(); //dummy
		ChangePassword(PassPtr1, PassPtr2);
	}
}
/********************************************************************************************************/
void InformMC1(uint8 SelectedFn)
{
	UART_sendByte(SelectedFn);
}

/********************************************************************************************************/

/* Description:
 * Function used for saving the new password in the EEPROM
 *
 * INPUTS:
 * 		uint8 * PassPtr: pointer to the string where the password is saved
 *
 * OUTPUTS:	N/A
 */

void EEPROMStorePassword(uint8 * PassPtr)
{
	uint8 Character ,Counter= 0;
	uint16 FirstAddress = 0x0312;
	do
	{
		Character = PassPtr[Counter];
		EEPROM_writeByte( FirstAddress + Counter , Character); /* Write current character in the external EEPROM */
		_delay_ms(10);
		Counter++;
	}
	while(Character != NULL_PTR);
}
/********************************************************************************************************/

/* Description:
 * Function used for reading the saved password in the EEPROM
 *
 * INPUTS:
 * 		uint8 * PassPtr: pointer to the string where the password will be saved
 *
 * OUTPUTS:	N/A
 */

void EEPROMRetrivePassword(uint8 * PassPtr)
{
	uint8 Counter= 0;
	uint16 FirstAddress = 0x0312;
	do
	{
		//_delay_ms(10);
		EEPROM_readByte( FirstAddress + Counter , PassPtr + Counter ); /* Write current character in the external EEPROM */
		Counter++;
	}
	while( PassPtr[Counter-1] != NULL_PTR);
	//PassPtr[Counter] = NULL_PTR;
}
/********************************************************************************************************/

/* Description:
 * Function used for:
 *  Interrupt Service Routine for timer0 compare mode
 *  Set g_FinshedCounting flag to 1 every 1 second
 *
 * INPUTS:	N/A
 *
 * OUTPUTS:	N/A
 */

void TickCounter(void)
{
	g_tick++;
	if(g_tick == NUMBER_OF_COMPARE_MTACHES_PER_SECOND )
	{
		g_FinshedCounting=1;
		g_tick = 0; //clear the tick counter again to count a new 0.5 second
	}
}
/********************************************************************************************************/

/* Description:
 * Function used for:
 *  Initializing Timer0 with the desired values
 *  Loop until reaching the desired number of delayed seconds
 *
 * INPUTS:
 * 		uint8 Seconds: Number of desired seconds to delay
 *
 * OUTPUTS:	N/A
 */
void CountByTimer1(uint8 Seconds)
{
	Timer1_DeInit();
	Timer1_ConfigType Timer1_Structure={COMPARE,F_CPU_1024,0,250};
	Timer1_init(&Timer1_Structure);
	Timer1_setCallBack(TickCounter);
	uint8 SecondsCounter=0;
	while(1)
	{

		if(g_FinshedCounting==1){
			SecondsCounter++;
			g_FinshedCounting=0;
		}

		if(SecondsCounter==Seconds){
			SecondsCounter=0;
			Timer1_DeInit();
			return;
		}
	}
}

