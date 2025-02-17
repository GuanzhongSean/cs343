#include <uActor.h>

#include "q2quicksort.h"

template <typename T>
struct SortMessage : public uActor::Message {
	T* values;
	unsigned int low;
	unsigned int high;
	unsigned int depth;

	SortMessage(T* values, unsigned int low, unsigned int high,
				unsigned int depth)
		: uActor::Message(uActor::Delete),
		  values(values),
		  low(low),
		  high(high),
		  depth(depth) {}
};

template<typename T>
_Actor QuicksortActor {
    uActor::Allocation receive(uActor::Message& msg) {
        Case(SortMessage<T>, msg) {
            if (msg_d->low < msg_d->high && msg_d->depth > 0) {
                unsigned int pivotIndex = partition(msg_d->values, msg_d->low, msg_d->high);

                uActor::start();
                *new QuicksortActor<T>() | *new SortMessage<T>(msg_d->values, msg_d->low, pivotIndex, msg_d->depth - 1);
                *new QuicksortActor<T>() | *new SortMessage<T>(msg_d->values, pivotIndex + 1, msg_d->high, msg_d->depth - 1);
                uActor::stop();
            } else if (msg_d->low < msg_d->high) {
                sequentialQuicksort(msg_d->values, msg_d->low, msg_d->high);
            }
        }
        return uActor::Delete;
    }
};

template<typename T>
void quicksort(T values[], unsigned int low, unsigned int high, unsigned int depth) {
    uActor::start();
    *new QuicksortActor<T>() | *new SortMessage<T>(values, low, high, depth);
    uActor::stop();
}
