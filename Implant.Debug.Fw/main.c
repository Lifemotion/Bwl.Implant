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
	PORTB |= 0x4;
	PORTB |= (1<<1);
	PORTB &= ~(1<<0);
	DDRA  &= ~(1<<1);
	PUEA &= ~(1<<2);
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

int main(void)
{
	UBRR0H = (unsigned char)(UBRR_VALUE>>8);
	UBRR0L = (unsigned char)UBRR_VALUE;
	UCSR0A = (0<<U2X0);
	UCSR0B = (1<<RXEN0);
	UCSR0B &= ~(1<<TXEN0);
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);
	receive_mode();
	PORTB &= ~0x4;
	PORTA &= ~0x8;
	while (1)
	{
		/*if(UCSR0A & (1<<RXC0))
		{
			unsigned char data = UDR0;
			if(data = 0xAA){				
				_delay_ms(5);*/
				transmit_mode();
				UDR0 = 0x55;
				while ( !( UCSR0A & (1<<UDRE0)));
			/*}
		}
		PORTB &= ~0x4;
		PORTA &= ~0x8;*/
	}
}

