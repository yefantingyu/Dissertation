#include "Problem.hpp"
#include "Macro.hpp"

Problem *problem = Problem::getInstance();

Problem *Problem::instancer = NULL;


Problem *Problem::getInstance() {
  if (instancer == NULL) {
    instancer = new Problem();
  }
  return instancer;
}
Problem::~Problem() {
  // delete[] best_sol->tour;
  
}
void Problem::initothers() {
    // init AFSs and CUSTOMERS vectors
    AFs.clear();
    Customers.clear();
    int afsId = 0;
    for (int i = 0; i < ACTUAL_PROBLEM_SIZE; i++) {
        if (is_charging_station(i)) {
            AFs.push_back(i);
            afsIdMap.insert(pair<int, int>(i, afsId++));
        } else {
            Customers.push_back(i);
        }
    }


    // Initialize instance of fw planner
    fw = floydWarshall(AFs.size());
    fw.planPaths();

}
void Problem::initialist_solution(){
    best_sol = new solution;
    best_sol->tour = new int[4*NUM_OF_CUSTOMERS];
    best_sol->id = 1;
    best_sol->steps = 0;
    best_sol->tour_length = INT_MAX;
}

double Problem::get_current_best() { return current_best; }
double Problem::get_evals() { return evals; }
void Problem::read(string flagt, char *filename) {
  instance = filename;
  flag = flagt;
  instance_path = string("./DataSet/") + instance;
  read_parameters();
  read_instance();
}

void Problem::read_parameters() {
  int i;
  char line[CHAR_LEN];
  char *keywords;
  char Delimiters[] = " :=\n\t\r\f\v";

  if (flag == "VNS") {
    ifstream fin("./Parameter/VNS.txt");
    int maxtrials;
    double restart_ratiot;
    while ((fin.getline(line, CHAR_LEN - 1))) {
      if (!(keywords = strtok(line, Delimiters))) {
        continue;
      }
      if (!strcmp(keywords, "Max_trails")) {
        if (!sscanf(strtok(NULL, Delimiters), "%d", &maxtrials)) {
          cout << "Max_trails error" << endl;
          exit(0);
        }
        vns_parameter.Max_trails = maxtrials;
      } else if (!strcmp(keywords, "restart_ratio")) {
        if (!sscanf(strtok(NULL, Delimiters), "%lf", &restart_ratiot)) {
          cout << "restart_ratio error" << endl;
          exit(0);
        }
        vns_parameter.restart_ratio = restart_ratiot;
      }
    }
  }
  else if (flag == "SA") {
    ifstream fin("./Parameter/SA.txt");
    int maxtrials;
    double T_start;
    double T_end;
    double delta;

    while ((fin.getline(line, CHAR_LEN - 1))) {
      if (!(keywords = strtok(line, Delimiters))) {
        continue;
      }
      if (!strcmp(keywords, "Max_trails")) {
        if (!sscanf(strtok(NULL, Delimiters), "%d", &maxtrials)) {
          cout << "Max_trails error" << endl;
          exit(0);
        }
        sa_parameter.Max_trails = maxtrials;
      } else if (!strcmp(keywords, "T_start")) {
        if (!sscanf(strtok(NULL, Delimiters), "%lf", &T_start)) {
          cout << "restart_ratio error" << endl;
          exit(0);
        }
        sa_parameter.T_start =T_start;
      }
      else if (!strcmp(keywords, "T_end")) {
        if (!sscanf(strtok(NULL, Delimiters), "%lf", &T_end)) {
          cout << "restart_ratio error" << endl;
          exit(0);
        }
        sa_parameter.T_end =T_end;
      }else if (!strcmp(keywords, "delta")) {
        if (!sscanf(strtok(NULL, Delimiters), "%lf", &delta)) {
          cout << "restart_ratio error" << endl;
          exit(0);
        }
        sa_parameter.delta =delta;
      }
      else{
        return;
      }
    }
  }
}

void Problem::read_instance() {
  int i;
  char line[CHAR_LEN];
  char *keywords;
  char Delimiters[] = " :=\n\t\r\f\v";
  ifstream fin(instance_path);
  while ((fin.getline(line, CHAR_LEN - 1))) {

    if (!(keywords = strtok(line, Delimiters)))
      continue;
    if (!strcmp(keywords, "DIMENSION")) {
      if (!sscanf(strtok(NULL, Delimiters), "%d", &problem_size)) {
        cout << "DIMENSION error" << endl;
        exit(0);
      }
    } else if (!strcmp(keywords, "EDGE_WEIGHT_TYPE")) {
      char *tempChar;
      if (!(tempChar = strtok(NULL, Delimiters))) {
        cout << "EDGE_WEIGHT_TYPE error" << endl;
        exit(0);
      }
      if (strcmp(tempChar, "EUC_2D")) {
        cout << "not EUC_2D" << endl;
        exit(0);
      }
    } else if (!strcmp(keywords, "CAPACITY")) {
      if (!sscanf(strtok(NULL, Delimiters), "%d", &MAX_CAPACITY)) {
        cout << "CAPACITY error" << endl;
        exit(0);
      }
    } else if (!strcmp(keywords, "VEHICLES")) {
      if (!sscanf(strtok(NULL, Delimiters), "%d", &MIN_VEHICLES)) {
        cout << "VEHICLES error" << endl;
        exit(0);
      }
    } else if (!strcmp(keywords, "ENERGY_CAPACITY")) {
      if (!sscanf(strtok(NULL, Delimiters), "%d", &BATTERY_CAPACITY)) {
        cout << "ENERGY_CAPACITY error" << endl;
        exit(0);
      }
    } else if (!strcmp(keywords, "ENERGY_CONSUMPTION")) {
      if (!sscanf(strtok(NULL, Delimiters), "%lf", &energy_consumption)) {
        cout << "ENERGY_CONSUMPTION error" << endl;
        exit(0);
      }
    } else if (!strcmp(keywords, "STATIONS")) {
      if (!sscanf(strtok(NULL, Delimiters), "%d", &NUM_OF_STATIONS)) {
        cout << "STATIONS error" << endl;
        exit(0);
      }
    } else if (!strcmp(keywords, "VEHICLES")) {
      if (!sscanf(strtok(NULL, Delimiters), "%d", &NUM_OF_VEHICLES)) {
        cout << "NUM_OF_VEHICLES" << endl;
        exit(0);
      }
    } else if (!strcmp(keywords, "OPTIMAL_VALUE")) {
      if (!sscanf(strtok(NULL, Delimiters), "%lf", &OPTIMUM)) {
        cout << "OPTIMAL_VALUE error" << endl;
        exit(0);
      }
    } else if (!strcmp(keywords, "NODE_COORD_SECTION")) {
      if (problem_size != 0) {
        /*prroblem_size is the number of customers plus the depot*/
        NUM_OF_CUSTOMERS = problem_size - 1;
        ACTUAL_PROBLEM_SIZE = problem_size + NUM_OF_STATIONS;

        node_list.resize(ACTUAL_PROBLEM_SIZE);

        for (i = 0; i < ACTUAL_PROBLEM_SIZE; i++) {
          fin >> node_list[i].id;
          fin >> node_list[i].x >> node_list[i].y;
          node_list[i].id -= 1;
        }
        // compute the distances using initial objects
        generate_2D_matrix_double(distances, ACTUAL_PROBLEM_SIZE,
                                  ACTUAL_PROBLEM_SIZE);
      } else {
        cout << "wrong problem instance file" << endl;
        exit(1);
      }
    } else if (!strcmp(keywords, "DEMAND_SECTION")) {
      if (problem_size != 0) {

        int temp;
        // masked_demand = new int[problem_size];
        cust_demand.resize(ACTUAL_PROBLEM_SIZE);
        charging_station.resize(ACTUAL_PROBLEM_SIZE);
        for (i = 0; i < problem_size; i++) {
          fin >> temp;
          fin >> cust_demand[temp - 1];
        }

        // initialize the charging stations.
        // the depot is set to a charging station in the DEPOT_SECTION
        for (i = 0; i < ACTUAL_PROBLEM_SIZE; i++) {
          if (i < problem_size) {
            charging_station[i] = false;
          } else {
            charging_station[i] = true;
            cust_demand[i] = 0;
          }
        }
      }
    } else if (!strcmp(keywords, "DEPOT_SECTION")) {
      fin >> DEPOT;
      DEPOT -= 1;
      charging_station[DEPOT] = true;
    }
  }
  fin.close();

  if (ACTUAL_PROBLEM_SIZE == 0) {
    cout << "wrong problem instance file" << endl;
    exit(1);
  } else {
    compute_distances();
  }
  TERMINATION = 25000 * ACTUAL_PROBLEM_SIZE;
  STOP_CNT = 25000 * ACTUAL_PROBLEM_SIZE - 5;
}
/****************************************************************/
/* Returns the distance between two points: from and to. Can be */
/* used to evaluate a part of the solution. The fitness         */
/* evaluation count will be proportional to the problem size    */
/****************************************************************/
double Problem::get_distance(int from, int to) {
  // adds partial evaluation to the overall fitness evaluation count
  // It can be used when local search is used and a whole evaluation is not
  // necessary
  evals += (1.0 / ACTUAL_PROBLEM_SIZE);

  return distances[from][to];
}

/****************************************************************/
/* Overloaded fitness evaluation for vector representation      */
/****************************************************************/
/****************************************************************/
/* Overloaded fitness evaluation for vector representation      */
/****************************************************************/
double Problem::fitness_evaluation(vector<int> &tour) {
  double tour_length = 0;
  for (int i = 0; i < tour.size() - 1; i++) {
    tour_length += distances[tour[i]][tour[i + 1]];
  }

  if (tour_length < current_best)
    current_best = tour_length;

  evals++;

  return tour_length;
}

// bool Problem::full_validity_check(vector<int> &tour) {
//     cout << "VALIDITY CHECK\n";
//     auto test1 = isValidTour(tour);
//     cout << "My test: " << test1 << endl;

//     initialize_heuristic();
//     for (int i = 0; i < tour.size(); i++) {
//         best_sol->tour[i] = tour[i];
//     }
//     best_sol->steps = tour.size();
//     auto test2 = check_solution(best_sol->tour, best_sol->steps);
//     cout << "Provided test: " << test2 << endl;

//     cout << "FITNESS\n";
//     cout << "My fitness: " << fitness_evaluation(tour) << endl;
//     cout << "Provided fitness: " << fitness_evaluation(best_sol->tour,
//     best_sol->steps) << endl;

//     std::set<int> customers;
//     for (auto c:tour) {
//         if (!is_charging_station(c)) {
//             customers.insert(c);
//         }
//     }
//     cout << "Customers served: " << customers.size() << "/" <<
//     NUM_OF_CUSTOMERS << endl; bool test3 = (customers.size() ==
//     NUM_OF_CUSTOMERS);

//     return (test1 and test2 and test3);
// }

/****************************************************************/
/* Returns the energy consumed when travelling between two      */
/* points: from and to.                                         */
/****************************************************************/
double Problem::get_energy_consumption(int from, int to) {

  /*DO NOT USE THIS FUNCTION MAKE ANY CALCULATIONS TO THE ROUTE COST*/
  return energy_consumption * distances[from][to];
}

/****************************************************************/
/* Get energy to unit                                           */
/****************************************************************/
double Problem::get_energy_per_unit() { return energy_consumption; }

/****************************************************************/
/* Returns the demand for a specific customer                   */
/* points: from and to.                                         */
/****************************************************************/
int Problem::get_customer_demand(int customer) {
  if (customer == -1) {
    cout << "Warning customer invalid - 1!\n";
  }

  return cust_demand[customer];
}

/****************************************************************/
/* Returns true when a specific node is a charging station;     */
/* and false otherwise                                          */
/****************************************************************/
bool Problem::is_charging_station(int node) {
  if (node == -1) {
    cout << "Warning node invalid - 1!\n";
  }

  bool flag = false;
  if (charging_station[node] == true)
    flag = true;
  else
    flag = false;
  return flag;
}

void print_solution(vector<int> routes, int size) {
  int i;
  for (i = 0; i < size - 1; i++) {
    cout << routes[i] << "-";
  }
  cout << routes[i] << endl;
}

/****************************************************************/
/*Compute and return the euclidean distance of two objects      */
/****************************************************************/
double Problem::euclidean_distance(int i, int j) {
  double xd, yd;
  double r = 0.0;
  xd = node_list[i].x - node_list[j].x;
  yd = node_list[i].y - node_list[j].y;
  r = sqrt(xd * xd + yd * yd);
  return r;
}

/****************************************************************/
/*Compute the distance matrix of the problem instance           */
/****************************************************************/
void Problem::compute_distances(void) {
  int i, j;
  for (i = 0; i < ACTUAL_PROBLEM_SIZE; i++) {
    for (j = 0; j < ACTUAL_PROBLEM_SIZE; j++) {
      distances[i][j] = euclidean_distance(i, j);
    }
  }
}

/****************************************************************/
/*Generate and return a two-dimension array of type double      */
/****************************************************************/
void Problem::generate_2D_matrix_double(vector<vector<double>> &matrix, int n, int m) {

  matrix.resize(n);
  for (int i = 0; i < n; i++) {
    matrix[i].resize(m);
  }
  // initialize the 2-d array
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      matrix[i][j] = 0.0;
    }
  }
}
/****************************************************************/
/* Reset the evaluation counter for a new indepedent run        */
/****************************************************************/
void Problem::init_evals() { evals = 0; }
/*******************************************************************/
/* Reset the best solution quality so far for a new indepedent run */
/*******************************************************************/
void Problem::init_current_best() { current_best = INF; }

/*
 * Returns remaining loading capacity after performing the given evrp tour.
 */
int Problem::getRemainingLoad(vector<int> evrpTour) {
  int load = 0;
  for (auto node : evrpTour) {
    if (node == 0) {
      load = MAX_CAPACITY;
    } else {
      load -= get_customer_demand(node);
    }
  }
  return load;
}
/*
 * Returns remaining fuel capacity after performing the given evrp tour.
 */
double Problem::getRemainingBattery(vector<int> evrpTour) {
  double battery = 0;
  for (int i = 0; i < evrpTour.size(); i++) {
    int cur = evrpTour[i];
    if (i > 0) {
      int prev = evrpTour[i - 1];
      battery -= get_energy_consumption(prev, cur);
    }
    if (is_charging_station(cur)) {
      battery = BATTERY_CAPACITY;
    }
  }
  return battery;
}
/*
 * Returns the closest AFS to the given node.
 */
int Problem::getClosestAFS(int node) {
  auto minDist = DBL_MAX;
  int closest;
  for (int i = 0; i < ACTUAL_PROBLEM_SIZE; i++) {
    if (is_charging_station(i) and i != node) {
      double dist = get_distance(node, i);
      if (dist < minDist) {
        minDist = dist;
        closest = i;
      }
    }
  }
  return closest;
}
/*
 * Returns the closest AFS to the given goal, that is reachable from the current
 * position.
 */
int Problem::getReachableAFSClosestToGoal(int cur, int goal, double battery) {
  auto minDist = DBL_MAX;
  int closest = -1;
  for (int i = 0; i < ACTUAL_PROBLEM_SIZE; i++) {
    if (is_charging_station(i) && i != cur &&
        battery >= get_energy_consumption(cur, i)) {
      double dist = get_distance(i, goal);
      // cout << "Station: " << i << ", distance: " << dist << endl;
      if (dist < minDist) {
        minDist = dist;
        closest = i;
      }
    }
  }
  return closest;
}

bool Problem::isValidTour(vector<int> tour) {
  int load = 0;
  double battery = 0;
  int custCnt = 0;
  for (int i = 0; i < tour.size(); i++) {
    int cur = tour[i];
    // Load check
    if (cur == 0) {
      load = MAX_CAPACITY;
    } else {
      load -= get_customer_demand(cur);
      if (load < 0)
        return false;
    }
    // Battery check
    if (i > 0) {
      int prev = tour[i - 1];
      battery -= get_energy_consumption(prev, cur);
      if (battery < 0)
        return false;
    }
    if (is_charging_station(cur)) {
      battery = BATTERY_CAPACITY;
    } else {
      custCnt++;
    }
  }
  // Closed tour check
  if ((tour[0] != 0) || (tour[tour.size() - 1] != 0)) {
    return false;
  }
  if (custCnt < NUM_OF_CUSTOMERS) {
    return false;
  }
  return true;
}

/*
 * This constructor initializes the planning graph from global list AFSs.
 */
floydWarshall::floydWarshall(int nV) : nV(nV) {
    next = vector<vector<int>>(nV, vector<int>(nV, -1));
    dist = vector<vector<int>>(nV, vector<int>(nV, INF));
    for (int i = 0; i < nV; i++) {
        for (int j = i; j < nV; j++) {
            if (i == j) {
                dist[i][j] = 0;
                next[i][j] = j;
            } else {
                int start = problem->AFs[i];
                int goal =  problem->AFs[j];
                double consumption = problem->get_energy_consumption(start, goal);
                problem->get_distance(start, goal); // Called here just to increase evals, so that the usage is 100% legal
                if (consumption <= problem->BATTERY_CAPACITY) {
                    dist[i][j] = consumption;
                    dist[j][i] = consumption;
                    next[i][j] = j;
                    next[j][i] = i;
                }
            }
        }
    }
}

/*
 * This constructor can be used for initialization by any graph.
 */
floydWarshall::floydWarshall(vector<vector<int>> &graph) : dist(graph) {
    next = vector<vector<int>>(nV, vector<int>(nV, -1));
    nV = graph.size();
    for (int i = 0; i < nV; i++) {
        for (int j = 0; j < nV; j++) {
            next[i][j] = j;
        }
    }
}

/*
 * Plans all pairs of shortest paths, run just once.
 */
void floydWarshall::planPaths() {
    int i, j, k;
    for (k = 0; k < nV; k++) {
        for (i = 0; i < nV; i++) {
            for (j = 0; j < nV; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }
}

void floydWarshall::printMatrix(vector<vector<int>> &matrix) {
    for (auto i = 0; i < nV; i++) {
        for (auto j = 0; j < nV; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

/*
 * Returns shortest path from u to v.
 * If afsIds = true, returns AFS ids as stored in the global AFSs vector, otherwise returns internal ids used by the floydWarshall instance.
 */
vector<int> floydWarshall::getPath(int u, int v, bool afsIds) {
    vector<int> path;
    if (next[u][v] == -1) {
        return path;
    }
    path.push_back(u);
    while (u != v) {
        u = next[u][v];
        path.push_back(u);
    }
    if (afsIds) {
        for (auto &n:path) {
            n = problem->AFs[n];
        }
    }

    return path;
}
