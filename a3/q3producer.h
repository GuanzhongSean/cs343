#ifndef __Q3PRODUCER_H__
#define __Q3PRODUCER_H__

#include <uPRNG.h>

#include "q3buffer.h"

_Task Producer {
	BoundedBuffer<int>& buffer;
	const int Produce;
	const int Delay;

	void main() {
		PRNG prng;
		for (int i = 1; i <= Produce; i++) {
			yield(prng(Delay));	 // Random delay before producing
			buffer.insert(i);
		}
	}

   public:
	Producer(BoundedBuffer<int> & buffer, const int Produce, const int Delay)
		: buffer(buffer), Produce(Produce), Delay(Delay) {}
};

#endif	// __Q3PRODUCER_H__
