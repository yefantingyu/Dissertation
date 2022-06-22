#ifndef EVRP_Greedy_HPP
#define EVRP_Greedy_HPP

#include "Solver.hpp"
class GreedySearch {
public:

    Solver best_solution;
    Solver current_solution;

    GreedySearch();

    void run(Solver &sol);


};

GreedySearch::GreedySearch(){

}


void GreedySearch::run(Solver &sol){

}


#endif //EVRP_EVRP_HPP

