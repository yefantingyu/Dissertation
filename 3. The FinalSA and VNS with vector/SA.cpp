#include "Algorithm.hpp"
#include "Problem.hpp"
void local_search(vector<int> &tour, bool merge, bool firstImprove,
                  vector<FunptrOperator> neighborhoods) {
  int i = 0;

  // nlSize ~ number of available neighborhoods
  auto nlSize = static_cast<unsigned>(neighborhoods.size());

  // available[i] == true ~ neighborhood i is available
  std::vector<bool> available(neighborhoods.size(), true);

  // Iterate until there are available neighborhoods
  while (nlSize > 0) {
    // Pick neighborhood index randomly among available neighborhoods
    auto neighborhoodIdx = getRandomAmongAvailable(nlSize, available);
    // Attempt to improve the tour in the selected neighborhood
    if ((*neighborhoods[neighborhoodIdx])(tour, firstImprove)) {
      // If success, then set all neighborhoods to be available again
      nlSize = static_cast<unsigned>(neighborhoods.size());
      std::fill(available.begin(), available.end(), true);
      if (merge) {
        mergeAFSs(tour);
      }
      double a = get_evals();
      double b = termination;
      // cout << "RVND iter: " << ++i << ", evals: " << get_evals() << ",
      // fitness: " << fitness_evaluation(tour) << ", progress: " << a/b <<
      // endl;
    } else {
      // If not success, then make the current neighborhood unavailable and
      // decrease nlSize
      available[neighborhoodIdx] = false;
      --nlSize;
    }
  }
}

vector<int> sa_simu(int run, vector<FunptrOperator> neighborhoods,
                    vector<int> construction) {
  int n = actual_problem_size;
  auto current = construction;
  double current_score = fitness_evaluation(current);
  auto current_best = construction;
  double current_best_score = fitness_evaluation(current_best);
  auto total_best = construction;
  double total_best_score = fitness_evaluation(total_best);

  int counter = 0;
  double current_temperature = saparameter.T_start;

  // Outer loop
  // Attempt at most vns_restarts iters. of VNS
  while (current_temperature > saparameter.T_end && counter < STOP_CNT) {
    auto current = current_best;
    generalizedDoubleBridge(current, 2);
    local_search(current, true, false, neighborhoods);
    current_score = fitness_evaluation(current);

    double a = get_evals();
    double b = termination;

    default_random_engine generatort(rand());
    uniform_int_distribution<int> distribution(0, 1);
    auto r = distribution(generatort);
    double difference = current_best_score - current_score;
    if(difference > 0){
        if(current_best_score>total_best_score){
            total_best = current;
            current_best  = current;
        }

    }


    if (difference > 0 || difference <0 && r < std::exp(difference / current_temperature)) {
      current_best = current;
      current_best_score = current_score;
      cout<<"Run "<<run<<" Iteration: "<<counter<< " State: Update "
           << " Current temp " << current_temperature
           << ", current: " << current_score << ", current best: " << current_best_score<<" Total best: "<<total_best_score
           << ", progress: " << a / b << endl;
    } else {
      cout<<"Run "<<run<<" Iteration: "<<counter<< " State: Non-Update "
           << " Current temp " << current_temperature
           << ", current: " << current_score << ", current best: " << current_best_score<<" Total best: "<<total_best_score
           << ", progress: " << a / b << endl;
        }

    current_temperature =
        current_temperature / (1 + current_temperature * saparameter.delta);
    counter++;
  }
return total_best;
}
vector<int> sa_simu2(int run, vector<FunptrOperator> neighborhoods,
                     vector<int> construction) {
  int n = actual_problem_size;
  auto best = construction;
  double best_score = fitness_evaluation(best);

  int counter = 0;
  double current_temperature = saparameter.T_start;

  // Outer loop
  // Attempt at most vns_restarts iters. of VNS
  while (current_temperature > saparameter.T_end && counter < STOP_CNT) {
    auto current = best;
    generalizedDoubleBridge(current, 2);
    local_search(current, true, false, neighborhoods);
    double current_score = fitness_evaluation(current);

    double a = get_evals();
    double b = termination;

    default_random_engine generatort(rand());
    uniform_int_distribution<int> distribution(0, 1);
    auto r = distribution(generatort);
    double difference = best_score - current_score;
    if (difference > 0 || r < std::exp(difference / current_temperature)) {
      best = current;
      best_score = current_score;
      cout << "Update Run " << run << " Counter: " << counter
           << " Current temp " << current_temperature
           << ", current: " << current_score << ", best: " << best_score
           << ", progress: " << a / b << endl;
    } else {
      cout << "Non-Run " << run << " Counter: " << counter << " Current temp "
           << current_temperature << ", current: " << current_score
           << ", best: " << best_score << ", progress: " << a / b << endl;
    }

    current_temperature =
        current_temperature / (1 + current_temperature * saparameter.delta);
    counter++;
  }
  return best;
}

/*implement your heuristic in this function*/
void sa_run_heuristic(int run) {
  DBCA dbca;
  dbca.generated_dbca_parameter();
  auto construction = dbca.generate_initial_solution();
  std::vector<FunptrOperator> selectedOperators{
      AFSrealoc_one, AFSrealoc_more_than_one, twoOpt, onePoint, twoPoint,
      threePoint};
  bool merge = true;
  bool firstImprove = false;
  auto evrpTour = sa_simu(run, selectedOperators, construction);

  full_validity_check(evrpTour);

  // Fill best_sol structure
  for (int i = 0; i < evrpTour.size(); i++) {
    best_sol.tour[i] = evrpTour[i];
  }
  best_sol.steps = evrpTour.size();
  best_sol.tour_length = fitness_evaluation(evrpTour);
}
/*implement your heuristic in this function*/
void sa2_run_heuristic(int run) {
  DBCA dbca;
  dbca.generated_dbca_parameter();
  auto construction = dbca.generate_initial_solution();
  std::vector<FunptrOperator> selectedOperators{
      AFSrealoc_one, AFSrealoc_more_than_one, twoOpt, onePoint, twoPoint,
      threePoint};

  auto evrpTour = sa_simu2(run, selectedOperators, construction);

  full_validity_check(evrpTour);

  // Fill best_sol structure
  for (int i = 0; i < evrpTour.size(); i++) {
    best_sol.tour[i] = evrpTour[i];
  }
  best_sol.steps = evrpTour.size();
  best_sol.tour_length = fitness_evaluation(evrpTour);
}