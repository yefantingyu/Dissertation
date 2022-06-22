#ifndef UTILS_HPP
#define UTILS_HPP
#include <cstdlib>
#include <vector>
#include <fstream>
#include<iostream>
#include<limits.h>
#include <vector>
#include<stdlib.h>
#include "Solver.hpp"
#include "Problem.hpp"

void start_run(int r, Solver solver){

  srand(r); //random seed
  solver.init_evals();
  solver.init_current_best();
  cout << "Start Runing Round: " << r << " with random seed " << r << endl;
}
/*sets the termination conidition for your heuristic*/
bool termination_condition(Solver solver,double rate = 1.0) {

  bool flag;
  if(solver.evals >= rate * TERMINATION)
    flag = true;
  else
    flag = false;

  return flag;
}



#endif // FILE_OPERATION_HPP