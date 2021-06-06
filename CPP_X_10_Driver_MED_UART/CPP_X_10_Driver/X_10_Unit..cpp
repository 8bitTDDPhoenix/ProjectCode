/*
 * X_10_Unit.cpp
 *
 * Created: 15/05/2021 19.28.24
 *  Author: Yevhen
 */ 
#include <avr/io.h>
#include "X_10_Unit.h"
#include "util/delay.h"

X_10_Unit::X_10_Unit(X_10_Codes unitName, uint8_t pinNumber):unitAdress_(unitName),pinNumber_(pinNumber)
{
	 state_ = UNIT_OFF;
	 DDRA |= 0xFF;
};

X_10_Codes X_10_Unit::getAdress() const { return unitAdress_;};

void X_10_Unit::turnOFF()
{
	if(state_ == UNIT_ON)
	{
		state_ = UNIT_OFF;
		PORTA &= ~(1<<pinNumber_);
	}
}

void X_10_Unit::turnON()
{
	if(state_ == UNIT_OFF)
	{
		state_ = UNIT_ON;
		PORTA |= (1<<pinNumber_);
	}
}

X_10_Unit_state X_10_Unit::getState() const
{
	return state_;
}