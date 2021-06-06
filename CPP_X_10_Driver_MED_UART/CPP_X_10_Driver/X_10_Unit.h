
/*
* X_10_unit.h
*
* Created: 05/05/2021 17.23.28
* Author: Yevhen
* AVR Compiler don´t like virtual functions, so no implementation of virtual or pure virtual. 
*/


#ifndef __X_10_UNIT_H__
#define __X_10_UNIT_H__

#include "X_10_types.h"

class X_10_Unit
{
	public:
		X_10_Unit(X_10_Codes unitName, uint8_t pinNumber);// All units are on PA1-7.
		X_10_Codes getAdress() const;
		void turnON();
		void turnOFF();
		X_10_Unit_state getState() const;
	private:
		X_10_Unit_state state_;
		const X_10_Codes unitAdress_;
		const uint8_t pinNumber_;
};

#endif
