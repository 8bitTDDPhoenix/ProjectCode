/* 
* X_10_transmitter.h
*
* Created: 02/05/2021 16.52.50
* Author: Yevhen
*/


#ifndef __X_10_TRANSMITTER_H__
#define __X_10_TRANSMITTER_H__
#include <stdio.h>
#include <stdbool.h>
#include "X_10_Cbuffer.h"


class X_10_transmitter
{
public:
	X_10_transmitter(X_10_transceiver_data_t& transceiverData);
	void transmitMessage(X_10_Codes houseCode, X_10_Codes uintCode, X_10_Codes commandCode);
	void run();
	
private:
	uint32_t encodeForTransmitting(const uint16_t& message);
	uint16_t encodeForBuffer(X_10_Codes houseCode, X_10_Codes keyCode);
	X_10_Cbuffer<uint16_t> buffer_;
	X_10_transceiver_data_t& transceiverData_;
};

#endif //__X_10_TRANSMITTER_H__
