#include "Random.hpp"
#include "Solution.hpp"
#include "./Clock.h"
#include <algorithm>

// const int NUMBEROFANTS = 6;
const int NUM_OF_INDVS = 200;

class SACO {
public:
	SACO (int num_of_ants, double alpha, double beta, double q, double ro, double taumax, int initCity,double alpha_SAt, double beta_SAt, double t_currentt, double t_endt);
	virtual ~SACO ();
	
	FILE *log_performancet;
    char *perf_filenamet;

	
	Problem *problemt = Problem::getInstance();

	void init (int _r);
	
	void connectCITIES (int cityi, int cityj);
	void setCITYPOSITION (int city, double x, double y);
	
	void printPHEROMONES ();
	
	void optimize ();
	void free_SACO();
    void run(Solutioner &cur_sol);

private:
	bool exists (int cityi, int cityc);
	bool vizited (int antk, int size, int c);
	double PHI (int cityi, int cityj, int *visited);
	double PHI (int cityi, int cityj, int antk, int cnt);
	
	int city (int count);
	int route (int antk);
	
	void updatePHEROMONES ();
    void perturbationPHEROMONES();
	void local_search(int * order, int size);
	
	int NUMBEROFCITIES, INITIALCITY,NUMBEROFANTS;
	double ALPHA, BETA, Q, RO, TAUMAX, K_T, K_B, t_cnt, b_cnt,  alpha_SA,  beta_SA, t_current,  t_end;
    double PERTU_RATE;
	
	double BESTLENGTH;
	int *BESTROUTE;

	int **GRAPH, **ROUTES;
	double **CITIES, **PHEROMONES, **DELTAPHEROMONES, **PROBS;
    vector<Solutioner> Ants;
	Randoms *randoms;
};

class SimulateAnnealing{
public:
    Solutioner best_solution;
    Solutioner cur_sol;

    double alpha = 1000;
    double beta = 0.333333;
    double t_current = 1.0;
    double t_cool = 0.9999;
    double t_end = 0.02;
    double t_greedy = 10.0;
    double t_v_factor = 10.0;
    double optimal_fitness = INT_MAX;
		int K_T				= 6;
	int K_B				= 3;
	int b_cnt			= 0;
	int t_cnt 			= 0;

    FILE *log_performancet;
    char *perf_filenamet;
    Problem *problemt = Problem::getInstance();

    SimulateAnnealing(double alpha_SAt, double beta_SAt, double t_currentt, double t_endt);
    ~SimulateAnnealing();
    void run();
    void init();
	void optimize();
	int route ();
	void local_search(int * order, int size);
};
class GA{
	public:
Solutioner population[5 * NUM_OF_INDVS];
    /* Ranking for each indv */
double rank[5 * NUM_OF_INDVS];
Problem *problemt = Problem::getInstance();


void init();
int choose_by_rank(double prob);
void Repopulation();
    /* crossover function, return two new indvs */
    void distribute_crossover(Solutioner parent_1, 
                              Solutioner parent_2, 
                              int idx);
      void Selection();
	  void Evolution();
	  void optimize();
	  void compute_rank(int n);
	  
};




