#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <future>

using namespace std;
using namespace std::chrono;

template <class T>
using vector2D = vector<vector<T>*>;


class ListGraph {
public:
	vector2D<int> adj;
	int vertex_count = 0;

	// O(v)
	ListGraph(int v) {
		this->vertex_count = v;
		for (size_t i = 0; i < this->vertex_count; i++)
		{
			this->adj.push_back(new vector<int>(this->vertex_count));
		}
	}

	~ListGraph() {
		for (size_t i = 0; i < adj.size(); i++)
		{
			this->adj.at(i)->clear();
			delete this->adj.at(i);
		}
		this->adj.clear();
	}

	// O(1)
	ListGraph& addEdge(int source, int destination) {
		this->adj.at(source)->push_back(destination);
		this->adj.at(destination)->push_back(source);
		return *this;
	}

	// takes the lower half of the matrix and inserts it (which means (V^2)/2 - V) => O(V^2)
	ListGraph& randomize(float p) {
		for (size_t i = 1; i < this->vertex_count; i++)
			for (size_t j = 0; j < i; j++) {

				//generate random number between 0 to 1. If the number is smaller than p, we add the edge.
				if (((float)rand() / RAND_MAX) < p) {
					this->addEdge(i, j);
				}
			}

		return *this;
	}

	friend std::ostream& operator<<(std::ostream& os, const ListGraph& dt);

	
};



std::ostream& operator<<(std::ostream& os, const ListGraph& graph)
{
	for (size_t i = 0; i < graph.vertex_count; i++)
	{
		int vec_size = graph.adj.at(i)->size();

		os << "[";
		for (size_t j = 0; j < vec_size; j++) {
			os << graph.adj.at(i)->at(j);
			if (j < (vec_size - 1))
				os << ",";
		}
		os << "]" << std::endl;
	}
	return os;
}

void run_test(double p) {
	auto g = ListGraph(1000);
	g.randomize(p);
}

int main() {
	srand(time(NULL));

	cout << "Starting..." << endl;


	for (int i = 0; i < 500; i += 1) {
		auto start = high_resolution_clock::now();

		auto task1 = std::async(run_test, 0.1);
		auto task2 = std::async(run_test, 0.2);
		auto task3 = std::async(run_test, 0.3);
		auto task4 = std::async(run_test, 0.4);
		auto task5 = std::async(run_test, 0.5);
		auto task6 = std::async(run_test, 0.6);
		auto task7 = std::async(run_test, 0.7);
		auto task8 = std::async(run_test, 0.8);
		auto task9 = std::async(run_test, 0.9);

		auto stop = high_resolution_clock::now();
		cout << "finished test number " << i << " at " << duration_cast<microseconds>(stop - start).count() << " microseconds" << endl;
	}
	return 0;
}