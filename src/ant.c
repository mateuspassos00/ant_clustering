#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ant_clustering.h"

// ========================================================================================
// CASO HOMOGÊNEO: prob_drop_simplified + f_function_simplified
// 4 GRUPOS: prob_drop + f_function
// 15 GRUPOS: 
// ========================================================================================


int mod(int a, int b) {
    return (a % b + b) % b;
}

item_list *check_items(ant *ant) {
    env *env = ant->env;
    cell **map = env->map;
    position *ant_pos = ant->pos;
    int r = env->ant_los;

    item_list *nearby_items = (item_list *) malloc(sizeof(item_list));
    nearby_items->size = 0;
    nearby_items->items = NULL;
    
    for (int dx = -r; dx <= r; dx++) { // 'varredura' centrada na célula da formiga
        for (int dy = -r; dy <= r; dy++) {
            int x = mod(ant_pos->x + dx, env->rows);
            int y = mod(ant_pos->y + dy, env->cols);
            
            if (map[x][y].item) {
                if(map[x][y].item->carried == NULL) {
                    nearby_items->size++;
                    nearby_items->items = realloc(
                        nearby_items->items,
                        nearby_items->size * sizeof(item *)
                    );
                    nearby_items->items[nearby_items->size - 1] = map[x][y].item;
                }
            }
        }
    }

    return nearby_items;
}

float distance(item *i1, item *i2) {
    float d0_diff = i1->data[0] - i2->data[0];
    float d1_diff = i1->data[1] - i2->data[1];

    float d = sqrtf(d0_diff * d0_diff + d1_diff * d1_diff);
    return d;
}

// f(x) E [0,1] <-- VERIFICAR NORMALIZAÇÃO
float f_function(ant* ant, item *item, item_list *nearby_items) {
    env* e = ant->env;
    float alpha = ant->env->alpha;
    int num_cells = 2 * e->ant_los + 1;
    // int num_cells = nearby_items->size;
    num_cells *= num_cells;
    
    float sum = 0;
    for(int i = 0; i < nearby_items->size; i++) {
        sum += (1 - distance(item, nearby_items->items[i]) / alpha);
    }

    float f = sum / num_cells;
    return f > 0 ? f : 0;
}

// caso homogêneo:
// quanto maior a fração de itens percebidos (# itens / # celulas)
// maior a chance de largar o item, o contrário também é verdade
float f_function_simplified(ant* ant, item *item, item_list *nearby_items) {
    env* e = ant->env;
    float alpha = ant->env->alpha;
    int num_cells = 2 * e->ant_los + 1;
    // int num_cells = nearby_items->size;
    num_cells *= num_cells;
    
    return nearby_items->size / num_cells;
}

float prob_pickup(ant *ant, item* item, item_list *nearby_items) {
    env *env = ant->env;
    
    float k_1 = env->k_1;
    
    float pp = k_1 / (k_1 + f_function(ant, item, nearby_items));

    return pp * pp;
}

float prob_drop(ant *ant, item_list *nearby_items) {
    env *env = ant->env;
    item *item = ant->carry;
    
    float k_2 = env->k_2;
    
    float f = f_function(ant, item, nearby_items);
    
    return f < k_2 ? 2 * f : 1;
}

float prob_drop_simplified(ant *ant, item_list *nearby_items) {
    env *env = ant->env;
    item *item = ant->carry;
    
    float k_2 = env->k_2;
    
    float f = f_function(ant, item, nearby_items);

    float pd = f / (k_2 + f);
    
    return pd * pd;
}

void drop_item(ant *ant) {
    env *env = ant->env;
    int x = ant->pos->x;
    int y = ant->pos->y;

    if (ant->carry != NULL && env->map[x][y].item == NULL) {
        env->map[x][y].item = ant->carry;   
        ant->carry->pos->x = x;             
        ant->carry->pos->y = y;
        ant->carry->carried = NULL;         
        ant->carry = NULL;
    }
}

void pick_up_item(ant *ant, item *candidate) {
    if (!ant || !candidate) return;
    env *e = ant->env;
    int r = candidate->pos->x;
    int c = candidate->pos->y;
    
    ant->carry = candidate;
    candidate->carried = ant;
    e->map[r][c].item = NULL;
}

int move(ant *ant) {
    if (ant == NULL) return -1;
    env *env = ant->env;
    item *carrying = ant->carry;
    
    ant->next_dir = rand() % 4;
    int r = ant->pos->x;
    int c = ant->pos->y;

    switch (ant->next_dir) {
        case UP:
            r = mod(r - 1, env->rows);
            break;
        case DOWN:
            r = mod(r + 1, env->rows);
            break;
        case LEFT:
            c = mod(c - 1, env->cols);
            break;
        case RIGHT:
            c = mod(c + 1, env->cols);
            break;
    }

    if (env->map[ant->pos->x][ant->pos->y].ant == ant)
        env->map[ant->pos->x][ant->pos->y].ant = NULL;

    ant->pos->x = r;
    ant->pos->y = c;    
    env->map[r][c].ant = ant; // usar uma lista caso não se queira perder a referência
    
    item_list *nearby_items = check_items(ant);
    float prob;

    if (carrying) {
        prob = prob_drop(ant, nearby_items);
        float rrand = (float)rand() / (float)RAND_MAX;
        if (rrand < prob) {
            drop_item(ant);
        }
    } else {        
        for (int i = 0; i < nearby_items->size; i++) {
            float pp = prob_pickup(ant, nearby_items->items[i], nearby_items);
            // pegar/dropar probabilístico.. pode ser também escolhendo a prob máx
            float rrand = (float)rand() / (float)RAND_MAX; // normalizando o número aleatório
            if (rrand < pp) {
                pick_up_item(ant, nearby_items->items[i]);
                break;
            }
        }
    }

    free(nearby_items->items);
    free(nearby_items);

    return 0;
}