/*
 * Stealth
 * December 2022 - January 2023
 * BERNIER Valentin - VILAYVANH Mickael
 */

#ifndef _PLAYER_
#define _PLAYER_

#include <stdlib.h>
#include "Direction.h"
#include "Map.h"
#include "Util.h"
#include "Relics.h"

#define BASE_SPEED 0.1

typedef struct player {
    Direction dir;
    double x;
    double y;
    double speed;
    int mana;
    int mana_max;
    int visible;
    int mana_consumed;
} Player;

/**
 * @brief Initializes a player.
 * 
 * @param start_x x coordinate of starting point
 * @param start_y y coordinate of starting point
 * @param dest Pointer's adress to redirect allocated space
 * @return int Error code
 */
int init_player(int start_x, int start_y, Player **dest);

/**
 * @brief Updates player's velocity and correct his position.
 * Resets player's velocity in case no direction is selected.
 * @param player Pointer to a player
 * @param direction Direction selected, can be NONE
 * @param base_speed_multiplier Value that is added each frame to gain velocity
 * @param base_speed Base value of speed V
 * @param limit Limit of speed_multiplier to cap speed acceleration
 */
void update_player_velocity(Player *player, 
            Direction direction, 
            double base_speed_multiplier, 
            double base_velocity,
            double limit);

/**
 * @brief Makes a player visible
 * 
 * @param player Pointer to a player
 */
void turn_off_visibility(Player *player);

/**
 * @brief Makes a player not visible
 * 
 * @param player Pointer to a player
 */
void turn_on_visibility(Player *player);

/**
 * @brief Sets player's speed at base value (0.1) and
 * sets his direction to the given one.
 * 
 * @param player Pointer to a player
 * @param direction Direction selected
 */
void reset_player_velocity(Player *player, Direction direction);

/**
 * @brief Free memory allocated for a player.
 * 
 * @param player Pointer to a player
 */
void free_player(Player *player);

/**
 * @brief Checks if player's position is hitting tiles to 
 * gain mana from them. Mana taken from tiles will disappear from
 * them.
 * 
 * @param player Pointer to a player
 * @param map Pointer to the map generated to get its current state
 * @param tile_size tile's size
 * @param ptr Pointer to void mana tile linked link
 */
void player_mana_check_from_tiles(Player *player, Map *map, int tile_size, Linkedlink *ptr);

/**
 * @brief Allows the player to take a relic if standing on it
 * 
 * @param player Pointer to a player
 * @param player Pointer to a relics
 * @param tile_size tile's size
 */
void take_relics(Player * player, Relics * relics, int tile_size);

#endif