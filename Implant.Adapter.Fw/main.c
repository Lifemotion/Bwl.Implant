/*
* Implant.Debug.Fw.c
*
* Created: 20.07.2017 16:18:35
* Author : gus10
*/
#define F_CPU 8000000
#define BAUD 600
#include <avr/io.h>
#include <util/setbaud.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include "libs/bwl_uart.h"
#include "libs/bwl_simplserial.h"

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
	PORTB |= (1<<1);
	PORTB |= (1<<0);
	DDRA  &= ~(1<<2);
	PUEA &= ~(1<<2);
}


void var_delay_ms(int ms)
{
	for(int i=0;i<ms;i++)_delay_ms(1.0);
}

void sserial_send_start(unsigned char portindex)
{
	transmit_mode();
}

void sserial_send_end(unsigned char portindex)
{
	receive_mode();
}

void sserial_process_request(unsigned char portindex)
{
	if(sserial_request.command==1){
		PORTB |= 0x4;
		PORTA |= 0x8;
		sserial_response.datalength = 0;
		sserial_send_response();
	}
	if(sserial_request.command==2){
		PORTB &= ~0x4;
		PORTA &= ~0x8;
		sserial_response.datalength = 0;
		sserial_send_response();
	}
}
void board_init()
{
	wdt_enable(WDTO_8S);
	UBRR0H = (unsigned char)(UBRR_VALUE>>8);
	UBRR0L = (unsigned char)UBRR_VALUE;
	UCSR0A = (0<<U2X0);
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);
	PORTB &= ~0x4;
	PORTA &= ~0x8;
	DDRB  |= 0x7;
}

void board_led(unsigned char state)
{	
	DDRB  |= 0x7;
	if(state){
		PORTB |= 0x4;
		PORTA |= 0x8;
	}else{
		PORTB &= ~0x4;
		PORTA &= ~0x8;
	}
}

int main(void)
{
	board_init();
	sserial_request.address_to = 0;
	sserial_request.datalength = 0;
	while (1)
	{
		wdt_reset();
		sserial_request.command = 1;
		board_led(sserial_send_request_wait_response(0, 300));
		_delay_ms(500);
		sserial_request.command = 2;
		board_led(sserial_send_request_wait_response(0, 300));
		_delay_ms(500);
	}
}

