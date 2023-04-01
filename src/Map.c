/*
 * Stealth
 * December 2022 - January 2023
 * BERNIER Valentin - VILAYVANH Mickael
 */

#include "../include/Map.h"

#include <stdlib.h>
#include <stdio.h>

void print_map(const Map *map) {
    int i, j;
    for (i = 0; i < map->height; ++i) {
        for (j = 0; j < map->width; ++j) {
            printf("%s", map->layout[i][j] == WALL ? "# " : ". ");
        }
        printf("\n");
    }
}

static void create_wall(Map * map, Room room) {
    int i, random_number;

    random_number = rand() % 2;

    /* North wall */
    if (map->layout[room.line - 1][room.column] == FLOOR) {
        for (i = 1; i <= room.width; ++i) {
            map->layout[room.line - 1][room.column - 1 + i] = WALL;
            if ((random_number == 0 && i <= 3) || (random_number == 1 && i >= room.width - 2))
                map->layout[room.line - 1][room.column - 1 + i] = DOOR;
        }
    }
    /* South wall */
    if (map->layout[room.line + room.height][room.column] == FLOOR) {
        for (i = 1; i <= room.width; ++i) {
            map->layout[room.line + room.height][room.column - 1 + i] = WALL;
            if ((random_number == 0 && i <= 3) || (random_number == 1 && i >= room.width - 2))
                map->layout[room.line + room.height][room.column - 1 + i] = DOOR;
        }
    }

    /* West wall */
    if (map->layout[room.line][room.column - 1] == FLOOR) {
        for (i = 1; i <= room.height; ++i) {
            map->layout[room.line - 1 + i][room.column - 1] = WALL;
            if ((random_number == 0 && i <= 3) || (random_number == 1 && i >= room.height - 2))
                map->layout[room.line - 1 + i][room.column - 1] = DOOR;
        }
    }

    /* East wall */
    if (map->layout[room.line][room.column + room.width] == FLOOR) {
        for (i = 1; i <= room.height; ++i) {
            map->layout[room.line - 1 + i][room.column + room.width] = WALL;
            if ((random_number == 0 && i <= 3) || (random_number == 1 && i >= room.height - 2))
                map->layout[room.line - 1 + i][room.column + room.width] = DOOR;
        }
    }
}

static void update_rooms(Room room, Room * room1, Room * room2, int wall, int inv) {
    if (!inv) {
        room1->column = room.column;
        room1->line = room.line;
        room1->width = wall - room1->column;
        room1->height = room.height;
        room2->column = wall + 1;
        room2->line = room.line;
        room2->width = room.width - room1->width - 1;
        room2->height = room.height;
    } else {
        room1->column = room.column;
        room1->line = room.line;
        room1->width = room.width;
        room1->height = wall - room1->line;
        room2->column = room.column;
        room2->line = wall + 1;
        room2->width = room.width;
        room2->height = room.height - room1->height - 1;
    }
}

static void divide_rooms(Map * map, Room room, int minside) {
    int x, x_size, inv, wall;
    Room room1, room2;

    create_wall(map, room);

    if (room.width > room.height) {
        x = room.column;
        x_size = room.width;
        inv = 0;
    } else {
        x = room.line;
        x_size = room.height;
        inv = 1;
    }

    if (x_size < 2 * minside + 1)
        return;

    if (x_size < 4 * minside) {
        if (rand() % 2 == 0)
            return;
    }

    wall = x + minside + rand() % (x_size - 2 * minside);

    update_rooms(room, &room1, &room2, wall, inv);

    divide_rooms(map, room1, minside);
    divide_rooms(map, room2, minside);
}

static int init_mana_tiles(Map * map) {
    int i, j;

    map->mana_tiles.layout = (ManaState **) malloc(sizeof(ManaState *) * map->height);
    if (!map->mana_tiles.layout) {
        return -1;
    }
    for (i = 0; i < map->height; ++i) {
        map->mana_tiles.layout[i] = (ManaState *) malloc(sizeof(ManaState) * map->width);
        if (!map->mana_tiles.layout[i]) {
            return -1;
        }
        for (j = 0; j < map->width; ++j) {
            if (map->layout[i][j] == FLOOR || map->layout[i][j] == DOOR) {
                map->mana_tiles.layout[i][j] = MANA_TILE;
            }
            else {
                map->mana_tiles.layout[i][j] = IMPOSSIBLE;
            }
        }
    }
    return 0;
}

int generate_map(Map * map, unsigned int width, unsigned int height) {
    int i, j;
    Room room;

    map->width = width;
    map->height = height;

    map->layout = (Tile **) malloc(sizeof(Tile *) * height);
    if (!map->layout) {
        return -1;
    }
    for (i = 0; i < height; ++i) {
        map->layout[i] = (Tile *) calloc(width, sizeof(Tile));
        if (!map->layout[i]) {
            return -1;
        }

        if (i == 0 || i == height - 1) {
            for (j = 0; j < width; ++j) {
                map->layout[i][j] = WALL;
            }
        }
        else {
            map->layout[i][0] = WALL;
            map->layout[i][width - 1] = WALL;
        }
    }

    room.column = 1;
    room.line = 1;
    room.width = width - 2;
    room.height = height - 2;

    divide_rooms(map, room , 9);

    map->layout[1][1] = EXIT;
    map->layout[1][2] = EXIT;
    map->layout[2][1] = EXIT;
    map->layout[2][2] = EXIT;

    if (init_mana_tiles(map) == -1) return -1;
    map->mana_amount = mana_amount(map->mana_tiles.layout, map->height, map->width);
    return 0;
}

int coord_to_case(double coord, int tile_size) {
    return (coord - 0.5 * tile_size) / tile_size;
}

int generate_map_for_menu(Map * map) {
    int i, j, width, height;

    width = 80;
    height = 45;
    map->width = width;
    map->height = height;

    map->layout = (Tile **) malloc(sizeof(Tile *) * height);
    if (!map->layout) {
        return -1;
    }
    for (i = 0; i < height; ++i) {
        map->layout[i] = (Tile *) calloc(width, sizeof(Tile));
        if (!map->layout[i]) {
            return -1;
        }

        if (i == 0 || i == height - 1) {
            for (j = 0; j < width; ++j) {
                map->layout[i][j] = WALL;
            }
        }
        else {
            map->layout[i][0] = WALL;
            map->layout[i][width - 1] = WALL;
        }
    }

    for (i = 6; i < 14; ++i) {
        for (j = 24; j < 56; ++j) {
            map->layout[i][j] = WALL;
        }
    }

    for (i = 1; i <= 12; ++i) {
        map->layout[height - i][width - 12] = WALL;
        map->layout[height - 12][width - i] = WALL;
    }

    map->layout[1][1] = EXIT;
    map->layout[1][2] = EXIT;
    map->layout[2][1] = EXIT;
    map->layout[2][2] = EXIT;

    map->mana_tiles.layout = NULL;

    return 0;
}

void free_map(Map * map) {
    int i;

    for (i = 0; i < map->height; ++i) {
        free(map->layout[i]);
    }
    free(map->layout);

    if (map->mana_tiles.layout) {
        for (i = 0; i < map->height; ++i) {
            free(map->mana_tiles.layout[i]);
        }
        free(map->mana_tiles.layout);
    }
}
