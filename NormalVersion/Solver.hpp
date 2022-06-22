#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include "Problem.hpp"
using namespace std;
class Solver{
public:
    Problem *problemt;

    double evals;
    double current_best;
    string flag;
    char * midlist;
    vector<string> parameter_list;

    // SACO * aco;
    vector<double> conv;

    Solver(string methods, char*list, Problem & problem);
    void init();
    void test();
    void get_par_list();
    void init_algorithm();
    void init_evals();					    //initializes the evaluations
    void init_current_best();				//initializes the best solution quality
    void start_run(int r);
    bool termination_condition(double rate = 1.0);

    void optimize(int run);
    void save_solution(int run);

};


    

