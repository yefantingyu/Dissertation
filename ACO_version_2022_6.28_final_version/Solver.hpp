#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include "Methods.hpp"
using namespace std;
class Solver{
public:
    Problem *problem=Problem::getInstance();
    SACO *aco;
    SimulateAnnealing * SA;
    GA* ga;

    vector<int> int_parameter;
    vector<int> double_parameter;

    string flag;
    char * midlist;
    vector<string> parameter_list;
    vector<double> conv;

    Solver(string methods);
    ~Solver();
    void init();
    void test();
    void get_par_list();
    void init_algorithm(int r);
    void start_run(int r);
    void end_run(int r);
    bool termination_condition(double rate = 1.0);

    void optimize(int run);
    void save_solution(int run);


      FILE *log_performance;
        char *perf_filename;
        double* perf_of_trials;
        void isFileExist(string filename);
        void read_parameter_files();
        void open_result_file();
        void get_mean(int r, double value);
        double mean(double* values, int size);
        double stdev(double* values, int size, double average);
        double best_of_vector(double *values, int l );
        double worst_of_vector(double *values, int l );
        void close_file(void);
        void freeall();
};

    

