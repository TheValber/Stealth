/*
 * Stealth
 * December 2022 - January 2023
 * BERNIER Valentin - VILAYVANH Mickael
 */

#include "../include/Game.h"

static int is_win(const Player * player, const Relics * relics, int square_size) {
    int x, y, i;
    x = player->x;
    y = player->y;
    if (x + square_size / 2 <= 3 * square_size && y + square_size / 2 <= 3 * square_size) {
        for (i = 0; i < NB_RELICS; ++i) {
            if (relics->list[i].is_present) {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

static void end_screen(int status, Font * font, char ** name) {
    display_end_screen(status, font);
    if (status) {
        ask_name(font, name);
    }
    while (1) {
        if (is_escape_pressed()) {
            break;
        }
    }
}

int game() {
    Player * player;
    Linkedlink mana_tile_void;
    Map map;
    Enemies enemies;
    Relics relics;
    int square_size;
    struct timespec end_time, new_time, end_chrono, start_chrono;
    double frametime;
    Font font;
    char * name;

    load_font(&font);

    if (generate_map(&map, 60, 45) == -1) {
        return -1;
    }
    square_size = tile_size(&map);
    if (init_player(2 * square_size, 2 * square_size, &player) == -1) {
        return -1;
    }
    if (init_enemies(&enemies, &map, square_size) == -1) {
        return -1;
    }
    if (init_relics(&relics, &map, square_size) == -1) {
        return -1;
    }
    init_linkedlink(&mana_tile_void);
    clock_gettime(CLOCK_REALTIME, &end_time);
    clock_gettime(CLOCK_REALTIME, &start_chrono);
    while (1) {
        update_gui(player, &map, &enemies, &relics, &font);

        if (is_win(player, &relics, square_size)) {
            clock_gettime(CLOCK_REALTIME, &end_chrono);
            end_screen(1, &font, &name);
            save_score(&start_chrono, &end_chrono, player->mana_consumed, name);
            break;
        }

        if (enemies_see_player(&enemies, player, &map, square_size)) {
            end_screen(0, &font, &name);
            break;
        }

        if (is_escape_pressed()) {
            break;
        }

        player_action(player, &map, square_size, &mana_tile_void);

        take_relics(player, &relics, square_size);

        enemy_action(&enemies, &map, square_size);
        enemies_see_relics(&enemies, &relics, &map, square_size);
        
        player_mana_check_from_tiles(player, &map, square_size, &mana_tile_void);
        
        frametime = wait_time_for_FPS(&end_time, &new_time);
        update_panic_time(&enemies, frametime);
    }
    free_linkedlink(&mana_tile_void);
    free_player(player);
    free_map(&map);
    free_font(&font);
    
    return 0;
}
