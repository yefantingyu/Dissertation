#ifndef EVRP_UTILS_HPP
#define EVRP_UTILS_HPP
#include <vector>
// #include "Problem.hpp"
#include "Permutation.hpp"
using namespace std;


class floydWarshall {
    int nV; // graph size
    Problem* problem = Problem::getInstance();
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

class Operators{
public:
    Operators(){
        fw = floydWarshall(problem->AFs.size());
        fw.planPaths();
    }
    Problem* problem = Problem::getInstance();
    floydWarshall fw;
    
    double triangle_adition(int afs, int b, int c);
    double triangle_adition2(int afs, int b, int c, double dist_bc);
    bool twoStringMove(vector<int> &tour, int i, int j, int X, int Y);
    bool twoString(vector<int> &tour, int X, int Y, bool firstImrpove);
    double twoStringCostUpdate(vector<int> &tour, int i, int j, int X, int Y);
    bool onePoint(vector<int> &tour, bool firstImprove) {
        return twoString(tour, 0, 1, firstImprove);
    }
    bool twoPoint(vector<int> &tour, bool firstImprove) {
        return twoString(tour, 1, 1, firstImprove);
    }
    bool threePoint(vector<int> &tour, bool firstImprove) {
        return twoString(tour, 1, 2, firstImprove);
    }
    void twoOptMove(vector<int> &tour, int i, int j);
    bool twoOpt(vector<int> &tour, bool firstImprove);
    double twoOptCostUpdate(vector<int> &tour, int i, int j);
    bool AFSrealoc_one(vector<int> &tour, bool firstImprove);
    bool AFSrealoc_more_than_one(vector<int> &tour, bool firstImprove);
    double AFSrealoc_more(vector<int> &tour);
    vector<int> tsp2evrp_zga_mini(vector<int> tspTour);
    bool lost_customers(vector<int> test_tour, int orig_number);
    double AFSrealoc_one_afs(vector<int> &tour, int original_afs_at);
    bool AFSrealoc_common(vector<int> &tour, bool firstImprove, bool more_than_one);
    bool addAndCheckLastN(int node, bool reset = false);
    bool get_to_depot_possibly_through_afss(vector<int> &evrpTour);
    void generalizedDoubleBridge(vector<int> &tour, unsigned p) ;
    vector<int> tsp2evrp_zga_relaxed(vector<int> tspTour);
    void mergeAFSs(vector<int> &tour);
    
};








#endif //EVRP_UTILS_HPP