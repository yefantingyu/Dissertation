#include "Algorithm.hpp"
#include "FileOperator.hpp"
#include <cstdlib>
#include <iostream>

using namespace std;


// Problem * problem = Problem::getInstance();

/*initialiazes a run for your heuristic*/
void start_run(int r) {
  srand(r); // random seed
  problem->init_evals();
  problem->init_current_best();
  cout << "Run: " << r << " with random seed " << r << endl;
}

/*gets an observation of the run for your heuristic*/
void end_run(int r) {

  foperator.get_mean(r - 1, problem->get_current_best()); // from stats.h
  cout << "End of run " << r << " with best solution quality "
       << problem->get_current_best()
       << " total evaluations: " << problem->get_evals() << endl;
  cout << " " << endl;
}

/*sets the termination conidition for your heuristic*/
bool termination_condition(void) {
  bool flag;
  if (problem->get_evals() >= problem->TERMINATION)
    flag = true;
  else
    flag = false;

  return flag;
}

/****************************************************************/
/*                Main Function                                 */
/****************************************************************/
int main(int argc, char *argv[]) {
  int run;
  problem->read(argv[1], argv[2]);
  foperator.init();

  /* for*/
  start_run(run);
  problem->initialist_solution();
  problem->initothers();

  // code begin
  if (problem->flag == "ACO") {
    ACO aco(run);
    while (!termination_condition()) {
      aco.optimize();
    }
    //save solution
  }
  else if(problem->flag =="VNS"){
    //VNS vns(run);
    //vns.init();
    //while (!termination_condition()) {
      //vns->optimize();
    //}
    //save solution
  }
  else{
    SA sa(run);
    while (!termination_condition()) {
      sa.optimize();
    }
    
    foperator.save_results();
  }

  // code end

  // end for

  // close stats
  // free everything

  return 0;
}