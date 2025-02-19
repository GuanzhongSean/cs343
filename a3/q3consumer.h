#ifndef __Q3CONSUMER_H__
#define __Q3CONSUMER_H__

#include <uPRNG.h>

#include "q3buffer.h"

_Task Consumer {
	BoundedBuffer<int>& buffer;
	const int Delay;
	int& sum;

	void main() {
		PRNG prng;
		try {
			while (true) {
				yield(prng(Delay));	 // Random delay before consuming
				int value = buffer.remove();
				sum += value;
			}
		} catch (BoundedBuffer<int>::Poison&) {
			return;	 // Terminate when poisoned
		}
	}

   public:
	Consumer(BoundedBuffer<int> & buffer, const int Delay, int& sum)
		: buffer(buffer), Delay(Delay), sum(sum) {}
};

#endif	// __Q3CONSUMER_H__
