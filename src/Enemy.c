/*
 * Stealth
 * December 2022 - January 2023
 * BERNIER Valentin - VILAYVANH Mickael
 */

#include "../include/Enemy.h"
#include <stdlib.h>
#include "../include/Gameplay.h"
#include "../include/Util.h"

static void generate_enemy_coord(double * x, double * y, const Map * map, int tile_size) {
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

int init_enemies(Enemies * enemies, const Map * map, int tile_size) {
    int i;

    enemies->is_panic_mode = 0;
    enemies->panic_time = 0;
    for (i = 0; i < NB_ENEMIES; ++i) {
        generate_enemy_coord(&(enemies->list[i].x), &(enemies->list[i].y), map, tile_size);
        enemies->list[i].dir = rand() % 4;
        enemies->list[i].speed = (0.3 + 0.5 * rand() / (double) RAND_MAX);
        enemies->list[i].field_of_view = 4;
    }

    return 0;
}

static void change_enemy_state(Enemy * enemy, int is_panic_mode) {
    Direction tmp;

    if (is_panic_mode)
        enemy->speed = 1;
    else
        enemy->speed = (0.3 + 0.5 * rand() / (double) RAND_MAX);

    do {
        tmp = rand() % 4;
    } while (tmp == enemy->dir);
    enemy->dir = tmp;
}

static Enemy create_enemy_for_check_wall_panic_mode(const Enemy * enemy, int tile_size) {
    Enemy result;
    result = *enemy;
    switch (result.dir) {
        case UP:
            result.y -= tile_size;
            break;
        case DOWN:
            result.y += tile_size;
            break;
        case RIGHT:
            result.x += tile_size;
            break;
        case LEFT:
            result.x -= tile_size;
            break;
        default:
            break;
    }
    return result;
}

static void move_enemies(Enemies * enemies, const Map * map, int tile_size) {
    int i;
    double speed;
    Enemy tmp;
    for (i = 0; i < NB_ENEMIES; ++i) {
        speed = enemies->list[i].speed * V;
        if (check_wall_collide_by_direction(&enemies->list[i], enemies->list[i].dir, map, V, tile_size)) {
            change_enemy_state(&enemies->list[i], enemies->is_panic_mode);
            break;
        }
        if (enemies->is_panic_mode) {
            tmp = create_enemy_for_check_wall_panic_mode(&enemies->list[i], tile_size);
            if (check_wall_collide_by_direction(&tmp, enemies->list[i].dir, map, V, tile_size)) {
                change_enemy_state(&enemies->list[i], enemies->is_panic_mode);
                break;
            }
        }
        switch (enemies->list[i].dir) {
            case UP:
                enemies->list[i].y -= speed;
                break;
            case DOWN:
                enemies->list[i].y += speed;
                break;
            case RIGHT:
                enemies->list[i].x += speed;
                break;
            case LEFT:
                enemies->list[i].x -= speed;
                break;
            default:
                break;
        }
    }
}

void enemy_action(Enemies * enemies, const Map * map, int tile_size) {
    int i;
    for (i = 0; i < NB_ENEMIES; ++i) {
        if (rand() % 120 == 0) {
            change_enemy_state(&enemies->list[i], enemies->is_panic_mode);
        }
    }
    move_enemies(enemies, map, tile_size);
}

void activate_panic_mode(Enemies * enemies) {
    int i;
    enemies->is_panic_mode = 1;
    enemies->panic_time = 30.0;
    for (i = 0; i < NB_ENEMIES; ++i) {
        enemies->list[i].field_of_view = 6;
        enemies->list[i].speed = 1;
    }   
}

void desactivate_panic_mode(Enemies * enemies) {
    int i;
    enemies->is_panic_mode = 0;
    enemies->panic_time = 0.0;
    for (i = 0; i < NB_ENEMIES; ++i) {
        enemies->list[i].field_of_view = 4;
        enemies->list[i].speed = (0.3 + 0.5 * rand() / (double) RAND_MAX);
    }   
}

static int check_col(double x0, double y0, double x1, double y1, int range, const Map * map, int tile_size) {
    int a;
    double pa, ya;
    if (x1 < x0) {
        switch_double(&x0, &x1);
        switch_double(&y0, &y1);
    }
    for (a = (int) (x0 / tile_size) * tile_size; a < (int) ((x1 + tile_size) / tile_size) * tile_size; a += tile_size) {
        pa = ((double)a - x1) / (x0 - x1);
        if (pa >= 0 && pa <= 1) {
            ya = y1 + ((y0 - y1) / (x0 - x1)) * ((double)a - x1);
            if (map->layout[(int) ya / tile_size][a / tile_size] == WALL)
                return 0;
            if (map->layout[(int) ya / tile_size][a / tile_size - 1] == WALL)
                return 0;
        }
    }
    return 1;
}

static int check_line(double x0, double y0, double x1, double y1, int range, const Map * map, int tile_size) {
    int a;
    double pa, xa;
    if (y1 < y0) {
        switch_double(&x0, &x1);
        switch_double(&y0, &y1);
    }
    for (a = (int) (y0 / tile_size) * tile_size; a < (int) ((y1 + tile_size) / tile_size) * tile_size; a += tile_size) {
        pa = ((double)a - y1) / (y0 - y1);
        if (pa >= 0 && pa <= 1) {
            xa = x1 + ((x0 - x1) / (y0 - y1)) * ((double)a - y1);
            if (map->layout[a / tile_size][(int) xa / tile_size] == WALL)
                return 0;
            if (map->layout[a / tile_size - 1][(int) xa / tile_size] == WALL)
                return 0;
        }
    }
    return 1;
}

static int enemy_see_player(const Enemy * enemy, const Player * player, const Map * map, int tile_size) {
    double x0, y0, x1, y1;
    x0 = enemy->x;
    y0 = enemy->y;
    x1 = player->x;
    y1 = player->y;
    if (square_double(x0 - x1) + square_double(y0 - y1) > square_double(enemy->field_of_view * tile_size)) {
        return 0;
    }
    if (!check_col(x0, y0, x1, y1, enemy->field_of_view, map, tile_size)) {
        return 0;
    }
    if (!check_line(x0, y0, x1, y1, enemy->field_of_view, map, tile_size)) {
        return 0;
    }
    return 1;
}

int enemies_see_player(const Enemies * enemies, const Player * player, const Map * map, int tile_size) {
    int i;
    if (player->visible == 0) {
        return 0;
    }
    for (i = 0; i < NB_ENEMIES; ++i) {
        if (enemy_see_player(&enemies->list[i], player, map, tile_size)) {
            return 1;
        }
    }
    return 0;
}

static int enemy_see_relic(const Enemy * enemy, const Relic * relic, const Map * map, int tile_size) {
    double x0, y0, x1, y1;
    x0 = enemy->x;
    y0 = enemy->y;
    x1 = relic->x;
    y1 = relic->y;
    if (square_double(x0 - x1) + square_double(y0 - y1) > square_double(enemy->field_of_view * tile_size)) {
        return 0;
    }
    if (!check_col(x0, y0, x1, y1, enemy->field_of_view, map, tile_size)) {
        return 0;
    }
    if (!check_line(x0, y0, x1, y1, enemy->field_of_view, map, tile_size)) {
        return 0;
    }
    return 1;
}

int enemies_see_relics(Enemies * enemies, Relics * relics, const Map * map, int tile_size) {
    int i, j;
    for (i = 0; i < NB_RELICS; ++i) {
        if (relics->list[i].is_present || !relics->list[i].is_not_detected) {
            continue;
        }
        for (j = 0; j < NB_ENEMIES; ++j) {
            if (enemy_see_relic(&enemies->list[j], &relics->list[i], map, tile_size)) {
                activate_panic_mode(enemies);
                relics->list[i].is_not_detected = 0;
                return 1;
            }
        }
    }
    return 0;
}

void update_panic_time(Enemies * enemies, double frametime) {
    if (!enemies->is_panic_mode) {
        return;
    }
    if (enemies->panic_time <= 0) {
        desactivate_panic_mode(enemies);
        return;
    }
    enemies->panic_time -= frametime;
}

int init_enemies_for_menu(Enemies * enemies, int tile_size) {
    int i;

    enemies->is_panic_mode = 0;
    enemies->panic_time = 0;
    for (i = 0; i < NB_ENEMIES; ++i) {
        enemies->list[i].x = 75 * tile_size;
        enemies->list[i].y = 40 * tile_size;
        enemies->list[i].dir = rand() % 4;
        enemies->list[i].speed = (0.3 + 0.5 * rand() / (double) RAND_MAX);
        enemies->list[i].field_of_view = 4;
    }

    return 0;
}
