#include "Methods.hpp"
#include <cstdio>
#include <iostream>
#include <cstdlib>

#include <cmath>
#include <limits>
#include <climits>
#include <algorithm>
#include <cstring>
#include "Macro.hpp"

using namespace std;


SACO::SACO (int num_of_ants,double alpha, double beta, double q, double ro, double taumax,
		int initCity,double alpha_SAt, double beta_SAt, double t_currentt, double t_endt) {
	ALPHA 			= alpha;
	BETA 			= beta;
	Q 				= q;
	RO 				= ro;
	TAUMAX 			= taumax;
	K_T				= 6;
	K_B				= 3;
	b_cnt			= 0;
	t_cnt 			= 0;
	PERTU_RATE		= 0.1;
	alpha_SA = alpha_SAt;
	beta_SA = beta_SAt;
	t_current = t_currentt;
	t_end = t_endt;
	NUMBEROFANTS = num_of_ants;
	
}
SACO::~SACO () {

	for(int i=0; i< problemt->ACTUAL_PROBLEM_SIZE; i++) {
		delete [] CITIES[i];
		delete [] PHEROMONES[i];
		delete [] DELTAPHEROMONES[i];
		if(i < problemt->ACTUAL_PROBLEM_SIZE - 1) {
			delete [] PROBS[i];
		}
	}
	delete [] CITIES;
	delete [] PHEROMONES;
	delete [] DELTAPHEROMONES;
	delete [] PROBS;

	
}

void SACO::init (int _r) {
	Ants.resize(NUMBEROFANTS );
	CITIES 			= new double*[problemt->ACTUAL_PROBLEM_SIZE];
	PHEROMONES 		= new double*[problemt->ACTUAL_PROBLEM_SIZE];
	DELTAPHEROMONES = new double*[problemt->ACTUAL_PROBLEM_SIZE];
	PROBS 			= new double*[problemt->ACTUAL_PROBLEM_SIZE-1];
	
	for(int i=0; i<problemt->ACTUAL_PROBLEM_SIZE; i++) {
		CITIES[i] 			= new double[2];
		PHEROMONES[i] 		= new double[problemt->ACTUAL_PROBLEM_SIZE];
		DELTAPHEROMONES[i] 	= new double[problemt->ACTUAL_PROBLEM_SIZE];
		PROBS[i] 			= new double[2];
		for (int j=0; j<2; j++) {
			CITIES[i][j] = -1.0;
			PROBS[i][j]  = -1.0;
		}
		for (int j=0; j<problemt->ACTUAL_PROBLEM_SIZE; j++) {
			PHEROMONES[i][j] 		= 0.0;
			DELTAPHEROMONES[i][j] 	= 0.0;
		}
	}	
	randoms = new Randoms (_r);	

	for (int i=0; i<NUMBEROFANTS; i++) {
		for (int j=0; j<problemt->ACTUAL_PROBLEM_SIZE; j++) {
			Ants[i].solution[j] = 0;
		}
	}

    for (int i = 0; i < problemt->ACTUAL_PROBLEM_SIZE; i++){
        for (int j = i + 1; j < problemt->ACTUAL_PROBLEM_SIZE; j++){
            connectCITIES(i, j);
        }
    }
	problemt->compute_nearest_points();

    problemt->best_sol = new solution;
    problemt->best_sol->tour = new int[MAX_NODE];
    problemt->best_sol->id = 1;
    problemt->best_sol->steps = 0;
    problemt->best_sol->tour_length = INF;
    // compute_nearest_points();

	//  perf_filenamet = new char[CHAR_LEN];
	//  sprintf(perf_filenamet, "./Result/%s.mid.txt", problemt->instance);
	//      // for performance
    // if ((log_performancet = fopen(perf_filenamet, "a")) == NULL) {
    //     std::cout << "Failed to open stats file " << perf_filenamet << std::endl;
    //     exit(2);
    // }

}

void SACO::local_search(int * order, int size){
    static int l, r, x, y, i, j, u0, v0, u1, v1;
    static double t1, t2;
    static bool stop;
    l = 0;
    r = size - 1;
    while (true) {
        stop = true;
        for(i = l; i <= r; ++i) {
            for(j = r; j > i; --j) {
                u0 = order[i], u1 = order[i - 1];
                v0 = order[j], v1 = order[j + 1];

                if(i - 1 < l)
                    u1 = 0;
                if(j + 1 > r)
                    v1 = 0;

                t1 =problemt-> get_distance(u1, u0)+ problemt->get_distance(v0, v1);
                t2 = problemt->get_distance(u1, v0) + problemt->get_distance(u0, v1);

                if (t1 > t2) {
                    for(x = i, y = j; x <= y; ++x, --y) {
                        std::swap(order[x], order[y]);
                    }
                    stop = false;
                }

            }
        }
        if (stop) break;
    }
}

void SACO::connectCITIES (int cityi, int cityj) {
	PHEROMONES[cityi][cityj] = randoms -> Uniforme() * TAUMAX;
	PHEROMONES[cityj][cityi] = PHEROMONES[cityi][cityj];
}
void SACO::setCITYPOSITION (int city, double x, double y) {
	CITIES[city][0] = x;
	CITIES[city][1] = y;
}
void SACO::printPHEROMONES () {	
	cout << " PHEROMONES: " << endl;
	cout << "  | ";
	for (int i=0; i<problemt->ACTUAL_PROBLEM_SIZE; i++) {
		printf("%5d   ", i);
	}
	cout << endl << "- | ";
	for (int i=0; i<problemt->ACTUAL_PROBLEM_SIZE; i++) {
		cout << "--------";
	}
	cout << endl;
	for (int i=0; i<problemt->ACTUAL_PROBLEM_SIZE; i++) {
		cout << i << " | ";
		for (int j=0; j<problemt->ACTUAL_PROBLEM_SIZE; j++) {
			if (i == j) {
				printf ("%5s   ", "x");
				continue;
			}
			printf ("%7.3f ", PHEROMONES[i][j]);
		}
		cout << endl;
	}
	cout << endl;
}

// bool SACO::visited (int antk, int size,  int c) {
// 	if (c == DEPOT or is_charging_station(c)) return false;
// 	for (int l=0; l<size; l++) {
// 		if (Ants[antk].solution[l] == -1) {
// 			break;
// 		}
// 		if (Ants[antk].solution[l] == c) {
// 			return true;
// 		}
// 	}
// 	return false;
// }

double SACO::PHI (int cityi, int cityj, int *visited) {
	double ETAij = (double) pow (1 / problemt->get_distance(cityi, cityj), BETA);
	double TAUij = (double) pow (PHEROMONES[cityi][cityj],   ALPHA);
	
	double sum = 1e-10;
	for (int c=1; c<=problemt->NUM_OF_CUSTOMERS; c++) {
        if (!visited[c]) {
            double ETA = (double) pow (1 / problemt->get_distance (cityi, c), BETA);
            double TAU = (double) pow (PHEROMONES[cityi][c],   ALPHA);
            sum += ETA * TAU;
        }	
	}
	return (ETAij * TAUij) / sum;
}


int SACO::city (int count) {
	double xi = randoms -> Uniforme();
	int i = 0;
	double total = 0;
	for (int i = 0; i < count; i++){
		total += PROBS[i][0];
	}
	xi *= total;
	double sum = PROBS[i][0];
	while (sum < xi) {
		i++;
		sum += PROBS[i][0];
	}
	return (int) PROBS[i][1];
}

int SACO::route (int antk) {
	Ants[antk].solution[0] = 0;
	static int gen_temp[MAX_NODE];
	static int visited[MAX_NODE];
	memset(visited, 0, sizeof(visited));
    int cnt = 1;
    static int from, to;
    static double energy_temp, capacity_temp;
    energy_temp = problemt->BATTERY_CAPACITY;
    capacity_temp = problemt->MAX_CAPACITY;
    int start_depot = 0, end_depot = 0;
	int n_customer = 0;
	// cout << "st\n";
	while (n_customer < problemt->NUM_OF_CUSTOMERS){
		from = Ants[antk].solution[cnt - 1];
		//  cout << "from: " << from << " cnt: " << cnt << "\n";
		int count = 0;
		for (to = 1; to <= problemt->NUM_OF_CUSTOMERS; to++) {
			if (from == to) 
				continue;	

            // cout << to << " mid1\n";
            if (capacity_temp < problemt->get_customer_demand(to)){
                continue;
            }
			
			if (!visited[to]) {
				PROBS[count][0] = PHI (from, to, visited);
				PROBS[count][1] = (double) to;
				count++;
			}
		}
		// deadlock
		if (0 == count) {
			// cout << "dead\n";
			energy_temp = problemt->BATTERY_CAPACITY;
			capacity_temp = problemt->MAX_CAPACITY;
			end_depot = cnt;
			Ants[antk].solution[cnt++] = problemt->DEPOT;
			int len = end_depot - start_depot + 1;
			for (int i = 0; i < len; i++){
				gen_temp[i] = Ants[antk].solution[start_depot + i];
			}
			local_search(gen_temp, end_depot - start_depot + 1);
			int end_pos = start_depot;
			Ants[antk].complete_subgen(Ants[antk].solution, gen_temp, 0, len - 1, end_pos);
			Ants[antk].solution[end_pos + 1] = 0;
			cnt = end_pos + 1;
			start_depot = end_pos;
			continue;
		}
		int next_city = city(count);
		Ants[antk].solution[cnt++] = next_city;
		visited[next_city] = 1;

		capacity_temp -= problemt->get_customer_demand(next_city);
		energy_temp -= problemt->get_energy_consumption(from,next_city);
		n_customer ++;
	}
	// cout << "mid\n";
    int next_city = 0;
    Ants[antk].solution[cnt++] = next_city;
	end_depot = cnt - 1;
	int len = end_depot - start_depot + 1;
	for (int i = 0; i < len; i++){
		gen_temp[i] = Ants[antk].solution[start_depot + i];
	}
	local_search(gen_temp, end_depot - start_depot + 1);
	int end_pos = start_depot;
	Ants[antk].complete_subgen(Ants[antk].solution, gen_temp, 0, len - 1, end_pos);
	Ants[antk].solution[end_pos++] = 0;
	cnt = end_pos;
    Ants[antk].set_steps(cnt);
	// cout << "end\n";
    if(problemt->check_solution(Ants[antk].solution, cnt)) {
        Ants[antk].set_fitness(problemt->fitness_evaluation(Ants[antk].solution, cnt, true));
		// Ants[antk].show();
		return true;
    } else {
		return false;
	}
}


void SACO::updatePHEROMONES () {
    sort(Ants.begin(), Ants.end(), [](Solutioner x, Solutioner y){
        return x.get_fitness() < y.get_fitness();
    });
    
	for (int k=0; k<NUMBEROFANTS - 1; k++) {
		double rlength = Ants[k].get_fitness();
		for (int r=0; r< Ants[k].get_steps()-1; r++) {
			int cityi = Ants[k].solution[r];
			int cityj = Ants[k].solution[r+1];
			DELTAPHEROMONES[cityi][cityj] += (NUMBEROFANTS - k) / rlength;
			DELTAPHEROMONES[cityj][cityi] += (NUMBEROFANTS - k) / rlength;
		}
	}

    for (int j = 0; j < problemt->best_sol->steps - 1; j++){
			int cityi = problemt->best_sol->tour[j];
			int cityj = problemt->best_sol->tour[j + 1];
			DELTAPHEROMONES[cityi][cityj] += NUMBEROFANTS / problemt->best_sol->tour_length;
			DELTAPHEROMONES[cityj][cityi] += NUMBEROFANTS / problemt->best_sol->tour_length;
    }


	for (int i=0; i<problemt->ACTUAL_PROBLEM_SIZE; i++) {
		for (int j=0; j<problemt->ACTUAL_PROBLEM_SIZE; j++) {
			PHEROMONES[i][j] = (1 - RO) * PHEROMONES[i][j] + RO * DELTAPHEROMONES[i][j];
			DELTAPHEROMONES[i][j] = 0.0;
		}	
	}
}


void SACO::perturbationPHEROMONES () {
	double sum = 0;

	for (int i=0; i<problemt->ACTUAL_PROBLEM_SIZE; i++) {
		for (int j=0; j<problemt->ACTUAL_PROBLEM_SIZE; j++) {
			sum += PHEROMONES[i][j];
		}	
	}
	double mean = sum / (problemt->ACTUAL_PROBLEM_SIZE * problemt->ACTUAL_PROBLEM_SIZE);
	// cout << sum << "\n";
	for (int i=0; i<problemt->ACTUAL_PROBLEM_SIZE; i++) {
		for (int j=0; j<problemt->ACTUAL_PROBLEM_SIZE; j++) {
			PHEROMONES[i][j] = PHEROMONES[i][j] * (1 - PERTU_RATE) + (PERTU_RATE) * mean;
		}	
	}
}



void SACO::optimize () {
    for (int k = 0; k < NUMBEROFANTS; k++) {
		// cout << "start " << k << "\n";
		bool is_fessible = false;
        do {
            Ants[k].set_steps(0);
			// cout << "mid\n";
            is_fessible = route(k);
            // Ants[k].show();
			// cout << (is_fessible?"Fessible":"Not Fessible") << "\n";
        } while (not is_fessible);
		// cout << "Done\n";
		// if (not check_solution(Ants[k].solution, Ants[k].get_steps())){
		// 	cout << "Warning!\n";
		// }
		int st_pos = 0;	
		int cnt = 0;
		Ants[k].set_num_of_tours(0);
		int n_tours = 0;
		for (int i = 1; i < Ants[k].get_steps(); i++){
			if (problemt->is_charging_station(Ants[k].solution[i])){
				if (Ants[k].solution[i] == problemt->DEPOT){
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
        if (Ants[k].get_fitness() < problemt->best_sol->tour_length){
            problemt->best_sol->tour_length = Ants[k].get_fitness();
            problemt->best_sol->steps = Ants[k].get_steps();    
            for(int j = 0; j < problemt->best_sol->steps; j++){
                problemt->best_sol->tour[j] = Ants[k].solution[j];
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
		for (int i = 0; i < NUMBEROFANTS; i++)
			run(Ants[i]);
		b_cnt = 0;
	}
    // cout << endl << "updating PHEROMONES . . .";
    updatePHEROMONES ();

	if (t_cnt == K_T){
		perturbationPHEROMONES();
	}
        // Ants[0].show();	
    // cout << " done!" << endl << endl;
    // printPHEROMONES ();

}
void  SACO::run(Solutioner &cur_sol) {
    Clock timer;
    Solutioner new_solution;
    double t_cool = 0.9999;
    double t_greedy = 10.0;
    double alpha = 1000;
    double beta = 0.333333;

    double improve;

    // cout << "initial fitness: " << cur_sol.get_fitness() << "\n";

    // int t_v = (int) (ACTUAL_PROBLEM_SIZE * this->t_v_factor);
    int cnt_div = 0;
    
    int G = 0;
	int stop=0;
    
    double sqrt_n = log10(problemt-> ACTUAL_PROBLEM_SIZE);
    vector<double> conv;

    while (problemt->evals < problemt->TERMINATION && t_current > t_end){
        t_greedy = problemt->ACTUAL_PROBLEM_SIZE * beta;
        // double prob = (double)rand() / RAND_MAX;
        t_cool = (alpha * sqrt_n - 1.0) / (alpha * sqrt_n);
        do {
            conv.push_back(cur_sol.get_fitness());
            new_solution.copy_order(cur_sol);
            
            double rand_t = (double) rand() / (double) RAND_MAX;
            if(rand_t <= 0.5){
                new_solution.greedy_1();
            } else {
                new_solution.greedy_2();
            }
            new_solution.setup();
            improve = cur_sol.get_fitness() - new_solution.get_fitness();
            G++;

            if (improve > 0)
                break;

            if(new_solution.get_fitness() + 1e10 > INF) {
                continue;
            }

            /* Termination */
            if (G >= t_greedy) {
                double upper = abs(new_solution.get_fitness() - cur_sol.get_fitness() ) / 
                    abs(new_solution.get_fitness() - problemt->best_sol->tour_length + 1e-5);
                // double rho = exp(upper) * t_current;
                double accept_prob = exp(- upper / t_current);
                // cout << upper << " " << cur_sol.get_fitness() << " " << new_solution.get_fitness() << " "
                    // << best_sol->tour_length << " prob: " << accept_prob << " " << t_current << "\n";
                // getchar();
                double r = ((double) rand() / (RAND_MAX));
                if(accept_prob > r){
                    cur_sol.copy_order(new_solution);
                }
                
                /* Compulsive Accept */
                cnt_div ++;
                break;
            }
            
        } while (improve < 0 && problemt->evals < problemt->TERMINATION );
    
        cur_sol.copy_order(new_solution);
        if(new_solution.is_valid_solution()) {
            if(problemt->best_sol->tour_length > cur_sol.get_fitness()){
                problemt->best_sol->tour_length = cur_sol.get_fitness();
                problemt->best_sol->steps = cur_sol.get_steps();    
                for(int j = 0; j < problemt->best_sol->steps; j++){
                    problemt->best_sol->tour[j] = cur_sol.solution[j];
                }

                
            }
        } 
		//fprintf(log_performancet,"Iteration %d ",stop);
		//fprintf(log_performancet,"Result %f ",problemt->best_sol->tour_length);
		//fprintf(log_performancet,"\n");
		stop++;
        t_current *= t_cool;
        t_current = max(t_current, t_end);

        G = 0;
    }//end while
    //problemt->save_conv(conv, problemt->instance);
}

void SACO::free_SACO(){
    delete[] problemt->best_sol->tour;
    delete problemt->best_sol;
	fclose(log_performancet);
}
