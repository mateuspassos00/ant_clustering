#include <stdio.h>
#include <stdlib.h>
#include "ant_clustering.h"

item *create_items(int num_itens, const char *path/*, int data_dim*/) {
    item *items = (item*) malloc(num_itens * sizeof(item));
    
    FILE *fp = fopen64(path, "rt");
    if(fp == NULL) perror("erro ao abrir arquivo de dados");
    
    int type_temp;
    for(int i = 0; i < num_itens; i++) {        
        items[i].carried = NULL;
        items[i].pos = (position*) malloc(sizeof(position));
        fscanf(fp, "%f,%f,%d\n", &items[i].data[0], &items[i].data[1], &type_temp);
        items[i].type = (enum item_type) (type_temp - 1); // labels: 1, 2, 3, 4.. enum: 0, 1, 2, 3..        
    }

    fclose(fp);
    
    return items;
}

ant *create_ants(env *env) {
    ant *ants = (ant*) malloc(env->num_ants * sizeof(ant));

    for(int i = 0; i < env->num_ants; i++) {        
        ants[i].carry = NULL;
        ants[i].env = env;
        ants[i].pos = (position*) malloc(sizeof(position));        
    }    
    
    return ants;
}

cell **create_map(int rows, int cols) {
    cell **m = (cell**) malloc(rows * sizeof(cell*));
    for(int i = 0; i < cols; i++) {
        m[i] = (cell*) malloc(cols * sizeof(cell));        
    }

    // evitar undefined behaviour
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            m[i][j].ant = NULL;
            m[i][j].item = NULL;
        }
    }

    return m;
}

int distribute_itens(env *env) {
    if(env == NULL) return -1;
    
    int items_placed = 0;

    while(items_placed < env->num_items) {
        int x = rand() % env->cols;
        int y = rand() % env->rows;
        
        if(env->map[x][y].item == NULL) {                        
            env->map[x][y].item = &env->list_items[items_placed];                        
            
            env->list_items[items_placed].pos->x = x;
            env->list_items[items_placed].pos->y = y;
            
            items_placed++;
        }
    }    

    return 0;
}

int distribute_ants(env *env) {
    if(env == NULL) return -1;
    
    int ants_placed = 0;

    while(ants_placed < env->num_ants) {
        int x = rand() % env->cols;
        int y = rand() % env->rows;
                
        if(env->map[x][y].ant == NULL) {            
            env->map[x][y].ant = &env->list_ants[ants_placed];

            env->list_ants[ants_placed].pos->x = x;
            env->list_ants[ants_placed].pos->y = y;

            ants_placed++;
        }
    }            
    
    return 0;
}

env *create_env(int rows, int cols, int num_ants, int num_items, int ant_los, float k_1, float k_2, float alpha, const char *path) {
    env *e = (env*) malloc(sizeof(env));
        
    e->rows = rows;
    e->cols = cols;
    e->map = create_map(rows, cols);
    
    e->k_1 = k_1;
    e->k_2 = k_2;
    e->alpha = alpha;
    
    e->num_items = num_items;    
    e->list_items = create_items(e->num_items, path);
    
    e->num_ants = num_ants;
    e->list_ants = create_ants(e);
    e->ant_los = ant_los;

    distribute_itens(e);
    distribute_ants(e);

    return e;
}

int print_env(env *e) {
    if(e == NULL) return -1;
    
    printf("-------------------------------------------------------ANT CLUSTERING--------------------------------------------------------------\n");
    for(int i = 0; i < e->rows; i++) {
        printf("| ");
        for(int j = 0; j < e->cols; j++) {            
            if(e->map[i][j].item != NULL) {                
                printf("%c ", (char) (e->map[i][j].item->type + 65));
            } else printf(". ");            
        }        
        printf("|\n");
    }
    printf("-----------------------------------------------------------------------------------------------------------------------------------\n");

    return 0;
}

void free_items(item *items, int num_items) {
    for(int i = 0; i < num_items; i++) free(items[i].pos);
    free(items);
}

void free_ants(ant *ants, int num_ants) {
    for(int i = 0; i < num_ants; i++) free(ants[i].pos);
    free(ants);
}

void free_map(cell **map, int rows, int cols) {
    for(int i = 0; i < rows; i++) free(map[i]);
    free(map);
}

int destroy_env(env *e) {
    if(e == NULL) return -1;
        
    free_ants(e->list_ants, e->num_ants);
    free_items(e->list_items, e->num_items);
    free_map(e->map, e->rows, e->cols);
    
    free(e);

    return 0;
}