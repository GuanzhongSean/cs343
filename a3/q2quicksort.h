#ifndef Q2QUICKSORT_H
#define Q2QUICKSORT_H

#include <algorithm>

using std::swap;

template <typename T>
void quicksort(T values[], unsigned int low, unsigned int high,
			   unsigned int depth);

template <typename T>
unsigned int partition(T arr[], unsigned int low, unsigned int high) {
	T pivot = arr[low + (high - low) / 2];
	swap(arr[low + (high - low) / 2], arr[high]);
	int i = low - 1;
	for (unsigned int j = low; j < high; j++) {
		if (arr[j] < pivot) {
			i++;
			swap(arr[i], arr[j]);
		}
	}
	swap(arr[i + 1], arr[high]);
	return i + 1;
}

template <typename T>
void sequentialQuicksort(T arr[], unsigned int low, unsigned int high) {
	if (low < high) {
		unsigned int pivotIndex = partition(arr, low, high);
		sequentialQuicksort(arr, low, pivotIndex);
		sequentialQuicksort(arr, pivotIndex + 1, high);
	}
}

#if defined(CBEGIN)
#include "q2quicksort_cobegin.cc"
#elif defined(TASK)
#include "q2quicksort_task.cc"
#elif defined(ACTOR)
#include "q2quicksort_actor.cc"
#else
#error "Unknown concurrency type. Use CBEGIN, TASK, or ACTOR."
#endif

#endif	// Q2QUICKSORT_H
