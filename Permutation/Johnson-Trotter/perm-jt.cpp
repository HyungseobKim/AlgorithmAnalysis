#include "perm-jt.h"
#include <algorithm>
#include <iostream>

PermJohnsonTrotter::PermJohnsonTrotter(int size)
	: m_size(size), m_permutation(size), m_direction(size)
{
	for (int i = 0; i < m_size; ++i)
	{
		m_permutation[i] = i+1;
		m_direction[i] = -1;
	}
}

bool PermJohnsonTrotter::Next()
{
	int largestMobile = -1;
	int largestIndex = -1;
	
	for (int i = 0; i < m_size; ++i)
	{
		const int pointing = i + m_direction[i];
		const int curr = m_permutation[i];
		
		if (pointing >= 0 && pointing < m_size && // It is valid
			curr > m_permutation[pointing]) // It is mobile
		{
			// It is the largest, untill now
			if (curr > largestMobile)
			{
				largestMobile = curr;
				largestIndex = i;
			}
		}
	}
	
	// No mobile = last permutation
	if (largestIndex < 0) return false;
	
	// Swap the largest mobile with what it is pointing
	std::swap(m_permutation[largestIndex], m_permutation[largestIndex + m_direction[largestIndex]]);
	std::swap(m_direction[largestIndex], m_direction[largestIndex + m_direction[largestIndex]]);
	
	for (int i = 0; i < m_size; ++i)
	{
		// If it is larger than swapped element,
		if (m_permutation[i] > largestMobile)
			m_direction[i] *= -1; // reverse the direction
	}
	
	return true;
}

const std::vector<int>& PermJohnsonTrotter::Get() const
{
	return m_permutation;
}