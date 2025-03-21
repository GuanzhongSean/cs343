#include "q2quicksort.h"

template <typename T>
_Task QuicksortTask {
	T* values;
	unsigned int low;
	unsigned int high;
	unsigned int depth;

	void main() {
		if (low < high && depth > 0) {
			unsigned int pivotIndex = partition(values, low, high);
			QuicksortTask leftTask{values, low, pivotIndex, depth - 1};
			QuicksortTask rightTask{values, pivotIndex + 1, high, depth - 1};
		} else if (low < high) {
			sequentialQuicksort(values, low, high);
		}
	}

   public:
	QuicksortTask(T values[], unsigned int low, unsigned int high, unsigned int depth)
		: values(values), low(low), high(high), depth(depth) {}
};

template <typename T>
void quicksort(T values[], unsigned int low, unsigned int high, unsigned int depth) {
	if (depth > 0) {
		QuicksortTask task{values, low, high, depth};
	} else {
		sequentialQuicksort(values, low, high);
	}
}
