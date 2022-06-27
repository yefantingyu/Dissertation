#include "Macro.hpp"
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<cstring>
#include<math.h>
#include<algorithm>
#include<fstream>
#include<time.h>
#include<limits.h>
#include <vector>
#include <limits.h>
#include <string>
using namespace std;



struct node {
  int id;
  double x;
  double y;
};
struct solution{
  int *tour;	//this is what the fitness_evaluation function in EVRP.hpp will evaluate
  int id;
  double tour_length; //quality of the solution
  int steps; //size of the solution
  //the format of the solution is as follows:
  //*tour:  0 - 5 - 6 - 8 - 0 - 1 - 2 - 3 - 4 - 0 - 7 - 0
  //*steps: 12
  //this solution consists of three routes:
  //Route 1: 0 - 5 - 6 - 8 - 0
  //Route 2: 0 - 1 - 2 - 3 - 4 - 0
  //Route 3: 0 - 7 - 0
};

class Problem{
public:
  char* instance;          //Name of the instance
  string instance_path;


  /*Parameter for the file*/
  int TERMINATION;
  struct node *node_list;     //List of nodes with id and x and y coordinates
  int *cust_demand;                //List with id and customer demands
  bool* charging_station;
  double **distances;              //Distance matrix
  int problem_size;                //Problem dimension read
  double energy_consumption;

  int DEPOT;                       //depot id (usually 0)
  int NUM_OF_CUSTOMERS;       //Number of customers (excluding depot)
  int ACTUAL_PROBLEM_SIZE;        //Tottal number of customers, charging stations and depot
  int NUM_OF_VEHICLES;
  double OPTIMUM;
  int NUM_OF_STATIONS;
  int BATTERY_CAPACITY;       //maximum energy of vehicles
  int MAX_CAPACITY;           //capacity of vehicles
  int MIN_VEHICLES;
  std::vector<short int> nearest[MAX_NODE];

  solution *best_sol;
  double current_best;
  double evals;

  Problem(){};
  ~Problem();
  Problem(const Problem&);
  Problem& operator=(const Problem&);
  void read_instance();
  static Problem* getInstance();
  
 void set_instance(char* filename);
  double euclidean_distance(int i, int j);
  void  compute_distances(void);
  double **  generate_2D_matrix_double(int n, int m);
  void compute_nearest_points();
  double get_distance(int from, int to);
  double fitness_evaluation(int *routes, int size, bool save);
  void print_solution(int *routes, int size); 				//used to print the solution
  bool check_solution(int *routes, int size); 				//used to validate the solution

  double get_energy_consumption(int from, int to);	//returns the energy consumption
  int get_customer_demand(int customer);				//returns the customer demand
  bool is_charging_station(int node);					//returns true if node is a charging station

  void set_current_best();
  void set_evals();
  void save_conv(std::vector<double> conv, std::string name);
  void save_solution(std::string algorithm, std::string task, int run);
  void init_evals();					    //initializes the evaluations
  void init_current_best();				//initializes the best solution quality

  private:
	static Problem* instancer;
  class Deletor {
		~Deletor() {
			if(Problem::instancer != NULL)
				delete Problem::instancer;
		}
	};
	static Deletor deletor;

};








