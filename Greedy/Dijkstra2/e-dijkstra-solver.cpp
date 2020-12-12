#include "e-dijkstra-solver.h"
#include <fstream> // ifstream
#include <vector> // std::vector
#include <algorithm> // std::find_if, std::swap
#include <limits> // std::numeric_limits

#include <iostream>

using Charge = std::pair<int, int>;
using Edge = std::pair<int, int>;

int g_range = 0;

bool operator<(const Charge& lhs, const Charge& rhs)
{
	return lhs.first + lhs.second * g_range < rhs.first + rhs.second * g_range;
}

bool operator>(const Charge& lhs, const Charge& rhs)
{
	return rhs < lhs;
}

struct Node
{
	enum class Status
	{
		Default,
		OnHeap,
		Evaluated
	};

	Status status = Status::Default;
	int location = -1;
	unsigned int iteration = 0;

	Charge charge = Charge(-1, -1);
	std::vector<Edge> neighbors{};
};

class Heap
{
public:
	Heap(int size)
		: m_heap(std::vector<Node*>(size))
	{}

	Node* pop()
	{
		--m_size;
		return m_heap[m_top++];
	}

	void push(Node* node)
	{
		m_heap[m_size] = node;
		++m_size;
	}

	void decreaseKey(Node* node)
	{
		const int location = node->location;
		int pos = 0;

		if (node->status == Node::Status::OnHeap)
		{
			pos = m_top + m_size - 1;
			while (m_heap[pos]->location != location)
				--pos;
		}
		else // It was not on heap
		{
			pos = m_heap.size() - 1;
			while (m_heap[pos]->location != location)
				--pos;

			std::swap(m_heap[pos], m_heap[m_top + m_size]);
			pos = m_top + m_size;

			node->status = Node::Status::OnHeap;
			++m_size;
		}

		const Charge charge = node->charge;
		while (pos > m_top)
		{
			Node*& prev = m_heap[pos - 1];

			if (prev->charge < charge)
			{
				std::swap(m_heap[pos], prev);
				--pos;
			}
			else
				break;
		}
	}

	void setStart(int location)
	{
		auto pos = std::find_if(m_heap.begin(), m_heap.end(), [&](Node* p) { return p->location == location; });
		std::swap(*pos, *m_heap.begin());

		m_top = 0;
		m_size = 1;
	}

	bool empty() { return !m_size; }

private:
	int m_top = 0;
	int m_size = 0;
	std::vector<Node*> m_heap;
};

using Map = std::vector<Node>;

bool e_dijkstra(Map& map, Heap& heap, int totalLocations, int maxRecharge, int range, unsigned int& iteration);

int e_dijkstra_solver(char const* filename)
{
	int totalLocations;
	int maxRecharge;
	int totalEdges;

	std::ifstream in(filename, std::ifstream::in);
	in >> totalLocations;
	in >> maxRecharge;
	in >> totalEdges;

	Map map(totalLocations);
	Heap heap(totalLocations);

	int rangeMin = std::numeric_limits<int>::max();
	int rangeMax = 0;

	for (int i = 0; i < totalEdges; ++i)
	{
		int begin;
		in >> begin;

		int destination;
		in >> destination;

		int weight;
		in >> weight;

		Node& beginNode = map[begin];
		beginNode.location = begin;
		beginNode.neighbors.push_back(Edge(destination, weight));

		Node& destinationNode = map[destination];
		destinationNode.location = destination;
		destinationNode.neighbors.push_back(Edge(begin, weight));
	}

	for (Node& node : map)
		heap.push(&node);

	// Find the smallest edge of each location, and then choose the biggest two among them
	rangeMin = 0;
	int rangeMin2 = 0;
	for (const Node& node : map)
	{
		int minWeight = std::numeric_limits<int>::max();
		const auto& neighbors = node.neighbors;
		for (const Edge& edge : neighbors)
		{
			if (minWeight > edge.second)
				minWeight = edge.second;
		}
		if (rangeMin < minWeight)
			rangeMin = minWeight;
		else if (rangeMin2 < minWeight)
			rangeMin2 = minWeight;
	}

	// If all locations are not neighbor of all other location, there must be a path passing at least two edges
	if (totalEdges < totalLocations * (totalLocations + 1) / 2)
		rangeMin += rangeMin2;
	// It is total needed electricity, so divide by maximum number of recharging
	rangeMin /= maxRecharge;

	unsigned int iteration = 0;

	// Find maximum range which test result is true firstly.
	rangeMax = rangeMin * 3;
	while (!e_dijkstra(map, heap, totalLocations, maxRecharge, rangeMax, iteration))
	{
		rangeMax *= 2;
		++iteration;
	}
	if (rangeMax/3 == rangeMin)
		rangeMin = rangeMax / 3;
	else
		rangeMin = rangeMax / 2;

	// Binary Search
	while (rangeMin != rangeMax)
	{
		int middle = (rangeMin + rangeMax) / 2;

		if (e_dijkstra(map, heap, totalLocations, maxRecharge, middle, iteration))
			rangeMax = middle;
		else
		{
			if (rangeMin + 1 == rangeMax)
				return rangeMax;
			rangeMin = middle;
		}
		++iteration;
	}

	return rangeMax;
}

bool e_dijkstra(Map& map, Heap& heap, int totalLocations, int maxRecharge, int range, unsigned int& iteration)
{
	g_range = range;

	for (int source = 0; source < totalLocations; ++source)
	{
		Node& start = map[source];
		start.status = Node::Status::OnHeap;
		start.charge = Charge(range, maxRecharge - 1);
		start.iteration = iteration;
		heap.setStart(source);

		while (!heap.empty())
		{
			Node* curr = heap.pop();

			curr->status = Node::Status::Evaluated;
			const Charge& currCharge = curr->charge;

			for (const Edge& edge : curr->neighbors)
			{
				Node& neighbor = map[edge.first];
				if (neighbor.iteration == iteration && neighbor.status == Node::Status::Evaluated) continue;

				int weight = edge.second;
				if (weight > range) continue; // unreachable

				Charge newCharge = currCharge;
				if (weight <= newCharge.first) // do not need recharge
					newCharge.first -= weight;
				else if (newCharge.second > 0) // do recharge
				{
					newCharge.first = range - weight;
					--newCharge.second;
				}
				else // need recharge, but can't = unreachable
					continue;

				if (neighbor.iteration != iteration || neighbor.charge < newCharge)
				{
					// Update
					neighbor.iteration = iteration;
					neighbor.charge = newCharge;
					heap.decreaseKey(&neighbor);
				}
			}
		}

		for (const Node& node : map)
		{
			if (node.iteration != iteration)
				return false;
			else if (node.status != Node::Status::Evaluated)
				return false;
		}

		++iteration;
	}

	return true;
}