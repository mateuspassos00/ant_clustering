#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "ant_clustering.h"

#define ROWS 64
#define COLS 64
#define ITENS 400
#define AGENTS 100
#define K_1 0.3 // [0,1]
#define K_2 0.6 // [0,1]
#define ALPHA 11.8029
#define ITERATIONS 2000000
#define LOS 1

// PARA CASO HOMOGÊNEO, USAR REGRA RELACIONAL SIMPLES

int main(void) {
    srand(time(NULL));
    
    int ret;

    env *e = create_env(ROWS, COLS, AGENTS, ITENS, LOS, K_1, K_2, ALPHA, "base_4_grupos.csv");
    if(e == NULL) perror("erro ao criar ambiente");

    ret = print_env(e); assert(ret == 0);
    
    // // Move uma formiga e mostra o ambiente
    // for(int i = 0; i < ITERATIONS; i++) {
    //     for(int i = 0; i < AGENTS; i++) {
    //         ret = move(&e->list_ants[i]); assert(ret == 0);
    //         ret = print_env(e); assert(ret == 0);
    //     }
    // }
    
    ret = destroy_env(e); assert(ret == 0);

    return 0;
}