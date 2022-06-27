#include "Random.hpp"
#include "Solution.hpp"
#include "./Clock.h"
#include <algorithm>

const int NUMBEROFANTS = 8;
const int NUM_OF_INDVS = 200;

class SACO {
public:
	SACO (double alpha, double beta, double q, double ro, double taumax, int initCity);
	virtual ~SACO ();
	
	FILE *log_performancet;
    char *perf_filenamet;

	Solutioner Ants[NUMBEROFANTS];
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
	
	int NUMBEROFCITIES, INITIALCITY;
	double ALPHA, BETA, Q, RO, TAUMAX, K_T, K_B, t_cnt, b_cnt;
    double PERTU_RATE;
	
	double BESTLENGTH;
	int *BESTROUTE;

	int **GRAPH, **ROUTES;
	double **CITIES, **PHEROMONES, **DELTAPHEROMONES, **PROBS;

	Randoms *randoms;
};




