/*
* Implant.Debug.Fw.c
*
* Created: 20.07.2017 16:18:35
* Author : gus10
*/
#define F_CPU 8000000
#define BAUD 1200
#include <avr/io.h>
#include <util/setbaud.h>
#include <util/delay.h>

void receive_mode()
{
	UCSR0B |= (1<<RXEN0);
	UCSR0B &= ~(1<<TXEN0);
	DDRB  |= 0x7;
	
	PORTB &= ~0x01;
	PORTB |= 0x02;
	DDRA  &= ~(1<<1);
	PUEA &= ~(1<<1);
}

void transmit_mode()
{
	UCSR0B &= ~(1<<RXEN0);
	UCSR0B |= (1<<TXEN0);
	DDRB  |= 0x7;
	PORTB &= ~0x4;
	PORTB |= (1<<1);
	PORTB |= (1<<0);
	DDRA  &= ~(1<<2);
	PUEA &= ~(1<<2);
}

unsigned char counter = 0;
int main(void)
{
	UBRR0H = (unsigned char)(UBRR_VALUE>>8);
	UBRR0L = (unsigned char)UBRR_VALUE;
	UCSR0A = (0<<U2X0);
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);
	transmit_mode();
	unsigned long delay = 0;
	PORTB &= ~0x4;
	PORTA &= ~0x8;
	while (1)
	{
		//UDR0 = 0xAA;
		//while ( !( UCSR0A & (1<<UDRE0)));
		receive_mode();
		//while(delay++<200000){
			if(UCSR0A & (1<<RXC0))
			{
				//unsigned char data = UDR0;
				//if(data==0x55){
					for(int i=0;i<10;i++){
						PORTB ^= 0x4;
						PORTA ^= 0x8;
						_delay_ms(50);
					}
				//}
				//break;
			}
		//}
		delay = 0;
		//transmit_mode();
	}
}

