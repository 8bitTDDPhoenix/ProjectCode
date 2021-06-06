/* 
* X_10_Cbuffer.h
*
* Created: 02/05/2021 15.38.25
* Author: Yevhen
*/

#ifndef __X_10_CBUFFER_H__
#define __X_10_CBUFFER_H__

#include "X_10_types.h"

const static int BUFFER_SIZE = 20;

template <typename T>
class X_10_Cbuffer
{
public:
	X_10_Cbuffer() {
		newestIndex_ = 0;
		readIndex_ = 0;
		for (int i = 0; i < BUFFER_SIZE; i++)
		{
			bufferArray[i] = T();
		}
	};
	void push(const T& newElement) {
		
		if( ((newestIndex_ + 1) % BUFFER_SIZE) != readIndex_) // Check if write overflows.
		{
			newestIndex_ = ++newestIndex_ % BUFFER_SIZE;
			bufferArray[newestIndex_] = newElement; // Write new element into the buffer;
		}
	};
	void pop(T& outElement)
	{
		if(newestIndex_ != readIndex_)
		{
			readIndex_ = ++readIndex_ % BUFFER_SIZE;
			outElement = bufferArray[readIndex_]; // Read next element out.
		}
	};
	X_10_Cbuffer_State getBufferState() const // Important to check the sate like this and not set state in push or pop, It will create race conditions otherwise. 										 // if interrupts are allowed to use the buffer at the same time as main.
	{
		if(newestIndex_ == readIndex_)
		{
			return BUFFER_EMTY;
		}
		else if(newestIndex_ + 1 == readIndex_)
		{
			return BUFFER_FULL;
		}
		
		return BUFFER_NOT_EMPTY;
	}
private:
	uint8_t newestIndex_, readIndex_;
	T bufferArray[BUFFER_SIZE];
};

#endif //__X_10_CBUFFER_H__
