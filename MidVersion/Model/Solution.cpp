#include "Solution.hpp"
#include<vector>
#include <assert.h>
using namespace std;

// 产生一个初始的路径，有两种方法，optimal generation or random generation， 默认是random
void Solutioner::init_route(string _type = "random"){
    this->num_of_tours = 0;// 刚开始的时候，路径肯定为0，因为还没开始干事情呢
    if (_type == "optimal")
        opt_generate();
    else
        rand_generate();
    setup();
}

// 其实就干了一件事， 比如 13 5   67    89   分别属于Route 0 1 2 那么这步干的是就是， 随便给个点 tour——index（8）=2 告诉大家这个点在哪个tour上
void Solutioner::set_tour_index() {
    for(int j = 0, k; j < this->num_of_tours; j++) {// 其实就
        for(k = tours[j].left; k <= tours[j].right; k++) {
            tour_index[order[k]] = j;
        }
    }
}

void Solutioner::setup(){
    set_tour_index();
    this->fitness = INF;
    local_search();
    complete_gen();
}


void Solutioner::rand_generate(){
    /* order[0]=1, order[1]=2  这部把所有的初始按照顺序联系起来
    // index_of_customer[1]=0, index_of_customer[2]=1
    // 0-22  不管怎么样，repo和所有的customer都考虑到了
    order[1]: repo 原点
    */
    for(int i = 0; i < NUM_OF_CUSTOMERS; i++) {  
        order[i] = i + 1;
        index_of_customer[i + 1] = i;
    }

    // 把整个东西进行随机的置换，生成所及的一列数据0-num——of——customer随机打乱
    for(int i = 0; i < NUM_OF_CUSTOMERS; i++) {
        int idx_1, idx_2;
        idx_1 = rand() % NUM_OF_CUSTOMERS;
        idx_2 = rand() % NUM_OF_CUSTOMERS;
        swap(order[idx_1], order[idx_2]);
        swap(index_of_customer[order[idx_1]], index_of_customer[order[idx_2]]);
    }
    double capacity = 0;
    int idx = 0;

    for(int i = 0; i <= NUM_OF_CUSTOMERS; i++) {
        int customer = order[i];
        double demand = get_customer_demand(customer); // 获取这个customer的需求
        if (capacity + demand <= MAX_CAPACITY && i < NUM_OF_CUSTOMERS) {
            capacity += demand; // 如果能满足需求，就继续往下走，
        } else {//不能满足需求，就要停下来考虑一下
            // 如果不能满足需求，抱歉，要产生新的tours了，也就是派新的车了，此时比如1-3-5  不行了， 那就断掉  (1,4) 下一次从5出发
            tours[this->num_of_tours++] = {idx, i - 1};  // 先计算，再自加
            capacity = 0;
            idx = i;
        }
    }
    set_tour_index();
}

// generate a new order
void Solutioner::opt_generate() {
    vector<int> have(NUM_OF_CUSTOMERS + 1, 0);
    for(int i = 0; i < NUM_OF_CUSTOMERS; i++) {
        order[i] = i + 1;
        index_of_customer[i + 1] = i;
    }
    for(int i = 0; i < NUM_OF_CUSTOMERS; i++) {
        int idx_1, idx_2;
        idx_1 = rand() % NUM_OF_CUSTOMERS;
        idx_2 = rand() % NUM_OF_CUSTOMERS;
        swap(index_of_customer[order[idx_1]], index_of_customer[order[idx_2]]);
        swap(order[idx_1], order[idx_2]);
    }
    int first_customer_index, capacity, idx;
    idx = 0;
    while(idx < NUM_OF_CUSTOMERS) {
        // rand()%N return random number from 0 to N-1
        first_customer_index = rand()%(NUM_OF_CUSTOMERS - idx) + idx;
        index_of_customer[order[idx]] = first_customer_index;
        swap(order[first_customer_index], order[idx]);
        first_customer_index = idx;
        int first_customer = order[idx];
        have[first_customer] = 1;
        capacity = get_customer_demand(first_customer);
        idx++;

        for(int customer : nearest[first_customer]){
            if(have[customer]) continue;
            if(capacity + get_customer_demand(customer) <= MAX_CAPACITY) {
                have[customer] = 1;
                capacity += get_customer_demand(customer);
                index_of_customer[order[idx]] = index_of_customer[customer];
                swap(order[idx], order[index_of_customer[customer]]);
                idx++;
            } else{
                tours[this->num_of_tours++] = {first_customer_index, idx - 1};
                break;
            }
        }
    }
    tours[this->num_of_tours++] = {first_customer_index, NUM_OF_CUSTOMERS - 1};
    if (TYPE == 1){
        redistribute_customer();
    }
}

void Solutioner::redistribute_customer(){
    set_tour_index();
    int customer;
    int have[NUM_OF_CUSTOMERS + 1];
    for (int i = 0; i <= NUM_OF_CUSTOMERS; i++){
        have[i] = 0;
    }
    double cap1 = 0, cap2 = 0;
    for(int i = tours[this->num_of_tours - 1].left; i <= tours[this->num_of_tours - 1].right; i++){
        have[order[i]] = 1;
    }

    // choose a customer in last tour
    cap1 = get_capacity_of_tour(this->num_of_tours - 1);
    int l = tours[this->num_of_tours - 1].left;
    int r = tours[this->num_of_tours - 1].right;
    customer = order[rand() % (r - l + 1) + l];

    for(int x: nearest[customer]){
        if(have[x]) continue;
        cap2 = get_capacity_of_tour(tour_index[x]);

        // Better ?
        if(cap1 + get_customer_demand(x) <= MAX_CAPACITY
            && abs(cap1 + get_customer_demand(x) - (cap2 - get_customer_demand(x))) < abs(cap1 - cap2)){

            // . convert
            int t = -1;
            for(int i = 0; i < this->num_of_tours - 1; i++){
                for(int j = tours[i].left; j <= tours[i].right; j++){
                    if(order[j] == x){
                        swap(order[j], order[j + 1]);
                        t = 0;
                    }
                }

                if(t == 0){
                    tours[i].right--;
                    tours[i + 1].left--;
                }
            }
            have[x] = 1;
            cap1 += get_customer_demand(x);
            assert(this->num_of_tours > 0);
            tour_index[x] = this->num_of_tours - 1;
            int l = tours[this->num_of_tours - 1].left;
            int r = tours[this->num_of_tours - 1].right;
            customer = order[rand() % (r - l + 1) + l];
        } else{
            break;
        }
    }
}

void Solutioner::local_search(){

    static int l, r, x, y, i, j, u0, v0, u1, v1;
    static double t1, t2;
    static bool stop;
    for(int id_tour = 0; id_tour < this->num_of_tours; id_tour++) {  // 对所有的tour都要进行一遍local search
        l = tours[id_tour].left;  // 一个tours的左边界
        r = tours[id_tour].right; // 一个tours的右边界
        while (true) {
            stop = true;
            for(i = l; i <= r; ++i) {  // 从左边界出发， 向右边走
                for(j = r; j > i; --j) { // 从有边界出发，向左走， 直到j>i
                    u0 = order[i], u1 = order[i - 1];
                    v0 = order[j], v1 = order[j + 1];   // 选取左， 左边的左点， 右 右边的右点， 四个点

                    if(i - 1 < l)
                        u1 = 0;
                    if(j + 1 > r)
                        v1 = 0;

                    t1 = get_distance(u1, u0)+ get_distance(v0, v1);// u0-u1-----v0-v1
                    t2 = get_distance(u1, v0) + get_distance(u0, v1);//u0-v1----u1-v0
                    // 只要满足条件，就进行替换
                    if (t1 > t2) {
                        for(x = i, y = j; x <= y; ++x, --y) {
                            swap(order[x], order[y]);
                        }
                        stop = false;
                    }

                }
            }
            if (stop) break;
        }
    }
}

//Insert energy station by optimal way
void Solutioner::complete_gen() {

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

// Complete a tour from l to r
bool Solutioner::complete_subgen(int* full_path, int* gen_temp, int l, int r, int &cnt){
    int have[MAX_NODE];
    int first_id = cnt;
    double energy = BATTERY_CAPACITY;

    for(int j = l; j <= r; j++) {
        remaining_energy[j] = have[j] = 0;
    }
    static int from, to;
    // remaining_energy[i]: remaining energy after visiting point i
    remaining_energy[l] = BATTERY_CAPACITY;

    int num_finding_safe = 0;
    for(int j = l; j < r; j++) {
        from = gen_temp[j];
        to = gen_temp[j + 1];
        
        if(get_energy_consumption(from, to) <= energy) {
            full_path[cnt++] = from;
            energy -= get_energy_consumption(from, to);
            remaining_energy[j + 1] = energy;
            continue;
        }

        bool stop = false;
        
        while(!stop){
            num_finding_safe ++;
            if(have[j] || num_finding_safe == MAX_NUM_FINDING_SAFE) {
                this->fitness = INF;
                return false;
            }
            have[j] = 1;
            int best_station = nearest_station(from, to, energy);
            if(best_station == -1) {
                while(have[j] && j - 1 >= l){
                    j--; 
                    while(full_path[--cnt] != gen_temp[j]);
                    energy = remaining_energy[j];
                    from = gen_temp[j];
                    to = gen_temp[j + 1];
                }
            } else {
                energy = BATTERY_CAPACITY - get_energy_consumption(best_station, to);
                if(to == 0) energy = BATTERY_CAPACITY;
                if(energy <= remaining_energy[j + 1]){
                    while(have[j] && j - 1 >= l){
                        j--; 
                        while(full_path[--cnt] != gen_temp[j]){
                            energy = remaining_energy[j];
                            from = gen_temp[j];
                            to = gen_temp[j + 1];
                        }
                    }
                } else{
                    full_path[cnt++] = from;
                    full_path[cnt++] = best_station;
                    remaining_energy[j + 1] = energy;
                    stop = true;
                }
            }
        }
    }
    l = first_id;
    r = cnt;
    full_path[r] = 0;
    remaining_energy[l] = BATTERY_CAPACITY;
    for(int i = l + 1; i <= r; i++){
        remaining_energy[i] = remaining_energy[i - 1] - get_energy_consumption(full_path[i], full_path[i - 1]);
        if(is_charging_station(full_path[i])){
            remaining_energy[i] = BATTERY_CAPACITY;
        }
    }
    if (TYPE == 1)
        optimize_station(full_path, l, r);
    return true;
}
bool Solutioner::check_full_capacity() {
    int capa = 0;
    for (int i = 0; i < this->num_of_tours; i++) {
        capa = 0.0;
        for (int j = tours[i].left; j <= tours[i].right; j++) {
            capa += get_customer_demand(order[j]);

        }
        if (capa > MAX_CAPACITY)
            return false;
    }
    return true;
}


bool Solutioner::is_valid_solution() {
    return check_solution(solution, this->steps);
}

bool Solutioner::is_valid_order() {
    int have[NUM_OF_CUSTOMERS + 1];

    for(int i = 0; i <= NUM_OF_CUSTOMERS; i++) {
        have[i] = 0;
    }
    
    int cnt = 0;
    for(int i = 0; i < NUM_OF_CUSTOMERS; i++) {
        if (order[i] < 1 || order[i] > NUM_OF_CUSTOMERS) return false;
        if (!have[order[i]]) {
            have[order[i]] = 1;
            cnt++;
        }
    }
    return (cnt == NUM_OF_CUSTOMERS);
}

double Solutioner::get_capacity_of_tour(int tour_id) {
    double capa = 0;
    for(int i = tours[tour_id].left; i <= tours[tour_id].right; i++) 
        capa += get_customer_demand(order[i]);
    
    return capa;
}

void Solutioner::show(){
    std::cout << "-----------\nOrder: \n";
    for(int i = 0; i < NUM_OF_CUSTOMERS; i++){
        std::cout << order[i] << " ";
    }
    cout << "\n";
    cout << "Number of steps: " << this->steps << "\n";
    for(int i = 0; i < this->steps; i++){
        std::cout << solution[i] << " ";
    }
    cout << "\n";
    for (int t = 0; t < this->num_of_tours; t++){
        cout << "Tour " << t << " : " <<  tours[t].left << " " << tours[t].right << "\n";
    }
    cout << "Fitness: " << this->fitness << "\n";
    std::cout << "-----------\n";
}







