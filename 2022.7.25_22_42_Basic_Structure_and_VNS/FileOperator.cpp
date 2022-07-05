#include "FileOperator.hpp"
#include "Problem.hpp"
#include <cstdio>

FILE *log_performance_for_final;// 20
FILE *log_performance_for_iteration;//1-0000000

char *perf_filename;
char *perf_filename1;
char *perf_filename2;
char *perf_filename3;

double* perf_of_trials;
double* perf_of_trials1;
double* perf_of_trials2;
double* perf_of_trials3;

void open_stats(void){
    //Initialize
    perf_of_trials = new double[max_trials];

    for(int i =0; i < max_trials; i++){
        perf_of_trials[i] = 0.0;
    }


  //initialize and open output files
  perf_filename = new char[CHAR_LEN];
  sprintf(perf_filename, "./Result/Trails/%s.txt",instance);

  //for performance
  if ((log_performance_for_final = fopen(perf_filename,"a")) == NULL) {
      std::cout << "Failed to open stats file " << perf_filename << std::endl;
      exit(2);
  }
  //initialize and open output files

}

void close_stats(void){
  int i,j;
  double perf_mean_value, perf_stdev_value;
  fprintf(log_performance_for_final,"Algorithm: %s",flag.c_str());
  fprintf(log_performance_for_final,"\n");
  //For statistics
  for(i = 0; i < max_trials; i++){
    fprintf(log_performance_for_final, "%.6f", perf_of_trials[i]);
    fprintf(log_performance_for_final,"\n");

  }

  perf_mean_value = mean(perf_of_trials,max_trials);
  perf_stdev_value = stdev(perf_of_trials,max_trials,perf_mean_value);
  fprintf(log_performance_for_final,"Mean %f\t ",perf_mean_value);
  fprintf(log_performance_for_final,"\tStd Dev %f\t ",perf_stdev_value);
  fprintf(log_performance_for_final,"\n");
  fprintf(log_performance_for_final, "Min: %f\t ", best_of_vector(perf_of_trials,max_trials));
  fprintf(log_performance_for_final,"\n");
  fprintf(log_performance_for_final, "Max: %f\t ", worst_of_vector(perf_of_trials,max_trials));
  fprintf(log_performance_for_final,"\n");


  fclose(log_performance_for_final);
 

}


void free_stats(){

  //free memory
  delete[] perf_of_trials;

}

void save_results() {
  char *perf_filenamet = new char[CHAR_LEN];
  sprintf(perf_filenamet, "./Result/Iteration/%s.%s.txt",flag.c_str(),instance);
  log_performance_for_iteration = fopen(perf_filenamet, "a");
  fprintf(log_performance_for_iteration, "Iteration\tResults\n");
  double a1 = (double)iteration_record_results.size()/25000;
  if(a1>1){
  int j=0;
  for (double i = 0; i < iteration_record_results.size(); i=i+a1) {
    fprintf(log_performance_for_iteration, "%d \t%lf", j, iteration_record_results[floor(i)]);
    fprintf(log_performance_for_iteration, "\n");
    j++;
  }
  }
  else{
  for (int i = 0; i < iteration_record_results.size(); i++) {
    fprintf(log_performance_for_iteration, "%d \t%lf", i, iteration_record_results[i]);
    fprintf(log_performance_for_iteration, "\n");
    
  }
  }
  fclose(log_performance_for_iteration);
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

