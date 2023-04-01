/*
 * Stealth
 * December 2022 - January 2023
 * BERNIER Valentin - VILAYVANH Mickael
 */

#ifndef __MAP__
#define __MAP__

#include "Spells.h"

typedef enum {
    FLOOR,
    WALL,
    DOOR,
    EXIT
} Tile;

typedef struct map {
    unsigned int width;
    unsigned int height;
    unsigned int mana_amount;
    Tile ** layout;
    ManaTiles mana_tiles;
} Map;

typedef struct room {
    unsigned int column;
    unsigned int line;
    unsigned int width;
    unsigned int height;
} Room;

/**
 * @brief Prints the map on terminal
 * 
 * @param map Pointer to a map
 */
void print_map(const Map *map);

/**
 * @brief Generate a map with a given size
 * 
 * @param map Pointer to map
 * @param width Width of the map
 * @param height Height of the map
 * @return Error code
 */
int generate_map(Map * map, unsigned int width, unsigned int height);

/**
 * @brief Converts coordinates from graphic window to
 * case's scale
 * 
 * @param coord   
 * @param tile_size 
 * @return int 
 */
int coord_to_case(double coord, int tile_size);

/**
 * @brief Generate a map with some settings to match the menu tutorial
 * 
 * @param map Pointer to map
 * @return Error code
 */
int generate_map_for_menu(Map * map);

/**
 * @brief Free memory allocated for a map.
 * 
 * @param map Pointer to a map
 */
void free_map(Map * map);

#endif
