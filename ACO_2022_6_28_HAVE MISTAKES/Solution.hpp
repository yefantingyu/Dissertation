#include "Problem.hpp"

/*
Include the basic methods for working on the solutions
*/


// segment 这个代码是储存一个数据段的最左端和最右端， 比如一个route为1 3 7 9 4 8， 那么这个段表示为 (1, 8)
struct segment{
    int left, right; 
    segment(){};
    segment(int l, int r): left(l), right(r){}
}; 



class Solutioner{
public:
    Problem *problem = Problem::getInstance(); 

    //整个问题会被分为多个tours，一辆汽车由于空间不足，或者电量不足，只能行驶在某个地方就停止了，这时候要派新的车，也就是产生了新的tours
    //在数据段中我们一般是不考虑0位置，也就是不考虑起始点
    //比如 0-1-3-0-4-6-8-5-0
    // tours(1,3)   (4,5)
    segment tours[MAX_NODE];  
    /* The order of customers 
    如果是0-1-2-3-4-5-6怎么理解呢？就是order[0] =1  order[1]=2   储存的是把远点算在内，数据集中从1开始，而不是从0开始
    */
    int order[MAX_NODE];// 1 3 5 7 8 9 order


    /* Mapping customer and a tour
    index_of_customer [1]=0 说明这个储存的是所有customer， 并包含远点的index
    索引从零开始，但是customer的数量从1开始，数据集中是这么表示的
     */
    int index_of_customer[MAX_NODE];// 1 3 5 7 8 9 order
     void mutation();
    /* Mapping customer and a tour 
    // 其实就干了一件事， 比如 13 5   67    89   分别属于Route 0 1 2 那么这步干的是就是， 随便给个点 tour——index（8）=2 告诉大家这个点在哪个tour上
    */
    int tour_index[MAX_NODE];// 1 3 5 7 8 9 order

    int remaining_energy[MAX_NODE];
    int gen_temp[MAX_NODE]; //是order的加强版，把0都加上去了
    int full_path[MAX_NODE]; 
    short int _path[MAX_NODE];

    int solution[MAX_NODE];

    /* the number of steps in all tours */
    int steps = 0;

    /* The fitness of individuals */
    double fitness = INF;

    /* The number of tours
    也可以以理解为，整个问题由多少个小车解决的
     */  
    int num_of_tours = 0;
    
    /* The penalty rate for invalid solution */
    double PENALTY = 1.3;

    // 产生一个初始路径
    void init_route(std::string _type);

    /* Generate an individual */
    void setup();
    /* generate random initial indv*/
    void rand_generate();
    /* generate optimal initial indv*/
    void opt_generate();
    /* copy a other: order, tours and fitness*/
    void copy_order(Solutioner);
    /* Checking valid order of individual*/
    bool is_valid_order();
    
    /* Checking valid solution*/
    bool is_valid_solution(int *t, int size); 
    bool is_valid_solution(); // this
    
    /* Checking fully capacity, return True if invalid */
    bool check_full_capacity();
    
    /* Calculates the length of a tour */
    float calcTourLength(const std::vector<int> & tour) const;
    
    /* Returns the distance between cities i and j */
    double get_capacity_of_tour(int tour_id);

    /* Reoptimize */
    void local_search();

    /* Seting index for each chromosome */
    void set_tour_index();

    /* Insert energy station by optimal way */
    void complete_gen();

    /* Insert energy station by optimal way for each tours */
    bool complete_subgen(int* full_path, int * gen_temp, int l, int r, int &cnt);

    /* mutation method for GA */

    /* greedy method for SA */
    void greedy_1();
    void greedy_2();

    /* balanced method */
    void redistribute_customer();

    /* implement algo */
    void setting_path();
    
    /* optimize_station - a sub steps in complete_gen function */
    void optimize_station(int *full_path, int l, int r);

    /* find a nearest energy station by Dijkstra's alg from left to right */
    int nearest_station(int from, int to, double energy);

    /* find a nearest energy station by Dijkstra's alg from right to left */
    int nearest_station_back(int from, int to, double energy);

    /* Shows order */
    void show();

    /* return fitness */
    double get_fitness(){
        return this->fitness;
    }
    /* return fitness */
    void set_fitness(double f){
        this->fitness = f;
    }

    /* add penalty for false indv */
    void add_penalty(){
        this->fitness *= this-> PENALTY;
    }

    /* return the number of steps in all tours */
    int get_steps(){
        return this->steps;
    }

    /* return the number of steps in all tours */
    void set_steps(int s){
        this->steps = s;
    }

    /* return the number of tours */
    int get_num_of_tours(){
        return this->num_of_tours;
    }

    void set_num_of_tours(int n){
        this->num_of_tours = n;
    }

    int TYPE = 1;




};