#include "Permutation.hpp"
#include <assert.h>
void Permutation::initgraph(){
    next = vector<vector<int>>(graph_size, vector<int>(graph_size, -1));
    dist = vector<vector<int>>(graph_size, vector<int>(graph_size, INF));
    for (int i = 0; i < graph_size; i++) {
        for (int j = i; j < graph_size; j++) {
            if (i == j) {
                dist[i][j] = 0;
                next[i][j] = j;
            } else {
                int start = problem->AFs[i];
                int goal =problem->AFs[j];
                double consumption = problem->get_energy_consumption(start, goal);
                problem->get_distance(start, goal); // Called here just to increase evals, so that the usage is 100% legal
                if (consumption <= problem->BATTERY_CAPACITY) {
                    dist[i][j] = consumption;
                    dist[j][i] = consumption;
                    next[i][j] = j;
                    next[j][i] = i;
                }
            }
        }
    }
    for (int k = 0; k < graph_size; k++) {
        for (int i = 0; i < graph_size; i++) {
            for (int j = 0; j < graph_size; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }
}
vector<int> Permutation::getgraph(int u, int v, bool afsIds) {
    vector<int> path;
    if (next[u][v] == -1) {
        return path;
    }
    path.push_back(u);
    while (u != v) {
        u = next[u][v];
        path.push_back(u);
    }
    if (afsIds) {
        for (auto &n:path) {
            n = problem->AFs[n];
        }
    }

    return path;
}


bool Permutation::route_checker(int node, bool reset){
    // check 4 elements
    bool signal = false;
    if(reset){
        checker_counter = 0;
        memset(checker_4_array,-1,4);
    }
    // check 4 elements
    checker_counter = checker_counter + 1;
    if(checker_counter<=4){
        signal = true;
    }
    else{
        if((checker_4_array[0]!=checker_4_array[2])||(checker_4_array[1]!=checker_4_array[3])){signal = true;}
    }

    if(signal){
        checker_4_array[checker_Last] = node;
        checker_Last = (checker_Last+1) % 4;
        return true;
    }
    else{
        return false;
    }
}

bool Permutation::pass_afs_to_depot(vector<int> & tour){
    double remain_battery = problem->getRemainingBattery(tour) - problem->get_energy_consumption(tour.back(), 0);
    if(remain_battery >= 0){
        tour.push_back(0); // if can back, directly back
    }
    else{ // if cannot back, should pass the battery, then customer then ... back   准确的说就是不能直接到，就干点别的事
       int closestAFS = problem->getClosestAFS(tour.back());
       int closestAFSid = problem->afsIdMap[closestAFS];
       vector<int> path = getgraph(closestAFSid, problem->afsIdMap[0]);
       tour.insert(tour.begin(), path.begin(),path.end());
    }
    return route_checker(0);
}

/*
因为之前产生的事没有充电站的单纯的TSP问题，现在要把充电站加进来

*/
vector<int> Permutation:: Tour_generation(vector<int> tsp){
    vector<int> evrp;
    evrp.push_back(0);
    // int nextid = 0;  // 下一个要去的customer， tsp上的点
    for(int nextid=0; nextid<tsp.size(); nextid++){ //把tsp中每一个点都考虑进来
        double load_value = problem->getRemainingLoad(tsp) < problem->get_customer_demand(tsp[nextid]);
        if(load_value>0){ // 既然还能往下走，就看看电够不够到达重点  
            if(!pass_afs_to_depot(evrp)){ // 如果不能， 说明没什么可以参考的价值，如果能， 这个路线就成功的插入了
                break;
            }
        }
        else{
            int closestAFSToGoal = problem->getClosestAFS(tsp[nextid]);
            double remainingBattery = problem->getRemainingBattery(evrp);
            double energyToNext = problem->get_energy_consumption(evrp.back(), tsp[nextid]);
            double nextToAFS = problem->get_energy_consumption(tsp[nextid], closestAFSToGoal);
            if (remainingBattery - energyToNext >= nextToAFS) {
                evrp.push_back(tsp[nextid]);
                nextid++;
                if (!route_checker(nextid - 1)) break;
                // (3) Can return to the closest AFS via the next customer? -> NO
            } else {
                // (4) Can reach nearest AFS? -> NO
                // NOTE: this can be optimized, too...
                int closestAFS = problem->getReachableAFSClosestToGoal(evrp.back(), tsp[nextid], problem->getRemainingBattery(evrp));
                bool canReach = problem->getRemainingBattery(evrp) - problem->get_energy_consumption(evrp.back(), closestAFS) >= 0;
                assert(canReach);
                // (4) Can reach nearest AFS? -> YES
                evrp.push_back(closestAFS);
                if (!route_checker(closestAFS)) break;
            }
        }
    }
    pass_afs_to_depot(evrp);
    return evrp;
}

void Permutation::generalizedDoubleBridge(vector<int> &tour, unsigned p){
    auto n = tour.size();
    p = (p + 1 > n) ? n - 1 : p;
    default_random_engine generator(rand());
    auto distribution = uniform_int_distribution<int>(0,n-2);
    std::vector<bool> throwsBool(n, false);
        for (unsigned i = 0; i < p; ++i) {
        int throwI;
        do {
            throwI = distribution(generator);
        } while (throwsBool[throwI]);
        throwsBool[throwI] = true;
    }


    



    std::vector<std::vector<int>> pathStrings{}; // these are indices
    for (unsigned i = 0; i < n; ++i) {
        std::vector<int> pathString{};
        while (!throwsBool[i] && i < n - 1) {
            pathString.emplace_back(i++);
        }
        pathString.emplace_back(i);
        pathStrings.push_back(pathString);
    }
    std::vector<int> newPositions(pathStrings[0]);
    std::vector<int> roulette = std::vector<int>(pathStrings.size());
    for (unsigned i = 0; i < roulette.size(); ++i) roulette[i] = i;
    std::shuffle(roulette.begin() + 1, roulette.end(), generator);
        /// Connect the strings to get the new path.
    for (unsigned k = 1; k < pathStrings.size(); ++k) {

        /// Pick a next string to be connected to the new path.
        auto &nextString = pathStrings[roulette[k]];

        /// Flip a coin.
        auto side = uniform_int_distribution<unsigned>(0, 1)(generator);
        if (side == 0) {

            /// If side == 0, then connect the next string in ascending order.
            for (int i : nextString) {
                newPositions.push_back(i);
            }

        } else {

            /// If side == 1, then connect the next string in descending order.
            for (unsigned i = 0; i < nextString.size(); ++i) {
                newPositions.push_back(nextString[nextString.size() - i - 1]);
            }
        }
    }
        /// Now get the path's vertices.
    std::vector<int> newPath(n);
    for (unsigned j = 0; j < n; ++j) {
        newPath[j] = tour[newPositions[j]];
    }
    // Fix the perturbed tour
    newPath = Tour_generation(newPath);
    mergeAFSs(newPath);
    tour = newPath;
}
void Permutation::mergeAFSs(vector<int> &tour) {
    int i = 0;
    while (i != tour.size() - 1) {
        if (tour[i] == tour[i+1]) { // remove element at index i+1
            tour.erase(tour.begin() + i + 1);
        } else {
            i++;
        }
    }
}
