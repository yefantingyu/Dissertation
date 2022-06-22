#include <cstdlib>
#include <cstring>
#include<iostream>
#include<string>
#include<fstream>
#include <vector>
#include "FileOperator.hpp"
#include "utils.hpp"
#include "Solver.hpp"
using namespace std;

int main(int argc, char *argv[]) {
    method = argv[1];
    instance = argv[2];
    char* paramter_list =argv[3];
    instance_path = instance_path + instance;
    instance_reader(instance_path);
    initialise_stats();


    Solver solver(method, paramter_list);
    solver.init();

    for(int run = 1; run <= MAX_TRIALS; run++){
        start_run(run,solver);
        while(!termination_condition(solver,1)){
            solver.optimize(run);
        }
        solver.save_solution(algorithm, problem_instance, run); 
        solver.freeall();
    }



}
