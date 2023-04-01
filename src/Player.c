/*
 * Stealth
 * December 2022 - January 2023
 * BERNIER Valentin - VILAYVANH Mickael
 */

#include "../include/Player.h"

#include <stdio.h>

int init_player(int start_x, int start_y, Player **dest){
    Player *res = (Player *)malloc(sizeof(Player));
    if (!res) 
        return -1;
    res->x = (double) start_x;
    res->y = (double) start_y;
    res->mana = 0;
    res->mana_max = 500;
    res->dir = NONE;
    res->visible = 1;
    res->speed = BASE_SPEED;
    res->mana_consumed = 0;
    *dest = res;
    return 0;
}

void reset_player_velocity(Player *player, Direction direction){
    player->speed = BASE_SPEED;
}

static void update_player_position(Player *player, double base_speed){
    Direction dir = player->dir;
    double dist = base_speed * player->speed;
    /* In case a diagonal direction is set, we use Pythagore's principale */
    double diagonal_dist = sqrt_double(square_double(dist) / 2);
    switch(dir){
        case UP:
            player->y -= dist;
            break;
        case DOWN:
            player->y += dist;
            break;
        case RIGHT:
            player->x += dist;
            break;
        case LEFT:
            player->x -= dist;
            break;
        case DOWN_LEFT:
            player->y += diagonal_dist;
            player->x -= diagonal_dist;
            break;
        case DOWN_RIGHT:
            player->y += diagonal_dist;
            player->x += diagonal_dist;
            break;
        case UP_LEFT:
            player->y -= diagonal_dist;
            player->x -= diagonal_dist;
            break;
        case UP_RIGHT:
            player->y -= diagonal_dist;
            player->x += diagonal_dist;
            break;
        default:
            break;
    }
}

void turn_on_visibility(Player *player){
    player->visible = 1;
}

void turn_off_visibility(Player *player){
    player->visible = 0;
}

void update_player_velocity(Player *player, Direction direction,
    double base_speed_multiplier, double base_velocity, double limit){
    player->dir = direction;
    if (direction != NONE){
        player->speed = min_double(player->speed + base_speed_multiplier, limit);
        /*
            There was an else to reset velocity in case direction changed but
            we decided to remove it to give more fluidity on the gameplay
        */
        /* Update player position based on his new speed and direction */
        update_player_position(player, base_velocity);
    } else {
        reset_player_velocity(player, direction);
    }
}

void player_mana_check_from_tiles(Player *player, Map *map, int tile_size, Linkedlink *ptr){
    int pix, index_x, index_y;
    double theta, radius;
    radius = (double)tile_size * 0.5;
    /* In range of a tile's side, we consider all pix to get right angle 
    from topleft, clockwise. Particularly: if pix = 0 or pix = tile_size,
    the angle is the same since we've done a whole circle */
    for (pix = 0; pix < tile_size; pix++){
        if (player->mana >= player->mana_max) {
            return;
        }
        theta = 2 * PI * pix / tile_size;
        /* Convert coordinate to index */
        index_x = coord_to_case(player->x + radius + cos(theta) * radius, tile_size);
        index_y = coord_to_case(player->y + radius + sin(theta) * radius, tile_size);
        /* Checking if the tile contains mana */
        if (map->mana_tiles.layout[index_y][index_x] == MANA_TILE){
            player->mana++;
            map->mana_tiles.layout[index_y][index_x] = VOID_TILE;
            /* Store address of tile with mana consumed */
            add_address(&(map->mana_tiles.layout[index_y][index_x]), ptr);
        }
    }
}

void free_player(Player *player){
    free(player);
}

void take_relics(Player * player, Relics * relics, int tile_size) {
    int i;
    double px, py, rx, ry;

    px = player->x;
    py = player->y;

    for (i = 0; i < NB_RELICS; ++i) {
        rx = relics->list[i].x;
        ry = relics->list[i].y;
        if (square_double(px - rx) + square_double(py - ry) <= square_double(tile_size)) {
            relics->list[i].is_present = 0;
        }
    }
}
