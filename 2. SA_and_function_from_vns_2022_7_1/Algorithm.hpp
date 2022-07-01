#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP
#include <vector>
#include "Operators.hpp"
#include "Macro.hpp"
#include "FileOperator.hpp"
#include "Random.hpp"

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
    //Prepare output: sets for different parameters-> Sets -> Vertices
    void density_connected(int current_p, int cluster, int min_pt, vector<dbca_Point> &points);
    vector<int> tsp2evrp_zga_relaxed(vector<int> tspTour);
    vector<int> clarke_wright(bool capacitated, bool clusters, vector<int> node_list);
    vector<vector<vector<int>>> generated_dbca_parameter();
    vector<int> generate_initial_solution();
    bool get_to_depot_possibly_through_afss(vector<int> &evrpTour);
    bool addAndCheckLastN(int node, bool reset = false);
};


class Ant{
    public:
        int step;
        int solution[MAX_NODE];

};



class ACO{
    public:
    int run;
    int num_of_ants;
    double taumax;
    int actual_problem_size;
    Randoms *randomt;



    Problem* problem = Problem::getInstance();
    // vector<vector<int>> cities;
    vector<vector<double>> pheromones;
    vector<vector<double>> deltapheromones;
    vector<vector<double>> Probability;
    vector<Ant> Ants;
    
    
    
    ACO (int run);
    void optimize();
    void set_initial_information();
    void construct_route();
    void update_upheromones();
    double update_proability(int cityi, int cityj, int *visited);

};

class SA{
    public:
    std::vector<FunptrOperator> selectedOperators{
        AFSrealoc_one,
        AFSrealoc_more_than_one,
        twoOpt,
        onePoint,
        twoPoint,
        threePoint
    };

    int run;
    double start_temperature;
    double end_temperature;
    double delta;
    SA(int run);
    DBCA dbca;
    void generalizedDoubleBridge(vector<int> &tour, unsigned p);
    void mergeAFSs(vector<int> &tour);
    void Local_Search(vector<int> &tour, bool firstImprove, vector<FunptrOperator> neighborhoods);
    void optimize();
};






#endif