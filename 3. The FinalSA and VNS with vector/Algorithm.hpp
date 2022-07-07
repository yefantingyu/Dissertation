#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP
#include <vector>
#include "Operators.hpp"
#include "Macro.hpp"
#include "FileOperator.hpp"
using namespace std;
using FunptrOperator = bool (*)(vector<int> &, bool);
using FunptrConstruction = vector<int> (*)();

class dbca_Point {
public:
    int id;
    int neighbours_count = 0;
    int cluster_ID;
    vector<int> adjacentPoints;
};
class DBCA{
public:
    const int dbca_OUTLIER = -2;
    const int dbca_NOT_CLASSIFIED = -1;
    //Prepare output: sets for different parameters-> Sets -> Vertices
    void density_connected(int current_p, int cluster, int min_pt, vector<dbca_Point> &points);
    vector<int> tsp2evrp_zga_relaxed(vector<int> tspTour);
    vector<int> clarke_wright(bool capacitated, bool clusters, vector<int> node_list);
    vector<vector<vector<int>>> generated_dbca_parameter();
    vector<int> generate_initial_solution();
    bool get_to_depot_possibly_through_afss(vector<int> &evrpTour);
    bool addAndCheckLastN(int node, bool reset = false);
};




using FunptrOperator = bool (*)(vector<int> &, bool);
using FunptrConstruction = vector<int> (*)();

int getRandomAmongAvailable(unsigned availableCount, const vector<bool> &available);
void sa_run_heuristic(int run);
void sa2_run_heuristic(int run);
vector<int> sa_simu(int run,vector<FunptrOperator> neighborhoods, vector<int>  construction);
vector<int> sa_simu2(int run,vector<FunptrOperator> neighborhoods, vector<int>  construction);


void vns_run_heuristic(int run);
void vns_run2_heuristic(int run);
void rvnd(vector<int> &tour, bool merge, bool firstImprove, vector<FunptrOperator> neighborhoods);
vector<int> ms_vns(int run, bool merge, bool firstImprove, int p, double restart_ratio, vector<FunptrOperator> neighborhoods, vector<int>  construction);
vector<int>ms_vnst(int run,bool merge, bool firstImprove, int p, double restart_ratio, vector<FunptrOperator> neighborhoods, vector<int> construction);
    

#endif