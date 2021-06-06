/*
 * CPP_X_10_Driver.cpp
 *
 * Created: 02/05/2021 15.37.03
 * Author : Yevhen
 */ 

#include <avr/io.h>
#include <stdint.h>

#include "X_10_transmitter.h"
#include "X_10_receiver.h"
#include "X_10_clk.h"
#include "X_10_Unit.h"
#include "uart.h"

extern X_10_transceiver_data_t transceiverData; // Declared global for the interrupts
extern X_10_Cbuffer<uint32_t> receiverBuffer; // Declared global for the interrupts
extern bool start_stop;

int main(void)
{
	InitUART(9600,8,1);
	X_10_clkInit();
	transceiverData.receiverFrame  = 0;
	transceiverData.receiverFrameIndex = 0;
	transceiverData.receiverState = IDLE;
	transceiverData.transmittedN = 0;
	transceiverData.transmitterFrame = 0;
	transceiverData.transmitterFrameIndex = 0; 
	transceiverData.trasnmitterState = IDLE;

	X_10_Unit myLapme(LAMPE, PINA3);
	X_10_Unit myTv(TV, PINA1);
	
	X_10_receiver receiver(transceiverData, receiverBuffer); // Create receiver.
	receiver.addUnit(&myLapme);
	receiver.addUnit(&myTv);
	
	
	X_10_transmitter transmitter(transceiverData); // Create transmitter.
	
	
	// To test Transmitter and Receiver create a Demo run. 
	uint32_t var = 0;
    while (1) 
    {   	
		var++;
		if(var == 200000U && start_stop == true)
		{
			transmitter.transmitMessage(LIVINGROOM,TV,ON);
			transmitter.transmitMessage(LIVINGROOM,LAMPE,OFF);

			
		}
		if(var == 400000U && start_stop == true)
		{
			transmitter.transmitMessage(LIVINGROOM,LAMPE,ON);
			transmitter.transmitMessage(LIVINGROOM,TV,OFF);
			
		}
		if (var == 400000U)
		{
			var = 0;
		}
		transmitter.run();
		receiver.run();	
    }
	return 0;
}

