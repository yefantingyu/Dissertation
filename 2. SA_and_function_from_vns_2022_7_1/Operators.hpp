#ifndef OPERATORS_HPP
#define OPERATORS_HPP

#include <vector>
#include "Problem.hpp"
#include "assert.h"

using namespace std;
using FunptrOperator = bool (*)(vector<int> &, bool);
using FunptrConstruction = vector<int> (*)();


bool twoStringMove(vector<int> &tour, int i, int j, int X, int Y);

bool twoString(vector<int> &tour, int X, int Y, bool firstImrpove);

double twoStringCostUpdate(vector<int> &tour, int i, int j, int X, int Y);

inline bool onePoint(vector<int> &tour, bool firstImprove) {
    return twoString(tour, 0, 1, firstImprove);
}

inline bool twoPoint(vector<int> &tour, bool firstImprove) {
    return twoString(tour, 1, 1, firstImprove);
}

inline bool threePoint(vector<int> &tour, bool firstImprove) {
    return twoString(tour, 1, 2, firstImprove);
}

void twoOptMove(vector<int> &tour, int i, int j);

bool twoOpt(vector<int> &tour, bool firstImprove);

double twoOptCostUpdate(vector<int> &tour, int i, int j);

bool AFSrealoc_one(vector<int> &tour, bool firstImprove);
bool AFSrealoc_more_than_one(vector<int> &tour, bool firstImprove);

vector<int> tsp2evrp_zga_mini(vector<int> tspTour);
bool addAndCheckLastN(int node, bool reset = false);
bool get_to_depot_possibly_through_afss(vector<int> &evrpTour);
vector<int> tsp2evrp_zga_relaxed(vector<int> tspTour);

#endif //EVRP_UTILS_HPP


