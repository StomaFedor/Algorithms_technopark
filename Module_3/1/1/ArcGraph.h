#pragma once
#include "IGraph.h"

class ArcGraph : public IGraph {
public:
	ArcGraph(int vertexCount);
	ArcGraph(const IGraph& graph);

	void AddEdge(int from, int to) override;
	int VerticesCount() const override;
	std::vector<int> GetNextVertices(int vertex) const override;
	std::vector<int> GetPrevVertices(int vertex) const override;

private:
	std::vector<std::pair<int, int>> adjPairs;
	int count;
};