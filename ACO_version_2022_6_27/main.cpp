#include "Macro.hpp"
#include "Solver.hpp"
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main(int argc, char *argv[]) {
  Problem *problem = Problem::getInstance();
  problem->set_instance(argv[1]);
  problem->read_instance();

  Solver solver(argv[2], argv[3]);
  solver.init();
  solver.open_file();

  for (int run = 1; run <= MAX_TRIALS; run++) {
    solver.start_run(run);
    while (!solver.termination_condition(1)) {
      solver.optimize(run);
    }
    // solver.save_solution(run);
    solver.freeall();
    solver.end_run(run);
  }
  solver.close_file();
}
