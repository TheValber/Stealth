/*
 * Stealth
 * December 2022 - January 2023
 * BERNIER Valentin - VILAYVANH Mickael
 */

#include "../include/Menu.h"

static int check_player_on_buttons(const Player * player, int square_size) {
    int x, y;
    x = player->x;
    y = player->y;
    /* Play */
    if (x - square_size / 2 >= 490 && x + square_size / 2 <= 789 && y - square_size / 2 >= 281 && y + square_size / 2 <= 350) {
        return 1;
    }
    /* Scores */
    if (x - square_size / 2 >= 490 && x + square_size / 2 <= 789 && y - square_size / 2 >= 365 && y + square_size / 2 <= 434) {
        return 2;
    }
    /* Exit */
    if (x - square_size / 2 >= 490 && x + square_size / 2 <= 789 && y - square_size / 2 >= 448 && y + square_size / 2 <= 517) {
        return 3;
    }
    return 0;
}

int main_menu() {
    Player * player;
    Map map;
    Enemies enemies;
    Relics relics;
    int square_size, tmp;
    struct timespec end_time, new_time;
    MenuImages menu_images;

    load_menu_images(&menu_images);

    if (generate_map_for_menu(&map) == -1) {
        return -1;
    }
    square_size = tile_size(&map);
    if (init_player(515, 315, &player) == -1) {
        return -1;
    }
    player->mana = 1000000;
    if (init_enemies_for_menu(&enemies, square_size) == -1) {
        return -1;
    }
    if (init_relics_for_menu(&relics, square_size) == -1) {
        return -1;
    }

    clock_gettime(CLOCK_REALTIME, &end_time);

    while (1) {
        update_menu_gui(player, &menu_images, &map, &enemies, &relics);

        player_action(player, &map, square_size, NULL);
        take_relics(player, &relics, square_size);
        enemy_action(&enemies, &map, square_size);

        if (is_return_pressed()) {
            if ((tmp = check_player_on_buttons(player, square_size))) {
                free_player(player);
                free_map(&map);
                free_menu_images(&menu_images);
                return tmp;
            }
        }
        
        wait_time_for_FPS(&end_time, &new_time);
    }
    return -1;
}
