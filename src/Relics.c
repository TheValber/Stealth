/*
 * Stealth
 * December 2022 - January 2023
 * BERNIER Valentin - VILAYVANH Mickael
 */

#include "../include/Relics.h"

static void generate_relic_coord(int * x, int * y, const Map * map, int tile_size) {
    int line, col;
    double dist;

    while (1) {
        line = rand() % map->height;
        col = rand() % map->width;

        dist = square_double(1.5 - line) + square_double(1.5 - col);

        if (map->layout[line][col] == FLOOR && dist > 400) {
            *x = (col + 0.5) * tile_size;
            *y = (line + 0.5) * tile_size;
            return;
        }
    }
}

int init_relics(Relics * relics, const Map * map, int tile_size) {
    int i;

    for (i = 0; i < NB_RELICS; ++i) {
        generate_relic_coord(&(relics->list[i].x), &(relics->list[i].y), map, tile_size);
        relics->list[i].is_present = 1;
        relics->list[i].is_not_detected = 1;
    }

    return 0;
}

int init_relics_for_menu(Relics * relics, int tile_size) {
    int i;

    for (i = 0; i < NB_RELICS; ++i) {
        relics->list[i].x = (65 + 4 * i + 0.5) * tile_size;
        relics->list[i].y = 5.5 * tile_size;
        relics->list[i].is_present = 1;
        relics->list[i].is_not_detected = 1;
    }

    return 0;
}
