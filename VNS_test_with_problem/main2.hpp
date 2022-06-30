#include "Algorithm.hpp"
#include "Macro.hpp"
#include "Operators.hpp"
// #include "Solver.hpp"
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


// #include "Permutation.hpp"
#include "random"
using namespace std;



int main(int argc, char *argv[]) {
  Problem *problem = Problem::getInstance();
  problem->set_instance(argv[2]);
  problem->read_instance();

  Operators hoperator;

  DBCA dbca;

  /*
  One run

  */
  problem->init_evals();
  problem->init_current_best();
  problem->init();
  Permutation permutation;
  permutation.initgraph();
  auto construction = dbca.generate_initial_solution();
  vector<int> constructiont = construction;
  cout << "The begin size of the vector is "<< constructiont.size()<<endl;
  cout <<"The begin tour"<<endl;
  for (int i = 0; i < constructiont.size(); i++) {
    cout << constructiont[i] << " - ";
  }
  cout << "\n"<<endl;
  
  // permutation.generalizedDoubleBridge(constructiont, 2);
  // cout << "the size of the vector is "<< constructiont.size()<<endl;
  // for (int i = 0; i < constructiont.size(); i++) {
  //   cout << constructiont[i] << " - ";
  // }
  // cout << endl;

    int n = problem->ACTUAL_PROBLEM_SIZE;
    int vns_restarts = n*0.35;
    int vns_cnt = 0;
    vector<int> very_best;
    double best_score;
    auto best = construction;
    double current_score;
    best_score =  problem->fitness_evaluation(best);
     while (vns_cnt<100) {
        // Attempt at most vns_restarts iters. of VNS
            auto current = best;
            permutation.generalizedDoubleBridge(current, 2);
            twoOpt(current,false);
            hoperator.mergeAFSs(current);
            // if(problem->full_validity_check(current)){
              if(problem->isValidTour(current)){
              current_score = problem->fitness_evaluation(current);
              if ((current_score < best_score)) {
                  best = current;
                  best_score = current_score;
              }
            }
            else{
              vns_cnt++;
              cout <<"jump"<<endl;
              continue;
            }
            cout <<"Iteration  "<<vns_cnt <<"current_score "<<current_score <<"best score " <<best_score <<endl;
            for (int i = 0; i < current.size(); i++) {
              cout << current[i] << " - ";
            }
            cout<<endl;
          vns_cnt++;
       }

  
}