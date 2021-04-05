#include <iostream>
#include <vector>
#include <string>
using namespace std;
class MatrixGraph {
public:
	bool** adjacencyMatrix; 
	int vertex_count;

	MatrixGraph(int vertex_count) {
		this->vertex_count = vertex_count;
		adjacencyMatrix = new bool*[vertex_count];
		for (size_t i = 0; i < vertex_count; i++)
			adjacencyMatrix[i] = new bool[vertex_count]();
	}

	~MatrixGraph()
	{
		for (size_t i = 0; i < this->vertex_count; i++)
		{
			delete[] adjacencyMatrix[i];
		}
		delete[] adjacencyMatrix;
	}

	
	MatrixGraph* randomize(float p) {
		srand(time(NULL));
		for (size_t i = 1; i < this->vertex_count; i++)
			for (size_t j = 0; j < i; j++) {
				//generate random number between 0 to 1. If the number is smaller than p, we add the edge.
				adjacencyMatrix[j][i] = adjacencyMatrix[i][j] = ((float)rand() / RAND_MAX) < p;
			}

		return this;
	}

	friend std::ostream& operator<<(std::ostream& os, const MatrixGraph& dt);
};



std::ostream& operator<<(std::ostream& os, const MatrixGraph& graph)
{
	for (size_t i = 0; i < graph.vertex_count; i++)
	{
		os << "[";
		for (size_t j = 0; j < graph.vertex_count; j++) {
			os << graph.adjacencyMatrix[i][j];
			if (j < (graph.vertex_count - 1))
				os << ",";
		}
		os << "]" << std::endl;
	}
	return os;
}

MatrixGraph* build_random_graph(int v, float p) {
	MatrixGraph* g = new MatrixGraph(v);
	g->randomize(p);
	return g;
}