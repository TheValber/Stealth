/*
 * Stealth
 * December 2022 - January 2023
 * BERNIER Valentin - VILAYVANH Mickael
 */

#ifndef __ENEMY__
#define __ENEMY__

#include "Player.h"
#include "Map.h"
#include "Relics.h"

#define NB_ENEMIES 5

typedef struct enemy {
    Direction dir;
    double x;
    double y;
    double speed;
    int field_of_view;
} Enemy;

typedef struct enemies {    
    Enemy list[NB_ENEMIES];
    int is_panic_mode;
    double panic_time;
} Enemies;

/**
 * @brief Initializes all ennemies.
 * 
 * @param enemies Pointer to enemies
 * @param map Pointer to the map generated to get its current state
 * @param tile_size tile's size
 * @return int Error code
 */
int init_enemies(Enemies * enemies, const Map * map, int tile_size);

/**
 * @brief Computes all enemies action (mouvements and change of state).
 * If they hit a wall, they randomly change direction.
 * Ennemies have a small chance (1/120) to change direction randomly while not hitting walls.
 * 
 * @param enemies Pointer to enemies
 * @param map Pointer to the map generated to get its current state
 * @param tile_size tile's size
 */
void enemy_action(Enemies * enemies, const Map * map, int tile_size);

/**
 * @brief Sets enemies mode to panic:
 * They have a wider field of view and move faster.
 * 
 * @param enemies Pointer to enemies
 */
void activate_panic_mode(Enemies * enemies);

/**
 * @brief Disable enemies panic mode.
 * 
 * @param enemies Pointer to enemies
 */
void desactivate_panic_mode(Enemies * enemies);

/**
 * @brief Checks if a player is in sight of an enemy among all of them.
 * 
 * @param enemies Pointer to enemies
 * @param player Pointer to a player
 * @param map Pointer to the map generated to get its current state
 * @param tile_size tile's size
 * @return int 1 if seen, else 0.
 */
int enemies_see_player(const Enemies * enemies, const Player * player, const Map * map, int tile_size);

/**
 * @brief Checks if a missing relic is in sight of an enemy among all of them.
 * 
 * @param enemies Pointer to enemies
 * @param relics Pointer to relics
 * @param map Pointer to the map generated to get its current state
 * @param tile_size tile's size
 * @return int 1 if seen, else 0.
 */
int enemies_see_relics(Enemies * enemies, Relics * relics, const Map * map, int tile_size);

/**
 * @brief Update the remaining time of panic mode
 * 
 * @param enemies Pointer to enemies
 * @param frametime Time elapsed since last frame
 */
void update_panic_time(Enemies * enemies, double frametime);

/**
 * @brief Initializes all ennemies with some settings to match the menu tutorial
 * 
 * @param enemies Pointer to enemies
 * @param tile_size tile's size
 * @return int Error code
 */
int init_enemies_for_menu(Enemies * enemies, int tile_size);

#endif
