#include "Problem.hpp"
#include "Macro.hpp"
#include <cstring>
#include <fstream>
#include <set>
#include <sstream>

using namespace std;
// Input parameter path
string instance; // the name for the instance
string instance_path; 
string parameter_path;
string trial_output_path;
string iteration_output_path;
string flag; // the flag for the algorithms: SA , ACO   VNS

// 在循环中用于控制变量的变量
int iteration;
int termination;
int STOP_CNT;
int max_trial_base;

// 一些实在的参数
int DEPOT;// the begin depo
int max_trials;
int max_iteration; 
int actual_problem_size; //all the points in the files, customers, stations depo
int problem_size; // the number of customers + depo
int num_of_customers;// the number of customers, not include the depo
int vehicle_capacity;
int battery_capacity;
double energy_consumption_rate;
int num_of_stations;// charing stations, not include the depo
double optimum; //the optimum value for the instance file



struct node *node_list;
int*cust_demand; // the customer demand for the problem
bool* charging_station; // if the current node is a charging station 
int* customers;
int* AFSs;
map<int,int> afsIdMap;// customer  index in arrary, not the id
double** distances;


struct AcoParameter acoparameter;// the aco parameter
int evals;
double current_best;
vector<double> iteration_record_results;
solution best_sol;


void isFileExist(string filename){
  fstream fs;
  fs.open(filename,ios::in);
  if(!fs){
    string command = "mkdir -p " + filename;
    system(command.c_str());
  }
}
void get_instance_name(){
    vector<string> ans;
    istringstream stream(instance_path);
    const char & seperator = '/';
    string word;
    while (getline(stream, word, seperator)) {
        ans.push_back(word);
    }
    instance = ans.back();
}

//   0     1  2  3   4        5   6          7   8                9    10
// ./main -m ACO -i E-223232 -p ./parameter -o ./result
// ./main -m ACO -i E-223232 -p ./parameter -o ./20termjination -oi ./2000files   
void read_arguments(int argc, char*argv[]){


  int i; 
  for(int i = 1; i <argc; i++){
    if(string(argv[i]) == "-m"){
      flag = argv[i + 1];
    }
    else if(string(argv[i]) == "-i"){
      instance_path = argv[i + 1];
    }
    else if(string(argv[i]) == "-p"){
      parameter_path = argv[i + 1];
    }
    else if(string(argv[i]) == "-o"){
      trial_output_path = argv[i + 1];
    }
    else if(string(argv[i]) == "-oi"){
      iteration_output_path = argv[i + 1];
    }
    else{
      continue;
    }
  }
  isFileExist("Result");
  isFileExist("Parameter");
  isFileExist("./Result/Iteration");
  isFileExist("./Result/Trials");
  get_instance_name();
}

void read_parameters() {
  int i;
  char line[CHAR_LEN];
  char *keywords;
  char Delimiters[] = " :=\n\t\r\f\v";
  if (flag == "ACO") {
    ifstream fin(instance_path);
    string namet;
    while ((fin.getline(line, CHAR_LEN - 1))) {
      if (!(keywords = strtok(line, Delimiters))) {
        continue;
      }
      if (!strcmp(keywords, instance.c_str())) {
        fin >> namet;
        fin >> max_trials;
        fin >> namet;
        fin >> max_trial_base;
        fin >> namet;
        fin >> acoparameter.alpha;
        fin >> namet;
        fin >> acoparameter.beta;
        fin >> namet;
        fin >> acoparameter.rho;
        fin >> namet;
        fin >> acoparameter.ras_ranks;
      }
    }
  }
  termination = max_trial_base*actual_problem_size;
  STOP_CNT = max_trial_base*actual_problem_size - 5;
}
void read_instances() {
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

      else if (strcmp(tempChar, "EUC_2D")) {
        cout << "not EUC_2D" << endl;
        exit(0);
      }
    } else if (!strcmp(keywords, "CAPACITY")) {
      if (!sscanf(strtok(NULL, Delimiters), "%d", &vehicle_capacity)) {
        cout << "CAPACITY error" << endl;
        exit(0);
      }
    } else if (!strcmp(keywords, "ENERGY_CAPACITY")) {
      if (!sscanf(strtok(NULL, Delimiters), "%d", &battery_capacity)) {
        cout << "ENERGY_CAPACITY error" << endl;
        exit(0);
      }
    } else if (!strcmp(keywords, "ENERGY_CONSUMPTION")) {
      if (!sscanf(strtok(NULL, Delimiters), "%lf", &energy_consumption_rate)) {
        cout << "ENERGY_CONSUMPTION error" << endl;
        exit(0);
      }
    } else if (!strcmp(keywords, "STATIONS")) {
      if (!sscanf(strtok(NULL, Delimiters), "%d", &num_of_stations)) {
        cout << "STATIONS error" << endl;
        exit(0);
      }
    } else if (!strcmp(keywords, "OPTIMAL_VALUE")) {
      if (!sscanf(strtok(NULL, Delimiters), "%lf", &optimum)) {
        cout << "OPTIMAL_VALUE error" << endl;
        exit(0);
      }
    } else if (!strcmp(keywords, "NODE_COORD_SECTION")) {
      if (problem_size != 0) {
        /*prroblem_size is the number of customers plus the depot*/
        num_of_customers = problem_size - 1;
        actual_problem_size = problem_size + num_of_stations;

        // termination = 25000*actual_problem_size;
        // STOP_CNT = 25000*actual_problem_size - 5;

        node_list = new node[actual_problem_size];

        for (i = 0; i < actual_problem_size; i++) {
          // store initial objects
          fin >> node_list[i].id;
          fin >> node_list[i].x >> node_list[i].y;
          node_list[i].id -= 1;
        }
        // compute the distances using initial objects
        distances = generate_2D_matrix_double(actual_problem_size, actual_problem_size);

      } else {
        cout << "wrong problem instance file" << endl;
        exit(1);
      }
    } else if (!strcmp(keywords, "DEMAND_SECTION")) {
      if (problem_size != 0) {

        int temp;
        // masked_demand = new int[problem_size];
       cust_demand = new int[actual_problem_size];
       charging_station = new bool[actual_problem_size];

        for (i = 0; i < problem_size; i++) {
          fin >> temp;
          fin >> cust_demand[temp - 1];
        }

        // initialize the charging stations.
        // the depot is set to a charging station in the DEPOT_SECTION
        for (i = 0; i < actual_problem_size; i++) {
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
  if (actual_problem_size == 0) {
    cout << "wrong problem instance file" << endl;
    exit(1);
  } else {
    compute_distances();
  }
}
void compute_distances(void) {

    int i, j;
    for (i = 0; i < actual_problem_size; i++) {
        for (j = 0; j < actual_problem_size; j++) {
            distances[i][j] = euclidean_distance(i, j);
        }
    }
}
double euclidean_distance(int i, int j) {
    double xd, yd;
    double r = 0.0;
    xd = node_list[i].x - node_list[j].x;
    yd = node_list[i].y - node_list[j].y;
    r = sqrt(xd * xd + yd * yd);
    return r;
}

// void read_parameters() {
//   int i;
//   char line[CHAR_LEN];
//   char *keywords;
//   char Delimiters[] = " :=\n\t\r\f\v";
//   if (flag == "VNS") {
//     ifstream fin(whole_parameter_path);
//     int maxtrials;
//     int ssh;
//     double restart_ratiot;
//     string namet;
//     while ((fin.getline(line, CHAR_LEN - 1))) {
//       if (!(keywords = strtok(line, Delimiters))) {
//         continue;
//       }
//       if (!strcmp(keywords, instance)) {
//         fin >> namet;
//         fin >> maxtrials;
//         fin >> namet;
//         fin >> ssh;
//         fin >> namet;
//         fin >> restart_ratiot;
//         max_trial_base = ssh;
//         max_trials = maxtrials;
//         vnsparameter.restart_ratio = restart_ratiot;
//         cout << max_trial_base<<endl;
//         cout << max_trials<<endl;
//         cout << vnsparameter.restart_ratio<<endl;

//       }
//     }
//   }
//   else if (flag == "SA") {
//         ifstream fin(whole_parameter_path);
//     int maxtrials;
//     string namet;
//         while ((fin.getline(line, CHAR_LEN - 1))) {
//       if (!(keywords = strtok(line, Delimiters))) {
//         continue;
//       }
//       if (!strcmp(keywords, instance)) {
//         fin >> namet;
//         fin >> maxtrials;
//         fin >> namet;
//         fin >> max_trial_base;
//         fin >> namet;
//         fin >> saparameter.T_start;
//         fin >> namet;
//         fin >>saparameter.T_end;
//         fin>>namet;
//         fin>>saparameter.delta;
//         max_trials = maxtrials;
//       }
//     }
//   }


// }






// void initialise_whole_problem_world(){
//     best_sol.tour = new int[4*num_of_customers];
//     best_sol.id = 1;
//     best_sol.steps = 0;
//     best_sol.tour_length = INT_MAX;


// }
// void compute_nearest_points() {
//     for(int i = 1; i <= num_of_customers; i++) {
//         memset(nearest_neighourhood[i],0,num_of_customers);
//         for(int j = 0; j < num_of_customers; j++) {
//             nearest_neighourhood[i][j] = j + 1;
//         }

//         sort(nearest_neighourhood, nearest_neighourhood + num_of_customers, [&, i](int j, int k) {
//             return get_distance(i, j) < get_distance(i, k);
//         });
//     }
// }

// void init_evals() {evals = 0;}
// void init_current_best(){current_best = INT_MAX;}
// double get_current_best() {return current_best;}
// double get_evals() {return evals;}





// double get_distance(int from, int to) {
//   evals += (1.0 / actual_problem_size);
//   return distances[from][to];
// }

// double fitness_evaluation(vector<int> &tour) {
//   double tour_length = 0;
//   for (int i = 0; i < tour.size() - 1; i++) {
//     tour_length += distances[tour[i]][tour[i + 1]];
//   }
//   if (tour_length < current_best)
//     current_best = tour_length;
//   evals++;
//   return tour_length;
// }
// double get_energy_consumption(int from, int to) {
//   /*DO NOT USE THIS FUNCTION MAKE ANY CALCULATIONS TO THE ROUTE COST*/
//   return energy_consumption_rate * distances[from][to];
// }

// /****************************************************************/
// /* Get energy to unit                                           */
// /****************************************************************/
// double get_energy_per_unit() { return energy_consumption_rate;}

// /****************************************************************/
// /* Returns the demand for a specific customer                   */
// /* points: from and to.                                         */
// /****************************************************************/
// int get_customer_demand(int customer) {
//   if (customer == -1) {
//     cout << "Warning customer invalid - 1!\n";
//   }

//   return cust_demand[customer];
// }

// /****************************************************************/
// /* Returns true when a specific node is a charging station;     */
// /* and false otherwise                                          */
// /****************************************************************/
// bool is_charging_station(int node) {
//   if (node == -1) {
//     cout << "Warning node invalid - 1!\n";
//   }

//   bool flag = false;
//   if (charging_station[node] == true)
//     flag = true;
//   else
//     flag = false;
//   return flag;
// }

// void print_solution(vector<int> routes, int size) {
//     int   i;
//     for( i = 0 ; i < size; i++ ) {
//          cout << routes[i] <<  " - ";
//     }
// }

// /****************************************************************/
// /*Generate and return a two-dimension array of type double      */
// /****************************************************************/
double** generate_2D_matrix_double(int n, int m) {
  double **matrix;

  matrix = new double*[n];
  for ( int i = 0 ; i < n ; i++ ) {
    matrix[i] = new double[m];
  }
  //initialize the 2-d array
  for(int i = 0; i < n; i++){
    for(int j = 0; j < m; j++) {
      matrix[i][j] = 0.0;
    }
  }
  return matrix;
}

// /*
//  * Returns remaining loading capacity after performing the given evrp tour.
//  */
// int getRemainingLoad(vector<int> evrpTour) {
//   int load = 0;
//   for (auto node : evrpTour) {
//     if (node == 0) {
//       load = vehicle_capacity;
//     } else {
//       load -= get_customer_demand(node);
//     }
//   }
//   return load;
// }
// /*
//  * Returns remaining fuel capacity after performing the given evrp tour.
//  */
// double getRemainingBattery(vector<int> evrpTour) {
//   double battery = 0;
//   for (int i = 0; i < evrpTour.size(); i++) {
//     int cur = evrpTour[i];
//     if (i > 0) {
//       int prev = evrpTour[i - 1];
//       battery -= get_energy_consumption(prev, cur);
//     }
//     if (is_charging_station(cur)) {
//       battery = battery_capacity;
//     }
//   }
//   return battery;
// }
// /*
//  * Returns the closest AFS to the given node.
//  */
// int getClosestAFS(int node) {
//   auto minDist = DBL_MAX;
//   int closest;
//   for (int i = 0; i < actual_problem_size; i++) {
//     if (is_charging_station(i) and i != node) {
//       double dist = get_distance(node, i);
//       if (dist < minDist) {
//         minDist = dist;
//         closest = i;
//       }
//     }
//   }
//   return closest;
// }
// /*
//  * Returns the closest AFS to the given goal, that is reachable from the current
//  * position.
//  */
// int getReachableAFSClosestToGoal(int cur, int goal, double battery) {
//   auto minDist = DBL_MAX;
//   int closest = -1;
//   for (int i = 0; i < actual_problem_size; i++) {
//     if (is_charging_station(i) && i != cur &&
//         battery >= get_energy_consumption(cur, i)) {
//       double dist = get_distance(i, goal);
//       // cout << "Station: " << i << ", distance: " << dist << endl;
//       if (dist < minDist) {
//         minDist = dist;
//         closest = i;
//       }
//     }
//   }
//   return closest;
// }

// bool isValidTour(vector<int> tour) {
//   int load = 0;
//   double battery = 0;
//   int custCnt = 0;
//   for (int i = 0; i < tour.size(); i++) {
//     int cur = tour[i];
//     // Load check
//     if (cur == 0) {
//       load = vehicle_capacity;
//     } else {
//       load -= get_customer_demand(cur);
//       if (load < 0)
//         return false;
//     }
//     // Battery check
//     if (i > 0) {
//       int prev = tour[i - 1];
//       battery -= get_energy_consumption(prev, cur);
//       if (battery < 0)
//         return false;
//     }
//     if (is_charging_station(cur)) {
//       battery = battery_capacity;
//     } else {
//       custCnt++;
//     }
//   }
//   // Closed tour check
//   if ((tour[0] != 0) || (tour[tour.size() - 1] != 0)) {
//     return false;
//   }
//   if (custCnt < num_of_customers) {
//     return false;
//   }
//   return true;
// }




// void mergeAFSs(vector<int> &tour) {
//     int i = 0;
//     while (i != tour.size() - 1) {
//         if (tour[i] == tour[i+1]) { // remove element at index i+1
//             tour.erase(tour.begin() + i + 1);
//         } else {
//             i++;
//         }
//     }
// }
// double fitness_evaluation(int *routes, int size) {
//     int i;
//     double tour_length = 0.0;

//     //the format of the solution that this method evaluates is the following
//     //Node id:  0 - 5 - 6 - 8 - 0 - 1 - 2 - 3 - 4 - 0 - 7 - 0
//     //Route id: 1 - 1 - 1 - 1 - 2 - 2 - 2 - 2 - 2 - 3 - 3 - 3
//     //this solution consists of three routes:
//     //Route 1: 0 - 5 - 6 - 8 - 0
//     //Route 2: 0 - 1 - 2 - 3 - 4 - 0
//     //Route 3: 0 - 7 - 0
//     for (i = 0; i < size - 1; i++)
//         tour_length += distances[routes[i]][routes[i + 1]];

//     if (tour_length < current_best)
//         current_best = tour_length;

//     //adds complete evaluation to the overall fitness evaluation count
//     evals++;

//     return tour_length;
// }


// // bool full_validity_check(vector<int> &tour) {
// //     cout << "VALIDITY CHECK\n";
// //     auto test1 = isValidTour(tour);
// //     cout << "My test: " << test1 << endl;

// //     best_sol.tour = new int[4*num_of_customers];
// //     best_sol.id = 1;
// //     best_sol.steps = 0;
// //     best_sol.tour_length = INT_MAX;

// //     for (int i = 0; i < tour.size(); i++) {
// //         best_sol.tour[i] = tour[i];
// //     }
// //     best_sol.steps = tour.size();
// //     auto test2 = check_solution(best_sol.tour, best_sol.steps);
// //     cout << "Provided test: " << test2 << endl;

// //     cout << "FITNESS\n";
// //     cout << "My fitness: " << fitness_evaluation(tour) << endl;
// //     cout << "Provided fitness: " << fitness_evaluation(best_sol.tour, best_sol.steps) << endl;

// //     set<int> customerst;
// //     for (auto c:tour) {
// //         if (!is_charging_station(c)) {
// //             customerst.insert(c);
// //         }
// //     }
// //     cout << "Customers served: " << customerst.size() << "/" << num_of_customers << endl;
// //     bool test3 = (customerst.size() == num_of_customers);

// //     return (test1 and test2 and test3);
// // }
// bool check_solution(int *t, int size) {
//     int i, from, to;
//     double energy_temp = battery_capacity;
//     double capacity_temp = vehicle_capacity;
//     double distance_temp = 0.0;

//     for (i = 0; i < size - 1; i++) {
//         from = t[i];
//         to = t[i + 1];
//         capacity_temp -= get_customer_demand(to);
//         energy_temp -= get_energy_consumption(from, to);
//         distance_temp += get_distance(from, to);
//         if (capacity_temp < 0.0) {
//             cout << "error: capacity below 0 at customer " << to << endl;
//             // print_solution(t, size);
// //            exit(1);
//             return false;
//         }
//         if (energy_temp < 0.0) {
//             cout << "error: energy below 0 from " << from << " to " << to << endl;
//             cout << energy_temp << endl;
//             // print_solution(t, size);
// //            exit(1);
//             return false;
//         }
//         if (to == DEPOT) {
//             capacity_temp = vehicle_capacity;
//         }
//         if (is_charging_station(to) == true || to == DEPOT) {
//             energy_temp = battery_capacity;
//         }
//     }
//     if (distance_temp != fitness_evaluation(t, size)) {
//         cout << "error: check fitness evaluation" << endl;
//         return false;
//     }

//     return true;
// }

void free_heuristic(){

  delete[] best_sol.tour;

}
void free_problem(){
  int i;
  delete[] node_list;
  delete[] cust_demand;
  delete[] charging_station;
  delete[] customers;
  delete[] AFSs;
  delete[] distances;
  for(i = 0; i < actual_problem_size; i++) {
    delete[] distances[i];
  }

  
}

