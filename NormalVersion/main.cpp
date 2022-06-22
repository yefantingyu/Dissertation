#include <cstdlib>
#include <cstring>
#include<iostream>
#include<string>
#include<fstream>
#include <vector>
#include "Solver.hpp"
#include "FileOperator.hpp"
#include "Macro.hpp"
using namespace std;

int main(int argc, char *argv[]) {
    Problem problem(argv[2]);
    problem.instance_reader();
    Solver solver(argv[1],argv[3], problem);
    solver.init();
    FileOperator operatorr;
    operatorr.open_file(problem.instance);

    for(int run = 1; run <= MAX_TRIALS; run++){
        solver.start_run(run);
        // while(!solver.termination_condition(1)){
        //     solver.optimize(run);
        // }
        // solver.save_solution(algorithm, problem_instance, run); 
        // solver.freeall();
    }



}
