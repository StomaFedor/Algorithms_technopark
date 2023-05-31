//﻿Требуется отыскать самый короткий маршрут между городами.Из города может выходить дорога, которая возвращается в этот же город.
//Требуемое время работы O((N + M)log N), где N – количество городов, M – известных дорог между ними.
//N ≤ 10000, M ≤ 250000.
//Длина каждой дороги ≤ 10000.

#include <cassert>
#include <iostream>
#include <vector>
#include <queue>

struct WaysCount
{
	bool isVisited = false;
	int count = 0;
};

struct QueueCmp
{
	bool operator()(std::pair<int, int> l, std::pair<int, int> r) { return l.first > r.first; }
};

class WeightListGraph {
public:
	WeightListGraph(int vertexCount);

	WeightListGraph(const WeightListGraph&) = delete;
	WeightListGraph(WeightListGraph&&) = delete;

	void AddEdge(int from, int to, int weight);
	int VerticesCount() const;
	std::vector<std::pair<int, int>> GetNextVerticesWithWeight(int vertex) const;
	std::vector<std::pair<int, int>> GetPrevVerticesWithWeight(int vertex) const;
	int Dijkstra(int from, int to) const;

private:
	std::vector<std::vector<std::pair<int, int>>> adjLists;
};

WeightListGraph::WeightListGraph(int vertexCount)
{
	adjLists.resize(vertexCount);
}

void WeightListGraph::AddEdge(int from, int to, int weight)
{
	assert(from >= 0 && from < adjLists.size());
	assert(to >= 0 && to < adjLists.size());

	adjLists[from].push_back(std::make_pair(to, weight));
	adjLists[to].push_back(std::make_pair(from, weight));
}

int WeightListGraph::VerticesCount() const
{
	return adjLists.size();
}

std::vector<std::pair<int, int>> WeightListGraph::GetNextVerticesWithWeight(int vertex) const
{
	assert(vertex >= 0 && vertex < adjLists.size());
	return adjLists[vertex];
}

std::vector<std::pair<int, int>> WeightListGraph::GetPrevVerticesWithWeight(int vertex) const
{
	assert(vertex >= 0 && vertex < adjLists.size());
	return adjLists[vertex];
}

int WeightListGraph::Dijkstra(int from, int to) const
{
	std::vector<WaysCount> visited(VerticesCount());
	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, QueueCmp> bfsQ;

	bfsQ.push(std::make_pair(0, from));
	visited[from].isVisited = true;
	while (bfsQ.size() > 0) {
		int current = bfsQ.top().second;
		bfsQ.pop();
		std::vector<std::pair<int, int>> adjList = GetNextVerticesWithWeight(current);
		for (int i = 0; i < adjList.size(); ++i) {
			if (!visited[adjList[i].first].isVisited) {
				bfsQ.push(std::make_pair(visited[current].count + adjList[i].second, adjList[i].first));
				visited[adjList[i].first].isVisited = true;
				visited[adjList[i].first].count = visited[current].count + adjList[i].second;
			}
			else if (visited[adjList[i].first].count > visited[current].count + adjList[i].second) {
				visited[adjList[i].first].count = visited[current].count + adjList[i].second;
				bfsQ.push(std::make_pair(visited[current].count + adjList[i].second, adjList[i].first));
			}
		}
	}
	return visited[to].count;
}

int main()
{
	int n, m;
	std::cin >> n >> m;
	WeightListGraph graph(n);
	for (int i = 0; i < m; ++i) {
		int first, second, time;
		std::cin >> first >> second >> time;
		graph.AddEdge(first, second, time);
	}
	int from, to;
	std::cin >> from >> to;
	std::cout << graph.Dijkstra(from, to);
}