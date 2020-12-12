#include "perm-lexicographical.h"
#include <algorithm>
#include <iostream>

bool nextPermLexicographical (std::vector<int>& p)
{
	int size = p.size();
	int pivotIndex = size;
	
	// 1. From right to left, find the first a(i) < a(i+1)
	for (int i = size-1; i > 0; --i)
	{
		if (p[i-1] < p[i])
		{
			pivotIndex = i-1;
			break;
		}
	}
	
	// Nothing to move
	if (pivotIndex == size) return false;
	
	// 2. Inserts {a(0), ..., a(i-1)} -> just make them remain
	// 3. Find the smallest a(k) within the tail such that a(k) > a(i)
	int pivot = p[pivotIndex];
	int smallestIndex = pivotIndex+1;
	int smallest = p[smallestIndex];
	
	for (int i = smallestIndex+1; i < size; ++i)
	{
		int curr = p[i];
		if (curr > pivot) // a(k) > a(i)
		{
			if (curr < smallest) // It is the smallest, untill now
			{
				smallest = curr;
				smallestIndex = i;
			}
		}
	}
	
	// 4. Insert a(k) at the end of the p(j+1)
	std::swap(p[pivotIndex], p[smallestIndex]);
	
	// 5. Insert at the end of p(j+1) all remaining elements in increasing order.
	std::sort(p.begin()+pivotIndex+1, p.end());
	
	return true;
}
