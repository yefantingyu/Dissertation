#ifndef EVRP_EVRP_HPP
#define EVRP_EVRP_HPP

#include<math.h>
#include "string.h"
#include <cstring>
#include <string>
#include<iostream>
#include<stdlib.h>
#include<limits.h>
#include<stdio.h>
#include<time.h>
#include <vector>
#include <fstream>
#include <cstdio>

#define CHAR_LEN 100
#define TERMINATION 25000*ACTUAL_PROBLEM_SIZE  	//DO NOT CHANGE THE NUMBER
#define MAX_TRIALS 	20 									//DO NOT CHANGE THE NUMBER
using namespace std;

FILE *log_performance;
//output files
char *perf_filename;
double* perf_of_trials;


struct node {
  int id;
  double x;
  double y;
};
char* problem_instance;          //Name of the instance
char* problem_instance_path;          //Name of the instance
int DEPOT;                       //depot id (usually 0)
int NUM_OF_CUSTOMERS;       //Number of customers (excluding depot)
int ACTUAL_PROBLEM_SIZE;        //Tottal number of customers, charging stations and depot
double OPTIMUM;
int NUM_OF_STATIONS;
int BATTERY_CAPACITY;       //maximum energy of vehicles
int MAX_CAPACITY;           //capacity of vehicles
int MIN_VEHICLES;

struct node *node_list;     //List of nodes with id and x and y coordinates
int *cust_demand;                //List with id and customer demands
bool* charging_station;
double **distances;              //Distance matrix
int problem_size;                //Problem dimension read
double energy_consumption;

double evals;
double current_best;









/****************************************************************/
/*Compute and return the euclidean distance of two objects      */
/****************************************************************/
double euclidean_distance(int i, int j) {
  double xd,yd;
  double r = 0.0;
  xd = node_list[i].x - node_list[j].x;
  yd = node_list[i].y - node_list[j].y;
  r  = sqrt(xd*xd + yd*yd);
  return r;
}

/****************************************************************/
/*Compute the distance matrix of the problem instance           */
/****************************************************************/
void compute_distances(void) {
int i, j;
  for(i = 0; i < ACTUAL_PROBLEM_SIZE; i++){
    for(j = 0; j < ACTUAL_PROBLEM_SIZE; j++){
      distances[i][j] = euclidean_distance(i,j);
    }
  }
}

/****************************************************************/
/*Generate and return a two-dimension array of type double      */
/****************************************************************/
double ** generate_2D_matrix_double(int n, int m){
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





#endif //EVRP_EVRP_HPP