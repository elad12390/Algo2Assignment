#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <future>
#include <unordered_set>
#include <queue>
#include <fstream>
// ******************************** Declarations ****************************** //

	// ******** Templates *********** //

		/**
		* This is an std::vector - Because of confusion with math vectors i used an alias
		*/
		template <class T>
		using array_list = std::vector<T>;

		/**
		* This is a list of lists, for comfort purposes.
		*/
		template <class T>
		using array_list_2D = array_list<array_list<T>>;

		/**
		* Print a given array_list.
		* @params vec - The array list to print
		*/
		template <class T>
		void print_array_list(array_list<T> list);


	// ******** Classes ************* //

		enum BFS_COLOR
		{
			NONE = 0,
			WHITE = 1,
			GREY = 2,
			BLACK = 3
		};

		// deprecated (because tests don't use bfs colors array and parent array)
		struct BFSResult
		{
			array_list<BFS_COLOR> colors;
			array_list<int> dist;
			//std::vector<int> parent;
		};

		class ListGraph
		{
			array_list_2D<int> adj;
			int vertex_count = 0;
		public:

			/**
			* Creates a graph with [v] edges.
			* @param v - number of edges for the new graph
			*/
			ListGraph(int v);

			/**
			* Getter function for the vertex number.
			* @returns const int& - readonly property of the vertex number.
			*/
			const int& get_size();

			/**
			* Add an edge from specified source to destination
			* 
			* @param (int) source - The source node
			* @param (int) destination - The source destination
			* 
			* @return ListGraph& - returns a reference to the current graph for chaining commands
			*/
			ListGraph& addEdge(int source, int destination);

			/**
			* Randomize the current graph - takes the lower half of the matrix and inserts it (which means (V^2)/2 - V) => O(V^2)
			* 
			* @param (float) p - the probability of an edge to be created between two vertice
			* @return ListGraph& - returns a reference to the current graph for chaining commands
			*/
			ListGraph& randomize(float p);

			/**
			* Run BFS algorithm on the current graph
			* If the graph is empty it returns null.
			* 
			* @param startNode - where to start the BFS algorithm from
			* 
			* @returns std::unique_ptr<array_list<int>> - a smart pointer containing a distance array from the start node to each other nodes
			*/
			std::unique_ptr<array_list<int>> BFS(int startNode);

			/**
			* Calculates the diameter of the current graph
			* @returns int - The diameter of the graph OR (0) if the graph is empty
			*/
			int calc_diameter();

			/**
			* Checks if there's a node with no connections to it.
			* @returns bool - true if found at least one node
			*/
			bool is_isolated();

			/**
			* Checks if the graph has only one connected component
			* @returns True if the graph is Connected.
			*/
			bool connectivity();

			friend std::ostream& operator<<(std::ostream& os, const ListGraph& dt);
		};


	// ******** Tests *************** //

		/**
		* Prints a nice loading bar.
		* @param progress - a number between 0 to 1 that represents the current progress.
		*/
		void print_progress_bar(float progress);

		/**
		* Creates an array of 10 probabilities
		* @param threshold - the probability to generate according to.
		* @returns A smart pointer to an array with the first 5 numbers smaller than [threshold] and last 5 numbers larger than [threshold]
		*/
		std::unique_ptr<array_list<float>> create_threshold_probabilities(float threshold);

		void run_test_one(double p, bool expectedResult, int* resultCounter);
		void test_number_one(array_list<float>* thresholdProbabilities, int resultCounter[10]);

		void run_test_two(double p, bool largerThanTwo, int* resultCounter);
		void test_number_two(array_list<float>* thresholdProbabilities, int resultCounter[10]);

		void run_test_three(double p, bool expectedResult, int* resultCounter);
		void test_number_three(array_list<float>* thresholdProbabilities, int resultCounter[10]);

		/**
		* Creates a csv file of the test results
		* @param name - The name of the file to be saved.
		* @param pArr - The array of the probabilities tested with.
		* @param results[10] - An array of size 10 of the results for each probability.
		*/
		void save_csv_test_file(std::string name, array_list<float>* pArr, int results[10]);

		/**
		* Serves as the main function to run all tests in series.
		*/
		void run_tests();


// ******************************** Declarations End ****************************** //



// ******************************** Definitions  ********************************** //


	// ******** Print Functions ************* //

		template <class T>
		void print_array_list(array_list<T> list)
		{
			std::cout << "[";
			int i = 0;
			for (auto item : list)
			{
				std::cout << item;
				if (i < (list.size() - 1))
				{
					std::cout << ",";
				}
				i++;
			}
			std::cout << "]" << std::endl;
		}

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

		void print_progress_bar(float progress)
		{
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


	// ******** List Graph Definitions ****** //

		ListGraph::ListGraph(int v)
		{
			this->vertex_count = v;
			for (size_t i = 0; i < this->vertex_count; i++)
			{
				array_list<int> v;
				this->adj.push_back(v);
			}
		}

		const int& ListGraph::get_size()
		{
			return this->vertex_count;
		}

		ListGraph& ListGraph::addEdge(int source, int destination)
		{
			this->adj.at(source).push_back(destination);
			this->adj.at(destination).push_back(source);
			return *this;
		}

		ListGraph& ListGraph::randomize(float p)
		{
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

		std::unique_ptr<array_list<int>> ListGraph::BFS(int startNode)
		{
			if (this->vertex_count == 0) return nullptr;


			array_list<BFS_COLOR> colors(this->vertex_count, BFS_COLOR::WHITE);
			std::unique_ptr<array_list<int>> dist = std::make_unique<array_list<int>>(this->vertex_count, -1);

			colors.at(startNode) = BFS_COLOR::GREY;
			dist->at(startNode) = 0;

			std::queue<int> q;
			q.push(startNode);

			while (!q.empty())
			{
				auto currentNode = q.front();
				for (const auto& node : this->adj.at(currentNode))
				{
					if (node < 0) continue;
					if (colors.at(node) == BFS_COLOR::WHITE)
					{
						colors.at(node) = BFS_COLOR::GREY;
						dist->at(node) = dist->at(currentNode) + 1;
						q.push(node);
					}
				}
				q.pop();
				colors.at(currentNode) = BFS_COLOR::BLACK;
			}
			return dist;
		}

		int ListGraph::calc_diameter()
		{
			if (this->vertex_count == 0) return 0;
			auto max = 0;

			for (size_t i = 0; i < this->vertex_count; i++)
			{
				auto dist = BFS(i);
				for (const auto& item : *dist)
				{
					if (item == -1) return -1;
					max = std::max(max, item);
				}
			}
			return max;
		}

		bool ListGraph::is_isolated()
		{
			for (const auto& item : this->adj)
			{
				if (item.empty())
					return 1;
			}
			return 0;
		}

		bool ListGraph::connectivity()
		{
			auto dist = BFS(0);
			for (const auto& item : *dist)
			{
				if (item == -1)
					return false;
			}
			return true;
		}


	// ******** Test Definitions ************ //

		std::unique_ptr<array_list<float>> create_threshold_probabilities(float threshold)
		{
			std::unique_ptr<array_list<float>> list = std::make_unique<array_list<float>>(10);
			auto tenPercent = threshold / 10;

			// smaller than
			list.get()->at(0) = (tenPercent * 2);
			list.get()->at(1) = (tenPercent * 4);
			list.get()->at(2) = (tenPercent * 5);
			list.get()->at(3) = (tenPercent * 6);
			list.get()->at(4) = (tenPercent * 8);

			// larger than
			list.get()->at(5) = (tenPercent * 12);
			list.get()->at(6) = (tenPercent * 14);
			list.get()->at(7) = (tenPercent * 15);
			list.get()->at(8) = (tenPercent * 16);
			list.get()->at(9) = (tenPercent * 18);

			return list;
		}

		void run_test_one(double p, bool expectedResult, int* resultCounter)
		{
			auto g = ListGraph(1000);
			g.randomize(p);
			bool result = g.connectivity();
			if(result == expectedResult)
				(*resultCounter)++;
		}

		void test_number_one(array_list<float>* thresholdProbabilities, int resultCounter[10])
		{
			if (thresholdProbabilities->size() < 10) return;
			const float prob1 = thresholdProbabilities->at(0);
			const float prob2 = thresholdProbabilities->at(1);
			const float prob3 = thresholdProbabilities->at(2);
			const float prob4 = thresholdProbabilities->at(3);
			const float prob5 = thresholdProbabilities->at(4);
			const float prob6 = thresholdProbabilities->at(5);
			const float prob7 = thresholdProbabilities->at(6);
			const float prob8 = thresholdProbabilities->at(7);
			const float prob9 = thresholdProbabilities->at(8);
			const float prob10 = thresholdProbabilities->at(9);


			auto start = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < 500; i++)
			{
				auto task1 = std::async(run_test_one, prob1, false, &(resultCounter[0]));
				auto task2 = std::async(run_test_one, prob2, false, &(resultCounter[1]));
				auto task3 = std::async(run_test_one, prob3, false, &(resultCounter[2]));
				auto task4 = std::async(run_test_one, prob4, false, &(resultCounter[3]));
				auto task5 = std::async(run_test_one, prob5, false, &(resultCounter[4]));

				auto task6 = std::async(run_test_one, prob6, true, &(resultCounter[5]));
				auto task7 = std::async(run_test_one, prob7, true, &(resultCounter[6]));
				auto task8 = std::async(run_test_one, prob8, true, &(resultCounter[7]));
				auto task9 = std::async(run_test_one, prob9, true, &(resultCounter[8]));
				auto task10 = std::async(run_test_one, prob10, true, &(resultCounter[9]));

				task10.wait();
				auto stop = std::chrono::high_resolution_clock::now();
				print_progress_bar((float)i / 500);
				std::cout << (((stop - start) / 1000000ll)).count() << " milliseconds" << std::endl;
			}
		}

		void run_test_two(double p, bool largerThanTwo, int* resultCounter)
		{
			auto g = ListGraph(1000);
			g.randomize(p);
			int diam = g.calc_diameter();
			diam = diam == -1 ? INT_MAX : diam;
			if(largerThanTwo ? diam > 2 : diam == 2)
				(*resultCounter)++;
		}

		void test_number_two(array_list<float>* thresholdProbabilities, int resultCounter[10])
		{
			if (thresholdProbabilities->size() < 10) return;
			const float prob1 = thresholdProbabilities->at(0);
			const float prob2 = thresholdProbabilities->at(1);
			const float prob3 = thresholdProbabilities->at(2);
			const float prob4 = thresholdProbabilities->at(3);
			const float prob5 = thresholdProbabilities->at(4);
			const float prob6 = thresholdProbabilities->at(5);
			const float prob7 = thresholdProbabilities->at(6);
			const float prob8 = thresholdProbabilities->at(7);
			const float prob9 = thresholdProbabilities->at(8);
			const float prob10 = thresholdProbabilities->at(9);


			auto start = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < 500; i++)
			{
				auto task1 = std::async(run_test_two, prob1, true, &(resultCounter[0]));
				auto task2 = std::async(run_test_two, prob2, true, &(resultCounter[1]));
				auto task3 = std::async(run_test_two, prob3, true, &(resultCounter[2]));
				auto task4 = std::async(run_test_two, prob4, true, &(resultCounter[3]));
				auto task5 = std::async(run_test_two, prob5, true, &(resultCounter[4]));

				auto task6 = std::async(run_test_two, prob6, false, &(resultCounter[5]));
				auto task7 = std::async(run_test_two, prob7, false, &(resultCounter[6]));
				auto task8 = std::async(run_test_two, prob8, false, &(resultCounter[7]));
				auto task9 = std::async(run_test_two, prob9, false, &(resultCounter[8]));
				auto task10 = std::async(run_test_two, prob10, false, &(resultCounter[9]));

				task10.wait();
				auto stop = std::chrono::high_resolution_clock::now();
				print_progress_bar((float)i / 500);
				std::cout << (((stop - start) / 1000000ll)).count() << " milliseconds" << std::endl;
			}
		}

		void run_test_three(double p, bool expectedResult, int* resultCounter)
		{
			auto g = ListGraph(1000);
			g.randomize(p);
			if(g.is_isolated() == expectedResult)
				(*resultCounter)++;

		}

		void test_number_three(array_list<float>* thresholdProbabilities, int resultCounter[10])
		{
			if (thresholdProbabilities->size() < 10) return;
			const float prob1 = thresholdProbabilities->at(0);
			const float prob2 = thresholdProbabilities->at(1);
			const float prob3 = thresholdProbabilities->at(2);
			const float prob4 = thresholdProbabilities->at(3);
			const float prob5 = thresholdProbabilities->at(4);
			const float prob6 = thresholdProbabilities->at(5);
			const float prob7 = thresholdProbabilities->at(6);
			const float prob8 = thresholdProbabilities->at(7);
			const float prob9 = thresholdProbabilities->at(8);
			const float prob10 = thresholdProbabilities->at(9);


			auto start = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < 500; i++)
			{
				auto task1 = std::async(run_test_three, prob1, true, &resultCounter[0]);
				auto task2 = std::async(run_test_three, prob2, true, &resultCounter[1]);
				auto task3 = std::async(run_test_three, prob3, true, &resultCounter[2]);
				auto task4 = std::async(run_test_three, prob4, true, &resultCounter[3]);
				auto task5 = std::async(run_test_three, prob5, true, &resultCounter[4]);

				auto task6 = std::async(run_test_three, prob6, false, &resultCounter[5]);
				auto task7 = std::async(run_test_three, prob7, false, &resultCounter[6]);
				auto task8 = std::async(run_test_three, prob8, false, &resultCounter[7]);
				auto task9 = std::async(run_test_three, prob9, false, &resultCounter[8]);
				auto task10 = std::async(run_test_three, prob10, false, &resultCounter[9]);

				task10.wait();
				auto stop = std::chrono::high_resolution_clock::now();
				print_progress_bar((float)i / 500);
				std::cout << (((stop - start) / 1000000ll)).count() << " milliseconds" << std::endl;
			}
		}

		void save_csv_test_file(std::string name, array_list<float>* pArr, int results[10])
		{
			const char SEPERATOR = ',';
			std::ofstream fout;
			fout.open(name + "_test.csv", std::ios::out | std::ofstream::trunc);

			// first line
			fout << "Probability" << SEPERATOR;
			for (auto p : *pArr)
			{
				fout << p << SEPERATOR;
			}
			fout << std::endl;

			fout << "Simulation results" << SEPERATOR;
			for (int i = 0; i < 10; i++)
			{
				fout << ((float)(results[i]) / 500) << SEPERATOR;
			}

			fout.close();
		}

		void run_tests()
		{
			std::srand(std::time(NULL));

			int V = 1000;

			float threshold1 = (float)std::log(V) / V;
			float threshold2 = std::sqrt(2 * threshold1);
			float threshold3 = threshold1;

			std::unique_ptr<array_list<float>> threshhold1Probabilities = create_threshold_probabilities(threshold1);
			std::unique_ptr<array_list<float>> threshhold2Probabilities = create_threshold_probabilities(threshold2);
			std::unique_ptr<array_list<float>> threshhold3Probabilities = create_threshold_probabilities(threshold3);

			int testOneResults[10] = { 0 };
			int testTwoResults[10] = { 0 };
			int testThreeResults[10] = { 0 };

			std::cout << "Starting tests..." << std::endl << std::endl;

			std::cout << "Test Number One:" << std::endl;
			test_number_one(threshhold1Probabilities.get(), testOneResults);
			save_csv_test_file("connectivity", threshhold1Probabilities.get(), testOneResults);

			std::cout << "Test Number Two:" << std::endl;
			test_number_two(threshhold2Probabilities.get(), testTwoResults);
			save_csv_test_file("diameter", threshhold2Probabilities.get(), testTwoResults);

			std::cout << "Test Number Three:" << std::endl;
			test_number_three(threshhold3Probabilities.get(), testThreeResults);
			save_csv_test_file("isolated", threshhold3Probabilities.get(), testThreeResults);
		}
		

	// ******** Main ************************ //

	int main()
	{
		run_tests();
		return 0;
	}

// ******************************** Definitions End ****************************** //
