#ifndef OPERATORS_HPP
#define OPERATORS_HPP
#include <cstdlib>
#include <cstring>
#include <vector>
#include "Problem.hpp"
#include <random>
#include <cfloat>
#include <algorithm>
#include "stdlib.h"

using namespace std;

class Permutation{
public:
    // for route checker
    int checker_4_array[4];
    int checker_Last = 0;
    int checker_counter = 0;


    //
    
    Problem* problem = Problem::getInstance();
    int graph_size=problem->AFs.size();
    vector<vector<int>> dist; // matrix of shortest distances; dist[i][j] ~ distance from i to j
    vector<vector<int>> next; // next[i][j] ~ next node on the shortest path from i to j

   
    void initgraph();
    vector<int> getgraph(int u, int v, bool afsIds=true);
    bool route_checker(int node, bool reset=false);
    bool pass_afs_to_depot(vector<int> & tour);
    vector<int> Tour_generation(vector<int> tspTour);
    void generalizedDoubleBridge(vector<int> &tour, unsigned p);
    void mergeAFSs(vector<int> &tour);


};




#endif //EVRP_UTILS_HPP