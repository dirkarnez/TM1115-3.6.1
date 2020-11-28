#include <p18f4520.h>
int Counter_01 = 0;
int Counter_02 = 0;

#pragma interrupt high_isr
void high_isr(void)
{
	if(PIR1bits.TMR1IF)
	{
		PIR1bits.TMR1IF = 0;
		PORTBbits.RB2 = !PORTBbits.RB2;

		TMR1H = 0xF0;
		TMR1L = 0x00;
	}
}

#pragma interrupt low_isr
void low_isr(void)
{
}

#pragma code high_vector=0x08
void interrupt_at_high_vector(void)
{
	_asm goto high_isr _endasm
}

#pragma code low_vector=0x18
void interrupt_at_low_vector(void)
{
	_asm goto low_isr _endasm
}

#pragma code
void main (void)
{

	int Count = 0;

	TRISB = 0xF0;
	PORTB = 0x00;

	// Interrupt Control Register 1
	INTCONbits.PEIE = 1; 
	INTCONbits.GIE = 1;
	PIE1bits.TMR1IE = 1;
	PIR1bits.TMR1IF = 0;
	T1CON = 0x0B;
	TMR1H = 0xF0;
	TMR1L = 0x00;

	while(1)
	{
		PORTB ^= 0x01;
		for(Count=0; Count<10000; Count++);

		if(PORTAbits.RA4 == 0) {
			PORTB |= 0x02;
			for(Count=0; Count<10000; Count++);
			PORTB &= ~0x02;
			for(Count=0; Count<10000; Count++);
		} else if(PORTAbits.RA4 == 1) {
			PORTB &= ~0x02;
		}
	}
}

/*
RB3 Control by ADC
RB2 Control by Timer
RB1 Control by Button
RB0 LED Blinking
*/