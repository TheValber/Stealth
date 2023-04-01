/*
 * Stealth
 * December 2022 - January 2023
 * BERNIER Valentin - VILAYVANH Mickael
 */

#ifndef _GAMEPLAY_
#define _GAMEPLAY_

#include <MLV/MLV_all.h>
#include "Direction.h"
#include "Player.h"
#include "Collide.h"
#define V 2.5
#define SPEED_MULTIPLIER 0.03

/**
 * @brief Computes player's action (keys detection, 
 * movements and skills activation).
 * 
 * @param player Pointer to a player
 */
void player_action(Player *player, const Map *map, int tile_size, Linkedlink *ptr);

/**
 * @brief Check if the return key is pressed
 * 
 * @return 1 if the return key is pressed
 */
int is_return_pressed();

/**
 * @brief Check if the escape key is pressed
 * 
 * @return 1 if the escape key is pressed
 */
int is_escape_pressed();

#endif