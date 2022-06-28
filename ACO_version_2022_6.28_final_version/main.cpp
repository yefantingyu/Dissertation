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
  problem->set_instance(argv[2]);
  problem->read_instance();
  Solver solver(argv[1]);
  solver.read_parameter_files();
  solver.open_result_file();

  for (int run = 1; run <= MAX_TRIALS; run++) {
    solver.start_run(run);
    solver.init_algorithm(run);
    while (!solver.termination_condition(1)) {
      solver.optimize(run);
    }
    solver.freeall();
    solver.end_run(run);
  }
  solver.close_file();
}
