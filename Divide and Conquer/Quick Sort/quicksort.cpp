#include "quicksort.h"
#include <algorithm>

unsigned Partition(int* a, unsigned first, unsigned last)
{
	unsigned i = first;
	const int pivot = a[last];
	
	// From start to before pivot
	for (unsigned j = first; j < last; ++j)
	{
		if (a[j] < pivot)
		{
			std::swap(a[j], a[i]);
			++i;
		}
	}
	
	std::swap(a[i], a[last]); // Sort pivot
	return i; // Pivot position
}

void quicksort(int* a, unsigned first, unsigned last)
{
	if (last-first < 2) return;
	
	const unsigned pivot = Partition(a, first, last-1);
	
	quicksort(a, first, pivot);
	quicksort(a, pivot+1, last);
}