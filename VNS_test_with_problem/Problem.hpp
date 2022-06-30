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
#include <map>
using namespace std;


struct node {
  int id;
  double x;
  double y;
};
struct solution{
  int *tour;
  int id;
  double tour_length;
  int steps;
};

class Problem{
public:
  char* instance;

  string instance_path;
  vector<int> Customers; // vector of all the customers
  vector<int> AFs; // vector of all the charging stations, including the depot!!!!!!!!!!!!!!
  map<int, int> afsIdMap;
  // extern floydWarshall fw;

  void init();
  int getRemainingLoad(vector<int>);
  double getRemainingBattery(vector<int> evrpTour);
  int getClosestAFS(int node);
  int getReachableAFSClosestToGoal(int cur, int goal, double battery);
  bool isValidTour(vector<int> tour);
  void mergeAFSs(vector<int> &tour); 


  
  
  int TERMINATION;
  int STOP_CNT;
  struct node *node_list;
  int *cust_demand;
  bool* charging_station;
  double **distances;
  int problem_size;
  double energy_consumption;

  int DEPOT;
  int NUM_OF_CUSTOMERS;
  int ACTUAL_PROBLEM_SIZE;
  int NUM_OF_VEHICLES;
  double OPTIMUM;
  int NUM_OF_STATIONS;
  int BATTERY_CAPACITY;
  int MAX_CAPACITY;
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
double fitness_evaluation(int *routes, int size);		//evaluates an EVRP solution
double fitness_evaluation(vector<int> &tour);



  void print_solution(int *routes, int size); 
  bool check_solution(int *routes, int size);
  double get_energy_consumption(int from, int to);
  int get_customer_demand(int customer);
  bool is_charging_station(int node);
  void set_current_best();
  double get_evals(){return evals;}									//returns the number of evaluations
  void set_evals();
  void save_conv(std::vector<double> conv, std::string name);
  void save_solution(std::string algorithm, std::string task, int run);
  void init_evals();
  void init_current_best();

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








