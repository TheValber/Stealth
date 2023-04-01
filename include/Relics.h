/*
 * Stealth
 * December 2022 - January 2023
 * BERNIER Valentin - VILAYVANH Mickael
 */

#ifndef __RELICS__
#define __RELICS__

#define NB_RELICS 3

#include <stdlib.h>
#include "Map.h"
#include "Util.h"

typedef struct relic {
    int x;
    int y;
    int is_present;
    int is_not_detected;
} Relic;

typedef struct relics {
    Relic list[NB_RELICS];
} Relics;

/**
 * @brief Initializes all relics.
 * 
 * @param relics Pointer to relics
 * @param map Pointer to the map
 * @param tile_size tile's size
 * @return int Error code
 */
int init_relics(Relics * relics, const Map * map, int tile_size);

/**
 * @brief Initializes all relics with some settings to match the menu tutorial
 * 
 * @param relics Pointer to relics
 * @param tile_size tile's size
 * @return int Error code
 */
int init_relics_for_menu(Relics * relics, int tile_size);

#endif
