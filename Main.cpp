#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <future>
#include <unordered_set>
#include <queue>

template <class T>
using array_list = std::vector<T>;

template <class T>
using vector2D = array_list<array_list<T>>;

template <class T>
void print_vector(std::vector<T> vec)
{
	std::cout << "[";
	int i = 0;
	for (auto item : vec)
	{
		std::cout << item;
		if (i < (vec.size() - 1))
		{
			std::cout << ",";
		}
		i++;
	}
	std::cout << "]" << std::endl;
}

enum BFS_COLOR {
	NONE = 0,
	WHITE = 1,
	GREY = 2,
	BLACK = 3
};

struct BFSResult {
	std::vector<BFS_COLOR> colors;
	std::vector<int> dist;
	//std::vector<int> parent;
};

class ListGraph
{
public:
	vector2D<int> adj;
	int vertex_count = 0;

	// O(v)
	ListGraph(int v)
	{
		this->vertex_count = v;
		for (size_t i = 0; i < this->vertex_count; i++)
		{
			std::vector<int> v(this->vertex_count, -1);
			this->adj.push_back(v);
		}
	}

	// O(1)
	ListGraph& addEdge(int source, int destination)
	{
		this->adj.at(source).push_back(destination);
		this->adj.at(destination).push_back(source);
		return *this;
	}

	// takes the lower half of the matrix and inserts it (which means (V^2)/2 - V) => O(V^2)
	ListGraph& randomize(float p) {
		for (size_t i = 1; i < this->vertex_count; i++) 
		{
			for (size_t j = 0; j < i; j++)
			{
				//generate random number between 0 to 1. If the number is smaller than p, we add the edge.
				if (((float)rand() / RAND_MAX) < p)
				{
					this->addEdge(i, j);
				}
			}
		}
		return *this;
	}

	std::vector<int> BFS(int startNode)
	{
		std::vector<BFS_COLOR> colors(this->vertex_count, BFS_COLOR::WHITE);
		std::vector<int> dist(this->vertex_count, -1);

		colors.at(startNode) = BFS_COLOR::GREY;
		dist.at(startNode) = 0;

		std::queue<int> q;
		q.push(startNode);

		while (!q.empty())
		{
			auto currentNode = q.front();
			for (auto& node : this->adj.at(currentNode))
			{
				if (node < 0) continue;
				if (colors.at(node) == BFS_COLOR::WHITE)
				{
					colors.at(node) = BFS_COLOR::GREY;
					dist.at(node) = dist.at(currentNode) + 1;
					q.push(node);
				}
			}
			q.pop();
			colors.at(currentNode) = BFS_COLOR::BLACK;
		}
		return dist;
	}

	int calc_diameter()
	{
		auto max = 0;

		for (size_t i = 0; i < this->vertex_count; i++)
		{
			auto dist = BFS(i);
			for (auto item : dist)
			{
				if (item == -1) return -1;
				if (item > max) { max = item; }
			}
		}
		return max;
	}

	bool Is_Isolated() {
		for (auto item : this->adj)
		{
			if (item.empty())
				return 1;
		}
		return 0;
	}

	bool connectivity() {
		auto dist = BFS(0);
		for (auto item : dist)
		{
			if (item == -1)
				return 0;
		}
		return 1;
	}

	friend std::ostream& operator<<(std::ostream& os, const ListGraph& dt);
};

std::ostream& operator<<(std::ostream& os, const ListGraph& graph)
{
	for (auto& col : graph.adj)
	{
		os << "[";
		int i = 0;
		for (auto& row : col)
		{
			os << row;
			if (i < col.size())
				os << ",";
			i++;
		}
		os << "]" << std::endl;
	}
	return os;
}

void run_test(double p)
{
	auto g = ListGraph(1000);
	g.randomize(p);
	g.calc_diameter();
}

void print_progress_bar(float progress) {
	int barWidth = 70;

	std::cout << "[";
	int pos = barWidth * progress;
	for (int i = 0; i < barWidth; ++i) {
		if (i < pos) std::cout << "=";
		else if (i == pos) std::cout << ">";
		else std::cout << " ";
	}
	std::cout << "] " << int(progress * 100.0) << " %\r";
	std::cout.flush();
	std::cout << std::endl;
}

int main()
{
	//auto graph = ListGraph(8);

	//graph
	//	.addEdge(0, 1)
	//	.addEdge(1, 2)
	//	.addEdge(2, 7)
	//	.addEdge(2, 3)
	//	.addEdge(4, 3)
	//	.addEdge(6, 3)
	//	.addEdge(5, 3);


	//
	//print_vector(bfsResult.get()->dist);
	//print_vector(bfsResult.get()->colors);
	//print_vector(bfsResult.get()->parent);

	//std::cout << graph.calc_diameter();

	srand(time(NULL));

	std::cout << "Starting..." << std::endl;


	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 500; i++)
	{
		
		auto task1 = std::async(run_test, 0.1);
		auto task2 = std::async(run_test, 0.2);
		auto task3 = std::async(run_test, 0.25);
		auto task4 = std::async(run_test, 0.3);
		auto task5 = std::async(run_test, 0.4);
		auto task6 = std::async(run_test, 0.6);
		auto task7 = std::async(run_test, 0.7);
		auto task8 = std::async(run_test, 0.75);
		auto task9 = std::async(run_test, 0.8);
		auto task10 = std::async(run_test, 0.9);
		task10.wait();
		auto stop = std::chrono::high_resolution_clock::now();
		print_progress_bar((float)i / 500);
		std::cout << (((stop - start) / 1000000ll)).count() << " milliseconds" << std::endl;
	}
	return 0;
}