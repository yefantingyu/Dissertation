#ifndef PROBELM_HPP
#define PROBELM_HPP
#include "Macro.hpp"
#include <algorithm>
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
using namespace std;

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
struct VNS {
  int Max_trails;
  double restart_ratio;
};
struct ACO {
  int Max_trails;
};
struct SA {
  int Max_trails;
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

class Problem {
public:
  char *instance;
  string instance_path;
  string flag;
  struct VNS vns_parameter;
  struct ACO aco_parameter;
  struct SA sa_parameter;
  floydWarshall fw;

  vector<int> Customers;
  vector<int> AFs;
  map<int, int> afsIdMap;

  Problem(){};
  ~Problem();
  Problem(const Problem &);
  Problem &operator=(const Problem &);

  static Problem *getInstance();
  void init_evals();
  void init_current_best();
  double get_current_best();
  double get_evals();

  /*
  read all the things
  */
  void read(string flagt, char *filename);
  void read_parameters();
  void read_instance();

  void init();

  /*
   calculate the matrix
   get the matrix

  */
  void generate_2D_matrix_double(vector<vector<double>> &matrix, int n, int m);
  void compute_distances(void);
  double euclidean_distance(int i, int j);

  /*
  Check the requirements

  */
  double fitness_evaluation(vector<int> &tour);
  int getRemainingLoad(vector<int>);
  double getRemainingBattery(vector<int> evrpTour);
  int getClosestAFS(int node);
  int getReachableAFSClosestToGoal(int cur, int goal, double battery);
  bool is_charging_station(int node);
  double get_distance(int from, int to);
  int get_customer_demand(int customer);
  double get_energy_consumption(int from, int to);
  double get_energy_per_unit();
  bool full_validity_check(vector<int> &tour);
  bool isValidTour(vector<int> tour);

  /*
  print solution

  */
  void print_solution(vector<int> route, int size);

  int TERMINATION;
  int STOP_CNT;
  vector<struct node> node_list;
  vector<int> cust_demand;
  vector<bool> charging_station;
  vector<vector<double>> distances;
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

private:
  static Problem *instancer;
  class Deletor {
    ~Deletor() {
      if (Problem::instancer != NULL)
        delete Problem::instancer;
    }
  };
  static Deletor deletor;






};
extern Problem *problem;



#endif
