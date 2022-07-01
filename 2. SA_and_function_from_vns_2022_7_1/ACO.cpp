#include "Algorithm.hpp"


ACO::ACO(int runt){
    run = runt;
    randomt = new Randoms (run);	
    num_of_ants = problem->aco_parameter.num_of_ants;
    actual_problem_size = problem->ACTUAL_PROBLEM_SIZE;
    taumax = problem->aco_parameter.taumax;

    Ants.resize(num_of_ants);
    // cities.resize(actual_problem_size);
    pheromones.resize(actual_problem_size);
    deltapheromones.resize(actual_problem_size);
    Probability.resize(actual_problem_size-1);//>??????????????????????

    for (int i=0; i< num_of_ants; i++) {
		for (int j=0; j<actual_problem_size; j++) {
			Ants[i].solution[j] = 0;
		}
	}

    for(int i=0; i<actual_problem_size; i++) {
		// cities[i] 			= new double[2];
		pheromones[i].resize(actual_problem_size,0);
		deltapheromones[i].resize(actual_problem_size,0);
		Probability[i].resize(2,-1);
	}	
    for (int i = 0; i < actual_problem_size; i++){
        for (int j = i + 1; j < actual_problem_size; j++){
           pheromones[i][j] = randomt -> Uniforme() * taumax;
	       pheromones[j][i] = pheromones[i][j];
        }
    }
    problem->initialist_solution();
    problem->initothers();
}
double ACO::update_proability(int cityi, int cityj, int *visited){
	double ETAij = (double) pow (1 / problem->get_distance(cityi, cityj), problem->aco_parameter.BETA);
	double TAUij = (double) pow (pheromones[cityi][cityj],   problem->aco_parameter.ALPHA);
	double sum = 1e-10;
	for (int c=1; c<=problem->NUM_OF_CUSTOMERS; c++) {
        if (!visited[c]) {
            double ETA = (double) pow (1 / problem->get_distance (cityi, c), problem->aco_parameter.BETA);
            double TAU = (double) pow (pheromones[cityi][c],   problem->aco_parameter.ALPHA);
            sum += ETA * TAU;
        }	
	}
	return (ETAij * TAUij) / sum;
}


void ACO::set_initial_information(){
    int current_solutioner[MAX_NODE];
    int temp_customer_visited[MAX_NODE];
    memset(temp_customer_visited, 0, sizeof(temp_customer_visited));
    int current_position = 1;
    int from, to;
    int start_depot = 0;
    int end_depot = 0;
    int num_customer = 0;
    double temp_energy = problem->BATTERY_CAPACITY;
    double temp_capacity = problem->MAX_CAPACITY;
    int counter = 0;

    for(int k=0; k<num_of_ants; k++){
        bool validate = false;
        while(!validate){
            Ants[k].step = 0;
            Ants[k].solution[0] = 0;
            while(num_customer < problem->NUM_OF_CUSTOMERS){
                from = Ants[k].solution[current_position - 1];
                counter = 0;
                for(to = 1; to<=problem->NUM_OF_CUSTOMERS;to++){
                    if(from==to){continue;}
                    if(temp_capacity <problem->get_customer_demand(to)){
                        continue;
                    }
                    if(!temp_customer_visited[to]){// 如果这个点没被访问
                        Probability[counter][0]=update_proability(from,to,temp_customer_visited);
                        Probability[counter][1] = (double) to;
                        counter++;
                    }
                }
                // 如果counter等于0，说明所有的城市都被访问了，要返回0了
                if(counter==0){
                    temp_energy = problem->BATTERY_CAPACITY;
			        temp_capacity  = problem->MAX_CAPACITY;
                    end_depot = current_position;
                    Ants[k].solution[current_position++] = problem->DEPOT;
                    int len = end_depot - start_depot + 1;
                    for (int i = 0; i < len; i++){
				        current_solutioner[i] = Ants[k].solution[start_depot + i];
			        }
                    //local_search();
                    int end_pos = start_depot;

                }

            }

        }
    }

}

void ACO::construct_route(){

}

void ACO::update_upheromones(){

}
void ACO::optimize(){
    for (int k = 0; k < num_of_ants; k++) {







    }
}
