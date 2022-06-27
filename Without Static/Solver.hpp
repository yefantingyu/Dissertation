#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include "Problem.hpp"
#include "FileOperator.hpp"
using namespace std;
class Solver{
public:
    Problem *problemt;
    FileOperator *operatorr;
    SACO *aco;

    string flag;
    char * midlist;
    vector<string> parameter_list;

    
    solution *best_sol;



    vector<double> conv;

    Solver(string methods, char*list, Problem & problem, FileOperator& operatorr);
    void init();
    void test();
    void get_par_list();
    void init_algorithm();
    void init_evals();					    //initializes the evaluations
    void init_current_best();				//initializes the best solution quality
    void start_run(int r);
    void end_run(int r);
    bool termination_condition(double rate = 1.0);

    void optimize(int run);
    void save_solution(int run);

};


    

