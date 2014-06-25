# define F_CPU 1000000UL
#include<avr/io.h>
#include "C:\Users\Diana\Desktop\LIBRERIAS\Macro_lcd.h"
#include <util/delay.h>
#include <stdlib.h>

int password_value (void);
int temperature (void);
void block_system(void); 
int counter(int z);
/**Set bit in port*/
#define sbi(port,bit)  (port |= (1<<bit))
/**Clear bit in port*/
#define cbi(port,bit)  (port &= ~(1<<bit))
/**Function to read the ADC*/
int ADC_read(void)
{
	int e;
    /**Enable the ADC*/
	sbi(ADCSRA, ADEN);
	// To save power, the voltage over the LDR and the NTC is turned off when not used
	// This is done by controlling the voltage from a I/O-pin (PORTF3)
	
	sbi(PORTF, PORTF3);     // Enable the VCP (VC-peripheral)
	sbi(DDRF, PORTF3);
/**Do single conversion*/
	ADCSRA |= (1<<ADSC);        // do single conversion
/**Wait for conversion done*/
	while(!(ADCSRA & 0x10));
	e=(ADCW>>2);
	return e;
}

/** Function Start*/
int main(void)
{
/** Define PortB as an output*/
	DDRB	= DDRB & 0b00101111;
/** Define PortE as an output*/
	DDRE	= DDRE & 0b11101100;


	PORTB	= PORTB	| 0b11010111;
	PORTE	= PORTE	| 0b11101100; 
/**Start LCD*/
	LCD_Init();
	//	ADC_init(0b00100000);
	ADCSRA = (1<<ADEN); 
	DDRF=0;
/**Declares an array of 3 int values*/
	int value[3];
/**Declares int variables*/
	int a,b,c,error,v, cont;
	error=0;
/**First value of the password*/
    a=3;
/**Second value of the password*/
	b=3;
/**Third value of the password*/
	c=3;
/**Declare three double password*/
	double T1, T0, cont1;
	DDRD = 255;
	

while(1)
{
/** Welcome message*/
	LCD_TEXT("WELCOME")
/** Delay*/
	_delay_ms(2000);
/**CLear the LCD*/
    CLEAR_LCD
/**Introduce your password*/
    LCD_TEXT("PASSWORD")
/** Nose */
	while(PINB & (1 << 7));
	while(!(PINB & (1 << 7)));
/**Password data loop */
	for (int i=0; i<3; i++)
	{
		while(PINB & (1 << 7))
		{
/**Call the function pasword_value to obtain the data*/
		   	v=password_value ();
			LCD_NUMBER(v) 
			_delay_ms(200);
		}
		while(!(PINB & (1 << 7)));
/**Store data in value array*/
		value[i]=password_value ();
		
	}
/**Check the password values introduced*/
	if (value[0]==a && value[1]==b && value[2]==c)
	{
   		CLEAR_LCD 	
		LCD_TEXT("OK")
		_delay_ms(2000);
/**Obtain the first temperature value */
		T0=temperature();    
/**Turn the light on, open the door PORTB0 */
        sbi(PORTB,0);
		cont1=cont=0;

		while (PORTB & 0b00000001)
		{
/**Call the function counter */
            cont=counter(cont);
/**Check the counter */
 			if(cont==1)
			{
/**Obtain the actual temperature in the security box */
				T1=temperature();
				LCD_TEXT("TEMP")
				_delay_ms(800);
				LCD_NUMBER(T1)
/**Conditional to control temperature*/
				if(T1>T0)
/**Temperature to high Turn cooler on */
					sbi(PORTB,1);
				else
/**Temperature ok the cooler is off */
					cbi(PORTB,1);
				_delay_ms(800);
				cont1=0;
			}
/**Conditional to activate the alarm*/
			if(cont>1)
			{
/**Turn alarm on */
			    sbi(PORTB,2);
				LCD_TEXT("ALARM")
				cbi(PORTB,0); 
				_delay_ms(1000);
/**Call the function block systen */
				block_system();
			}

		    if(cont<=0)
			{
 				if(cont1<10000)
				{
					cont1++;
				}
				else
				{
					LCD_TEXT("Exit")
					_delay_ms(500);
/**Turn off light and close the door */
				 	cbi(PORTB,0);
				}
			}

		}

	}

	else
	{
/**Sum of failed attempts */
		error++;
        
		if(error > 2)
		{
/**For more than three attempts the system clack the block_system */
			block_system();
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

/**Function to introduce and convert password values */
int password_value (void)
{	
	/**Declares an int variable */
    int value1=0;
    /**Declares a 16 bit unsigned variable */
    uint16_t  mv=0;
	/**Activate voltaje ADC */
    ADMUX = 1;
	mv = ADC_read();
    /**Conversion executed and store in the variable mv*/
	mv=ADCW;
    /**Store and calculate the variable value1 */
	value1= mv/17 ;
return value1;
}
/**Block function*/
void block_system(void)
{
	LCD_TEXT("LOCK")
    /**Wait until 7 is activated*/
	while(PINB & (1 << 7));
    /**Turn alarm off */
	cbi(PORTB,2);
}
/**Function to calculate the temperature*/
int temperature (void)
{
/**Declares a 16 bit unsigned variable*/
	float t,r,s;					
	uint16_t T=0;
	int x;
/**Activate temperature ADC*/
	ADMUX = 0;
/**The AD conversion is executed and store in the variable T */
    T= ADC_read();
 	T=ADCW;
    s=1024-T;
	t=T/s;
	r=log(t);
/**Conversion of temperature*/
	x=(4250/(r+14.2617))-273;
	return x;
}
/**Function to count people */
int counter(int z)
{
/**Count people */
    if(!(PINB & (1 << 7)) && (PINE & (1 << 5)))
	{
	   while(!(PINB & (1 << 7)) && (PINE & (1 << 5)));
/**Counting people in the entrace */
        if (!(PINE & (1 << 5)))
	       z++;
/**Counting people in the exit */
	   if((PINB & (1 << 7)))  
		   z--;
	 }
return z;
}
