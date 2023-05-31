#include "ArcGraph.h"

ArcGraph::ArcGraph(int vertexCount)
{
	count = vertexCount;
}
ArcGraph::ArcGraph(const IGraph& graph)
{
	count = graph.VerticesCount();
	for (int i = 0; i < count; ++i) {
		std::vector<int> vertices =  graph.GetNextVertices(i);
		for (int j = 0; j < vertices.size(); ++j)
			adjPairs.push_back(std::make_pair(i, vertices[j]));
	}
}
void ArcGraph::AddEdge(int from, int to)
{
	assert(from >= 0 && from < count);
	assert(to >= 0 && to < count);

	adjPairs.push_back(std::make_pair(from, to));
}
int ArcGraph::VerticesCount() const
{
	return count;
}
std::vector<int> ArcGraph::GetNextVertices(int vertex) const
{
	assert(vertex >= 0 && vertex < count);

	std::vector<int> result;
	for (int i = 0; i < adjPairs.size(); ++i)
		if (adjPairs[i].first == vertex)
			result.push_back(adjPairs[i].second);
	return result;
}
std::vector<int> ArcGraph::GetPrevVertices(int vertex) const
{
	assert(vertex >= 0 && vertex < count);

	std::vector<int> result;
	for (int i = 0; i < adjPairs.size(); ++i)
		if (adjPairs[i].second == vertex)
			result.push_back(adjPairs[i].first);
	return result;
}