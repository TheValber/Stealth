/*
 * Stealth
 * December 2022 - January 2023
 * BERNIER Valentin - VILAYVANH Mickael
 */

#ifndef _COLLIDE_
#define _COLLIDE_

#include "Direction.h"
#include "Util.h"
#include "Map.h"

typedef struct entity {
    Direction dir;
    double x;
    double y;
    double speed;
} Entity;

/**
 * @brief Compute colliding check depending on the direction selected.
 * First parameter ptr must be a struct that contains these exact same
 * type fields in this order :
 *  - Direction
 *  - double
 *  - double
 *  - double
 * representing an entity based on the current data choices made, 
 * so that the memory alignement to get the right value for calculation
 * wont miss.
 * 
 * @param ptr Pointer to an entity
 * @param dir Direction to be computed
 * @param map Pointer to a map that contains the current that of it
 * @param base_velocity Base value of velocity
 * @param tile_size tile's size
 * @return int Returns 0 if no walls are hit.
 */
int check_wall_collide_by_direction(const void *ptr, Direction dir, const Map *map, 
                                    double base_velocity, int tile_size);

#endif