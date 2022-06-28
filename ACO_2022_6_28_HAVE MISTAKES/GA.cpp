#include "Methods.hpp"
GA::GA(int number, double p){
    NUM_OF_INDVS = number;
    Pr_mutate = p;


}
void GA::init(){
    population.resize(3 * NUM_OF_INDVS);
    rank.resize(3 * NUM_OF_INDVS);
    problemt->best_sol = new solution;
    problemt->best_sol->tour = new int[MAX_NODE];
    problemt->best_sol->id = 1;
    problemt->best_sol->steps = 0;
    problemt->best_sol->tour_length = INF;
    problemt->compute_nearest_points();

    for(int i = 0; i < NUM_OF_INDVS; i++) {
        population[i].init_route("optimal");
        if (population[i].get_fitness() < problemt-> best_sol->tour_length){
             problemt->best_sol->tour_length = population[i].get_fitness();
             problemt->best_sol->steps = population[i].get_steps();    
            for(int j = 0; j <  problemt->best_sol->steps; j++){
                 problemt->best_sol->tour[j] = population[i].solution[j];
            }
        }
    }
}
void GA::optimize(){
    Repopulation();
    Selection();
}
void GA::compute_rank(int n){
    double sum = 0;
    double fit_min = INF;
    double fit_max = 0;
    for(int i = 0; i < n; i++){
        fit_min = min(fit_min, population[i].get_fitness());
        fit_max = max(fit_max, population[i].get_fitness());
    }
    for(int i = 0; i < n; i++){
        double temp_fit = pow((fit_max - population[i].get_fitness()) / (fit_max - fit_min + 1e-6), 2);
        sum += temp_fit;
        rank[i] = temp_fit;
    }
    for(int i = 0; i < n; i++){
        rank[i] /= sum;
        if(i > 0)
            rank[i] += rank[i - 1];
    }
}
void GA::Repopulation() {
    compute_rank(NUM_OF_INDVS);
    for(int i = 0; i < 2 * NUM_OF_INDVS; i += 2) {
        // choose the two parents
        double p1 = (double) rand() / (double) RAND_MAX;
        double p2 = (double) rand() / (double) RAND_MAX;
        int idx_1 = choose_by_rank(p1);
        int idx_2 = choose_by_rank(p2);
        while(p1 == p2)
            p2 = (double) rand() / (double) RAND_MAX;
        distribute_crossover(population[idx_1], population[idx_2], NUM_OF_INDVS + i);
    }
    int cnt = 0;
    for (int i = NUM_OF_INDVS; i < 3 * NUM_OF_INDVS; i++){
        if( population[i].get_steps() == 0){
            cnt++;
        }
        if (population[i].get_fitness() < problemt->best_sol->tour_length){
            problemt->best_sol->tour_length = population[i].get_fitness();
            problemt->best_sol->steps = population[i].get_steps();    
            for(int j = 0; j < problemt->best_sol->steps; j++){
                problemt->best_sol->tour[j] = population[i].solution[j];
            }
        }
    }
}
void GA::distribute_crossover(Solutioner parent_1, Solutioner parent_2, int idx) {

    int num = rand()%(problemt->NUM_OF_CUSTOMERS) + 1;// so ngau nhien tu 1 den size of customers
    int id1 = parent_1.tour_index[num];// customer 'num' of id1 tour in parent_1
    int id2 = parent_2.tour_index[num];// customer 'num' of id2 tour in parent_2
    int have[problemt->NUM_OF_CUSTOMERS + 1];
    int alens[problemt->NUM_OF_CUSTOMERS + 1];

    Solutioner child1;
    Solutioner child2;
    child1.copy_order(parent_1);
    child2.copy_order(parent_2);

    for(int i = 0; i <= problemt->NUM_OF_CUSTOMERS; i++){
        have[i] = 0;// have : exists in the alens or not
        alens[i] = 0;
    }

    int index = 0;

    // merge tour_id1, tour_id2 into alens
    for(int i = parent_1.tours[id1].right; i >= parent_1.tours[id1].left; i--) {
        alens[index++] = parent_1.order[i];
        have[parent_1.order[i]] = 1;
    }

    for(int i = parent_2.tours[id2].right; i >= parent_2.tours[id2].left; i--) {
        if(!have[parent_2.order[i]]) {
            alens[index++] = parent_2.order[i];
            have[parent_2.order[i]] = 1;
        }
    }
    int index2 = 0; // size of alens

    // Distribute alens to both of child1 and child2

    for(int i = 0; i < problemt->NUM_OF_CUSTOMERS; i++){
        if(have[child1.order[i]]){
            child1.order[i] = alens[--index];
        }
        if(have[child2.order[i]]){
            child2.order[i] = alens[index2++];
        }
    }
    child1.mutation();
    child2.mutation();
    population[idx].copy_order(child1);
   population[idx + 1].copy_order(child2);
    population[idx].setup();
    population[idx + 1].setup();
}


void GA::Selection() {

    sort(population.begin(), population.end(), [](Solutioner x, Solutioner y) {
            return x.get_fitness() < y.get_fitness();
    });

    compute_rank(2 * NUM_OF_INDVS);

    for (int i = 0; i < NUM_OF_INDVS; i++){
        double prob = (double) rand() / (double) RAND_MAX;
        int idx = choose_by_rank(prob);
        population[2 * NUM_OF_INDVS + i].copy_order(population[idx]);
    }
    
    for (int i = 0; i < NUM_OF_INDVS; i++){
        population[i].copy_order(population[NUM_OF_INDVS * 2 + i]);
    }
    
}
int  GA::choose_by_rank(double prob){
    return (int) (upper_bound(rank.begin(), rank.begin() + NUM_OF_INDVS, prob) - rank.begin());
}


