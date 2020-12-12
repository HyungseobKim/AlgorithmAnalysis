#include "closestpair.h"
#include <algorithm>
#include <limits>
#include <cmath>
#include <iostream>
#include <utility>

std::ostream& operator<< (std::ostream& os, Point const& p) {
	os << "(" << p.x << " , " << p.y << ") ";
	return os;
}

std::istream& operator>> (std::istream& os, Point & p) {
	os >> p.x >> p.y;
	return os;
}

////////////////////////////////////////////////////////////////////////////////
float closestPair_aux (const std::vector<Point>& points, std::vector<int>& indices, int index, int size, bool xOrder);

////////////////////////////////////////////////////////////////////////////////
float closestPair (std::vector< Point > const& points) {
	int size = points.size();

	if (size==0) throw "zero size subset";
	if (size==1) return std::numeric_limits<float>::max();
	
	std::vector<int> indices(size);
	for (int i = 0; i < size; ++i)
		indices[i] = i;
	
	std::sort(indices.begin(), indices.end(), [&](int lhs, int rhs) {
		return points[lhs].x < points[rhs].x;
	});
	
	bool xOrder = true;
	if (points[indices[0]].x == points[indices[size-1]].x)
	{
		std::sort(indices.begin(), indices.end(), [&](int lhs, int rhs) {
			return points[lhs].y < points[rhs].y;
		});
		xOrder = false;
	}

	return closestPair_aux(points, indices, 0, size, xOrder);
}

////////////////////////////////////////////////////////////////////////////////
float Distance(const Point& p1, const Point& p2)
{
	float xdiff = p1.x - p2.x;
	float ydiff = p1.y - p2.y;
	return std::sqrt(xdiff*xdiff + ydiff*ydiff);
}

float closestPair_aux (const std::vector<Point>& points, std::vector<int>& indices, int index, int size, bool xOrder) {
	
	if (size == 3) return std::min(Distance(points[indices[index]], points[indices[index+1]]), 
								   std::min(Distance(points[indices[index+1]], points[indices[index+2]]), Distance(points[indices[index]], points[indices[index+2]]))
								   );
	if (size == 2) return Distance(points[indices[index]], points[indices[index+1]]);
	if (size == 1) return std::numeric_limits<float>::max();
	if (size == 0) throw "zero size subset";

	const int median = index + size/2;
	
	float min_dist = closestPair_aux(points, indices, index, size/2, xOrder);
	float d2 = closestPair_aux(points, indices, median, size - size/2, xOrder);
	if (d2 < min_dist) min_dist = d2;
	
	const int last = index + size;
	
	if (xOrder)
	{
		const float medVal = points[indices[median]].x;
		
		for (int pL = index; pL < median; ++pL)
		{
			if (std::abs(points[indices[pL]].x - medVal) >= min_dist) continue;
			for (int pR = median; pR < last; ++pR)
			{
				if (std::abs(points[indices[pR]].x - medVal) >= min_dist) continue;
				
				const float distance = Distance(points[indices[pL]], points[indices[pR]]);
				if (distance < min_dist) min_dist = distance;
			}
		}
	}
	else
	{
		const float medVal = points[indices[median]].y;
		
		for (int pL = index; pL < median; ++pL)
		{
			if (std::abs(points[indices[pL]].y - medVal) >= min_dist) continue;
			for (int pR = median; pR < last; ++pR)
			{
				if (std::abs(points[indices[pR]].y - medVal) >= min_dist) continue;
				
				const float distance = Distance(points[indices[pL]], points[indices[pR]]);
				if (distance < min_dist) min_dist = distance;
			}
		}
	}
	
	return min_dist;
}

