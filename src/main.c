#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "ant_clustering.h"

#define ROWS 64
#define COLS 64
#define ITENS 600
#define AGENTS 15
#define K_1 0.9 // [0,1]
#define K_2 0.05 // [0,1]
#define ALPHA 0.11
#define ITERATIONS 50000000
#define LOS 1

int main(void) {
    srand(time(NULL));
    
    int ret;

    env *e = create_env(ROWS, COLS, AGENTS, ITENS, LOS, K_1, K_2, ALPHA, "data\\base_15_grupos_norm.csv");
    if(e == NULL) perror("erro ao criar ambiente");
    
    ret = print_item_list(e, "snapshots_15\\start.csv"); assert(ret == 0);
    
    for(long int i = 0; i < ITERATIONS; i++) {
        for(int j = 0; j < AGENTS; j++) {            
            if(i == (ITERATIONS / 4)) {ret = print_item_list(e, "snapshots_15\\q1.csv"); assert(ret == 0);}
            if(i == (ITERATIONS / 2)) {ret = print_item_list(e, "snapshots_15\\q2.csv"); assert(ret == 0);}
            if(i == (3 * ITERATIONS / 4)) {ret = print_item_list(e, "snapshots_15\\q3.csv"); assert(ret == 0);}

            ret = move(&e->list_ants[j]); assert(ret == 0);
        }
    }    

    ret = print_item_list(e, "snapshots_15\\finish.csv"); assert(ret == 0);
    
    ret = destroy_env(e); assert(ret == 0);

    return 0;
}