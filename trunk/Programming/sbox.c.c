#include<avr/io.h>
#include "easybutterfly.h"
#include <util/delay.h>
int password_value (void);
void block_system(void);
PROGRAM_INIT

ACTIVATE_ADC 								// To switch on the AD converter
ADC_CHANNEL(2)								// Select pin 0 of the ADC multiplexer
ACTIVATE_LCD 								// initialize the LCD. The output begins at the first position of the display.
CLEAR_LCD 									// Clears the LCD screen.
int value[3];
int a,b,c,error;
error=0;
a=1;
b=2;
c=3;
VAR(T1)
VAR(T0)
VAR(i)
PROGRAM_START

	LCD_TEXT("WELCOME")
	WAIT_SEC(30)
	CLEAR_LCD 								// Clears the LCD screen.
	LCD_TEXT("PASS")
	
	for (int i=0; i<3; i++)             	// get the 3 password numbers
	{
		while(PINB & (1 << 4));
		while(!(PINB & (1 << 4)));
		value[i]=password_value ();        	//call the adc
		
	}

	if (value[0]==a && value[1]==b && value[2]==c)       	//Check the password
	{
   		CLEAR_LCD 	
		LCD_TEXT("OK")
		TEMPERATURE(T0)
		SET_BIT(PORTD,0)
		if(!(PINB & (1 << 1)) ) 
	{ 
		while(!(PINB & (1 << 1)))	
		{
		}
		i=i+1;
		
		if(i==1)
		{
			LCD_TEXT("WELCOME!")
			PORTD = PORTD & ~(1 << 0); //port out
			while(1)
			{
			TEMPERATURE(T1)
			LCD_TEXT("TEMP")
			WAIT_SEC(0.8)
			LCD_NUMBER(T1)
			if(T1>T0)
			SET_BIT(PORTC,0)
			else
			CLEAR_BIT(PORTC,0)
			WAIT_SEC(1)
			}

		} 

		else
		{
		LCD_TEXT("ALARM")
		PORTD = PORTD & ~(1 << 0);
		CLEAR_BIT(PORTD,0) 
		}

	
	}
		
		
	}
		


	else 
	{
	
		error=error++;     						// sum the failed attempts 
		if(error > 3)       					// if it's higher than 3 go to block function
		{
		block_system();     					//call block function
		}
		else
		{
		DO_NOTHING;
		}
	}

	CLEAR_LCD 									// Clears the LCD screen.


	


PROGRAM_END



int password_value (void)
{	
int value1;     								// Declares an int variable
VAR16(mv) 
value1 = mv = 0;						 		// Declares a 16 bit unsigned variable
ADCONVERT (mv) 									//The AD conversion is executed and store in the variable mv
value1= (mv -51) / 102; 						// the number is obtained
return value1;									// return the number 
}

void block_system(void)           		    	// block funtion
{
	while(PINB & (1 << 7));						//wait until int 7 is activated 
}




