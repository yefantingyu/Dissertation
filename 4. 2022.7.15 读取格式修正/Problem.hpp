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
// The parameter for the ACO algorithm

struct AntStruct {
    int *tour; // 2*actual
    int *evrptour;
    int tour_size;     /* 路程中经过的点个数（depot可能出现多次） */
    int evrp_tour_size;
    bool *visited;      /* mark nodes visited status */
    bool *candidate;  /** 所有可配送的点(单次route中，目前车辆可以仍可配送的点) */
    int num_of_tours;
    double fitness;
};

struct AcoParameter {
  int num_of_ants;
  double rho;
  double alpha;
  double beta;
  int ras_ranks;
};
struct node {
  int id;
  double x;
  double y;
};
struct customer{
  int id;
  double x;
  double y;
  int demand;
};
struct Route{
  Route(int begint, int endt, int loadt,double energyt, double distt): begin(begint),end(endt),load(loadt), energy(energyt), dist(distt){}
  int begin;
  int end;
  int load;
  double energy;
  double dist;
};


struct solution {
  int *tour;
  int id;
  double tour_length;
  int steps;
};

// Input parameter path, the instance path and the location 
extern string instance;
extern string instance_path; // the name for the instance
extern string parameter_path;
extern string trial_output_path;
extern string iteration_output_path;
extern string flag; // the flag for the algorithms: SA , ACO   VNS
// Input parameter path, the instance path and the location 


// 在循环中用于控制变量的变量
extern int iteration;
extern int termination;
extern int STOP_CNT;
extern int max_trial_base;


// 一些实在的参数
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




extern struct node *node_list;     //List of nodes with id and x and y coordinates
extern int* cust_demand; // the customer demand for the problem
extern bool* charging_station; // if the current node is a charging station 
extern int* customers;
extern int* AFSs;
extern map<int,int> afsIdMap;// customer  index in arrary, not the id
extern double** distances;



// the algorithm parameter
extern struct AcoParameter acoparameter;// the aco parameter

// Parameters related to the final statistics
extern int evals;
extern double current_best;
extern vector<double> iteration_record_results;
extern solution best_sol;

//####################################################################### function####################

// read the basic parameters from the files
void isFileExist(string filename);
void get_instance_name(string);
void read_arguments(int argc, char* argv[]);
void read_instances();
void read_parameters();
//end reading






/* basic init functions */



/* problem functions*/
void compute_distances();
double** generate_2D_matrix_double(int n, int m);
double euclidean_distance(int i, int j);

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
void free_problem();
double fitness_evaluation(int *routes, int size);
bool check_solution(int *t, int size);
#endif
