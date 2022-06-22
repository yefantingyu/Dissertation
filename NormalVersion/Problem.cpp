#include "Problem.hpp"
#include "Macro.hpp"

Problem::Problem(char* filename){
    instance = filename;
    instance_path = instance_path + instance;
}

void Problem::instance_reader(){
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

