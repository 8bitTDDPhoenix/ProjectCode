/*
 * X_10_receiver.h
 *
 * Created: 02/05/2021 20.48.41
 *  Author: Yevhen
 ******************************
 */
 

#ifndef X_10_RECEIVER_H_
#define X_10_RECEIVER_H_
#include <stdio.h>
#include <stdbool.h>
#include "X_10_Cbuffer.h"
#include "X_10_Unit.h"

class X_10_receiver
{
	public:
	X_10_receiver(X_10_transceiver_data_t& transceiverData, X_10_Cbuffer<uint32_t>& receiverBuffer);
	void run();
	void addUnit(X_10_Unit *unit);
	
	private:
	uint16_t decode(uint32_t& newMessage);
	void interpret(uint16_t& decodedMessage);
	bool checkAddress(uint16_t& decodedMessage);
	X_10_transceiver_data_t& transceiverData_;
	X_10_Cbuffer<uint32_t>& buffer_;
	bool checkForError(const uint32_t& rxData) const;
	uint8_t unit_number_;
	X_10_Unit *unitArray[NUMBER_OF_UNITS+1];
};


#endif /* X_10_RECEIVER_H_ */