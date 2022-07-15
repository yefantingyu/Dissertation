#ifndef UTILS_HPP
#define UTILS_HPP
#include "Problem.hpp"
void allocate_space_for_one_dimen_array(int*& array, int dimensiont);
void allocate_space_for_one_dimen_array(bool*& array, int dimensiont);
void allocate_space_for_one_dimen_array(double*& array, int dimensiont);
void allocate_space_for_one_dimen_array(AntStruct & array, int dimensiont);

void allocate_space_for_two_dimen_array(int**& array, int m, int n);
void allocate_space_for_two_dimen_array(double**& array, int m, int n);


void free_one_dimen_array(int*& array);
void free_one_dimen_array(bool*& array);
void free_one_dimen_array(double*& array);
void free_one_dimen_array(AntStruct*& array);

void free_two_dimen_array(int**& array);
void free_two_dimen_array(double**& array);


#endif