#include <uCobegin.h>

#include "q2quicksort.h"

template <typename T>
void quicksort(T values[], unsigned int low, unsigned int high, unsigned int depth) {
	if (low < high) {
		if (depth > 0) {
			unsigned int pivotIndex = partition(values, low, high);
			COBEGIN
			BEGIN quicksort(values, low, pivotIndex, depth - 1);
			END BEGIN quicksort(values, pivotIndex + 1, high, depth - 1);
			END COEND
		} else {
			sequentialQuicksort(values, low, high);
		}
	}
}
