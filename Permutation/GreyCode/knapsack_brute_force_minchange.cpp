#include "knapsack_brute_force_minchange.h"
//#include <iostream>
#include <strings.h>

////////////////////////////////////////////////////////////////////////////////
GreyCode::GreyCode(int s) : m_size(1 << s) {}

std::pair< bool, std::pair< bool, int > > GreyCode::Next() 
{
	// Find next gray code
	int half = m_order >> 1;
	int grayCode = m_order ^ half;
	int diff = grayCode ^ m_grayCode;
	
	// Save result
	m_grayCode = grayCode;
	++m_order;
	
    int  pos = 0;// which position is modified 
    bool add = diff & m_grayCode;// is true if new value is 1 (add item), false otherwise 
    bool last = m_order >= m_size;// is this the last permutation 
	
	while (diff > 1)
	{
		++pos;
		diff >>= 1;
	}
	
    return std::make_pair( !last, std::make_pair( add, pos ) );
}

////////////////////////////////////////////////////////////////////////////////
std::vector<bool> knapsack_brute_force( std::vector<Item> const& items, Weight const& W )
{
	const int size = items.size();
	
	// Save values for each iteration
	std::vector<bool> bag(size, false);
	Weight totalWeights;
	int totalValues = 0;
	
	// Save the best so far
	std::vector<bool> best(size);
	int maxValues = 0;
	
	GreyCode grayCode(size);
	
	bool go = true;
	while (go)
	{
		std::pair<bool, std::pair<bool, int>> change = grayCode.Next();
		go = change.first;
		
		int pos = change.second.second;
		const Item& item = items[pos];
		if (change.second.first) // Added
		{
			totalWeights += item.GetWeight();
			totalValues += item.GetValue();
			bag[pos] = true;
		}
		else // Removed
		{
			totalWeights -= items[pos].GetWeight();
			totalValues -= item.GetValue();
			bag[pos] = false;
		}
		
		// Check and change the best
		if (totalValues > maxValues && totalWeights <= W)
		{
			maxValues = totalValues;
			best = bag;
		}
	}
	
	return best;
}