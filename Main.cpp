#include <iostream>
#include <vector>
#include<string>

class Graph {
public:
	bool** adjacencyMatrix; 
	int size;

	Graph(int vertexNumber) {
		this->size = vertexNumber;
		adjacencyMatrix = new bool*[vertexNumber];
		for (size_t i = 0; i < vertexNumber; i++)
			adjacencyMatrix[i] = new bool[vertexNumber]();
	}

	
	Graph* randomize(float p) { 
		srand(time(NULL));
		for (size_t i = 1; i < this->size; i++)
			for (size_t j = 0; j < i; j++)						
				//generate random number between 0 to 1. If the number is smaller than p, we add the edge.
				adjacencyMatrix[j][i] = adjacencyMatrix[i][j] = ((float)rand()/RAND_MAX) < p; 

		return this;
	}

	friend std::ostream& operator<<(std::ostream& os, const Graph& dt);
};



std::ostream& operator<<(std::ostream& os, const Graph& graph)
{
	for (size_t i = 0; i < graph.size; i++)
	{
		os << "[";
		for (size_t j = 0; j < graph.size; j++) {
			os << graph.adjacencyMatrix[i][j];
			if (j < (graph.size - 1))
				os << ",";
		}
		os << "]" << std::endl;
	}
	return os;
}

Graph* build_random_graph(int v, float p) {
	Graph* g = new Graph(v);
	g->randomize(p);
	return g;
}


int main() {
	Graph g = Graph(5);
	g.randomize(.3);
	std::cout << g;
	return 0;
} 