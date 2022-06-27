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
    Problem *problem = Problem::getInstance();
    problem->set_instance(argv[1]);
    problem->read_instance();

    Solver solver(argv[2],argv[3]);
    solver.init();
    solver.open_file();
   
    for(int run = 1; run <= MAX_TRIALS; run++){
        solver.start_run(run);
        while(!solver.termination_condition(1)){
            solver.optimize(run);
        }
        cout << "question begins here"<<endl;
       // solver.save_solution(run); 
         cout << "question end here"<<endl;;
        solver.freeall();
       cout << "question end here3"<<endl;;
        solver.end_run(run);
    }
    solver.close_file();
}
