
#include "ACO.hpp"
#include "Problem.hpp"
#include <cstdlib>
#include <iostream>

// bool termination_condition(ACO aco)
// {
//     //return ((iteration >= max_iteration) ||
//             (fabs(aco.best_so_far_ant.tour_length - optimum) < 10 * EPSILON);
// }

using namespace std;
int main(int argc, char *argv[]) {
  int run;
  read_arguments(argc, argv);
  read_parameters();
  read_instances();
  ACO aco; // 完成一些东西的初始化

  // begin for
  aco.init(); //完成一些基本的事物的初始化
  while (iteration<=1000) {
      aco.optimize();
      iteration += 1;
  }
  aco.free();
  //end for

  // for(run = 1; run <= max_trials; run++){
  //   aco.Trial_init();
  //   while (!termination_condition(aco)) {
  //     aco.optimize();
  //   }
  //   aco.Trial_free();
  // }
  // free_stats();

  // aco.freeall();
  // free_heuristic();
  // free_problem();
  return 0;
}