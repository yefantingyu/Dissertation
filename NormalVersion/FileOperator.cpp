#include "FileOperator.hpp"
#include "Macro.hpp"

FileOperator::FileOperator(){}

void FileOperator::isFileExist(string filename)
{
	fstream fs;
	fs.open(filename, ios::in);
	if (!fs)
	{
		cout << "File not exist" << endl;

        string command = "mkdir -p " + filename;
        system(command.c_str());
    }
}

void FileOperator::open_file(char* instance){
    isFileExist("./Result");

    perf_of_trials = new double[MAX_TRIALS];

    for (int i = 0; i < MAX_TRIALS; i++) { // initialise the perf_of_trials
        perf_of_trials[i] = 0.0;
    }

    // initialize and open output files
    perf_filename = new char[CHAR_LEN];
    sprintf(perf_filename, "./Result/%s.Result.txt", instance);

    // for performance
    if ((log_performance = fopen(perf_filename, "a")) == NULL) {
        std::cout << "Failed to open stats file " << perf_filename << std::endl;
        exit(2);
    }
  // initialize and open output files

}


void FileOperator::get_mean(int r, double value) {

  perf_of_trials[r] = value;

}

double FileOperator::mean(double* values, int size){
  int i;
  double m = 0.0;
  for (i = 0; i < size; i++){
      m += values[i];
  }
  m = m / (double)size;
  return m; //mean
}

double FileOperator::stdev(double* values, int size, double average){
  int i;
  double dev = 0.0;

  if (size <= 1)
    return 0.0;
  for (i = 0; i < size; i++){
    dev += ((double)values[i] - average) * ((double)values[i] - average);
  }
  return sqrt(dev / (double)(size - 1)); //standard deviation
}

double FileOperator::best_of_vector(double *values, int l ) {
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


double FileOperator::worst_of_vector(double *values, int l ) {
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



void FileOperator::close_stats(void){
  int i;
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


FileOperator::~FileOperator(){
  //free memory
  delete[] perf_of_trials;
}