#include "SetGraph.h"

SetGraph::SetGraph(int vertexCount)
{
	adjSet.resize(vertexCount);
	prevAdjSet.resize(vertexCount);
}
SetGraph::SetGraph(const IGraph& graph)
{
	adjSet.resize(graph.VerticesCount());
	prevAdjSet.resize(graph.VerticesCount());
	for (int i = 0; i < graph.VerticesCount(); ++i) {
		std::vector<int> nextVertices = graph.GetNextVertices(i);
		std::vector<int> prevVertices = graph.GetPrevVertices(i);
		for (int j = 0; j < nextVertices.size(); ++j)
			adjSet[i].insert(nextVertices[j]);
		for (int j = 0; j < prevVertices.size(); ++j)
			prevAdjSet[i].insert(prevVertices[j]);
	}
}
void SetGraph::AddEdge(int from, int to)
{
	assert(from >= 0 && from < adjSet.size());
	assert(to >= 0 && to < adjSet.size());

	adjSet[from].insert(to);
	prevAdjSet[to].insert(from);
}
int SetGraph::VerticesCount() const
{
	return adjSet.size();
}
std::vector<int> SetGraph::GetNextVertices(int vertex) const
{
	assert(vertex >= 0 && vertex < adjSet.size());
	std::vector<int> result;
	for (int number : adjSet[vertex])
		result.push_back(number);
	return result;
}
std::vector<int> SetGraph::GetPrevVertices(int vertex) const
{
	assert(vertex >= 0 && vertex < adjSet.size());
	std::vector<int> result;
	for (int number : prevAdjSet[vertex])
		result.push_back(number);
	return result;
}