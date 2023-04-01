/*
 * Stealth
 * December 2022 - January 2023
 * BERNIER Valentin - VILAYVANH Mickael
 */

#ifndef __SPELLS__
#define __SPELLS__

#include "LinkedLink.h"
#include <stdlib.h>

typedef enum {
    IMPOSSIBLE,
    VOID_TILE,
    MANA_TILE
} ManaState;

typedef struct manaTiles {
    ManaState ** layout;
} ManaTiles;

/**
 * @brief Returns the total amount of mana tiles that an
 * array contains
 * 
 * @param array Mana tile array 
 * @param li Number of line
 * @param col Number of column
 * @return int Total amount of mana
 */
int mana_amount(ManaState **array, int li, int col);

/**
 * @brief Release mana from void from mana tile's linked link.
 * 
 * @param ptr Pointer to a linkedlink.
 */
void release_mana(Linkedlink *ptr);

#endif
