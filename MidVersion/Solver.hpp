#ifndef Solver_HPP
#define Solver_HPP
#include "./Model/ACO.hpp"
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
class Solver{
public:
    double evals;
    double current_best;
    string flag;
    char * midlist;
    vector<string> parameter_list;

    SACO * aco;
    vector<double> conv;

    Solver(string methods, char*list);
    void init();
    void get_par_list();
    void init_algorithm();
    void init_evals();					    //initializes the evaluations
    void init_current_best();				//initializes the best solution quality


    void optimize(int run);
    void save_solution(int run);

};
Solver::Solver(string methods, char*list){
    flag  = methods;
    midlist = list;
};
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
        // cout<<number1<<" "<<number2<<" "<<number3<<" "<<number4<<" "<<number5<<" " <<number6<<" " <<endl;
        aco = new SACO (1, 1, 60, 0.1, 2, 0);
    }
}

void Solver::init_evals(){
    evals = 0;
}
void Solver::init_current_best(){
    current_best = INF;
}

void Solver::optimize(int run){
    if(flag == "ACO"){
        aco->optimize();
        if (run == 1){
          conv.push_back(best_sol->tour_length);
        }
    }
}
void Solver::save_solution(int run){
    save_solution(algorithm, problem_instance, run); 
}

    


#endif // FILE_OPERATION_HPP