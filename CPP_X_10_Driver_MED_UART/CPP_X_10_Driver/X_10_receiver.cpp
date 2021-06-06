/*
 * X_10_receiver.cpp
 *
 * Created: 02/05/2021 20.48.53
 *  Author: Yevhen
 */ 
#include "X_10_receiver.h"
#include "X_10_types.h"
#include <avr/io.h>

static uint32_t newMessage = 0;

static uint16_t decodedMessage = 0;

X_10_Codes keyCodeArray[10] ={ON,OFF,DIM,BRIGHT,ALL_UNITS_OFF,ALL_UNITS_ON};
	
X_10_receiver::X_10_receiver(X_10_transceiver_data_t& transceiverData, X_10_Cbuffer<uint32_t>& receiverBuffer):transceiverData_(transceiverData),buffer_(receiverBuffer)
{
	DDRD  &=  ~(1<<PD2); // Set PD2 as input.
	EICRA &= (~(1<<ISC20) | ~(1<<ISC21)); // Set PD2 to low level interrupt trigger. 
	unit_number_ = 0;
	for(uint8_t i = 0; i < NUMBER_OF_UNITS; i++)
	{
		unitArray[i] = NULL;
	}
	
}

bool X_10_receiver::checkForError(const uint32_t& rxData) const
{	
	uint32_t erroBitsMask = 0x7;
	uint32_t tempVar = 0; 
	for (uint8_t i = 0; i < X_10_receiv_message_length - 4; i++)
	{
		tempVar = rxData & (erroBitsMask << i);
		if ( (!(tempVar^(erroBitsMask << i))) || (!(tempVar^0)) ) // Check for three ones or zeros in a row 
		{
			return false;
		}
	}
	return true;
}

void X_10_receiver::run()
{
	if(buffer_.getBufferState() != BUFFER_EMTY)
	{	
		buffer_.pop(newMessage);
		
		if(checkForError(newMessage))
		{
			decodedMessage = decode(newMessage);	
			
			if(checkAddress(decodedMessage))
			{
				interpret(decodedMessage);
			}

		}	
	}	
}

void X_10_receiver::interpret(uint16_t& decodedMessage)
{
	
	for(uint8_t i = 1; i <= NUMBER_OF_UNITS; i++)
	{	
		if(!( (decodedMessage & TRANSCEIVER_CODE_MASK)^unitArray[i]->getAdress()) ) // Check if message is for any of the units connected to this transceiver.
		{
			unit_number_ = i;
			return;
		}
	}

	for(uint8_t i = 0; i < 10 ; i++)
	{
		if( !( (decodedMessage & TRANSCEIVER_CODE_MASK)^keyCodeArray[i]) )// Check is the message has a valid command. 
		{
			switch (i)
			{
				case 0:							
					unitArray[unit_number_]->turnON();	
				break;
				case 1:
					unitArray[unit_number_]->turnOFF();
				break;
				default:
				//Nothing;
				break;
			}
			return;
		}
	}	
}

void X_10_receiver::addUnit(X_10_Unit *unit)
{
	
	unitArray[++unit_number_] = unit;
}

uint16_t X_10_receiver::decode(uint32_t& newMessage)
{
	uint16_t decodedVar = 0;
	for(uint8_t i = 0; i < X_10_receiv_message_length/2 ; i++)
	{
		decodedVar |= ((~(newMessage >> 2 * i) & 1UL) << i); // Decode the message to normal bit style.
	}
	return decodedVar;
}

bool X_10_receiver::checkAddress(uint16_t& decodedMessage)
{
	if(!( ((decodedMessage & TRANSCEIVER_ADDRESS_MASK) >> 5)^(TRANSCEIVER_ADDRESS)) ) // Check if message is for this transceiver.
	{
		return true;				
	}
	return false;
}