#include "vector"
#include "Utils.hpp"
#include "Randoms.cpp"
using namespace std;

class ACO{
public:
int num_of_ants;
int num_of_nearest_neighbours;
int num_of_ls;

double alpha;
double betat;
double rho;
double Q;
double taumax;

int rnd_seed;
int **nearest;

double **pheromone;
double **deltapheromone;
double **probs;


AntStruct *Ants;
AntStruct best_so_far_ant;
double * prob_of_selection;
Randoms *randoms;

    
//   // begin for
//   aco.init(); //完成一些基本的事物的初始化
//   while (!termination_condition(aco)) {
//       aco.optimize();
//   }
//   aco.free();
//   //end for
//   aco.freeall();
//########################################## function
ACO();
~ACO();
// main class function
void init();
void optimize();
void free();

void construct_solution();

// utilities functions
void compute_nearest_points();
bool one_ant_finding_route(int k);
double phiupdate (int cityi, int cityj, int *visited);
int choosecity (int count);
void complete_gen();

};