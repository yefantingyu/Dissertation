#ifndef EVRP_Solver_HPP
#define EVRP_Solver_HPP

#include "variables.hpp"

#define MAX_NODE 1500



struct segment{
    int left;
    int right;
    segment(){};
    segment(int l, int r): left(l), right(r){} //寻找一段的左侧和右侧
};


class Solver{
public:
    segment Route[MAX_NODE];// All the position of the tour

        /* Mapping customer and a tour */
    int Route_index[MAX_NODE];// 1 3 5 7 8 9 order

    int Customer_index[MAX_NODE];// 1 3 5 7 8 9 order

        /* The order of customers */
    int order[MAX_NODE];// 1 3 5 7 8 9 order

    int solution[MAX_NODE];
    int num_of_routes = 0;


    void copy_Solver(Solver);

};

void Solver::copy_Solver(Solver other){
    for (int i = 0; i < NUM_OF_CUSTOMERS; i++){
        order[i] = other.order[i];
        Route_index[i] = other.Route_index[i];
    }
    this->num_of_tours = other.get_num_of_tours();
    for (int i = 0; i < this->num_of_tours; i++){
        tours[i].left = other.tours[i].left;
        tours[i].right = other.tours[i].right;
    }
    this->fitness = other.get_fitness();

}



#endif //EVRP_EVRP_HPP