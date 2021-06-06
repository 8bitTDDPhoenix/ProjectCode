/*
 * UartSerialAplication.c
 *
 * Author : R.B.AA
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000
#include <util/delay.h>
#include "uart_int.h"


ISR(USART0_RX_vect)
{
	char modtaget_tegn;

	// Aflæs UART modtager-register
	modtaget_tegn = UDR0;
	
	if (modtaget_tegn == '1')
	{	
		SendString("Command: ");
		SendChar(modtaget_tegn);
		SendString(" \nSystem now ON\r\n");
	}
	if (modtaget_tegn == '0')
	{
		SendString("Command: ");
		SendChar(modtaget_tegn);
		SendString(" \nSystem now OFF\r\n");
	}
	
}

int main()
{
	// Initier UART (med RX interrupt enable)
	InitUART(9600, 8, 1);  //Baudrate - Databit - Rx int enable
	
	// Global interrupt enable
	sei();
	// Loop	
	while (1)
	{
		
		
	}
}

