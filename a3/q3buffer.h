#ifndef __Q3BUFFER_H__
#define __Q3BUFFER_H__

#include <iostream>

#include "BargingCheck.h"

using namespace std;

template <typename T>
class BoundedBuffer {
   private:
	const unsigned int size;
	T *buffer;
	unsigned int front, rear, count;
	unsigned long int blockCount;
	bool poisoned;
#ifdef NOBUSY
	bool signal_flag = false;
	uCondLock blk;
#endif	// NOBUSY

	uOwnerLock mutex;
	uCondLock plk, clk;

	BCHECK_DECL;

   public:
	_Exception Poison{};
	BoundedBuffer(const unsigned int size = 10)
		: size(size),
		  front(0),
		  rear(0),
		  count(0),
		  blockCount(0),
		  poisoned(false) {
		buffer = new T[size];
	}
	~BoundedBuffer() {
		delete[] buffer;
	}
	unsigned long int blocks() {
		return blockCount;
	}
	void poison() {
#ifdef NOBUSY
		while (!blk.empty()) uThisTask().yield();
#endif	// NOBUSY
		mutex.acquire();
		poisoned = true;
		clk.broadcast();
		mutex.release();
	}
	void insert(T elem);
	T remove() __attribute__((warn_unused_result));
};

#ifdef BUSY	 // busy waiting implementation
template <typename T>
void BoundedBuffer<T>::insert(T elem) {
	mutex.acquire();
	PROD_ENTER;

	while (count == size) {
		blockCount++;
		plk.wait(mutex);
	}

	// Perform assertion
	assert(count < size);

	buffer[rear++ % size] = elem;
	count++;
	INSERT_DONE;

	CONS_SIGNAL(clk);
	clk.signal();

	mutex.release();
}

template <typename T>
T BoundedBuffer<T>::remove() {
	mutex.acquire();
	CONS_ENTER;

	while (count == 0) {
		if (poisoned) {
			mutex.release();
			_Throw Poison();
		}
		blockCount++;
		clk.wait(mutex);
	}

	// Perform assertion
	assert(count > 0);

	T elem = buffer[front++ % size];
	count--;
	REMOVE_DONE;

	PROD_SIGNAL(plk);
	plk.signal();

	mutex.release();
	return elem;
}
#endif	// BUSY

#ifdef NOBUSY  // no busy waiting implementation
template <typename T>
void BoundedBuffer<T>::insert(T elem) {
	mutex.acquire();
	PROD_ENTER;

	if (signal_flag) {
		blockCount++;
		blk.wait(mutex);
		signal_flag = false;
	}

	if (count == size) {
		if (!blk.empty()) {
			signal_flag = true;
			blk.signal();
		}
		blockCount++;
		plk.wait(mutex);
		signal_flag = false;
	}

	// Perform assertion
	assert(count < size);

	buffer[rear++ % size] = elem;
	count++;
	INSERT_DONE;

	if (!clk.empty()) {
		signal_flag = true;
		CONS_SIGNAL(clk);
		clk.signal();
	} else if (!blk.empty()) {
		signal_flag = true;
		blk.signal();
	}

	mutex.release();
}

template <typename T>
T BoundedBuffer<T>::remove() {
	mutex.acquire();
	CONS_ENTER;

	if (signal_flag) {
		blockCount++;
		blk.wait(mutex);
		signal_flag = false;
	}

	if (count == 0) {
		if (!blk.empty()) {
			signal_flag = true;
			blk.signal();
		}
		if (poisoned) {
			mutex.release();
			_Throw Poison();
		}
		blockCount++;
		clk.wait(mutex);
		signal_flag = false;
		if (count == 0 && poisoned) {
			mutex.release();
			_Throw Poison();
		}
	}

	// Perform assertion
	assert(count > 0);

	T elem = buffer[front++ % size];
	count--;
	REMOVE_DONE;

	// If producers are waiting, wake one up
	if (!plk.empty()) {
		signal_flag = true;
		PROD_SIGNAL(plk);
		plk.signal();
	} else if (!blk.empty()) {
		signal_flag = true;
		blk.signal();
	}

	mutex.release();
	return elem;
}
#endif	// NOBUSY

#endif	// __Q3BUFFER_H__
