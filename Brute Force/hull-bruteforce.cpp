#include "hull-bruteforce.h"
#include <algorithm>
#include <iostream>

bool Point::operator==( Point const& arg2 ) const {
    return ( (x==arg2.x) && (y==arg2.y) );
}

std::ostream& operator<< (std::ostream& os, Point const& p) {
	os << "(" << p.x << " , " << p.y << ") ";
	return os;
}

std::istream& operator>> (std::istream& os, Point & p) {
	os >> p.x >> p.y;
	return os;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//return value is (on left, on right)
std::pair<bool,bool> get_location(
		float const& p1x, //check which side of the line (p1x,p1y)-->(p2x,p2y) 
		float const& p1y, //point (qx,qy) is on
		float const& p2x,
		float const& p2y,
		float const& qx,
		float const& qy
		) 
{
	Point dir   = {p2x-p1x,p2y-p1y};
	Point norm  = {dir.y, -dir.x};
	Point point = {qx-p1x,qy-p1y};
	//scalar product is positive if on right side
	float scal_prod = norm.x*point.x + norm.y*point.y;
	return std::make_pair( (scal_prod<0), (scal_prod>0));
}

bool IsEdge(int index1, int index2, const std::vector<Point>& points)
{
	const int size = points.size();
	
	const Point& p1 = points[index1];
	const Point& p2 = points[index2];
	
	const Point& test = points[(index2+1)%size];
	const bool standard = get_location(p1.x, p1.y, p2.x, p2.y, test.x, test.y).first;
	
	for (int i = 0; i < size; ++i)
	{
		if (i == index1 || i == index2) continue;
		
		const Point& curr = points[i];
		if (get_location(p1.x, p1.y, p2.x, p2.y, curr.x, curr.y).first != standard)
			return false;
	}
	
	return true;
}

//returns a set of indices of points that form convex hull
std::set<int> hullBruteForce ( std::vector< Point > const& points ) {
	int num_points = points.size();
	//std::cout << "number of points " << num_points << std::endl;
	if ( num_points < 3 ) throw "bad number of points";

	std::set<int> hull_indices;
	
	const int size = points.size();
	for (int index1 = 0; index1 < size-1; ++index1)
	{
		for (int index2 = index1+1; index2 < size; ++index2)
		{
			if (IsEdge(index1, index2, points))
			{
				hull_indices.insert(index1);
				hull_indices.insert(index2);
			}
		}
	}
	
	return hull_indices;
}

int FindStartPoint(const std::vector<Point>& points)
{
	const int size = points.size();
	int min = 0;
	
	for (int i = 1; i < size; ++i)
		if (points[i].x < points[min].x)
			min = i;
	
	return min;
}

bool AllLeftSide(int index1, int index2, const std::vector<Point>& points)
{
	const int size = points.size();
	
	const Point& p1 = points[index1];
	const Point& p2 = points[index2];
	
	const bool standard = true;
	
	for (int i = 0; i < size; ++i)
	{
		if (i == index1 || i == index2) continue;
		
		const Point& curr = points[i];
		if (get_location(p1.x, p1.y, p2.x, p2.y, curr.x, curr.y).first != standard)
			return false;
	}
	
	return true;
}

std::vector<int> hullBruteForce2 ( std::vector< Point > const& points ) {
	int num_points = points.size();
	if ( num_points < 3 ) throw "bad number of points";

	std::vector<int> hull_indices;
	
	const int size = points.size();
	const int start = FindStartPoint(points);
	
	hull_indices.push_back(start);
	int index1 = start;
	
	do
	{
		for (int index2 = index1+1; index2 < index1+size; ++index2)
		{
			int index = index2%size;
			if (AllLeftSide(index1, index, points))
			{
				const int previous = hull_indices.size()-2;
				if (previous > 0 && index == hull_indices[previous]) continue;
				
				hull_indices.push_back(index);
				index1 = index;
				break;
			}
		}
	}while(index1 != start);
	
	hull_indices.pop_back();
	return hull_indices;
}
