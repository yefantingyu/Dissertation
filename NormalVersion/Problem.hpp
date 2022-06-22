#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<cstring>
#include<math.h>
#include<algorithm>
#include<fstream>
#include<time.h>
#include<limits.h>
#include <vector>
#include <limits.h>
#include <string>
using namespace std;



struct node {
  int id;
  double x;
  double y;
};
struct solution{
  int *tour;	//this is what the fitness_evaluation function in EVRP.hpp will evaluate
  int id;
  double tour_length; //quality of the solution
  int steps; //size of the solution
  //the format of the solution is as follows:
  //*tour:  0 - 5 - 6 - 8 - 0 - 1 - 2 - 3 - 4 - 0 - 7 - 0
  //*steps: 12
  //this solution consists of three routes:
  //Route 1: 0 - 5 - 6 - 8 - 0
  //Route 2: 0 - 1 - 2 - 3 - 4 - 0
  //Route 3: 0 - 7 - 0
};

class Problem{
public:
  char* instance;          //Name of the instance
  string instance_path = "./DataSet/";//Path


  /*Parameter for the file*/
  int TERMINATION;
  struct node *node_list;     //List of nodes with id and x and y coordinates
  int *cust_demand;                //List with id and customer demands
  bool* charging_station;
  double **distances;              //Distance matrix
  int problem_size;                //Problem dimension read
  double energy_consumption;

  int DEPOT;                       //depot id (usually 0)
  int NUM_OF_CUSTOMERS;       //Number of customers (excluding depot)
  int ACTUAL_PROBLEM_SIZE;        //Tottal number of customers, charging stations and depot
  int NUM_OF_VEHICLES;
  double OPTIMUM;
  int NUM_OF_STATIONS;
  int BATTERY_CAPACITY;       //maximum energy of vehicles
  int MAX_CAPACITY;           //capacity of vehicles
  int MIN_VEHICLES;
  std::vector<short int> nearest [2500];

  solution *best_sol;

  Problem();
  Problem(char* filename);
  void instance_reader();







  double euclidean_distance(int i, int j);
  void  compute_distances(void);
  double **  generate_2D_matrix_double(int n, int m);
};









// /****************************************************************/
// /* Returns true when a specific node is a charging station;     */
// /* and false otherwise                                          */
// /****************************************************************/
// bool is_charging_station(int node){
//   if (node == -1){
//     cout << "Warning node invalid - 1!\n";
//   }

//   bool flag = false;
//   if(charging_station[node] == true)
//     flag = true;
//   else
//     flag = false;
//   return flag;

// }



/****************************************************************/
/* Read the problem instance and generate the initial object    */
/* vector.                                                      */
/****************************************************************/


// /****************************************************************/
// /* Returns the distance between two points: from and to. Can be */
// /* used to evaluate a part of the solution. The fitness         */
// /* evaluation count will be proportional to the problem size    */
// /****************************************************************/
// double get_distance(int from, int to){
//   //adds partial evaluation to the overall fitness evaluation count
//   //It can be used when local search is used and a whole evaluation is not necessary
//   evals += (1.0/ACTUAL_PROBLEM_SIZE);

//   return distances[from][to];

// }


// void compute_nearest_points() {
//     for(int i = 1; i <= NUM_OF_CUSTOMERS; i++) {
//         nearest[i].assign(NUM_OF_CUSTOMERS, 0);  // 这步告诉人们一共给这个vector分配num——of_customer个空间，每个空间赋值为0
//         for(int j = 0; j < NUM_OF_CUSTOMERS; j++) {
//             nearest[i][j] = j + 1;
//         }

//         sort(nearest[i].begin(), nearest[i].end(), [&, i](int j, int k) {// 根据距离重新选择从一个点出发到另一个点的最短距离
//             return get_distance(i, j) < get_distance(i, k);
//         });
//     }
// }
// /****************************************************************/
// /* Returns the demand for a specific customer                   */
// /* points: from and to.                                         */
// /****************************************************************/
// int get_customer_demand(int customer){
//   if (customer == -1){
//     cout << "Warning customer invalid - 1!\n";
//   }

//   return cust_demand[customer];

// }
// /****************************************************************/
// /* Returns the energy consumed when travelling between two      */
// /* points: from and to.                                         */
// /****************************************************************/
// double get_energy_consumption(int from, int to) {

//     /*DO NOT USE THIS FUNCTION MAKE ANY CALCULATIONS TO THE ROUTE COST*/
//     return energy_consumption*distances[from][to];

// }
// /****************************************************************/
// /* Validates the routes of the solution. Taken as input         */
// /* an array of node indeces and its length                      */
// /****************************************************************/
// bool check_solution(int *t, int size){
//   // cout << "st\n";
//   int i, from, to;
//   double energy_temp = BATTERY_CAPACITY;
//   double capacity_temp = MAX_CAPACITY;
//   double distance_temp = 0.0;
//   static int visited[MAX_NODE];
//   for (int i = 0; i <= NUM_OF_CUSTOMERS; i++)
//     visited[i] = 0;
//   // cout << "Solution: ";
//   // for(i = 0; i < size; i++){
//   //   cout << t[i] << " ";
//   // } cout << "\n";
//   for(i = 0; i < size-1; i++){
//     from = t[i];
//     to = t[i+1];
//     if (from <= NUM_OF_CUSTOMERS) visited[from] += 1;
//     capacity_temp -= get_customer_demand(to);
//     energy_temp -= get_energy_consumption(from,to);
//     distance_temp += get_distance(from,to);
//     if(capacity_temp < 0.0) {
//       // cout << "Capacity is exceeded! The capacity is " << capacity_temp << " at " << from << " to " << to << "\n";
//       return false;
//     } 
//     if(energy_temp < 0.0) {
//       // cout << "Energy is exceeded! The energy is " << energy_temp << " at " << from << " to " << to << "\n";
//       return false;
//     } 
//     if(to == DEPOT) {
//       capacity_temp = MAX_CAPACITY;
//       energy_temp = BATTERY_CAPACITY;
//     }
//     if(is_charging_station(to)){
//       energy_temp = BATTERY_CAPACITY;
//     }
//   }
//   for (int i = 1; i <= NUM_OF_CUSTOMERS; i++){
//     if (visited[i] != 1){
//       // for (int j = 0; j < size; j++){
//       //   cout << t[j] << " ";
//       // }
//       // cout << size << ":\n";
//       // cout << "Not have the customer " << i << "!\n";
//       return false;
//     }
//   }
//   if(t[0] != 0 || t[size - 1] != 0){
//     // cout << "Warning false!\n";
//     return false;
//   }
//   return true;
// }


// /****************************************************************/
// /* Returns the solution quality of the solution. Taken as input */
// /* an array of node indeces and its length                      */
// /****************************************************************/
// double fitness_evaluation(int *routes, int size, bool save = true) {
//   int i;
//   double tour_length = 0.0;

//   //the format of the solution that this method evaluates is the following
//   //Node id:  0 - 5 - 6 - 8 - 0 - 1 - 2 - 3 - 4 - 0 - 7 - 0
//   //Route id: 1 - 1 - 1 - 1 - 2 - 2 - 2 - 2 - 2 - 3 - 3 - 3
//   //this solution consists of three routes:
//   //Route 1: 0 - 5 - 6 - 8 - 0
//   //Route 2: 0 - 1 - 2 - 3 - 4 - 0
//   //Route 3: 0 - 7 - 0
//   for (i = 0; i < size-1; i++)
//     tour_length += distances[routes[i]][routes[i+1]];
//   if (save)
//     if(tour_length < current_best){
//         current_best = tour_length;
//     }
//   //adds complete evaluation to the overall fitness evaluation count
//   evals++;

//   return tour_length;
// }

// void save_solution(string algorithm, string task, int run){
//     ofstream outfile2;
//     string file_name = "Data/location/" + algorithm + '_' +
//        task + '_' + to_string(run) +".txt";
//     outfile2.open(file_name);
//     outfile2 << best_sol->steps << "\n";
//     for(int i = 0; i < best_sol->steps; i++) {
//         outfile2 << node_list[best_sol->tour[i]].x  << " " << node_list[best_sol->tour[i]].y << " ";
//     }
//     outfile2.close();

//     ofstream outfile;
//     file_name = "Data/location/order_" + algorithm + '_' +
//        task + '_' + to_string(run) +".txt";
//     outfile.open(file_name);
//     for(int i = 0; i < ACTUAL_PROBLEM_SIZE; i++) {
//         if(i == 0) {
//             outfile << "0 ";
//         } else {
//             if(i < problem_size) {
//                 outfile << "1 ";
//             } else {
//                 outfile << "2 ";
//             }
//         }

//         outfile << node_list[i].x  << " " << node_list[i].y << " ";
//     }
//     outfile.close();

// }

// void save_conv(vector<double> conv, string name = "conv_file"){
//     ofstream outfile2;
//     string file_name = "Data/" + algorithm + '_' + name + ".txt";
//     outfile2.open(file_name);
//     for(double value: conv) {
//         outfile2 << value << "\n";
//     }
//     outfile2.close();
// }