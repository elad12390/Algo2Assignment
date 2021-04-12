#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <future>
#include <unordered_set>
#include <queue>
#include <fstream>

#define PRINTLN(x) std::cout << x << std::endl
#define PRINT_EMPTY_LINE() std::cout << std::endl


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
		void print_array_list(array_list<T>& list);


	// ******** Classes ************* //

		enum class BFS_COLOR
		{
			none = 0,
			white = 1,
			grey = 2,
			black = 3
		};
		
		enum TestToRun {
			TEST_1 = 0b100,
			TEST_2 = 0b010,
			TEST_3 = 0b001,
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
		    array_list_2D<int> adj_;
			int vertex_count_ = 0;
		public:

			/**
			* Creates a graph with [v] edges.
			* @param v - number of edges for the new graph
			*/
			ListGraph(const int &v);

			/**
			* Getter function for the vertex number.
			* @returns const int& - readonly property of the vertex number.
			*/
			const int& get_size() const;

			/**
			* Add an edge from specified source to destination
			* 
			* @param source - The source node
			* @param destination - The source destination
			* 
			* @return ListGraph& - returns a reference to the current graph for chaining commands
			*/
			ListGraph& add_edge(const int& source, const int& destination);

			/**
			* Randomize the current graph - takes the lower half of the matrix and inserts it (which means (V^2)/2 - V) => O(V^2)
			* 
			* @param p - the probability of an edge to be created between two vertice
			* @return ListGraph& - returns a reference to the current graph for chaining commands
			*/
			ListGraph& randomize(const float& p);

			/**
			* Run BFS algorithm on the current graph
			* If the graph is empty it returns null.
			* 
			* @param start_node - where to start the BFS algorithm from
			* 
			* @returns std::unique_ptr<array_list<int>> - a smart pointer containing a distance array from the start node to each other nodes
			*/
			std::unique_ptr<array_list<int>> BFS(const int& start_node) const;

			/**
			* Calculates the diameter of the current graph
			* @returns int - The diameter of the graph OR (0) if the graph is empty
			*/
			int calc_diameter() const;

			/**
			* Checks if there's a node with no connections to it.
			* @returns bool - true if found at least one node
			*/
			bool is_isolated() const;

			/**
			* Checks if the graph has only one connected component
			* @returns True if the graph is Connected.
			*/
			bool connectivity() const;

			friend std::ostream& operator<<(std::ostream& os, const ListGraph& graph);
		};


	// ******** Tests *************** //

		/**
		* Prints a nice loading bar.
		* @param progress - a number between 0 to 1 that represents the current progress.
		*/
		void print_progress_bar(const float &progress);

		/**
		* Creates an array of 10 probabilities
		* @param threshold - the probability to generate according to.
		* @param downJumpPercentage - (Zoom) the jumping percentage on the probabilities smaller than the threshold
		* @param upJumpPercentage - (Zoom) the jumping percentage on the probabilities larger than the threshold
		* @returns A smart pointer to an array with the first 5 numbers smaller than [threshold] and last 5 numbers larger than [threshold]
		*/
		std::unique_ptr<array_list<float>> create_threshold_probabilities(float threshold, float downJumpPercentage, float upJumpPercentage);

		void run_test_one(double p, bool expected_result, int* result_counter);
		void test_number_one(array_list<float>* threshold_probabilities, int result_counter[10]);

		void run_test_two(const double& p, const bool& larger_than_two, int* result_counter);
		void test_number_two(array_list<float>* threshold_probabilities, int result_counter[10]);

		void run_test_three(const double& p, const bool& expectedResult, int* result_counter);
		void test_number_three(array_list<float>* threshold_probabilities, int result_counter[10]);

		/**
		* Creates a csv file of the test result_counter
		* @param name - The name of the file to be saved.
		* @param p_arr - The array of the probabilities tested with.
		* @param result_counter - An array of size 10 of the results for each probability.
		*/
		void save_csv_test_file(const std::string& name, array_list<float>* p_arr, const int result_counter[10]);

		unsigned int ask_which_test_to_run();

		void run_test1();
		void run_test2();
		void run_test3();

		/**
		* Serves as the main function to run all tests in series.
		*/
		void run_tests(int num);


// ******************************** Declarations End ****************************** //



// ******************************** Definitions  ********************************** //


	// ******** Print Functions ************* //

		template <class T>
		void print_array_list(array_list<T>& list)
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
			for (auto& col : graph.adj_)
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

		void print_progress_bar(const float &progress)
		{
			const int bar_width = 100;

			std::cout << "[";
			const int pos = bar_width * progress;
			for (int i = 0; i < bar_width; ++i) {
				if (i < pos) std::cout << "=";
				else if (i == pos) std::cout << ">";
				else std::cout << " ";
			}
			std::cout << "] " << static_cast<int>(progress * 100.0) << " %\r";
			std::cout.flush();
			std::cout << std::endl;
		}


	// ******** List Graph Definitions ****** //

		ListGraph::ListGraph(const int &v)
		{
			this->vertex_count_ = v;
			for (size_t i = 0; i < this->vertex_count_; i++)
			{
				array_list<int> v;
				this->adj_.push_back(v);
			}
		}

		const int& ListGraph::get_size() const
		{
			return this->vertex_count_;
		}

		ListGraph& ListGraph::add_edge(const int &source, const int &destination)
		{
			this->adj_.at(source).push_back(destination);
			this->adj_.at(destination).push_back(source);
			return *this;
		}

		ListGraph& ListGraph::randomize(const float &p)
		{
			for (size_t i = 1; i < this->vertex_count_; i++)
			{
				for (size_t j = 0; j < i; j++)
				{
					//generate random number between 0 to 1. If the number is smaller than p, we add the edge.
					if ((static_cast<float>(rand()) / RAND_MAX) < p)
					{
						this->add_edge(i, j);
					}
				}
			}
			return *this;
		}

		std::unique_ptr<array_list<int>> ListGraph::BFS(const int &start_node) const
		{
			if (this->vertex_count_ == 0) return nullptr;


			array_list<BFS_COLOR> colors(this->vertex_count_, BFS_COLOR::white);
			std::unique_ptr<array_list<int>> dist = std::make_unique<array_list<int>>(this->vertex_count_, -1);

			colors.at(start_node) = BFS_COLOR::grey;
			dist->at(start_node) = 0;

			std::queue<int> q;
			q.push(start_node);

			while (!q.empty())
			{
				const auto& current_node = q.front();
				for (const auto& node : this->adj_.at(current_node))
				{
					if (node < 0) continue;
					if (colors.at(node) == BFS_COLOR::white)
					{
						colors.at(node) = BFS_COLOR::grey;
						dist->at(node) = dist->at(current_node) + 1;
						q.push(node);
					}
				}
				q.pop();
				colors.at(current_node) = BFS_COLOR::black;
			}
			return dist;
		}

		int ListGraph::calc_diameter() const
		{
			if (this->vertex_count_ == 0) return 0;
			auto max = 0;

			for (size_t i = 0; i < this->vertex_count_; i++)
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

		bool ListGraph::is_isolated() const
		{
			return std::any_of(this->adj_.begin(), this->adj_.end(), [](const array_list<int>& item) { return item.empty(); });
		}

		bool ListGraph::connectivity() const
		{
			const auto dist = BFS(0);
			return std::all_of(dist->begin(), dist->end(), [](const int& item) { return item != -1; });
		}


	// ******** Test Definitions ************ //

		std::unique_ptr<array_list<float>> create_threshold_probabilities(const float threshold, float downJumpPercentage, float upJumpPercentage)
		{
			std::unique_ptr<array_list<float>> list = std::make_unique<array_list<float>>(10);
			auto onePercent = threshold / 100;

			// smaller than
			// start from 50% climb up to 90%
			for (size_t i = 0; i < 5; i++)
				list.get()->at(i) = threshold - (onePercent * downJumpPercentage * (5 - i));

			// start from 110% climb up to 150%
			for (size_t i = 5, j = 0; i < 10; i++, j++)
				list.get()->at(i) = threshold + (onePercent * upJumpPercentage * (1 + j));

			return list;
		}

		void run_test_one(const double p, const bool expected_result, int* result_counter)
		{
			auto g = ListGraph(1000);
			g.randomize(p);
			bool result = g.connectivity();
			if(result == expected_result)
				(*result_counter)++;
		}

		void test_number_one(array_list<float>* threshold_probabilities, int result_counter[10])
		{
			if (threshold_probabilities->size() < 10) return;

			auto start = std::chrono::high_resolution_clock::now();
			std::future<void> operations[10];
			
			for (int i = 0; i < 500; i++)
			{
				for (size_t i = 0; i < 5; i++)
				{
					operations[i] = std::async(std::launch::async, run_test_one, (*threshold_probabilities)[i], false, &(result_counter[i]));
				}
				for (size_t i = 5; i < 10; i++)
				{
					operations[i] = std::async(std::launch::async, run_test_one, (*threshold_probabilities)[i], true, &(result_counter[i]));
				}

				auto stop = std::chrono::high_resolution_clock::now();
				print_progress_bar(static_cast<float>(i) / 500);
				PRINTLN("finished 10 operations in " << (((stop - start) / 1000000ll)).count() << " milliseconds");
				start = std::chrono::high_resolution_clock::now();
			}
		}

		void run_test_two(const double& p, const bool& larger_than_two, int* result_counter)
		{
			auto g = ListGraph(1000);
			g.randomize(p);
			int diam = g.calc_diameter();
			diam = diam == -1 ? INT_MAX : diam;
			if(larger_than_two ? diam > 2 : diam == 2)
				(*result_counter)++;
		}

		void test_number_two(array_list<float>* threshold_probabilities, int result_counter[10])
		{
			if (threshold_probabilities->size() < 10) return;
			std::future<void> operations[10];

			auto start = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < 500; i++)
			{
				for (size_t i = 0; i < 5; i++)
				{
					operations[i] = std::async(std::launch::async, run_test_two, (*threshold_probabilities)[i], true, &(result_counter[i]));
				}
				for (size_t i = 5; i < 10; i++)
				{
					operations[i] = std::async(std::launch::async, run_test_two, (*threshold_probabilities)[i], false, &(result_counter[i]));
				}

				auto stop = std::chrono::high_resolution_clock::now();
				print_progress_bar(static_cast<float>(i) / 500);
				PRINTLN("finished 10 operations in " << (((stop - start) / 1000000ll)).count() << " milliseconds");
				start = std::chrono::high_resolution_clock::now();
			}
		}

		void run_test_three(const double& p, const bool& expectedResult, int* result_counter)
		{
			auto g = ListGraph(1000);
			g.randomize(p);
			if(g.is_isolated() == expectedResult)
				(*result_counter)++;

		}

		void test_number_three(array_list<float>* threshold_probabilities, int result_counter[10])
		{
			if (threshold_probabilities->size() < 10) return;
			std::future<void> operations[10];

			auto start = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < 500; i++)
			{
				for (size_t i = 0; i < 5; i++)
				{
					operations[i] = std::async(std::launch::async, run_test_three, (*threshold_probabilities)[i], true, &(result_counter[i]));
				}
				for (size_t i = 5; i < 10; i++)
				{
					operations[i] = std::async(std::launch::async, run_test_three, (*threshold_probabilities)[i], false, &(result_counter[i]));
				}

				auto stop = std::chrono::high_resolution_clock::now();
				print_progress_bar(static_cast<float>(i) / 500);
				PRINTLN("finished 10 operations in " << (((stop - start) / 1000000ll)).count() << " milliseconds");
				start = std::chrono::high_resolution_clock::now();
			}
		}

		void save_csv_test_file(const std::string& name, array_list<float>* p_arr, const int result_counter[10])
		{
			const char SEPARATOR = ',';
			std::ofstream fout;
			fout.open(name + "_test.csv", std::ofstream::out | std::ofstream::trunc);

			// first line
			fout << "Probability" << SEPARATOR;
			for (auto p : *p_arr)
			{
				fout << p << SEPARATOR;
			}
			fout << std::endl;

			fout << "Simulation Results" << SEPARATOR;
			for (int i = 0; i < 10; i++)
			{
				fout << ((float)(result_counter[i]) / 500) << SEPARATOR;
			}

			fout.close();
		}

		void run_test1() //connectivity
		{
			std::srand(std::time(nullptr));
			const int v = 1000;
			const float threshold1 = static_cast<float>(std::log(v)) / v;
			const std::unique_ptr<array_list<float>> threshold1_probabilities = create_threshold_probabilities(threshold1, 6, 15);
			int testOneResults[10] = { 0 };
			PRINTLN("Starting test...");
			PRINT_EMPTY_LINE();
			
			PRINTLN("Test Number One:");
			test_number_one(threshold1_probabilities.get(), testOneResults);
			save_csv_test_file("connectivity", threshold1_probabilities.get(), testOneResults);
		}

		void run_test2() //diameter
		{
			std::srand(std::time(nullptr));
			const int v = 1000;
			const float threshold2 = std::sqrt(2 * static_cast<float>(std::log(v)) / v);
			const std::unique_ptr<array_list<float>> threshold2_probabilities = create_threshold_probabilities(threshold2,3,5);
			
			int testTwoResults[10] = { 0 };
			
			PRINTLN("Starting test...");
			PRINT_EMPTY_LINE();
			PRINTLN("Test Number Two:");
			
			test_number_two(threshold2_probabilities.get(), testTwoResults);
			save_csv_test_file("diameter", threshold2_probabilities.get(), testTwoResults);

		}
		
		void run_test3() //is_ioslated
		{
			std::srand(std::time(nullptr));
			const int v = 1000;
			const float threshold3 = static_cast<float>(std::log(v)) / v;
			const std::unique_ptr<array_list<float>> threshold3_probabilities = create_threshold_probabilities(threshold3,6.5,16);
			int testThreeResults[10] = { 0 };
			PRINTLN("Starting test...");
			PRINT_EMPTY_LINE();
			PRINTLN("Test Number Three:");
			test_number_three(threshold3_probabilities.get(), testThreeResults);
			save_csv_test_file("isolated", threshold3_probabilities.get(), testThreeResults);
		}
		
		void run_tests(unsigned int num)
		{
			if (!num) return;
			PRINTLN(TEST_2 << (unsigned int)(num & TEST_2));
			if (num & TEST_1)
				run_test1();
			if (num & TEST_2)
				run_test2();
			if (num & TEST_3)
				run_test3();
		}
		
		unsigned int ask_which_test_to_run()
		{
			std::string input;
			unsigned int result = 0;
			PRINT_EMPTY_LINE();
			PRINTLN("Please enter the tests you want to run");
			PRINTLN("\tIn this format: [0 OR 1][0 OR 1][0 OR 1]");
			PRINTLN("\tFor example: 101 will run Test1 and Test3");
			
			std::cin >> input;
			for (auto& letter : input)
			{
				if (letter != '1' && letter != '0')
				{
					PRINTLN("Error: The number inserted is not binary. exiting program.");
					return -1;
				}
				
				result *= 2;
				result += letter == '1' ? 1 : 0;
			}
			
			return result;
		}
	// ******** Main ************************ //

	int main()
	{
		const unsigned int tests_to_run = ask_which_test_to_run();
		if (tests_to_run < 0) return 0;
		PRINTLN(tests_to_run);
		run_tests(tests_to_run);
		return 0;
	}

// ******************************** Definitions End ****************************** //
