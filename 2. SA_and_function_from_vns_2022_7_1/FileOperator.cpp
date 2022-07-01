#include "FileOperator.hpp"
#include "Problem.hpp"
#include <cstdio>

Fileoperator foperator;
Fileoperator::Fileoperator() {}

void Fileoperator::init() {
 // Final_results.resize(problem->TERMINATION / problem->ACTUAL_PROBLEM_SIZE);
}

void Fileoperator::get_mean(int r, double value) { perf_of_trials[r] = value; }
void Fileoperator::save_results() {
  FILE *log_performancet;
  char *perf_filenamet = new char[CHAR_LEN];
  sprintf(perf_filenamet, "results.%s.txt", problem->instance);
  log_performancet = fopen(perf_filenamet, "a");
  fprintf(log_performancet, "Iteration\tResults\n");
  for (int i = 0; i < Final_results.size(); i++) {
    fprintf(log_performancet, "%d \t%lf", i, Final_results[i]);
    fprintf(log_performancet, "\n");
    
  }
  fclose(log_performancet);
}
