#include "Solver.hpp"
#include "Macro.hpp"
Solver::Solver(string methods, char*list){
    flag  = methods;
    midlist = list;
};
void Solver::test(){
    cout << problem->ACTUAL_PROBLEM_SIZE<<endl;
}

Solver::~Solver(){
  //free memory
 free(perf_of_trials);
}


void Solver::init(){
    get_par_list();
    init_algorithm();
}
void Solver::get_par_list(){
    const char * split = ", ";
    char* temp = strtok(midlist, split);
	while (temp != NULL)
	{
	parameter_list.push_back(string(temp));		
	temp = strtok(NULL, split);	// 下一个被分割的串
	}
}
void Solver::init_algorithm(){
    if(flag == "ACO"){
        double number1 = stod(parameter_list[0]);
        double number2 = stod(parameter_list[1]);
        double number3 = stod(parameter_list[2]);
        double number4 = stod(parameter_list[3]);
        double number5 = stod(parameter_list[4]);
        int number6 = stoi(parameter_list[5]);
        cout<<number1<<" "<<number2<<" "<<number3<<" "<<number4<<" "<<number5<<" " <<number6<<" " <<endl;
        aco = new SACO (1, 1, 60, 0.1, 2, 0);
    }
}

void Solver::start_run(int r){

  srand(r); //random seed
  problem->init_evals();
  problem->init_current_best();
  cout << "Start Runing Round: " << r << " with random seed " << r << endl;
}
/*sets the termination conidition for your heuristic*/
bool Solver::termination_condition(double rate) {

  bool flag;
  if(problem->evals >= rate * problem->TERMINATION)
    flag = true;
  else
    flag = false;

  return flag;
}
/*gets an observation of the run for your heuristic*/
void Solver::end_run(int r){
  get_mean(r-1,problem->current_best); //from stats.h
  cout << "End of run " << r << " with best solution quality " << problem->current_best << " total evaluations: " << problem->evals << endl;
  cout << " " << endl;
}



void Solver::optimize(int run){
    if(flag == "ACO"){
      aco->init(run);
        aco->optimize();
        if (run == 1){
          conv.push_back(problem->best_sol->tour_length);
        }
    }
}
void Solver::save_solution(int run){
    problem->save_solution(flag, problem->instance, run); 
}
void Solver::freeall(){
    delete[] problem->best_sol->tour;
    delete problem->best_sol;
}



void Solver::isFileExist(string filename)
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

void Solver::open_file(){
    isFileExist("./Result");

    perf_of_trials = new double[MAX_TRIALS];

    for (int i = 0; i < MAX_TRIALS; i++) { // initialise the perf_of_trials
        perf_of_trials[i] = 0.0;
    }

    // initialize and open output files
    perf_filename = new char[CHAR_LEN];
    sprintf(perf_filename, "./Result/%s.Result.txt", problem->instance);

    // for performance
    if ((log_performance = fopen(perf_filename, "a")) == NULL) {
        std::cout << "Failed to open stats file " << perf_filename << std::endl;
        exit(2);
    }
  // initialize and open output files

}


void Solver::get_mean(int r, double value) {

  perf_of_trials[r] = value;

}

double Solver::mean(double* values, int size){
  int i;
  double m = 0.0;
  for (i = 0; i < size; i++){
      m += values[i];
  }
  m = m / (double)size;
  return m; //mean
}

double Solver::stdev(double* values, int size, double average){
  int i;
  double dev = 0.0;

  if (size <= 1)
    return 0.0;
  for (i = 0; i < size; i++){
    dev += ((double)values[i] - average) * ((double)values[i] - average);
  }
  return sqrt(dev / (double)(size - 1)); //standard deviation
}

double Solver::best_of_vector(double *values, int l ) {
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


double Solver::worst_of_vector(double *values, int l ) {
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



void Solver::close_file(void){
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

