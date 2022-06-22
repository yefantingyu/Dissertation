#include "file_operation.hpp"
#include "variables.hpp"
#include "Solver.hpp"
#include "utils.hpp"


#include "Greedy.h"
#include "ACO.h"
#include "GA.h"
#include "AS.h"

using namespace std;

void end_project(){
    close_stats(); //close text files to calculate the mean result from the 20 runs stats.h
    free_stats();
    // free_heuristic();
    // free_EVRP();
}



int main(int argc, char *argv[]){
    problem_instance = argv[2]; // Read the name of the instance 
    string const& filepath = getFileName();
    read_problem(filepath);   //Read EVRP from file from EVRP.h 
    initialise_files();






    end_project();
}