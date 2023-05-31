#include "MatrixGraph.h"

MatrixGraph::MatrixGraph(int vertexCount)
{
	adjMatrix.resize(vertexCount);
	for (int i = 0; i < vertexCount; ++i)
		adjMatrix[i].resize(vertexCount);
}

MatrixGraph::MatrixGraph(const IGraph& graph) : MatrixGraph(graph.VerticesCount())
{
	for (int i = 0; i < graph.VerticesCount(); ++i) {
		std::vector<int> vertices = graph.GetNextVertices(i);
		for (int j = 0; j < vertices.size(); ++j)
			adjMatrix[i][vertices[j]] = true;
	}
}

void MatrixGraph::AddEdge(int from, int to)
{
	assert(from >= 0 && from < adjMatrix.size());
	assert(to >= 0 && to < adjMatrix.size());

	adjMatrix[from][to] = true;
}

int MatrixGraph::VerticesCount() const
{
	return adjMatrix.size();
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const
{
	assert(vertex >= 0 && vertex < adjMatrix.size());
	std::vector<int> result;
	for (int i = 0; i < adjMatrix.size(); ++i)
		if (adjMatrix[vertex][i] == true)
			result.push_back(i);
	return result;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const
{
	assert(vertex >= 0 && vertex < adjMatrix.size());
	std::vector<int> result;
	for (int i = 0; i < adjMatrix.size(); ++i)
		if (adjMatrix[i][vertex] == true)
			result.push_back(i);
	return result;
}