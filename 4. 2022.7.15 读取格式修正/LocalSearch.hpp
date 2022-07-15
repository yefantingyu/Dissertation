#include "Problem.hpp"
#include <cfloat>
class LocalSearch{
public:
bool isValidTour(int* tour, int size);
double twoOptCostUpdate(int*tour, int i, int j);
void twoOptMove(int*tour, int i, int j);
bool twoOpt(int* tour, int size) ;
};