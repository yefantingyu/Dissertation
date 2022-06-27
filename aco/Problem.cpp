#include "Problem.hpp"
#include "Macro.hpp"


Problem* Problem::instancer = NULL;

Problem* Problem::getInstance() {
  if(instancer == NULL) {
    instancer = new Problem();
  }
  return instancer;
}


void Problem::set_instance(char* filename){
    instance = filename;
    instance_path = string("./DataSet/") + instance;
}

Problem::~Problem(){
  int i;
  delete[] node_list;
  delete[] cust_demand;
  delete[] charging_station;

  for(i = 0; i < ACTUAL_PROBLEM_SIZE; i++) {
    delete[] distances[i];
  }

  delete[] distances;
}

void Problem::read_instance(){
    int i;
  char line[CHAR_LEN];
  char * keywords;
  char Delimiters[] = " :=\n\t\r\f\v";
  ifstream fin(instance_path);
  while((fin.getline(line, CHAR_LEN-1))){

    if(!(keywords = strtok(line, Delimiters)))
      continue;
    if(!strcmp(keywords, "DIMENSION")){
      if(!sscanf(strtok(NULL, Delimiters), "%d", &problem_size)){
	cout<<"DIMENSION error"<<endl;
	exit(0);
      }
    }
    else if(!strcmp(keywords, "EDGE_WEIGHT_TYPE")){
      char * tempChar;
      if(!(tempChar=strtok(NULL, Delimiters))){
	cout<<"EDGE_WEIGHT_TYPE error"<<endl;
	exit(0);
      }
      if(strcmp(tempChar, "EUC_2D")){
	cout<<"not EUC_2D"<<endl;
	exit(0);
      }
    }
    else if (!strcmp(keywords, "CAPACITY")){
       if(!sscanf(strtok(NULL,Delimiters), "%d", &MAX_CAPACITY)){
          cout << "CAPACITY error" << endl;
          exit(0);
       }
    }
    else if (!strcmp(keywords, "VEHICLES")){
       if(!sscanf(strtok(NULL,Delimiters), "%d", &MIN_VEHICLES)){
          cout << "VEHICLES error" << endl;
          exit(0);
       }
    }
    else if (!strcmp(keywords, "ENERGY_CAPACITY")){
       if(!sscanf(strtok(NULL,Delimiters), "%d", &BATTERY_CAPACITY)){
          cout << "ENERGY_CAPACITY error" << endl;
          exit(0);
       }
    }
    else if (!strcmp(keywords, "ENERGY_CONSUMPTION")){
       if(!sscanf(strtok(NULL,Delimiters), "%lf", &energy_consumption)){
          cout << "ENERGY_CONSUMPTION error" << endl;
          exit(0);
       }
    }
    else if (!strcmp(keywords, "STATIONS")){
       if(!sscanf(strtok(NULL,Delimiters), "%d", &NUM_OF_STATIONS)){
          cout << "STATIONS error" << endl;
          exit(0);
       }
    }
    else if (!strcmp(keywords, "VEHICLES")){
       if(!sscanf(strtok(NULL,Delimiters), "%d", &NUM_OF_VEHICLES)){
          cout << "NUM_OF_VEHICLES" << endl;
          exit(0);
       }
    }
    else if (!strcmp(keywords, "OPTIMAL_VALUE")){
       if(!sscanf(strtok(NULL,Delimiters), "%lf", &OPTIMUM)){
          cout << "OPTIMAL_VALUE error" << endl;
          exit(0);
       }
    }
    else if(!strcmp(keywords, "NODE_COORD_SECTION")){
      if(problem_size!=0){
         /*prroblem_size is the number of customers plus the depot*/
          NUM_OF_CUSTOMERS = problem_size - 1;
          ACTUAL_PROBLEM_SIZE = problem_size + NUM_OF_STATIONS;

          node_list = new node[ACTUAL_PROBLEM_SIZE];

        for(i=0; i < ACTUAL_PROBLEM_SIZE; i++){
	         //store initial objects
           fin>>node_list[i].id;
	         fin>>node_list[i].x>>node_list[i].y;
           node_list[i].id -=1;
        }
        //compute the distances using initial objects
        distances = generate_2D_matrix_double(ACTUAL_PROBLEM_SIZE, ACTUAL_PROBLEM_SIZE);
     } else {
           cout << "wrong problem instance file" << endl;
           exit(1);
     }
    }
    else if(!strcmp(keywords, "DEMAND_SECTION")){
     if(problem_size!=0){

       int temp;
       //masked_demand = new int[problem_size];
       cust_demand = new int[ACTUAL_PROBLEM_SIZE];
       charging_station = new bool[ACTUAL_PROBLEM_SIZE];
       for(i = 0; i < problem_size; i++){
        fin >> temp;
        fin >> cust_demand[temp-1];
       }

       //initialize the charging stations.
       //the depot is set to a charging station in the DEPOT_SECTION
       for(i = 0; i < ACTUAL_PROBLEM_SIZE; i++){
          if(i < problem_size) {
            charging_station[i] = false;
          } else {
            charging_station[i] = true;
            cust_demand[i] = 0;
          }
        }
      }
    }
    else if(!strcmp(keywords, "DEPOT_SECTION")){
      fin >> DEPOT;
      DEPOT-=1;
      charging_station[DEPOT] = true;
    }
  }
  fin.close();
  TERMINATION = 25000*ACTUAL_PROBLEM_SIZE;
  if(ACTUAL_PROBLEM_SIZE == 0) {
        cout << "wrong problem instance file" << endl;
        exit(1);
  } else {
    compute_distances();
  }

}
void Problem::compute_nearest_points() {
    for(int i = 1; i <= NUM_OF_CUSTOMERS; i++) {
        nearest[i].assign(NUM_OF_CUSTOMERS, 0);
        for(int j = 0; j < NUM_OF_CUSTOMERS; j++) {
            nearest[i][j] = j + 1;
        }

        sort(nearest[i].begin(), nearest[i].end(), [&, i](int j, int k) {
            return get_distance(i, j) < get_distance(i, k);
        });
    }
}
/****************************************************************/
/* Returns the distance between two points: from and to. Can be */
/* used to evaluate a part of the solution. The fitness         */
/* evaluation count will be proportional to the problem size    */
/****************************************************************/
double Problem::get_distance(int from, int to){
  //adds partial evaluation to the overall fitness evaluation count
  //It can be used when local search is used and a whole evaluation is not necessary
  evals += (1.0/ACTUAL_PROBLEM_SIZE);

  return distances[from][to];

}

/****************************************************************/
/* Returns the solution quality of the solution. Taken as input */
/* an array of node indeces and its length                      */
/****************************************************************/
double Problem::fitness_evaluation(int *routes, int size, bool save = true) {
  int i;
  double tour_length = 0.0;
  for (i = 0; i < size-1; i++)
    tour_length += distances[routes[i]][routes[i+1]];
  if (save)
    if(tour_length < current_best){
        current_best = tour_length;
    }
  evals++;

  return tour_length;
}
/****************************************************************/
/* Validates the routes of the solution. Taken as input         */
/* an array of node indeces and its length                      */
/****************************************************************/
bool Problem::check_solution(int *t, int size){
  // cout << "st\n";
  int i, from, to;
  double energy_temp = BATTERY_CAPACITY;
  double capacity_temp = MAX_CAPACITY;
  double distance_temp = 0.0;
  static int visited[MAX_NODE];
  for (int i = 0; i <= NUM_OF_CUSTOMERS; i++)
    visited[i] = 0;
  // cout << "Solution: ";
  // for(i = 0; i < size; i++){
  //   cout << t[i] << " ";
  // } cout << "\n";
  for(i = 0; i < size-1; i++){
    from = t[i];
    to = t[i+1];
    if (from <= NUM_OF_CUSTOMERS) visited[from] += 1;
    capacity_temp -= get_customer_demand(to);
    energy_temp -= get_energy_consumption(from,to);
    distance_temp += get_distance(from,to);
    if(capacity_temp < 0.0) {
      // cout << "Capacity is exceeded! The capacity is " << capacity_temp << " at " << from << " to " << to << "\n";
      return false;
    } 
    if(energy_temp < 0.0) {
      // cout << "Energy is exceeded! The energy is " << energy_temp << " at " << from << " to " << to << "\n";
      return false;
    } 
    if(to == DEPOT) {
      capacity_temp = MAX_CAPACITY;
      energy_temp = BATTERY_CAPACITY;
    }
    if(is_charging_station(to)){
      energy_temp = BATTERY_CAPACITY;
    }
  }
  for (int i = 1; i <= NUM_OF_CUSTOMERS; i++){
    if (visited[i] != 1){
      // for (int j = 0; j < size; j++){
      //   cout << t[j] << " ";
      // }
      // cout << size << ":\n";
      // cout << "Not have the customer " << i << "!\n";
      return false;
    }
  }
  if(t[0] != 0 || t[size - 1] != 0){
    // cout << "Warning false!\n";
    return false;
  }
  return true;
}

/****************************************************************/
/* Returns the energy consumed when travelling between two      */
/* points: from and to.                                         */
/****************************************************************/
double Problem::get_energy_consumption(int from, int to) {

    /*DO NOT USE THIS FUNCTION MAKE ANY CALCULATIONS TO THE ROUTE COST*/
    return energy_consumption*distances[from][to];

}

/****************************************************************/
/* Returns the demand for a specific customer                   */
/* points: from and to.                                         */
/****************************************************************/
int Problem::get_customer_demand(int customer){
  if (customer == -1){
    cout << "Warning customer invalid - 1!\n";
  }

  return cust_demand[customer];

}

/****************************************************************/
/* Returns true when a specific node is a charging station;     */
/* and false otherwise                                          */
/****************************************************************/
bool Problem::is_charging_station(int node){
  if (node == -1){
    cout << "Warning node invalid - 1!\n";
  }

  bool flag = false;
  if(charging_station[node] == true)
    flag = true;
  else
    flag = false;
  return flag;

}

void Problem::save_conv(vector<double> conv, string name = "conv_file"){
    ofstream outfile2;
    string file_name = "Result/cov_" + name + ".txt";
    outfile2.open(file_name);
    for(double value: conv) {
        outfile2 << value << "\n";
    }
    outfile2.close();
}

void Problem::save_solution(string algorithm, string task, int run){
    ofstream outfile2;
    string file_name = "Data/location/" + algorithm + '_' +
       task + '_' + to_string(run) +".txt";
    outfile2.open(file_name);
    outfile2 << best_sol->steps << "\n";
    for(int i = 0; i < best_sol->steps; i++) {
        outfile2 << node_list[best_sol->tour[i]].x  << " " << node_list[best_sol->tour[i]].y << " ";
    }
    outfile2.close();

    ofstream outfile;
    file_name = "Data/location/order_" + algorithm + '_' +
       task + '_' + to_string(run) +".txt";
    outfile.open(file_name);
    for(int i = 0; i < ACTUAL_PROBLEM_SIZE; i++) {
        if(i == 0) {
            outfile << "0 ";
        } else {
            if(i < problem_size) {
                outfile << "1 ";
            } else {
                outfile << "2 ";
            }
        }

        outfile << node_list[i].x  << " " << node_list[i].y << " ";
    }
    outfile.close();

}












































/****************************************************************/
/*Compute and return the euclidean distance of two objects      */
/****************************************************************/
double Problem::euclidean_distance(int i, int j) {
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
void  Problem::compute_distances(void) {
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
double **  Problem::generate_2D_matrix_double(int n, int m){
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
/****************************************************************/
/* Reset the evaluation counter for a new indepedent run        */
/****************************************************************/
void Problem::init_evals(){

  evals = 0;

}
/*******************************************************************/
/* Reset the best solution quality so far for a new indepedent run */
/*******************************************************************/
void Problem::init_current_best(){

   current_best = INF;

}


