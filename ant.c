#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ant_clustering.h"

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
            int x = (ant_pos->x + dx ) % env->cols;
            int y = (ant_pos->y + dy) % env->rows;
            
            if (map[x][y].item && map[x][y].item->carried == NULL) {
                nearby_items->size++;
                nearby_items->items = realloc(
                    nearby_items->items,
                    nearby_items->size * sizeof(item *)
                );
                nearby_items->items[nearby_items->size - 1] = map[x][y].item;
            }
        }
    }

    return nearby_items;
}

float distance(item *i1, item *i2) {
    float x_diff = i1->pos->x - i2->pos->x;
    float y_diff = i1->pos->y - i2->pos->y;

    float d = sqrtf(x_diff * x_diff + y_diff * y_diff);
    return d;
}

// f(x) E [0,1]
float f_function(ant* ant, item *item, item_list *nearby_items) {
    env* e = ant->env;
    int alpha = ant->env->alpha;
    int num_cells = 2 * e->ant_los + 1;
    num_cells *= num_cells;
    
    float f = 1 / num_cells, sum = 0;
    for(int i = 0; i < nearby_items->size; i++) {
        if(item->pos != nearby_items->items[i]->pos) sum += (1 - distance(item, nearby_items->items[i]) / alpha);
    }

    f *= sum;
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
    
    float f = f_function(ant, ant->carry, nearby_items);
    
    return f < k_2 ? 2 * f : 1;
}

void drop_item(ant *ant) {
    env *env = ant->env;
    
    env->map[ant->pos->x][ant->pos->y].item = ant->carry; // célula ganha a referência do item
    ant->carry->carried == NULL; // item 'deixa' de ser carregado (perde a referência da formiga)
    ant->carry = NULL; // formiga perde a referência do item
}

void pick_up_item(ant *ant, item *candidate) {
    ant->carry = candidate;
    candidate->carried = ant;
}

int move(ant *ant) {    
    if(ant == NULL) return -1;

    env *env = ant->env;
    cell **map = env->map;
    item *carrying = ant->carry;

    map[ant->pos->x][ant->pos->y].ant = NULL; // formiga saiu da célula antiga
    if(carrying) map[carrying->pos->x][carrying->pos->y].ant = NULL; // idem para o item (caso estiver carregando)
    
    ant->next_dir = rand() % sizeof(enum direcao);
    switch(ant->next_dir) {
        case UP: 
            ant->pos->x = (ant->pos->x - 1) % ant->env->rows; // atualizar a posição da formiga            
            if(carrying) carrying->pos->x - 1; // atualizar a posição do item (caso estiver carregando algo)
            break;
        case DOWN: 
            ant->pos->x = (ant->pos->x + 1) % ant->env->rows;
            if(carrying) carrying->pos->x + 1;
            break;
        case LEFT: 
            ant->pos->y = (ant->pos->y - 1) % ant->env->rows;
            if(carrying) carrying->pos->y - 1;
            break;
        case RIGHT:
            ant->pos->y = (ant->pos->y + 1) % ant->env->rows;
            if(carrying) carrying->pos->y + 1;
            break;
    }

    map[ant->pos->x][ant->pos->y].ant = ant; // atualizar a célula no mapa
    if(carrying) map[carrying->pos->x][carrying->pos->y].item = carrying; // idem para o item (caso estiver carregando)

    /*
     - Caso a formiga esteja carregando algo:
        - Verifica a probabilidade de largar,
            - Se for maior do que 70%, (????)
                - Larga na mesma célula;
                - Atualiza posição do item no mapa;
            - Se não, a formiga continua se deslocando,
                - Atualiza posição do item no mapa;
     
    - Caso a formiga NÃO esteja carregando algo:
        - Verifica se existem itens nas células adjacentes,
            - Se sim, verifica as probabilidades de pegar cada item,
                - Se todas as probabilidades forem inferiores a 70%, a formiga continua se deslocando;
                - Caso contrário, pega o item com a maior probabilidade;
            - Se não, a formiga continua se deslocando;    
    */

    item_list *nearby_items = check_items(ant);

    if(nearby_items->size > 0) { // Caso não houver nenhum item ao redor, continua andando
        if(carrying && prob_drop(ant, nearby_items) > .7) drop_item(ant);
        else if(carrying == NULL) {
            float max_valid_prob = 0, prob;
            item *candidate = NULL;
            for(int i = 0; i < nearby_items->size; i++) {
                if(prob = prob_pickup(ant, nearby_items->items[i], nearby_items) > .7 && prob > max_valid_prob) {
                    max_valid_prob = prob;
                    candidate = nearby_items->items[i];
                }
            }
            if(candidate) pick_up_item(ant, candidate);
        }
    }

    free(nearby_items->items);
    free(nearby_items);

    return 0;
}