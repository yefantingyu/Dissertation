#include "Algorithm.hpp"
#include "FileOperator.hpp"
#include "Problem.hpp"
#include <cstdlib>
#include <iostream>

using namespace std;

void start_run(int r) {
  srand(r);
  init_evals();
  init_current_best();
  cout << "Run: " << r << " with random seed " << r << endl;
}
void end_run(int r) {
  get_mean(r - 1, get_current_best()); // from stats.h
  cout << "End of run " << r << " with best solution quality "
       << get_current_best()
       << " total evaluations: " << get_evals() << endl;
  cout << " " << endl;
}


bool termination_condition(void) {
  bool flag;
  if (get_evals() >= termination)
    flag = true;
  else
    flag = false;

  return flag;
}
void run_heuristic(int run){
  if(flag=="VNS"){
    if(run==max_trials){
      vns_run2_heuristic(run);
    }
    else{
      vns_run_heuristic(run);
    }
  }
  // else if(flag=="SA"){
  //   if(run==max_trials){
  //     sa2_run_heuristic(run);
  //   }
  //   else{
  //     sa_run_heuristic(run);
  //   }
  // }
  // else{
  //   if(run==max_trials){
  //     aco2_heuristic(run);
  //   }
  //   else{
  //     aco_heuristic(run);
  //   }
  // }


}
/****************************************************************/
/*                Main Function                                 */
/****************************************************************/
int main(int argc, char *argv[]) {
  int run;
  read_instances(argv[1], argv[2]);
  read_parameters();
  open_stats();//open text files to store the best values from the 20 runs stats.h
  for(run = 1; run <= max_trials; run++){
    start_run(run);
    initialise_whole_problem_world();
    run_heuristic(run);
    end_run(run);
  }
  save_results();
  close_stats();
  free_stats();
  free_heuristic();
  return 0;
}