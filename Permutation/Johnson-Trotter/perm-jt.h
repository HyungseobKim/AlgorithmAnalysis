#ifndef PERM_JOHNSON_TROTTER_H
#define PERM_JOHNSON_TROTTER_H
#include <vector>

class PermJohnsonTrotter
{
public:
	PermJohnsonTrotter(int size);
	bool Next();
	const std::vector<int>& Get() const;
	
private:
	const int m_size;
	std::vector<int> m_permutation;
	std::vector<int> m_direction;
};
#endif
