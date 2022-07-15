#include "LocalSearch.hpp"
#include "Problem.hpp"
#include <cmath>
/*
 * Checks validity of given evrp tour.
 * Terminates right after reaching first invalid node.
 */
bool LocalSearch::isValidTour(int* tour,int size) {
    int load = 0;
    double battery = 0;
    int custCnt = 0;
    for (int i = 0; i < size; i++) {
        int cur = tour[i];
        // Load check
        if (cur == 0) {
            load = vehicle_capacity;
        } else {
            load -= get_customer_demand(cur);
            if (load < 0) return false;
        }
        if (is_charging_station(cur)){
        } else{
            custCnt++;
        }
    }
    // Closed tour check
    if ((tour[0] != 0) || (tour[size - 1] != 0)) {
        return false;
    }
    if (custCnt < num_of_customers) {
        return false;
    }
    return true;
}
double LocalSearch::twoOptCostUpdate(int*tour, int i, int j) {
    double cut1 = get_distance(tour[i - 1], tour[i]); // cut before i
    double cut2 = get_distance(tour[j], tour[j + 1]); // cut after j

    double removed = cut1 + cut2;

    double add1 = get_distance(tour[i - 1], tour[j]); // edge added after i - 1
    double add2 = get_distance(tour[i], tour[j + 1]); // edge added before j + 1

    double added = add1 + add2;


    return removed - added;
}
void LocalSearch::twoOptMove(int*tour, int i, int j) {
    reverse(tour + i, tour+ j + 1);
}

bool LocalSearch::twoOpt(int* tour, int size) {
    int* tmpTour = new int[size];
    int* bestTour = new int[size];
    bool improved = false;
    double impBest = INFINITY;
    for (int i = 1; i < size - 1; i++) {
        if (improved){
            break;
        }
        for (int j = i + 1; j < size - 1; j++) {
            if (improved){ 
                break;
            }
            auto impCurr = twoOptCostUpdate(tour, i, j);
            if (impCurr > impBest) {
                tmpTour = tour;
                twoOptMove(tmpTour, i, j);
                if (isValidTour(tmpTour,size)) {
                    impBest = impCurr;
                    bestTour = tmpTour;
                    improved = true;
                }
            }

        }
    }
    if (improved) {
        tour = bestTour;
    }
    delete[] tmpTour;
    delete[] bestTour;
    return improved;
}