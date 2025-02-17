#include <uCobegin.h>

#include "q2quicksort.h"

template <typename T>
void quicksort(T values[], unsigned int low, unsigned int high,
			   unsigned int depth) {
	if (low < high) {
		unsigned int pivotIndex = partition(values, low, high);
		if (depth > 0) {
			COBEGIN
				BEGIN quicksort(values, low, pivotIndex, depth - 1); END
				BEGIN quicksort(values, pivotIndex + 1, high, depth - 1); END
			COEND
		} else {
			quicksort(values, low, pivotIndex, 0);
			quicksort(values, pivotIndex + 1, high, 0);
		}
	}
}
