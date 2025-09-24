#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "ant_clustering.h"

#define ROWS 64
#define COLS 64
#define ITENS 400
#define AGENTS 100
#define K_1 0.01 // [0,1]
#define K_2 0.015 // [0,1]
#define ALPHA 30
#define ITERATIONS 2000000
#define LOS 1

int main(void) {
    srand(time(NULL));
    
    int ret;

    env *e = create_env(ROWS, COLS, AGENTS, ITENS, LOS, K_1, K_2, ALPHA, "data\\base_4_grupos.csv");
    if(e == NULL) perror("erro ao criar ambiente");
    
    ret = print_env_to_file(e, "snapshots_2\\start"); assert(ret == 0);
    
    for(long int i = 0; i < ITERATIONS; i++) {
        for(int j = 0; j < AGENTS; j++) {            
            if(i == (ITERATIONS / 4)) {ret = print_env_to_file(e, "snapshots_2\\q1"); assert(ret == 0);}
            if(i == (ITERATIONS / 2)) {ret = print_env_to_file(e, "snapshots_2\\q2"); assert(ret == 0);}
            if(i == (3 * ITERATIONS / 4)) {ret = print_env_to_file(e, "snapshots_2\\q3"); assert(ret == 0);}

            ret = move(&e->list_ants[j]); assert(ret == 0);
        }
    }
    
    ret = print_env_to_file(e, "snapshots_2\\final"); assert(ret == 0);
    
    ret = destroy_env(e); assert(ret == 0);

    return 0;
}