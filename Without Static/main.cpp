#include <cstdlib>
#include <cstring>
#include<iostream>
#include<string>
#include<fstream>
#include <vector>
#include "Solver.hpp"
#include "Macro.hpp"
using namespace std;

int main(int argc, char *argv[]) {
    static Problem problem(argv[2]);
    problem.instance_reader();
    FileOperator operatorr;
    operatorr.open_file(problem.instance);
    Solver solver(argv[1],argv[3], problem, operatorr);
    solver.init();


    for(int run = 1; run <= MAX_TRIALS; run++){
        solver.start_run(run);
        // while(!solver.termination_condition(1)){
        //     solver.optimize(run);
        // }
        // solver.save_solution(algorithm, problem_instance, run); 
        // solver.freeall();

        solver.end_run(run);
    }
    operatorr.close_stats();


}
