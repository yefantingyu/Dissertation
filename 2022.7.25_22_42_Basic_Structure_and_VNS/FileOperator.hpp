#ifndef FILE_OPERATOR_H
#define FILE_OPERATOR_H
#include<stdio.h>
#include<cmath>
#include "Problem.hpp"

void open_stats(void);									//creates the output file
void close_stats(void);									//stores the best values for each run
void get_mean(int r, double value);						//stores the observation from each run
void free_stats();										//free memory
double worst_of_vector(double *values, int l);
double best_of_vector(double *values, int l );
double stdev(double* values, int size, double average);
double mean(double* values, int size);
void get_mean(int r, double value);
void save_results();
#endif