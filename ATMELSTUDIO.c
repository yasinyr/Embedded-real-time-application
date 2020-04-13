//*********************************************************************************************************************
// Project 		LCD - Simple demonstration using custom LCD library (Embedded C)
// Target 		ATMEL ATmega2560 micro-controller and 1602A LCD (as supplied in the HBV sensors kit)
// Program		LCD_1602A_FixedMessage_PortsAG_2560_C.c
// Author		Yasin Yari
// Date			16th November 2019

// Function		Displays the entire character set on the upper row of the LCD
//				Displays a fixed string on the lower row of the LCD

// 	Ports		Uses port A for Data and Command values (8 bits, Output)
//				The LCD 'device Busy' flag also uses bit 7 of port A so sometimes this bit has to be set for Input

//				Uses port G for control		Register Select		Data mode(H)/Command mode(L)		Port G bit 0 Output
//											Data direction		Read(H)/Write(L)					Port G bit 1 Output
//											Enable				Pulse high while read/write occurs	Port G bit 2 Output
//*********************************************************************************************************************

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "LCD_LibraryFunctions_2560.h"
#include <stdlib.h>

uint8_t timerFlag = 0;
void InitialiseGeneral();
unsigned char temp;
unsigned char ch;
unsigned int  in;
 int16_t count_a = 0; 
 static volatile int pulse = 0;
 static volatile char i = '0';
 
 
 //void InitialiseGeneral();
 void InitialiseTimer3();
 void InitialiseGeneral();
 void InitialiseTimer4();
 //unsigned char ElapsedSeconds_Count;	
// ElapsedSeconds_Count = 0x00;
int main( void )
{
	
	unsigned char ch;
	
	DDRB = 0xff;
	DDRD= 0x00;
	PORTB = 0x00;
	PIND = 0x00;
	
	
	
	
	InitialiseGeneral();
	

	
	// Simple demonstration of LCD configuration and data output
	// *********************************************************
	unsigned char cData = 'A';
	int iCharacterPositionCount = 0;
	LCD_ShiftDisplay(false /*true = ON false = OFF*/, true /*true = shift right, false = shift left*/);
	LCD_Display_ON_OFF(true /*Display ON*/, false /*Cursor OFF*/, false /*Cursor Position OFF*/);
	LCD_Clear();
	LCD_SetCursorPosition(0 /*iColumnPosition 0 - 40 */, 0 /*iRowPosition 0 for top row, 1 for bottom row*/);
	LCD_WriteString("sdd");
	// Display the entire character set in sequence, on the top row of the LCD
	LCD_Home();
	// Display a fixed message on the bottom row of the LCD
	InitialiseTimer3();
	InitialiseTimer4();

		

	
	
	while(1)
    {		
		    i = 0;
			int j=0;
			while ((PIND & 1<<PD0))
			{	
				i++;
				//PORTB|= (1<<PB5);
				
			}
			
			if(i>0)
			{
				//	PORTB |= (1<<PB5);	//buzzer beep
					i=i/3;
					if (i<5)
						{PORTB|=(1<<PB5);     //Buzzer on
						 
					else
						PORTB&=~(1<<PB5);
						
					char i1=i%10+48;
					i=i/10;
					char i10=i%10+48;
					i=i/10;
					char i100=i%10+48;
					char dist[3]={i1,i10,i100};
					for (j=0;j<3;j++)
					{
						LCD_SetCursorPosition(j /*iColumnPosition 0 - 40 */, 1 /*iRowPosition 0 for top row, 1 for bottom row*/);
						LCD_WriteChar(dist[2-j] );
						LCD_SetCursorPosition(4 /*iColumnPosition 0 - 40 */, 1 /*iRowPosition 0 for top row, 1 for bottom row*/);
						LCD_WriteString("CMm");
						
						
					}
					
			}
			else
			{
			//	PORTB &= ~(1<<PB5);
			//	LCD_SetCursorPosition(0 /*iColumnPosition 0 - 40 */, 1 /*iRowPosition 0 for top row, 1 for bottom row*/);
			//	LCD_WriteString("4TriggerInterrupt!");
			}
			
		
		
		
		
	// start timer

	
	}



}


void InitialiseTimer3()		// Configure to generate an interrupt after a 0.5 Second interval
{
	TCCR3A = 0b00000000;	// Normal port operation (OC1A, OC1B, OC1C), Clear Timer on 'Compare Match' (CTC) waveform mode)
	TCCR3B = 0b00001101;	// CTC waveform mode, use prescaler 1024
	TCCR3C = 0b00000000;
	
	// For 1 MHz clock (with 1024 prescaler) to achieve a 0.5 second interval:
	// Need to count 500,000 clock cycles (but already divided by 1024)
	// So actually need to count to (500000 / 1024 =) 488 decimal, = 1E8 Hex
	OCR3AH = 0x00; // Output Compare Registers (16 bit) OCR1BH and OCR1BL
	OCR3AL = 0x7A;

	TCNT3H = 0b00000000;	// Timer/Counter count/value registers (16 bit) TCNT3H and TCNT3L
	TCNT3L = 0b00000000;
	TIMSK3 = 0b00000010;	// bit 1 OCIE3A		Use 'Output Compare A Match' Interrupt, i.e. generate an interrupt
	// when the timer reaches the set value (in the OCR3A registers)
}

ISR(TIMER3_COMPA_vect) // TIMER3_Overflow_Handler (Interrupt Handler for Timer 3)
{	
	TCNT4=0x0000;
	PORTB|= (1<<PB6);
	_delay_us(15);
	PORTB&= ~(1<<PB6);
	//LCD_SetCursorPosition(0 /*iColumnPosition 0 - 40 */, 1 /*iRowPosition 0 for top row, 1 for bottom row*/);
	//LCD_WriteString("1TriggerInterrupt!");
	

	

	
}

void InitialiseGeneral()
{
	LCD_Initilise(true/*false = 1 line mode, true = 2 line mode*/, false/*false = 5*8pixels, true = 5*11 pixels*/);
	// Includes:	Configuration of PortG direction for Output and PortA direction for Output
	//				Enable the LCD device
	//				Configure 1/2 line mode and small/large font size
	
	sei();			// Enable interrupts at global level set Global Interrupt Enable (I) bit
}


void InitialiseTimer4()		// counter for counting echo duration - finally it is not working properly so I didnt use it
{
	TCCR4A = 0b00000000;	// Normal mode
	TCCR4B = 0b00000001;	// clock I/O no prescaler - max will be 65535 which is more than enough for this project
	TCCR4C = 0b00000000;
	
	
	TCNT4H = 0b00000000;	// Timer/Counter count/value registers (16 bit) TCNT1H and TCNT1L
	TCNT4L = 0b00000000;
	TIMSK4 = 0b00000000;	// no overflow - no compare- we don't need interrupt
}
