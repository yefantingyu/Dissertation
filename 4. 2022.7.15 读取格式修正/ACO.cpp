#include "ACO.hpp"
#include "Macro.hpp" 
#include "LocalSearch.hpp"
#include "Utils.hpp"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <assert.h>
#include <vector>
using namespace std;


ACO::ACO(){
    alpha = acoparameter.alpha;
    betat= acoparameter.beta;
    rho = acoparameter.rho;


    int i;
    allocate_space_for_two_dimen_array(nearest,num_of_customers,num_of_customers);
    compute_nearest_points();


}
void ACO::init(){
    iteration = 0;
    allocate_space_for_two_dimen_array(pheromone,actual_problem_size,actual_problem_size);
    allocate_space_for_two_dimen_array(deltapheromone,actual_problem_size,actual_problem_size);
    allocate_space_for_two_dimen_array(probs,actual_problem_size-1,2);
    for(int i=0;i<num_of_ants;i++){
        allocate_space_for_one_dimen_array(Ants[i].tour,problem_size);
        memset(Ants[i].tour, 0, problem_size);
        allocate_space_for_one_dimen_array(Ants[i].evrptour,2*actual_problem_size);
        memset(Ants[i].evrptour, 0, 2*actual_problem_size);
        allocate_space_for_one_dimen_array(Ants[i].visited,num_of_customers);
        //allocate_space_for_one_dimen_array(Ants[i].candidate,num_of_customers);
        memset(Ants[i].visited, false, num_of_customers);
    }

    for(int i=0; i<actual_problem_size; i++) {
		pheromone[i] 		= new double[actual_problem_size];
		deltapheromone[i] 	= new double[actual_problem_size];
		probs[i] 			= new double[2];
		for (int j=0; j<2; j++) {
			probs[i][j]  = -1.0;
		}
		for (int j=0; j<actual_problem_size; j++) {
			pheromone[i][j] 		= 0.0;
			deltapheromone[i][j] 	= 0.0;
		}
	}
    randoms = new Randoms (time(NULL));	
    

    for (int i = 0; i < actual_problem_size; i++){
        for (int j = i + 1; j < actual_problem_size; j++){
            pheromone[i][j] = randoms -> Uniforme() * taumax;
	        pheromone[j][i] = pheromone[i][j];
        }
    }


    best_sol.tour = new int[2*actual_problem_size];
    best_sol.id = 1;
    best_sol.steps = 0;
    best_sol.tour_length = INF;	

}
double ACO::phiupdate (int cityi, int cityj, int *visited) {
	double ETAij = (double) pow (1 / get_distance(cityi, cityj), BETA);
	double TAUij = (double) pow (pheromone[cityi][cityj],   alpha);
	double sum = 1e-10;
	for (int c=1; c<=num_of_customers; c++) {
        if (!visited[c]) {
            double ETA = (double) pow (1 / get_distance (cityi, c), BETA);
            double TAU = (double) pow (pheromone[cityi][c],   alpha);
            sum += ETA * TAU;
        }	
	}
	return (ETAij * TAUij) / sum;
}
int ACO::choosecity (int count) {
	double xi = randoms -> Uniforme();
	int i = 0;
	double total = 0;
	for (int i = 0; i < count; i++){
		total += probs[i][0];
	}
	xi *= total;
	double sum = probs[i][0];
	while (sum < xi) {
		i++;
		sum += probs[i][0];
	}
	return (int) probs[i][1];
}
bool ACO::one_ant_finding_route(int k){
	Ants[k].evrptour[0] = 0;
    int cnt = 1;
    static int from, to;
    static double energy_temp, capacity_temp;
    energy_temp = battery_capacity;
    capacity_temp = vehicle_capacity;
    int start_depot = 0, end_depot = 0;
	int n_customer = 0;
	// cout << "st\n";
	while (n_customer < num_of_customers){
		from = Ants[k].evrptour[cnt - 1];
		//  cout << "from: " << from << " cnt: " << cnt << "\n";
		int count = 0;
		for (to = 1; to <= num_of_ants; to++) {
			if (from == to) 
				continue;	

            // cout << to << " mid1\n";
            if (capacity_temp < get_customer_demand(to)){
                continue;
            }
			
			if (!Ants[k].visited[to]) {
				probs[count][0] = phiupdate (from, to, visited);
				probs[count][1] = (double) to;
				count++;
			}
		}
		if (0 == count) {
        energy_temp = battery_capacity;
        capacity_temp = vehicle_capacity;
			end_depot = cnt;
			Ants[k].evrptour[cnt++] = DEPOT;
			int len = end_depot - start_depot + 1;
			for (int i = 0; i < len; i++){
                Ants[k].tour[i] = Ants[k].evrptour[start_depot + i];
			}
			Local_search(Ants[k], end_depot - start_depot + 1);
			int end_pos = start_depot;
			//Ants[k].complete_the_whole_path(Ants[k].solution, gen_temp, 0, len - 1, end_pos);
			Ants[k].evrptour[end_pos + 1] = 0;
			cnt = end_pos + 1;
			start_depot = end_pos;
			continue;
		}
		int next_city = choosecity(count);
		Ants[k].evrptour[cnt++] = next_city;
		Ants[k].visited[next_city] = true;

		capacity_temp -= get_customer_demand(next_city);
		energy_temp -= get_energy_consumption(from,next_city);
		n_customer ++;
	}
	// cout << "mid\n";
    int next_city = 0;
    Ants[k].evrptour[cnt++] = next_city;
	end_depot = cnt - 1;
	int len = end_depot - start_depot + 1;
	for (int i = 0; i < len; i++){
		 Ants[k].visited[i] = Ants[k].evrptour[start_depot + i];
	}
	Local_search(Ants[k], end_depot - start_depot + 1);
	int end_pos = start_depot;
	//Ants[k].complete_subgen(Ants[k].solution, gen_temp, 0, len - 1, end_pos);
	Ants[k].evrptour[end_pos++] = 0;
	cnt = end_pos;
    //Ants[k].set_steps(cnt);
    if(check_solution(Ants[k].evrptour, cnt)) {
        //Ants[k].set_fitness(fitness_evaluation(Ants[k].tour, cnt, true));
		// Ants[k].show();
		return true;
    } else {
		return false;
	}
}

void ACO::optimize () {
    for (int k = 0; k < num_of_ants; k++) {
		// cout << "start " << k << "\n";
		bool is_fessible = false;
        do {
            ants[k].steps = 0;
            one_ant_finding_route(k);
        } while (not is_fessible);
		int st_pos = 0;	
		int cnt = 0;
		ants[k].set_num_of_tours(0);
		int n_tours = 0;
		for (int i = 1; i < Ants[k].get_steps(); i++){
			if (is_charging_station(ants[k].solution[i])){
				if (Ants[k].solution[i] == DEPOT){
					Ants[k].tours[n_tours] = {st_pos, cnt - 1};
					st_pos = cnt;
					n_tours++;
				}
			} else{
				Ants[k].order[cnt++] = Ants[k].solution[i];
			}
		}	
		Ants[k].set_num_of_tours(n_tours);
		Ants[k].set_tour_index();
        if (Ants[k].get_fitness() < best_sol->tour_length){
            best_sol->tour_length = Ants[k].get_fitness();
            best_sol->steps = Ants[k].get_steps();    
            for(int j = 0; j < best_sol->steps; j++){
                best_sol->tour[j] = Ants[k].solution[j];
            }
    		// cout << "Best iter: " << best_sol->tour_length << "\n";
			t_cnt = 0;
			b_cnt = 0;
        }	else{
			b_cnt++;
			t_cnt++;
		}
		// Ants[k].show();
        // cout << " : ant " << k << " has ended!" << endl;		
    }		
	
	if (b_cnt == K_B){
		for (int i = 0; i < num_of_ants; i++)
			SA_optimizer.run(Ants[i]);
		b_cnt = 0;
	}
    updatepheromone ();

	if (t_cnt == K_T){
		perturbationpheromone();
	}

}
void ACO::complete_gen() {

    // insert depot
    int cnt = 0;
    gen_temp[0] = 0;
    for(int j = 0; j < this->num_of_tours; j++) {
        auto p = tours[j];
        for(int k = p.left; k <= p.right; k++) {
            gen_temp[++cnt] = order[k];
        }
        gen_temp[++cnt] = 0;
    }
    cnt = 0;

    // complete subtour from L . R
    static int l, r;
    for(int i = 0; i < this->num_of_tours; i++){
        auto seg = tours[i];
        l = seg.left + i;
        r = seg.right + i + 2;
        //insert charging station
        if(not complete_subgen(full_path, gen_temp, l, r, cnt)) {
            this->fitness = INF;
            return;
        }
    }
    full_path[cnt++] = 0;
    if(not check_solution(full_path, cnt)) {
        this->fitness = fitness_evaluation(full_path, cnt, false);
        // cout << this->fitness << "\n";
        add_penalty();
    } else{
        this->fitness = fitness_evaluation(full_path, cnt, true);
    }

    for (int i = 0; i < cnt; i++)
        solution[i] = full_path[i];
    
    this->steps = cnt;
}

void ACO::free(){
    int i;
    free_two_dimen_array(pheromone);
    free_two_dimen_array(deltapheromone);
}

ACO::~ACO(){
    int i;
    free_two_dimen_array(nearest);
}
