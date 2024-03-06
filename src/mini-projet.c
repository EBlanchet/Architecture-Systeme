#include <stdio.h>

#define POOL_SIZE 1000000000 /* 1 Giga */
static char _mem_pool[POOL_SIZE]; /* static pool of memory */

typedef struct bloc_libre {
    size_t taille;
    struct bloc_libre * suivant;
}Bloc_libre;

static Bloc_libre free_block_list_head = { 0, 0 };
static const size_t overhead = sizeof(size_t);
static const size_t alignement = 16;

void* my_malloc(size_t size){
    size = (size + sizeof(size_t) + (alignement - 1)) & ~ (alignement - 1); /* arrondir size au multiple le plus proche de alignement */
    Bloc_libre* block = free_block_list_head.suivant;
    Bloc_libre** head = &(free_block_list_head.suivant);
    while (block != 0) {
        if (block->taille >= size) {
            *head = block->suivant;
            return ((char*)block) + sizeof(size_t);
        }
        head = &(block->suivant);
        block = block->suivant;
    }

    block = (Bloc_libre*)sbrk(size); /* demande au système d'exploitation d'allouer un peu plus de mémoire */
    block->taille = size; 

    return ((char*)block) + sizeof(size_t);
}

void my_free(void* pouet){
    /* ajouter le bloc de mémoire dans la liste des blocs libres */
    Bloc_libre* block = (Bloc_libre*)(((char*)pouet) - sizeof(size_t));
    block->suivant = free_block_list_head.suivant;
    free_block_list_head.suivant = block;
}



