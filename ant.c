#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ant_clustering.h"

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
            int x = mod(ant_pos->x + dx, env->cols);
            int y = mod(ant_pos->y + dy, env->rows);
            
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
    num_cells *= num_cells;
    
    float f = 1.0 / num_cells, sum = 0;
    for(int i = 0; i < nearby_items->size; i++) {
        sum += (1 - distance(item, nearby_items->items[i]) / alpha);
    }

    sum > 0 ? f *= sum : f;
    return f > 0 ? f : 0;
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

void drop_item(ant *ant) {
    env *env = ant->env;
    
    env->map[ant->pos->x][ant->pos->y].item = ant->carry; // célula ganha a referência do item
    ant->carry->pos->x = ant->pos->x; // posição do item abandonado passa a ser a (última) posição da formiga
    ant->carry->pos->y = ant->pos->y;
    ant->carry->carried = NULL; // item perde a referência da formiga que o carrega
    ant->carry = NULL; // formiga perde a referência do item
}

void pick_up_item(ant *ant, item *candidate) {
    ant->carry = candidate;
    candidate->carried = ant;
}

int move(ant *ant) {    
    if(ant == NULL) return -1;

    env *env = ant->env;    
    item *carrying = ant->carry;
    
    ant->next_dir = rand() % sizeof(enum direcao);
    switch(ant->next_dir) {
        case UP: 
            ant->pos->x = mod(ant->pos->x - 1, env->rows); // atualizar a posição da formiga            
            break;
        case DOWN: 
            ant->pos->x = mod(ant->pos->x + 1, env->rows);
            break;
        case LEFT: 
            ant->pos->y = mod(ant->pos->y - 1, env->cols);
            break;
        case RIGHT:
            ant->pos->y = mod(ant->pos->y + 1, env->cols);
            break;
    }

    item_list *nearby_items = check_items(ant);
    if(carrying) {
        if (prob_drop(ant, nearby_items) > 0.5) drop_item(ant);
    } else {
        float max_valid_prob = 0.5, prob;
        item *candidate = NULL;
        for(int i = 0; i < nearby_items->size; i++) {
            if((prob = prob_pickup(ant, nearby_items->items[i], nearby_items)) > max_valid_prob) {
                max_valid_prob = prob;
                candidate = nearby_items->items[i];
            }
        }
        if(candidate) pick_up_item(ant, candidate);
    }

    free(nearby_items->items);
    free(nearby_items);

    return 0;
}