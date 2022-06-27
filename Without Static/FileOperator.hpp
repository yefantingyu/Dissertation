#include<cmath>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<cstring>
#include<math.h>
#include<fstream>
#include<time.h>
#include<limits.h>

using namespace std;

class FileOperator{
public:
  FILE *log_performance;
  char *perf_filename;
  double* perf_of_trials;


  FileOperator();
  ~FileOperator();
  void isFileExist(string filename);
  void open_file(char* instance);
  void get_mean(int r, double value);
  double mean(double* values, int size);
  double stdev(double* values, int size, double average);
  double best_of_vector(double *values, int l );
  double worst_of_vector(double *values, int l );
  void close_stats(void);
  
};


