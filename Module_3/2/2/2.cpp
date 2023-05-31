//Дан невзвешенный неориентированный граф.В графе может быть несколько кратчайших путей между какими - то вершинами.
//Найдите количество различных кратчайших путей между заданными вершинами.Требуемая сложность O(V + E).

#include <queue>
#include <cassert>
#include <iostream>
#include <vector>

struct WaysCount
{
	bool isVisited = false;
	int count = 0;
	int wave = 0;
};

class UndirectedListGraph {
public:
	UndirectedListGraph(int vertexCount);
	UndirectedListGraph(const UndirectedListGraph& graph);
	UndirectedListGraph(UndirectedListGraph&& graph) = delete;

	void AddEdge(int from, int to);
	int VerticesCount() const;
	std::vector<int> GetNextVertices(int vertex) const;
	std::vector<int> GetPrevVertices(int vertex) const;
	int GetWaysNum(int from, int to) const;

private:
	std::vector<std::vector<int>> adjLists;
};

UndirectedListGraph::UndirectedListGraph(int vertexCount)
{
	adjLists.resize(vertexCount);
}
UndirectedListGraph::UndirectedListGraph(const UndirectedListGraph& graph)
{
	adjLists.resize(graph.VerticesCount());
	for (int i = 0; i < graph.VerticesCount(); ++i) {
		adjLists[i] = graph.GetNextVertices(i);
	}
}

void UndirectedListGraph::AddEdge(int from, int to)
{
	assert(from >= 0 && from < adjLists.size());
	assert(to >= 0 && to < adjLists.size());

	adjLists[from].push_back(to);
	adjLists[to].push_back(from);
}
int UndirectedListGraph::VerticesCount() const
{
	return adjLists.size();
}
std::vector<int> UndirectedListGraph::GetNextVertices(int vertex) const
{
	assert(vertex >= 0 && vertex < adjLists.size());
	return adjLists[vertex];
}
std::vector<int> UndirectedListGraph::GetPrevVertices(int vertex) const
{
	assert(vertex >= 0 && vertex < adjLists.size());
	return adjLists[vertex];
}
int UndirectedListGraph::GetWaysNum(int from, int to) const
{
	std::vector<WaysCount> visited(VerticesCount());
	std::queue<int> bfsQ;
	bfsQ.push(from);
	visited[from].isVisited = true;
	visited[from].count = 1;
	while (bfsQ.size() > 0) {
		int current = bfsQ.front();
		bfsQ.pop();
		if (current == to)
			return visited[to].count;
		std::vector<int> adjList = GetNextVertices(current);
		for (int i = 0; i < adjList.size(); ++i) {
			if (!visited[adjList[i]].isVisited) {
				bfsQ.push(adjList[i]);
				visited[adjList[i]].isVisited = true;
				visited[adjList[i]].wave += visited[current].wave + 1;
				visited[adjList[i]].count += visited[current].count;
			} else if (visited[adjList[i]].wave == visited[current].wave + 1)
				visited[adjList[i]].count += visited[current].count;
		}
	}
	return 0;
}

int main()
{
	int v, n;
	std::cin >> v >> n;
	UndirectedListGraph graph(v);
	for (int i = 0; i < n; ++i) {
		int first, second;
		std::cin >> first >> second;
		graph.AddEdge(first, second);
	}
	int from, to;
	std::cin >> from >> to;
    std::cout << graph.GetWaysNum(from, to);
}