#ifndef FILE_OPERATOR_H
#define FILE_OPERATOR_H
#include<stdio.h>
#include<cmath>
#include "Problem.hpp"
class Fileoperator{
    public:
    FILE *log_performance;
    char *perf_filename;
    double* perf_of_trials;

    void get_mean(int r, double value);




};

extern Fileoperator foperator;



#endif