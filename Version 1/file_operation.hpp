#ifndef FILE_OPERATION_HPP
#define FILE_OPERATION_HPP

#include "variables.hpp"

void free_stats(){

  //free memory
  delete[] perf_of_trials;

}
void get_mean(int r, double value) {

  perf_of_trials[r] = value;

}


double mean(double* values, int size){
  int i;
  double m = 0.0;
  for (i = 0; i < size; i++){
      m += values[i];
  }
  m = m / (double)size;
  return m; //mean
}

double stdev(double* values, int size, double average){
  int i;
  double dev = 0.0;

  if (size <= 1)
    return 0.0;
  for (i = 0; i < size; i++){
    dev += ((double)values[i] - average) * ((double)values[i] - average);
  }
  return sqrt(dev / (double)(size - 1)); //standard deviation
}

double best_of_vector(double *values, int l ) {
  double min;
  int k;
  k = 0;
  min = values[k];
  for( k = 1 ; k < l ; k++ ) {
    if( values[k] < min ) {
      min = values[k];
    }
  }
  return min;
}


double worst_of_vector(double *values, int l ) {
  double max;
  int k;
  k = 0;
  max = values[k];
  for( k = 1 ; k < l ; k++ ) {
    if( values[k] > max ){
      max = values[k];
    }
  }
  return max;
}

/****************************************************************/
/* Read the problem instance and generate the initial object    */
/* vector.                                                      */
/****************************************************************/
void read_problem(string filename) {
  int i;
  char line[CHAR_LEN];
  char *keywords;
  char Delimiters[] = " :=\n\t\r\f\v";
  ifstream fin(filename);
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

        node_list = new node[ACTUAL_PROBLEM_SIZE];

        for (i = 0; i < ACTUAL_PROBLEM_SIZE; i++) {
          // store initial objects
          fin >> node_list[i].id;
          fin >> node_list[i].x >> node_list[i].y;
          node_list[i].id -= 1;
        }
        // compute the distances using initial objects
        distances =
            generate_2D_matrix_double(ACTUAL_PROBLEM_SIZE, ACTUAL_PROBLEM_SIZE);
      } else {
        cout << "wrong problem instance file" << endl;
        exit(1);
      }
    } else if (!strcmp(keywords, "DEMAND_SECTION")) {
      if (problem_size != 0) {

        int temp;
        // masked_demand = new int[problem_size];
        cust_demand = new int[ACTUAL_PROBLEM_SIZE];
        charging_station = new bool[ACTUAL_PROBLEM_SIZE];
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
}


void initialise_files(void) {
  perf_of_trials = new double[MAX_TRIALS];

  for (int i = 0; i < MAX_TRIALS; i++) { // initialise the perf_of_trials
    perf_of_trials[i] = 0.0;
  }

  // initialize and open output files
  perf_filename = new char[CHAR_LEN];
  sprintf(perf_filename, "./output/stats.%s.txt", problem_instance);

  // for performance
  if ((log_performance = fopen(perf_filename, "a")) == NULL) {
    std::cout << "Failed to open stats file " << perf_filename << std::endl;
    exit(2);
  }
  // initialize and open output files
}

void close_stats(void){
  int i,j;
  double perf_mean_value, perf_stdev_value;
 
  //For statistics
  for(i = 0; i < MAX_TRIALS; i++){
    //cout << i << " " << perf_of_trials[i] << endl;
    //cout << i << " " << time_of_trials[i] << endl;
    fprintf(log_performance, "%.2f", perf_of_trials[i]);
    fprintf(log_performance,"\n");

  }

  perf_mean_value = mean(perf_of_trials,MAX_TRIALS);
  perf_stdev_value = stdev(perf_of_trials,MAX_TRIALS,perf_mean_value);
  fprintf(log_performance,"Mean %f\t ",perf_mean_value);
  fprintf(log_performance,"\tStd Dev %f\t ",perf_stdev_value);
  fprintf(log_performance,"\n");
  fprintf(log_performance, "Min: %f\t ", best_of_vector(perf_of_trials,MAX_TRIALS));
  fprintf(log_performance,"\n");
  fprintf(log_performance, "Max: %f\t ", worst_of_vector(perf_of_trials,MAX_TRIALS));
  fprintf(log_performance,"\n");


  fclose(log_performance);
 

}







#endif // EVRP_EVRP_HPP