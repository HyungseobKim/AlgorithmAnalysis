#include "lis.h"

std::vector<unsigned>
longest_increasing_subsequence(std::vector<int> const& sequence) {
	//allocated/reserve table, init all to 0 - including init case
	const int size = sequence.size();
	std::vector<unsigned> table(size + 1, 1);
	std::vector<unsigned> parent(size + 1, 0);

	table[0] = 0;

	unsigned longest = 1;
	unsigned longest_index = 1;

	//main logic
	for (int index = 2; index <= size; ++index)
	{
		const int curr_num = sequence[index - 1];

		// From this number to start
		for (int i = index - 2; i >= 0; --i)
		{
			// If smaller number is founded
			if (sequence[i] < curr_num)
			{
				// If it is best so far
				unsigned length = table[i + 1];
				if (length >= table[index])
				{
					unsigned new_lenght = length + 1;
					table[index] = new_lenght;
					parent[index] = i + 1;

					// If it is the longest so far
					if (new_lenght > longest)
					{
						longest = new_lenght;
						longest_index = index;
					}
				}
			}
		}
	}

	//reconstruct subsequence
	std::vector<unsigned> answer(longest); //vector of indices corresponding to the LIS

	for (int i = longest - 1; i >= 0; --i)
	{
		answer[i] = longest_index - 1;
		longest_index = parent[longest_index];
	}

	return answer;
}
