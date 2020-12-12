#include "tsp.h"
#include <limits>  // numeric_limits
#include <algorithm> // std::min_element
#include <fstream> // ifstream
#include <numeric> // std::accumulate
#include <iostream>

#define MAX std::numeric_limits<int>::max()
using Path = std::pair<std::vector<int>, int>;

void ConstructMap(char const* filename, MAP& map, int& TotalCity)
{
	std::ifstream in(filename, std::ifstream::in);
	in >> TotalCity;
	for (int i = 0; i < TotalCity; ++i) {
		std::vector<int> row(TotalCity);
		for (int j = 0; j < TotalCity; ++j) {
			row[i] = MAX;
		}
		map.push_back(row);
	}
	int cost;
	for (int i = 0; i < TotalCity; ++i) {
		for (int j = i + 1; j < TotalCity; ++j) {
			in >> cost;
			map[i][j] = cost;
			map[j][i] = cost;
		}
	}
}

void SolveRecursive(const MAP& map, const int total, int depth, Path path, Path& bestSoFar, const std::vector<int>& lowests, int h)
{
	auto& cities = path.first;

	if (depth == total - 1)
	{
		path.second += map[cities[depth]][0];
		if (path.second >= bestSoFar.second) return;

		cities.push_back(0);
		bestSoFar.first = cities;
		bestSoFar.second = path.second;
		return;
	}

	if (path.second + h >= bestSoFar.second) return;

	++depth;
	const int size = cities.size();
	const int last = cities[depth - 1];
	
	const auto& destinations = map[last];
	
	std::sort(cities.begin() + depth, cities.end(), [&](int left, int right) {
		return destinations[left] - lowests[left] < destinations[right] - lowests[right]; 
		});
	
	int g;
	int next;
	int lowest;
	for (int i = depth; i < size; ++i)
	{
		next = cities[i];
		g = path.second + map[last][next];
		
		lowest = lowests[next];
		h -= lowest;
		std::swap(cities[depth], cities[i]);

		SolveRecursive(map, total, depth, Path(cities, g), bestSoFar, lowests, h);

		h += lowest;
		std::swap(cities[depth], cities[i]);
	}
}

std::vector<int> SolveTSP(char const* filename)
{
	MAP map;
	int total;
	ConstructMap(filename, map, total);

	Path bestSoFar(std::vector<int>(total), MAX);

	Path path(std::vector<int>(total), 0);
	auto& cities = path.first;
	for (int i = 0; i < total; ++i)
		cities[i] = i;

	std::vector<int> lowests(total);
	for (int i = 0; i < total; ++i)
	{
		const auto& row = map[i];
		lowests[i] = *std::min_element(row.begin(), row.end());
	}

	int h = std::accumulate(lowests.begin(), lowests.end(), 0);

	SolveRecursive(map, total, 0, path, bestSoFar, lowests, h);

	return bestSoFar.first;
}