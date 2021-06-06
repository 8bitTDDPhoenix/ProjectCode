/*
 * X_10_Types.h
 *
 * Created: 02/05/2021 15.54.58
 *  Author: Yevhen
 * For Setting up the Transceiver:
 * 1: Set NUMBER_OF_UNITS, as they are not dynamically allocated. 
 * 2: Set TRANSCEIVER_ADDRESS, can be selected from the of X_10_Codes.
 * 
 */ 


#ifndef X_10_TYPES_H_
#define X_10_TYPES_H_

#include <stdint.h>

#define TRANSCEIVER_ADDRESS LIVINGROOM
#define NUMBER_OF_UNITS 2

#define TRANSCEIVER_ADDRESS_MASK 0x1E0
#define TRANSCEIVER_CODE_MASK 0x1F
#define X_10_transmit_message_length 28
#define X_10_receiv_message_length 18

typedef enum{
	UNIT_ON,
	UNIT_OFF
}X_10_Unit_state;

typedef enum{
	START = 0b1110,
	ON = 0b00101,
	OFF = 0b00111,
	STATUS_ON = 0b11011,
	STATUS_OFF = 0b11101,
	STATUS_REQUEST = 0b11111,
	DIM = 0b01001,
	BRIGHT = 0b01011,
	ALL_UNITS_OFF = 0b00001,
	ALL_UNITS_ON = 0b00011,
	LIVINGROOM = 0b0110,
	LAMPE = 0b01100,
	TV = 0b11100
}X_10_Codes;


typedef enum{
	TRANSMITTING,
	RECEIVING,
	IDLE,
	DETECT_START_SEQUENCE
}X_10_Transceiver_State;

typedef enum{
	BUFFER_EMTY,
	BUFFER_NOT_EMPTY,
	BUFFER_FULL
}X_10_Cbuffer_State;


typedef struct X_10_transceiver_data
{
	uint32_t transmitterFrame;
	uint32_t receiverFrame;
	uint8_t transmitterFrameIndex;
	uint8_t receiverFrameIndex;
	uint8_t transmittedN;
	X_10_Transceiver_State trasnmitterState;
	X_10_Transceiver_State receiverState;
		
}X_10_transceiver_data_t;

#endif