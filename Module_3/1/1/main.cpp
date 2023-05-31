//Необходимо написать несколько реализаций интерфейса :
//ListGraph, хранящий граф в виде массива списков смежности,
//MatrixGraph, хранящий граф в виде матрицы смежности,
//SetGraph, хранящий граф в виде массива хэш - таблиц / сбалансированных деревьев поиска,
//ArcGraph, хранящий граф в виде одного массива пар{ from, to }.
//Также необходимо реализовать конструктор, принимающий const IGraph& .Такой конструктор должен скопировать переданный граф в создаваемый объект.
//Для каждого класса создавайте отдельные h и cpp файлы.
//Число вершин графа задается в конструкторе каждой реализации.

#pragma once
#include "ListGrath.h"
#include "MatrixGraph.h"
#include "SetGraph.h"
#include "ArcGraph.h"

void BFS(const IGraph& graph, int vertex, void (*visit)(int))
{
	std::vector<bool> visited(graph.VerticesCount(), false);
	std::queue<int> bfsQ;
	bfsQ.push(vertex);
	visited[vertex] = true;
	while (bfsQ.size() > 0) {
		int current = bfsQ.front();
		bfsQ.pop();
		visit(current);
		std::vector<int> adjList = graph.GetNextVertices(current);
		for (int i = 0; i < adjList.size(); ++i) {
			if (!visited[adjList[i]]) {
				bfsQ.push(adjList[i]);
				visited[adjList[i]] = true;
			}
		}
	}
}

int main()
{
	IGraph* listGraph = new ListGraph(5);
	listGraph->AddEdge(1, 2);
	listGraph->AddEdge(2, 3);
	listGraph->AddEdge(2, 4);
	listGraph->AddEdge(4, 3);
	listGraph->AddEdge(4, 1);
	std::cout << "List Graph" << std::endl;
	BFS(*listGraph, 4, [](int vertex) {  std::cout << vertex << " "; });
	std::cout << std::endl;

	IGraph* matrixGrath = new MatrixGraph(*listGraph);
	delete listGraph;
	std::cout << "Matrix Graph" << std::endl;
	BFS(*matrixGrath, 4, [](int vertex) {  std::cout << vertex << " "; });
	std::cout << std::endl;

	IGraph* setGrath = new SetGraph(*matrixGrath);
	delete matrixGrath;
	std::cout << "Set Graph" << std::endl;
	BFS(*setGrath, 4, [](int vertex) {  std::cout << vertex << " "; });
	std::cout << std::endl;

	IGraph* arcGrath = new ArcGraph(*setGrath);
	delete setGrath;
	std::cout << "Arc Graph" << std::endl;
	BFS(*arcGrath, 4, [](int vertex) {  std::cout << vertex << " "; });
	std::cout << std::endl;
	delete arcGrath;

	return 0;
}