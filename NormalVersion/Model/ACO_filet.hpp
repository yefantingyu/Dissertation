// #ifndef ACO_HPP
// #define ACO_HPP
// #include "./Solution.hpp"
// #include "Random.hpp"
// int num_ants = 1.5* NUM_OF_VEHICLES;

// /*
// 1. 蚂蚁数 m ： 1.5*vehicels
// 2. 信息素因子 alpha
// 3. 启发函数因子beta 
// 4. 信息素挥发因子ro
// 5. 信息素常数 Q
// 6. 信息素上界  上边界 taumax

// */
// class SACO{
// public:
//     SACO(double alpha, double beta, double q, double ro, double taumax, int initCity);
//     virtual ~SACO();

//     Solutioner *Ants;

//     void init(int run);
//     void init_pheromones(int cityi, int cityj);
//     void optimizer();
//     int route (int antk);
//     double PHI (int cityi, int cityj, int *visited);



// private:
//     double alpha, beta, q, ro, taumax, k_t, k_b, t_cnt, b_cnt, pertu_rate;
//     int num_cities, init_city;  //城市的数目，以及最开始的城市

//     double **cities;  // 一共有actual problem size个城市
//     double **pheromones;// 信息素就需要放在每个城市都放一遍
//     double **deltapheromones; 
//     double **probs;// problme size是不包含初始的0位置的

//     Randoms *randoms;// 随机数类

   

// };

// SACO::SACO(double alpha, double beta, double q, double ro, double taumax, int initCity){
// 	alpha 			= alpha;
// 	beta 			= beta;
// 	q 				= q;
// 	ro 				= ro;
// 	taumax 			= taumax;
// 	k_t				= 6;
// 	k_b				= 3;
// 	b_cnt			= 0;
// 	t_cnt 			= 0;
// 	pertu_rate		= 0.1;
//     Ants = new Solutioner[num_ants];
// }
// SACO::~SACO () {
// 	for(int i=0; i<ACTUAL_PROBLEM_SIZE; i++) {
// 		delete [] cities[i];
// 		delete [] pheromones[i];
// 		delete [] deltapheromones[i];
// 		if(i < ACTUAL_PROBLEM_SIZE - 1) {
// 			delete [] probs[i];
// 		}
// 	}
// 	delete [] cities;
// 	delete [] pheromones;
// 	delete [] deltapheromones;
// 	delete [] probs;
//     delete [] Ants;
// }
// void SACO::init(int run){

//     randoms = new Randoms (run); // 产生随机数类	


//     cities 	= new double*[ACTUAL_PROBLEM_SIZE];
//     pheromones 		= new double*[ACTUAL_PROBLEM_SIZE];
// 	deltapheromones = new double*[ACTUAL_PROBLEM_SIZE];
// 	probs 			= new double*[ACTUAL_PROBLEM_SIZE-1];

//     for(int i=0; i<ACTUAL_PROBLEM_SIZE; i++) {
//         // probs和cities都是二维数组，表示具体位置应该是
// 		cities[i] 			= new double[2];
//         probs[i] 			= new double[2];
//         // 长度为actual problem size， 可能村的是到各个位置路线的信息素，1对n的关系
// 		pheromones[i] 		= new double[ACTUAL_PROBLEM_SIZE];
// 		deltapheromones[i] 	= new double[ACTUAL_PROBLEM_SIZE];
		
// 		for (int j=0; j<2; j++) {// 初始各个位置都设置为-1
// 			cities[i][j] = -1.0;
// 			probs[i][j]  = -1.0;
// 		}
// 		for (int j=0; j<ACTUAL_PROBLEM_SIZE; j++) {  //初始的信息素浓度设置为0
// 			pheromones[i][j] 		= 0.0;
// 			deltapheromones[i][j] 	= 0.0;
// 		}
// 	}
//     for (int i=0; i<num_ants; i++) { // 把每个ant产生的结果都赋成0
// 		for (int j=0; j<ACTUAL_PROBLEM_SIZE; j++) {
// 			Ants[i].solution[j] = 0;
// 		}
// 	}

//     for (int i = 0; i < ACTUAL_PROBLEM_SIZE; i++){
//         for (int j = i + 1; j < ACTUAL_PROBLEM_SIZE; j++){
//             init_pheromones(i,j); // 
//         }
//     }


//     compute_nearest_points();
//     best_sol = new solution;
//     best_sol->tour = new int[MAX_NODE];
//     best_sol->id = 1;
//     best_sol->steps = 0;
//     best_sol->tour_length = INF;

// }
// void SACO::init_pheromones (int cityi, int cityj) {
// 	pheromones[cityi][cityj] = randoms -> Uniforme() * taumax; //初始化任意两个城市间的信息素， 0，1之间的随机数乘以信息素上界
// 	pheromones[cityj][cityi] = pheromones[cityi][cityj];
// }
// double SACO::PHI (int cityi, int cityj, int *visited) {// 轮盘选法，看看到哪个城市, 计算的是每个城市的P_{ij}k
// 	double ETAij = (double) pow (1 / get_distance(cityi, cityj), beta);
// 	double TAUij = (double) pow (pheromones[cityi][cityj],   alpha);
	
// 	double sum = 1e-10;
// 	for (int c=1; c<=NUM_OF_CUSTOMERS; c++) {
//         if (!visited[c]) {
//             double ETA = (double) pow (1 / get_distance (cityi, c), beta);
//             double TAU = (double) pow (pheromones[cityi][c],   alpha);
//             sum += ETA * TAU;
//         }	
// 	}
// 	return (ETAij * TAUij) / sum;
// }



// // 计算第k只蚂蚁的路程
// int SACO::route (int antk) {
// 	Ants[antk].solution[0] = 0;// 第k只蚂蚁从0开始的solution为0
// 	static int gen_temp[MAX_NODE];
// 	static int visited[MAX_NODE];
// 	memset(visited, 0, sizeof(visited)); // visited 全变成0
//     int cnt = 1;
//     static int from, to;
//     static double energy_temp, capacity_temp;
//     energy_temp = BATTERY_CAPACITY;
//     capacity_temp = MAX_CAPACITY;
//     int start_depot = 0, end_depot = 0;
// 	int n_customer = 0;
// 	while (n_customer < NUM_OF_CUSTOMERS){
// 		from = Ants[antk].solution[cnt - 1];
// 		int count = 0;
// 		for (to = 1; to <= NUM_OF_CUSTOMERS; to++) { //从from开始， 看看下一个应该到哪里去,要是货物入量不足了，就不能运货了
// 			if (from == to) 
// 				continue;	

//             if (capacity_temp < get_customer_demand(to)){// 如果这个蚂蚁走到的位置中拉货的数量满足不了了，这躺就不行了
//                 continue;
//             }
			
// 			if (!visited[to]) {// 若这个点没有被运到过------
// 				probs[count][0] = PHI (from, to, visited);//probs是一个概率举证，0位置放概率，一位置放到哪个地方
// 				probs[count][1] = (double) to;
// 				count++;
// 			}
//             //总的来说， 干了2件事情，首先，构建了prob矩阵，probs， 记录了从一个点到任一点的概率
// 		}
// 		// deadlock
// 		if (0 == count) {// 如果是从起点开始
// 			// cout << "dead\n";
// 			energy_temp = BATTERY_CAPACITY;
// 			capacity_temp = MAX_CAPACITY;
// 			end_depot = cnt;
// 			Ants[antk].solution[cnt++] = DEPOT;
// 			int len = end_depot - start_depot + 1;
// 			for (int i = 0; i < len; i++){
// 				gen_temp[i] = Ants[antk].solution[start_depot + i];
// 			}
// 			local_search(gen_temp, end_depot - start_depot + 1);
// 			int end_pos = start_depot;
// 			Ants[antk].complete_subgen(Ants[antk].solution, gen_temp, 0, len - 1, end_pos);
// 			Ants[antk].solution[end_pos + 1] = 0;
// 			cnt = end_pos + 1;
// 			start_depot = end_pos;
// 			continue;
// 		}
// 		int next_city = city(count);
// 		Ants[antk].solution[cnt++] = next_city;
// 		visited[next_city] = 1;

// 		capacity_temp -= get_customer_demand(next_city);
// 		energy_temp -= get_energy_consumption(from,next_city);
// 		n_customer ++;
// 	}
// 	// cout << "mid\n";
//     int next_city = 0;
//     Ants[antk].solution[cnt++] = next_city;
// 	end_depot = cnt - 1;
// 	int len = end_depot - start_depot + 1;
// 	for (int i = 0; i < len; i++){
// 		gen_temp[i] = Ants[antk].solution[start_depot + i];
// 	}
// 	local_search(gen_temp, end_depot - start_depot + 1);
// 	int end_pos = start_depot;
// 	Ants[antk].complete_subgen(Ants[antk].solution, gen_temp, 0, len - 1, end_pos);
// 	Ants[antk].solution[end_pos++] = 0;
// 	cnt = end_pos;
//     Ants[antk].set_steps(cnt);
// 	// cout << "end\n";
//     if(check_solution(Ants[antk].solution, cnt)) {
//         Ants[antk].set_fitness(fitness_evaluation(Ants[antk].solution, cnt, true));
// 		// Ants[antk].show();
// 		return true;
//     } else {
// 		return false;
// 	}
// }


// #endif // FILE_OPERATION_HPP