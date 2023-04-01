/*
 * Stealth
 * December 2022 - January 2023
 * BERNIER Valentin - VILAYVANH Mickael
 */

#ifndef _GUI_
#define _GUI_

#define FPS 60
#define WIN_WIDTH 1280
#define WIN_HEIGHT 720
#define MANA_GAUGE_SCALE 10

#define _POSIX_C_SOURCE 199309L
#include <time.h>

#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include <MLV/MLV_all.h>
#include "Data.h"
#include "Relics.h"
#include "Gameplay.h"
#include "Leaderboard.h"

/**
 * @brief Update the gui to display all informations
 * 
 * @param player Pointer to player
 * @param map Pointer to map
 * @param enemies Pointer to enemies
 * @param relics Pointer to relics
 * @param font Pointer to font
 */
void update_gui(const Player *player, const Map * map, const Enemies * enemies, const Relics * relics, const Font * font);

/**
 * @brief Return the size of a tile on the map
 * 
 * @param map Pointer to map
 * @return the size
 */
int tile_size(const Map * map);

/**
 * @brief Calculates and waits for the time it takes for the game to run at a fixed number of fps
 * 
 * @param end_time Pointer to the time at the start of the frame
 * @param new_time Pointer to the time at the end of the frame
 * @return The duration of the frame
 */
double wait_time_for_FPS(struct timespec * end_time, struct timespec * new_time);

/**
 * @brief Update the gui to display all informations on the menu
 * 
 * @param player Pointer to player
 * @param menu_images Pointer to the menu overlay
 * @param map Pointer to map
 * @param enemies Pointer to enemies
 * @param relics Pointer to relics
 */
void update_menu_gui(const Player *player, const MenuImages * menu_images, const Map * map, const Enemies * enemies, const Relics * relics);

/**
 * @brief Update the gui to display all informations on the end screen
 * 
 * @param status 1 if win, 0 if lose
 * @param font Pointer to font
 */
void display_end_screen(int status, Font * font);

/**
 * @brief Ask the player name with an input box
 * 
 * @param font Pointer to font
 * @param text Text enter by user
 */
void ask_name(Font * font, char ** text);

void display_leaderboards(const char *fpath1, const char *fpath2);

#endif