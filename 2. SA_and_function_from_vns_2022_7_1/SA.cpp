#include "Algorithm.hpp"
#include "FileOperator.hpp"
#include "Problem.hpp"
#include "cmath"
#include <algorithm>
#include <random>

int getRandomAmongAvailable(unsigned availableCount,
                            const vector<bool> &available) {
  //    default_random_engine
  //    generator(chrono::system_clock::now().time_since_epoch().count());
  default_random_engine generator(rand());
  uniform_int_distribution<int> distribution(0, availableCount - 1);
  auto r = distribution(generator);
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

SA::SA(int runt) {
  run = runt;
  start_temperature = problem->sa_parameter.T_start;
  end_temperature = problem->sa_parameter.T_end;
  delta = problem->sa_parameter.delta;
}

void SA::optimize() {
  int counter = 0;
  int iteration = 1;
  bool firstImprove = false;
  dbca.generated_dbca_parameter();
  auto construction = dbca.generate_initial_solution();
  auto inner_best_solution = construction;
  auto total_best_solution = construction;
  double current_temperature = start_temperature;
  double total_best_score = problem->fitness_evaluation(construction);
  double current_best_score = total_best_score;
  foperator.Final_results.push_back(total_best_score);
  while ((counter < problem->STOP_CNT) &&
         (current_temperature >= end_temperature)) {
    auto current_solution = inner_best_solution;
    generalizedDoubleBridge(current_solution, 2);
    Local_Search(current_solution, firstImprove, selectedOperators);
    double current_score = problem->fitness_evaluation(current_solution);
    double difference = current_score - current_best_score;
    if (difference > 0) {
      current_best_score = current_score;
      inner_best_solution = current_solution;
      if (current_best_score < total_best_score) {
        total_best_score = current_best_score;
        total_best_solution = inner_best_solution;
        cout << " Status: Best Update "
             << " Iteration: " << problem->get_evals()
             << "current temperature "<< current_temperature
             << " Current Best score: " << current_best_score
             << " total best score: " << total_best_score << endl;
      } else {
        cout << " Status: Good Update "
             << " Iteration: " << problem->get_evals()
             << " current temperature "<< current_temperature
             << " Current Best score: " << current_best_score
             << " total best score: " << total_best_score << endl;
      }

    } else {
      default_random_engine generatort(rand());
      uniform_int_distribution<int> distribution(0, 1);
      auto r = distribution(generatort);
      if (r <= std::exp(difference / current_temperature)) {
        current_best_score = current_score;
        inner_best_solution = current_solution;
        cout << " Status: less goog Update "
             << " Iteration: " << counter
             << " current temperature "<< current_temperature
             << " Current Best score: " << current_best_score
             << " total best score: " << total_best_score << endl;
      } else {
        cout << " Status: No Update "
             << " Iteration: " << counter
             << " current temperature "<< current_temperature
             << " Current Best score: " << current_best_score
             << " total best score: " << total_best_score << endl;
      }
    }

    current_temperature = delta * current_temperature;
    counter++;
    foperator.Final_results.push_back(total_best_score);
    // if (counter % problem->ACTUAL_PROBLEM_SIZE == 0) {
    //   foperator.Final_results[iteration++] = total_best_score;
    // }
  }
}
void SA::Local_Search(vector<int> &tour, bool firstImprove,
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
      mergeAFSs(tour);
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

void SA::generalizedDoubleBridge(vector<int> &tour, unsigned p) {
  auto n = tour.size();
  p = (p + 1 > n) ? n - 1 : p;

  /// First randomly choose p different positions on the path.
  //    default_random_engine
  //    generator(chrono::system_clock::now().time_since_epoch().count());
  default_random_engine generator(rand());
  auto distribution = uniform_int_distribution<int>(0, n - 2);
  std::vector<bool> throwsBool(n, false);
  for (unsigned i = 0; i < p; ++i) {
    int throwI;
    do {
      throwI = distribution(generator);
    } while (throwsBool[throwI]);
    throwsBool[throwI] = true;
  }

  /// Then split the path to (p + 1) strings :
  ///     pathStrings[0]: < 0, 1, ... , throw(1) >,
  ///     pathStrings[1]: < throw(1) + 1, ... , throw(2) >
  ///     ...
  ///     pathStrings[p - 1]: < throw(p - 1) + 1, ... , throw(p) >,
  ///     pathStrings[p]: < throw(p) + 1, ... , n >
  std::vector<std::vector<int>> pathStrings{}; // these are indices
  for (unsigned i = 0; i < n; ++i) {
    std::vector<int> pathString{};
    while (!throwsBool[i] && i < n - 1) {
      pathString.emplace_back(i++);
    }
    pathString.emplace_back(i);
    pathStrings.push_back(pathString);
  }

  /// Create new path that starts with the first string.
  std::vector<int> newPositions(pathStrings[0]);

  /// Create roulette and shuffle it to get an order in which the rest of the
  /// strings will be connected.
  std::vector<int> roulette = std::vector<int>(pathStrings.size());
  for (unsigned i = 0; i < roulette.size(); ++i)
    roulette[i] = i;
  std::shuffle(roulette.begin() + 1, roulette.end(), generator);

  /// Connect the strings to get the new path.
  for (unsigned k = 1; k < pathStrings.size(); ++k) {

    /// Pick a next string to be connected to the new path.
    auto &nextString = pathStrings[roulette[k]];

    /// Flip a coin.
    auto side = uniform_int_distribution<unsigned>(0, 1)(generator);
    if (side == 0) {

      /// If side == 0, then connect the next string in ascending order.
      for (int i : nextString) {
        newPositions.push_back(i);
      }

    } else {

      /// If side == 1, then connect the next string in descending order.
      for (unsigned i = 0; i < nextString.size(); ++i) {
        newPositions.push_back(nextString[nextString.size() - i - 1]);
      }
    }
  }

  /// Now get the path's vertices.
  std::vector<int> newPath(n);
  for (unsigned j = 0; j < n; ++j) {
    newPath[j] = tour[newPositions[j]];
  }

  // Fix the perturbed tour
  newPath = tsp2evrp_zga_relaxed(newPath);
  mergeAFSs(newPath);

  tour = newPath;
}

/*
 * Sequentially traverses tour and removes neighboring indentical nodes.
 */
void SA::mergeAFSs(vector<int> &tour) {
  int i = 0;
  while (i != tour.size() - 1) {
    if (tour[i] == tour[i + 1]) { // remove element at index i+1
      tour.erase(tour.begin() + i + 1);
    } else {
      i++;
    }
  }
}