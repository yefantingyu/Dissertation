#ifndef PROBELM_HPP
#define PROBELM_HPP
#include "Macro.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits.h>
#include <map>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>
#include <cstdio>
using namespace std;

// The class for calculating the shortest path of the dfferent nodes
class floydWarshall {
    int nV; // graph size
    vector<vector<int>> dist; // matrix of shortest distances; dist[i][j] ~ distance from i to j
    vector<vector<int>> next; // next[i][j] ~ next node on the shortest path from i to j

public:
    floydWarshall() = default;
    explicit floydWarshall(int nV);
    explicit floydWarshall(vector<vector<int>> &graph);
    void planPaths();
    vector<int> getPath(int u, int v, bool afsIds);
    void printMatrix(vector<vector<int>> &matrix);
};
struct VnsParameter {
  double restart_ratio;
};
struct AcoParameter {
  int num_of_ants;
  double taumax;
  double ALPHA;
  double BETA;

};
struct SAParameter {
  double T_start;
  double T_end;
  double delta;
  double restart_ratio;
};

struct node {
  int id;
  double x;
  double y;
};
struct solution {
  int *tour;
  int id;
  double tour_length;
  int steps;
};


// Input parameter path
extern char* instance; // the name for the instance
extern string whole_instance_path; // thw whole path for the instance
extern string whole_parameter_path; // the whole parameter path
extern string flag; // the flag for the algorithms: SA , ACO   VNS

// Problem parameters
extern int DEPOT;// the begin depo
extern int max_trials;
extern int max_iteration; 
extern int actual_problem_size; //all the points in the files, customers, stations depo
extern int problem_size; // the number of customers + depo
extern int num_of_customers;// the number of customers, not include the depo
extern int vehicle_capacity;
extern int battery_capacity;
extern double energy_consumption_rate;
extern int num_of_stations;// charing stations, not include the depo
extern double optimum; //the optimum value for the instance file

extern int termination;
extern int STOP_CNT;
extern int max_trial_base;



extern vector<node> node_list;     //List of nodes with id and x and y coordinates
extern vector<int> cust_demand; // the customer demand for the problem
extern vector<bool> charging_station; // if the current node is a charging station 
extern vector<int> customers;
extern vector<int> AFSs;
extern map<int,int> afsIdMap;// customer  index in arrary, not the id
extern vector<vector<double>> distances;
extern vector<vector<int>> nearest_neighourhood; // nn_list
extern floydWarshall fw;



// the algorithm parameter
extern struct VnsParameter vnsparameter; // the vns parameter
extern struct AcoParameter acoparameter;// the aco parameter
extern struct SAParameter saparameter;// the sa parameter

// statistic
extern int evals;
extern double current_best;
extern vector<double> iteration_record_results;
extern solution best_sol;
/* basic init functions */
void initialise_whole_problem_world();



/* problem functions*/
void compute_distances();
void generate_2D_matrix_double(int n, int m);
double euclidean_distance(int i, int j);
void read_instances(string type, char *filename);
void read_parameters();
double fitness_evaluation(vector<int> &tour);
bool is_charging_station(int node);
int getClosestAFS(int node);
double get_energy_per_unit();
int getRemainingLoad(vector<int> evrpTour);
double getRemainingBattery(vector<int> evrpTour);
int getReachableAFSClosestToGoal(int cur, int goal, double battery);

/* get and set function */
void init_evals();
void init_current_best();
double get_current_best();
double get_evals();
double get_distance(int from, int to);
bool isValidTour(vector<int>tour); 
int get_customer_demand(int customer);
double get_energy_consumption(int from, int to);
void mergeAFSs(vector<int> &tour);
bool full_validity_check(vector<int> &tour);

//free the functions
void free_heuristic();
double fitness_evaluation(int *routes, int size);
bool check_solution(int *t, int size);
extern floydWarshall fw;
#endif
