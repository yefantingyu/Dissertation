#ifndef ALGORITHM_h
#define ALGORITHM_h
#include <vector>
#include "Operators.hpp"
using namespace std;

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
    Problem* problem = Problem::getInstance();
    Operators operatort;
    //Prepare output: sets for different parameters-> Sets -> Vertices

    void density_connected(int current_p, int cluster, int min_pt, vector<dbca_Point> &points);
    vector<int> tsp2evrp_zga_relaxed(vector<int> tspTour);
    vector<int> clarke_wright(bool capacitated, bool clusters, vector<int> node_list);
    vector<vector<vector<int>>> generated_dbca_parameter();
    vector<int> generate_initial_solution();
};

#endif