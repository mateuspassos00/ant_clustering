#ifndef _ANT_CLUSTERING_H_
#define _ANT_CLUSTERING_H_

typedef struct env env;
typedef struct ant ant;
typedef struct item item;
typedef struct position position;
typedef struct cell cell;

enum item_type {
    A,
    B,
    C,
    D
};

enum direcao {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

typedef struct position {
    int x, y;
} position;

typedef struct item {
    float data[2];    
    enum item_type type; // <- NÃO EXISTE 
    position *pos;
    ant *carried; // se está sendo carregado por uma formiga ou não
} item;

typedef struct item_list {
    int size;
    item **items;
} item_list;

typedef struct ant {    
    item *carry; // NULL se não estiver carregando nada
    enum direcao next_dir;
    position *pos;
    env *env; // importante para acessar os itens e construir lista com os itens próximos
} ant;

typedef struct cell {
    // MUDAR: UMA CÉLULA PODE TER MAIS DE UMA FORMIGA
    // atualmente, uma célula só tem a referência da última formiga que se moveu para ela
    ant *ant;
    item *item;
} cell;

// LF model
typedef struct env {
    int rows, cols, num_ants, num_items, ant_los/*, data_dim*/;
    ant *list_ants;
    item *list_items;
    cell **map;
    float k_1, k_2, alpha;
} env;

env *create_env(int rows, int cols, int num_ants, int num_items, int ant_los, float k_1, float k_2, float alpha, const char *path/*, int data_dim*/); // cria o ambiente distribuindo aleatoriamente itens e formigas
int move(ant *a); // escolhe próxima direção aleatóriamente
int print_env(env *env);
int destroy_env(env *env);

#endif