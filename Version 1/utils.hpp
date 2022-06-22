#ifndef UTILS_HPP
#define UTILS_HPP
#include "variables.hpp"


using namespace std;

string getFileName(){
    return string("./Data/") + string(problem_instance);
}

void print_distance_matrix(void) {
    int i, j;
    cout << "\t";
    for (i = 0; i < ACTUAL_PROBLEM_SIZE - 1; i++) {
        cout << i << "\t";
    }
    cout << ACTUAL_PROBLEM_SIZE - 1 << endl;
    for (i = 0; i < ACTUAL_PROBLEM_SIZE; i++) {
        cout << i << "\t";
        for (j = 0; j < ACTUAL_PROBLEM_SIZE - 1; j++) {
            cout << distances[i][j] << "\t";
        }
        cout << distances[i][ACTUAL_PROBLEM_SIZE - 1] << endl;
    }
}


#endif //EVRP_EVRP_HPP