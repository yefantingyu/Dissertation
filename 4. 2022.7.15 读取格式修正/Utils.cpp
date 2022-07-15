#include "Utils.hpp"
void allocate_space_for_one_dimen_array(int*& array, int dimensiont){
    array = new int[dimensiont];
}
void allocate_space_for_one_dimen_array(bool*& array, int dimensiont){
    array = new bool[dimensiont];
}
void allocate_space_for_one_dimen_array(double*& array,int dimensiont){
    array = new double[dimensiont];
}
void allocate_space_for_one_dimen_array(AntStruct*& array,int dimensiont){
    array = new AntStruct[dimensiont];
}
void allocate_space_for_two_dimen_array(int**& array, int m, int n){
    int i;
    array = new int*[m];
    for(i=0; i<m;i++){
        array[i] = new int[n];
    }
}
void allocate_space_for_two_dimen_array(double**& array, int m, int n){
    int i;
    array = new double*[m];
    for(i=0; i<m;i++){
        array[i] = new double[n];
    }
}



void free_one_dimen_arrary(int*& array){
    delete [] array;
}
void free_one_dimen_arrary(bool*& array){
    delete [] array;
}
void free_one_dimen_arrary(double*& array){
    delete [] array;
}
void free_one_dimen_arrary(AntStruct*& array){
    delete [] array;
}
void free_two_dimen_array(int**& array){
    int i;
    int m = sizeof(array);
    delete [] array;
    for(i=0; i<m;i++){
        delete array[i];
    }
}
void free_two_dimen_array(double**& array){
    int i;
    int m = sizeof(array);
    delete [] array;
    for(i=0; i<m;i++){
        delete array[i];
    }
}

