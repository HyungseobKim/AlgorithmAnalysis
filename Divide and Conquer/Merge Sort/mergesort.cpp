#include "mergesort.h"
#include <string.h>

void mergesort_recursive(int* leftData, int* memory, unsigned size)
{
	if (size < 2) return;
	
	// Split
	const unsigned leftSize = size/2;
	const unsigned rightSize = size-leftSize;
	
	int* rightData = leftData+leftSize;
	
	mergesort_recursive(leftData, memory, leftSize);
	mergesort_recursive(rightData, memory+leftSize, rightSize);
	
	// Merge
	unsigned leftIndex = 0;
	unsigned rightIndex = 0;
	
	while (leftIndex < leftSize && rightIndex < rightSize)
	{
		if (leftData[leftIndex] < rightData[rightIndex])
		{
			memory[leftIndex + rightIndex] = leftData[leftIndex];
			++leftIndex;
		}
		else
		{
			memory[leftIndex + rightIndex] = rightData[rightIndex];
			++rightIndex;
		}
	}
	
	while (leftIndex < leftSize)
	{
		memory[leftIndex + rightIndex] = leftData[leftIndex];
		++leftIndex;
	}
	
	while (rightIndex < rightSize)
	{
		memory[leftIndex + rightIndex] = rightData[rightIndex];
		++rightIndex;
	}
	
	memcpy(leftData, memory, sizeof(int)*size);
}

void mergesort(int* a, unsigned p, unsigned r)
{
	int* memory = new int[r-p];
	mergesort_recursive(a+p, memory, r-p);
	delete[] memory;
}