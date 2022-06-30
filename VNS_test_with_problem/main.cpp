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

int getRandomAmongAvailable(unsigned availableCount,
                            const vector<bool> &available) {
  //    default_random_engine
  //    generator(chrono::system_clock::now().time_since_epoch().count());
  default_random_engine generator(rand());
  uniform_int_distribution<int> distribution(0, availableCount - 1);
  cout << "Random Error 2312312" << endl;

  auto r = distribution(generator);

  cout << "Random Error 231231211111111111" << endl;
  for (unsigned i = 0; i < available.size(); ++i) {
    if (available[i]) {
      if (r == 0) {
        return i;
      }
      --r;
    }
  }
  return -1;
}

void localSearch(vector<int> &tour, Operators &opt) {
  int nlSize = 5;
  std::vector<bool> available(5, true);
  // Iterate until there are available neighborhoods
  while (nlSize > 0) {
    // Pick neighborhood index randomly among available neighborhoods
    cout << "Random Error" << endl;
    // auto neighborhoodIdx = getRandomAmongAvailable(nlSize, available);
    auto neighborhoodIdx=0;
    bool changed = false;
    cout << "go out switch" << endl;
    cout << " " << neighborhoodIdx << endl;
    switch (neighborhoodIdx) {
    case 0:
      cout << "\n" << endl;
      changed = opt.AFSrealoc_one(tour, true);
      break;
    case 1:
      changed = opt.AFSrealoc_more_than_one(tour, true);
      break;
    case 2:
      changed = opt.twoOpt(tour, true);
      break;
    case 3:
      changed = opt.onePoint(tour, true);
      break;
    case 4:
      changed = opt.twoPoint(tour, true);
      break;
    case 5:
      changed = opt.threePoint(tour, true);
      break;
    default:
      break;
    }

    cout << "go end switch" << endl;
    // Attempt to improve the tour in the selected neighborhood
    if (changed) {
      // If success, then set all neighborhoods to be available again
      nlSize = 5;
      std::fill(available.begin(), available.end(), true);
      opt.mergeAFSs(tour);
    } else {
      // If not success, then make the current neighborhood unavailable and
      // decrease nlSize
      available[neighborhoodIdx] = false;
      --nlSize;
    }
  }
}

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
  cout << "the size of the vector is "<< constructiont.size()<<endl;
  for (int i = 0; i < constructiont.size(); i++) {
    cout << constructiont[i] << " - ";
  }
  cout << endl;
  permutation.generalizedDoubleBridge(constructiont, 2);
  cout << "the size of the vector is "<< constructiont.capacity()<<endl;
  for (int i = 0; i < constructiont.capacity(); i++) {
    cout << constructiont[i] << " - ";
  }
  cout << endl;
  
}