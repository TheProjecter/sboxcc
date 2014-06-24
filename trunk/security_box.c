 # define F_CPU 1000000UL

#include<avr/io.h>
#include "C:\Users\Diana\Desktop\LIBRERIAS\Macro_lcd.h"
#include <util/delay.h>
#include <stdlib.h>

int password_value (void);
int temperature (void);
void block_system(void); 

#define sbi(port,bit)  (port |= (1<<bit))   //set bit in port
#define cbi(port,bit)  (port &= ~(1<<bit))  //clear bit in port

int ADC_read(void)
{
	int e;
	ADCSRA |= (1<<ADSC);        // do single conversion
	while(!(ADCSRA & 0x10));    // wait for conversion done, ADIF flag active
	e=(ADCW>>2);
	return e;
}


int main(void)
{
/// Inputs: PB4(center), PB6(up), PB7(down), PE2(left) and PE3(right)
	DDRB	= DDRB & 0b00101111;
	DDRE	= DDRE & 0b00001100;
/// switch on pull up resistors
	PORTB	= PORTB	| 0b11010000;
	PORTE	= PORTE	| 0b00001100; 
  
	LCD_Init();
	//	ADC_init(0b00100000);
	ADCSRA = (1<<ADEN); 
	DDRF=0;

	//ADMUX = 2; ////  con este cambias el canal  por ejemplo para el canal 0 	ADMUX = 0;
	int value[3];
	int a,b,c,error,v, cont;
	error=0;
	a=3;
	b=3;
	c=3;
	double T1, T0, cont1;
	DDRD = 255;
	

while(1)
{
	LCD_TEXT("WELCOME")
	_delay_ms(2000);
	CLEAR_LCD 								// Clears the LCD screen.
	LCD_TEXT("PASSWORD")


	while(PINB & (1 << 7));
	while(!(PINB & (1 << 7)));							
	for (int i=0; i<3; i++)             	// get the 3 password numbers
	{
		while(PINB & (1 << 7))
		{ 
		   	v=password_value ();
			LCD_NUMBER(v) 
		}
		while(!(PINB & (1 << 7)));
		value[i]=password_value ();        	//call the adc
		
	}
	if (value[0]==a && value[1]==b && value[2]==c)       	//Check the password
	{
   		CLEAR_LCD 	
		LCD_TEXT("OK")
		_delay_ms(2000);
		T0=temperature();    
    	sbi(PORTB,0);	        // TUrn ligth on, open door PORTB0
		cont1=cont=0;

		while (PORTB & 0b00000001)           // Check counter
		{
         	if(!(PINE & (1 << 3)))      // Counter people
			{
			   if (!(PINE & (1 << 4)))
			   {
			   	   while(!(PINE & (1 << 3)));
			  	   while(!(PINE & (1 << 4)));
			   	   cont++;
			   }
			   			}
			if(!(PINE & (1 << 4)))      // Counter people
			{
			   if (!(PINE & (1 << 3)))
			   {
			  	   while(!(PINE & (1 << 4)));
			   	   while(!(PINE & (1 << 3)));
			       cont--;
			   }
			}
				
 			if(cont==1)
			{
			
				T1=temperature();
				LCD_TEXT("TEMP")
				_delay_ms(800);
				LCD_NUMBER(T1)
				if(T1>T0)
					sbi(PORTB,1);		//Turn cooler on
				else
					cbi(PORTB,1);		//Turn cooler off
				_delay_ms(800);
			}
			if(cont>1)
			{
			    sbi(PORTB,2);			//Turn alarm off
				LCD_TEXT("ALARM")
				cbi(PORTB,0); 
				_delay_ms(1000);
				block_system();
			}
			if(cont==0)
			{
 				if(cont1<5000)
				{
					cont1++;
				}
				else
				{
				 	cbi(PORTB,0);	        // TUrn ligth off, close door
				}
			}

		}

	}

	else
	{
	
		error++;     						// sum the failed attempts 
		if(error > 2)       					// if it's higher than 3 go to block function
		{
			block_system();     					//call block function
		}
		else
		{
			CLEAR_LCD
			LCD_TEXT("FAIL")
			_delay_ms(1000);
			CLEAR_LCD
		}
	}
}}



int password_value (void)
{	
	int value1=0;										// Declares an int variable
    uint16_t  mv=0;										// Declares a 16 bit unsigned variable
	ADMUX = 1;										// active Voltaje ADC
	mv = ADC_read();     							//The AD conversion is executed and store in the variable mv
	value1= mv / 25; 						// the number is obtained
return value1;									// return the number 
}

void block_system(void)           		    	// block funtion
{
	LCD_TEXT("LOCK")
	while(PINB & (1 << 7));						//wait until int 7 is activated 
	cbi(PORTB,2);		                        //Turn alarm off				
}

int temperature (void)
{	
	float t,r,s;					
	uint16_t T=0;									// Declares a 16 bit unsigned variable
	int x;
	ADMUX = 0;										// active Temperature ADC
    T= ADC_read();									//The AD conversion is executed and store in the variable T
 	T=ADCW;
    s=1024-T;
	t=T/s;
	r=log(t); 
	x=(4250/(r+14.2617))-273;
	return x;									// return the number 
}

