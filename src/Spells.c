/*
 * Stealth
 * December 2022 - January 2023
 * BERNIER Valentin - VILAYVANH Mickael
 */

#include "../include/Spells.h"

int mana_amount(ManaState **array, int li, int col){
    int cmpt, i, j;
    cmpt = 0;
    for (i = 0; i < li; i++){
        for (j = 0; j < col; j++){
            if (array[i][j] == MANA_TILE)
                cmpt++;
        }
    }
    return cmpt;
}

void release_mana(Linkedlink *ptr){
    int i, index;
    ManaState *tile;
    Link *prev = NULL;
    Link *current = ptr->head;
    /* If there is at least one member */
    if (ptr && ptr->nmemb > 4){
        prev = current;
        /* Choosing a random link index */
        index = rand() % ptr->nmemb;
        if (index == 0){
            ptr->head = current->next;
            tile = (ManaState *)current->addr;
            *tile = MANA_TILE;
            free(current);
            ptr->nmemb--;
        } else {
            for (i = 0; i < index; i++){
                prev = current;
                current = current->next;
            }
            tile = (ManaState *)current->addr;
            *tile = MANA_TILE;
            prev->next = current->next;
            free(current);
            ptr->nmemb--;
        }
    }
}