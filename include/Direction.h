/*
 * Stealth
 * December 2022 - January 2023
 * BERNIER Valentin - VILAYVANH Mickael
 */

#ifndef _DIRECTION_
#define _DIRECTION_

typedef enum direction {
    UP,
    LEFT,
    DOWN,
    RIGHT,
    UP_LEFT,
    UP_RIGHT,
    DOWN_LEFT,
    DOWN_RIGHT,
    NONE
} Direction;

/**
 * @brief Compares two directions.
 * 
 * @param dir1 First direction
 * @param dir2 Second direction
 * @return int Returns 0 if they are the same, else returns 1
 */
int direction_cmp(Direction dir1, Direction dir2);

#endif