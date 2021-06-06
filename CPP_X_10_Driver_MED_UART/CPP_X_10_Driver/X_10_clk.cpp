/*
 * X_10_clk.c
 *
 * Created: 29/04/2021 23.05.04
 * Author: Yevhen
  *Receiver samples the data after the rising edge clock interrupt routine ( rising edge clock interrupt routine enables sample interrupt routine ),
  *we can only queue one sample interrupt at a time (1 queue deep interrupt stack for each interrupt type).
  *The sample interrupt fires only if the data input pin is LOW ( low level external interrupt ) for a data '1',
  *default is no interrupt when data input pin is HIGH corresponding to data '0'. The low level external interrupt is disabled
  *after one low level external interrupt or at the falling edge clock interrupt routine.
  *Interrupt priroty IN0 -> IN7, CLOCK rising edge = INT1; CLOCK falling edge = INT0; DATA LOW LEVEL = INT2.
 */ 
#include "X_10_clk.h"
#include "X_10_types.h"
#include "X_10_Cbuffer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"

#define RESEND 2
#define START_MASK 0x0F

bool start_stop;

uint8_t start_code = 0;

X_10_transceiver_data_t transceiverData;

X_10_Cbuffer<uint32_t> receiverBuffer;

ISR(INT2_vect)// Interrupt for signal LOW (logical HIGH) interception. Otherwise a logical LOW is intercepted. 
{	
	if(transceiverData.receiverState == RECEIVING)
	{
		transceiverData.receiverFrame |= 1U;
		PORTA |= (1<<PINA2);// For testing purpose.
	}
	else
	{
		start_code |=  1U;
	}
	EIMSK &= ~(1<<INT2); // Disable external interrupt on pin PD2
}



ISR(INT1_vect) //Rising edge interrupt clock routine
{
	// Transmitter part of the rising edge interrupt clock routine
	if(transceiverData.trasnmitterState == TRANSMITTING)
	{		
		if((transceiverData.transmitterFrame >> transceiverData.transmitterFrameIndex & 1U))
		{
			PORTA &= ~(1<<PINA0);
			
		}		
	}
	
	//Receiver part of the rising edge interrupt clock routine
	if(transceiverData.receiverState == RECEIVING)
	{	
		transceiverData.receiverFrame <<=  1U;
	}
	else
	{
		start_code <<= 1U; // left shift	
	}
	
	EIMSK |= (1<<INT2); // Enable external interrupt on pin PD2
	
}

ISR(INT0_vect) //Falling edge interrupt routine
{

	EIMSK &= ~(1<<INT2); // Disable external interrupt on pin PD2, faster to do this then check if its already disabled.
	
	// Transmitter part of the falling edge interrupt routine.
	if(transceiverData.trasnmitterState == TRANSMITTING)
	{	
		PORTA |= (1<<PINA0);
		transceiverData.transmitterFrameIndex++;
		
		if(transceiverData.transmitterFrameIndex  == X_10_transmit_message_length)
		{
			transceiverData.transmittedN++;
			transceiverData.transmitterFrameIndex = 0;
			
			if(transceiverData.transmittedN == RESEND)
			{
				transceiverData.transmittedN = 0;
				transceiverData.trasnmitterState = IDLE;			
			}
		}	
	}
	
	// Receiver part of the falling edge interrupt routine.

	if(transceiverData.receiverState == RECEIVING)
	{
		transceiverData.receiverFrameIndex++;
		PORTA &= ~(1<<PINA2);// For testing purpose.
		
		if(transceiverData.receiverFrameIndex == X_10_receiv_message_length)
		{
			transceiverData.receiverState = DETECT_START_SEQUENCE;
			start_code = 0;
			receiverBuffer.push(transceiverData.receiverFrame);				
		}
	}
	else if(!((start_code & START_MASK)^START)) //Detect start sequence
	{
		transceiverData.receiverFrameIndex = 0;
		transceiverData.receiverFrame = 0;
		transceiverData.receiverState = RECEIVING;
	}	
}


ISR(TIMER0_OVF_vect) // Interrupt handler for Timer0 overflow.
{
	asm("nop");
}

char modtaget_tegn;

ISR(USART0_RX_vect)
{

	// Aflæs UART modtager-register
	modtaget_tegn = UDR0;
	
	if (modtaget_tegn == '1')
	{
		SendString("Command: ");
		SendChar(modtaget_tegn);
		SendString(" \nSystem now ON\r\n");
		start_stop = true;
	}
	if (modtaget_tegn == '0')
	{
		SendString("Command: ");
		SendChar(modtaget_tegn);
		SendString(" \nSystem now OFF\r\n");
		start_stop = false;
	}
	
}

void X_10_clkInit()
{
	DDRD  &=  ~(1<<PD0) | ~(1<<PD1); // Set Port PD0 and PD1 as input
	EIMSK |= (1<<INT0 | 1<<INT1); //Activate external interrupt on pin PD1 and PD0
	EICRA |= (1<<ISC01 | 1<<ISC11 | 1<< ISC10);  //Set PD1 to rising edge and PD0 to falling edge
	sei(); //Enable global interrupts
}
