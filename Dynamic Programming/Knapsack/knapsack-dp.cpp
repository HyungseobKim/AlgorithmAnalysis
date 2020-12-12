#include "knapsack-dp.h"
#include <iostream>
#include <numeric>

////////////////////////////////////////////////////////////
Item::Item( int const& weight, int const& value ) 
	: weight(weight), value(value) 
{
}

////////////////////////////////////////////////////////////
Item::Item( Item const& original ) 
	: weight(original.weight), value(original.value)
{
}

////////////////////////////////////////////////////////////
std::ostream& operator<< (std::ostream& os, Item const& item) {
	os << "(" << item.weight << " , " << item.value << ") ";
	return os;
}

////////////////////////////////////////////////////////////
std::istream& operator>> (std::istream& os, Item & item) {
	os >> item.weight >> item.value;
	return os;
}

////////////////////////////////////////////////////////////
typedef std::vector< std::vector<int> > Table; //2-dimensional table

////////////////////////////////////////////////////////////
//the returned value is a vector of indices
std::vector<int> knapsackDP( std::vector<Item> const& items, int const& W ) {
	int num_items = items.size();

    Table table(W + 1);
	for (auto& row : table)
		row = std::vector<int>(num_items + 1, 0);
	
	for (int w = 1; w <= W; ++w)
	{
		auto& row = table[w];
		for (int i = 1; i <= num_items; ++i)
		{
			const Item& item = items[i-1];
			
			const int diagonal = w - item.weight;
			if (diagonal >= 0)
				row[i] = std::max(row[i-1], table[diagonal][i-1] + item.value);
			else
				row[i] = row[i-1];
		}
	}

	//print final table - for debugging?
    //do not delete this code
    if ( num_items + W < 50 ) { //print only if table is not too big
        std::cout << "   ";
        for ( int n=0; n<=num_items; ++n) { std::cout << n << "     "; }
        std::cout << "  items\n        ";
        for ( int n=0; n<num_items; ++n) { std::cout << items[n].weight << "," << items[n].value<<"   "; }
        std::cout << "\n   ";
        for ( int n=0; n<=num_items; ++n) { std::cout << "------"; }
        std::cout << std::endl;

        for ( int w=0; w<=W; ++w) {
            std::cout << w << "| ";
            for ( int n=0; n<=num_items; ++n) {
                std::cout << table[w][n] << "     ";
            }
            std::cout << std::endl;
        }
    }
    //end do not delete this code 


	//figure out which items are in the bag based on the table
	std::vector<int> bag;
    
	for (int i = num_items, w = W; i > 0 && w > 0; --i)
	{
		const auto& row = table[w];
		const int index = i - 1;
		if (row[i] != row[index]) // It was taken
		{
			bag.push_back(index);
			w -= items[index].weight;
		}
	}
	
	return bag;
}

////////////////////////////////////////////////////////////
int valueBag( std::vector<Item> const& items, std::vector<int> const& bag ) {
	std::vector<int>::const_iterator it   = bag.begin(),
		                          it_e = bag.end();

	int accum = 0;
	//std::cout << "Bag ";
	for ( ; it != it_e; ++it) { 
		accum += items[ *it ].value; 
		//std::cout << *it << " ";
	}
	//std::cout << std::endl;
	return accum;
}

////////////////////////////////////////////////////////////
//prototype
//notice that auxiliary function returns value of the vector of items
//the actual vector is determined later from the table (similar to DP solution)
int knapsackRecMemAux( std::vector<Item> const& items, Table& table, int w, int i);

////////////////////////////////////////////////////////////
//function to kick start
std::vector<int> knapsackRecMem( std::vector<Item> const& items, int const& W ) {
	int num_items = items.size();

    Table table(W+1);
	table[0] = std::vector<int>(num_items + 1, 0);
	
	for (int i = 1; i <= W; ++i)
	{
		auto& row = table[i];
		
		row = std::vector<int>(num_items + 1, -1);
		row[0] = 0;
	}
	
	table[W][num_items] = knapsackRecMemAux(items, table, W, num_items);

	//print table - debugging?
    //do not delete this code
    if ( num_items + W < 50 ) { //print only if table is not too big
        std::cout << "   ";
        for ( int n=0; n<=num_items; ++n) { std::cout << n << "     "; }
        std::cout << "  items\n        ";
        for ( int n=0; n<num_items; ++n) { 
            std::cout << items[n].weight << "," << items[n].value<<"   "; 
        }
        std::cout << "\n   ";
        for ( int n=0; n<=num_items; ++n) { std::cout << "------"; }
        std::cout << std::endl;

        for ( int w=0; w<=W; ++w) {
            std::cout << w << "| ";
            for ( int n=0; n<=num_items; ++n) {
                std::cout << table[w][n] << "     ";
            }
            std::cout << std::endl;
        }
    }
    //end do not delete this code 

	//figure out which items are in the bag based on the table
	std::vector<int> bag;
    
	for (int i = num_items, w = W; i > 0 && w > 0; --i)
	{
		const auto& row = table[w];
		const int index = i - 1;
		if (row[i] != row[index]) // It was taken
		{
			bag.push_back(index);
			w -= items[index].weight;
		}
	}
	
	return bag;
}

////////////////////////////////////////////////////////////
//the real recursive function
int knapsackRecMemAux( std::vector<Item> const& items, Table& table, int w, int i) {
    
	// If we already computed this value, then do not compute again
	int curr = table[w][i];
	if (curr >= 0) return curr;
	
	const Item& item = items[i-1];
	
	const int not_take = knapsackRecMemAux(items, table, w, i-1);
	
	if (w >= item.weight)
		table[w][i] = std::max(not_take, knapsackRecMemAux(items, table, w - item.weight, i-1) + item.value);
	else
		table[w][i] = not_take;
	
	return table[w][i];
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
