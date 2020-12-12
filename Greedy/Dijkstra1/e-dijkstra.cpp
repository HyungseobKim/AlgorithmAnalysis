#include "e-dijkstra.h"
#include <fstream> // ifstream
#include <vector> // std::vector
#include <algorithm> // std::find_if, std::swap

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
	int iteration = -1;

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
		int pos = m_heap.size() - 1;
		while (m_heap[pos]->location != node->location)
			--pos;

		int posInsert = pos;
		while (posInsert > m_top)
		{
			Node*& prev = m_heap[posInsert - 1];

			if (prev->status != Node::Status::OnHeap || prev->charge < node->charge)
				--posInsert;
			else
				break;
		}

		if (posInsert != pos)
		{
			m_heap.erase(m_heap.begin() + pos);
			m_heap.insert(m_heap.begin() + posInsert, node);
		}

		if (node->status != Node::Status::OnHeap)
		{
			node->status = Node::Status::OnHeap;
			++m_size;
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

bool e_dijkstra(char const* filename, int range)
{
	g_range = range;
	
	int totalLocations;
	int maxRecharge;
	int totalEdges;

	std::ifstream in(filename, std::ifstream::in);
	in >> totalLocations;
	in >> maxRecharge;
	in >> totalEdges;

	std::vector<Node> map(totalLocations);
	Heap heap(totalLocations);

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

	for (int source = 0; source < totalLocations; ++source)
	{
		Node& start = map[source];
		start.status = Node::Status::OnHeap;
		start.charge = Charge(range, maxRecharge - 1);
		start.iteration = source;
		heap.setStart(source);

		while (!heap.empty())
		{
			Node* curr = heap.pop();

			curr->status = Node::Status::Evaluated;
			const Charge& currCharge = curr->charge;

			for (const Edge& edge : curr->neighbors)
			{
				Node& neighbor = map[edge.first];
				if (neighbor.iteration == source && neighbor.status == Node::Status::Evaluated) continue;
				
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

				if (neighbor.iteration != source || neighbor.charge < newCharge)
				{
					// Update
					neighbor.iteration = source;
					neighbor.charge = newCharge;
					heap.decreaseKey(&neighbor);
				}
			}
		}

		for (const Node& node : map)
		{
			if (node.iteration != source)
				return false;
			else if (node.status != Node::Status::Evaluated)
				return false;
		}
	}

	return true;
}