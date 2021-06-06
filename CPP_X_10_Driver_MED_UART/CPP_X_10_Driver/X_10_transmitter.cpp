/* 
* X_10_transmitter.cpp
*
* Created: 02/05/2021 16.52.50
* Author: Yevhen
*/

#include "X_10_transmitter.h"
#include <avr/io.h>
#define X_10_code_lenght 9
#define X_10_START 0b0111

static uint16_t buffedMessage = 0;

X_10_transmitter::X_10_transmitter(X_10_transceiver_data_t& transceiverData):buffer_(),transceiverData_(transceiverData)
{
	DDRB |= (1<<PB7); // Set Port PB7 as output for the 120kHz oscillator. 
	TIMSK0 = (1 << TOIE0); // Set interrupt on overflow for timer0.
	TCCR0A |= (1 << COM0A0)| (1 << WGM01); // Enable toggle on match on OCR0A and CTC mode om timer0.
	OCR0A = 65; // Set the the correct counter to value for 120kHz.
	TCCR0B |= (1 << CS00);// Set precalse to 1  start timer0.
}

uint16_t X_10_transmitter::encodeForBuffer(X_10_Codes houseCode, X_10_Codes keyCode)
{
	uint16_t bufferEncoded = houseCode;
	return bufferEncoded = bufferEncoded << 5 | keyCode;
}

uint32_t X_10_transmitter::encodeForTransmitting(const uint16_t& message)
{
	uint32_t frameEncoded = 0;
	for (uint8_t i = 0; i < X_10_code_lenght; i++)
	{
		frameEncoded |= 1UL << ((2 * i) + !(message >> (X_10_code_lenght - 1 - i ) & 1UL));
	}

	frameEncoded = frameEncoded << 4 | X_10_START;
	
	return frameEncoded;
}
void X_10_transmitter::transmitMessage(X_10_Codes houseCode, X_10_Codes uintCode, X_10_Codes commandCode)
{
	if(buffer_.getBufferState() != BUFFER_FULL)
	{
		buffer_.push(encodeForBuffer(houseCode,uintCode));
		buffer_.push(encodeForBuffer(houseCode,commandCode));
	}
}

void X_10_transmitter::run()
{
	if(buffer_.getBufferState() != BUFFER_EMTY && transceiverData_.trasnmitterState == IDLE)
	{
		buffer_.pop(buffedMessage);
		transceiverData_.transmitterFrame = encodeForTransmitting(buffedMessage);
		transceiverData_.trasnmitterState = TRANSMITTING;
	}
	
}


