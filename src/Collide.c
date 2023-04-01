/*
 * Stealth
 * December 2022 - January 2023
 * BERNIER Valentin - VILAYVANH Mickael
 */

#include "../include/Collide.h"

static int compute_dist_calculation(int li, int col, double x0, double y0, int tile_size, double radius){
    double x1, y1, dist;
    x1 = col * tile_size;
    y1 = li * tile_size;
    /* Euclidian distance calc */
    dist = sqrt_double(square_double(x0 - x1) + square_double(y0 - y1));
    /* Distance shorter than radius means that they are at least incountering each other */
    if (dist < radius)
        return -1;
    return 0;
}

static int check_collide_up(const void *ptr, const Map *map, double base_velocity, int tile_size){
    int li, col;
    double x0, y0, radius;
    Entity *entity = (Entity *)ptr;
    /* We calculate the next distance to add after computing velocity */
    double tmp_dist = base_velocity * entity->speed;
    x0 = entity->x;
    /* Adding distance */
    y0 = entity->y - tmp_dist;
    radius = tile_size * 0.5;
    /* Converting current coordinates into respective line/column */
    li = coord_to_case(y0 + radius, tile_size);
    col = coord_to_case(x0 + radius, tile_size);
    /* Straight top wall */
    if (map->layout[li - 1][col] == WALL){
        if (y0 < (li + 0.5) * tile_size){
            return -1;
        }
        return 0;
    }
    /* Top left wall */
    if (map->layout[li][col - 1] != WALL && map->layout[li - 1][col - 1] == WALL && x0 < (col + 0.5) * tile_size){
        return compute_dist_calculation(li, col, x0, y0, tile_size, radius);
    }
    /* Top right wall */
    if (map->layout[li][col + 1] != WALL && map->layout[li - 1][col + 1] == WALL && x0 >= (col + 0.5) * tile_size){
        return compute_dist_calculation(li, col + 1, x0, y0, tile_size, radius);
    }
    return 0;
}

static int check_collide_right(const void *ptr, const Map *map, double base_velocity, int tile_size){
    int li, col;
    double x0, y0, radius, tmp_dist;
    Entity *entity = (Entity *)ptr;
    tmp_dist = base_velocity * entity->speed;
    x0 = entity->x + tmp_dist;
    y0 = entity->y;
    radius = tile_size * 0.5;
    li = coord_to_case(y0 + radius, tile_size);
    col = coord_to_case(x0 + radius, tile_size);
    /* Straight right wall */
    if (map->layout[li][col + 1] == WALL){
        if (x0 > (col + 0.5) * tile_size){
            return -1;
        }
        return 0;
    }
    /* Top right wall */
    if (map->layout[li - 1][col] != WALL && map->layout[li - 1][col + 1] == WALL && y0 < (li + 0.5) * tile_size){
        return compute_dist_calculation(li, col + 1, x0, y0, tile_size, radius);
    }
    /* Down right wall */
    if (map->layout[li + 1][col] != WALL && map->layout[li + 1][col + 1] == WALL && y0 >= (li + 0.5) * tile_size){
        return compute_dist_calculation(li + 1, col + 1, x0, y0, tile_size, radius);
    }
    return 0;
}

static int check_collide_left(const void *ptr, const Map *map, double base_velocity, int tile_size){
    int li, col;
    double x0, y0, radius;
    Entity *entity = (Entity *)ptr;
    double tmp_dist = base_velocity * entity->speed;
    x0 = entity->x - tmp_dist;
    y0 = entity->y;
    radius = tile_size * 0.5;
    li = coord_to_case(y0 + radius, tile_size);
    col = coord_to_case(x0 + radius, tile_size);
    /* Straight left wall */
    if (map->layout[li][col - 1] == WALL){
        if (x0 < (col + 0.5) * tile_size){
            return -1;
        }
        return 0;
    }
    /* Top left wall */
    if (map->layout[li - 1][col] != WALL && map->layout[li - 1][col - 1] == WALL && y0 < (li + 0.5) * tile_size){
        return compute_dist_calculation(li, col, x0, y0, tile_size, radius);
    }
    /* Down left wall */
    if (map->layout[li + 1][col] != WALL && map->layout[li + 1][col - 1] == WALL && y0 >= (li + 0.5) * tile_size){
        return compute_dist_calculation(li + 1, col, x0, y0, tile_size, radius);
    }
    return 0;
}

static int check_collide_down(const void *ptr, const Map *map, double base_velocity, int tile_size){
    int li, col;
    double x0, y0, radius;
    Entity *entity = (Entity *)ptr;
    double tmp_dist = base_velocity * entity->speed;
    x0 = entity->x;
    y0 = entity->y + tmp_dist;
    radius = tile_size * 0.5;
    li = coord_to_case(y0 + radius, tile_size);
    col = coord_to_case(x0 + radius, tile_size);
    /* Straight down wall */
    if (map->layout[li + 1][col] == WALL){
        if (y0 > (li + 0.5) * tile_size){
            return -1;
        }
        return 0;
    }
    /* Down left wall */
    if (map->layout[li][col - 1] != WALL && map->layout[li + 1][col - 1] == WALL && x0 < (col + 0.5) * tile_size){
        return compute_dist_calculation(li + 1, col, x0, y0, tile_size, radius);
    }
    /* Down right wall */
    if (map->layout[li][col + 1] != WALL && map->layout[li + 1][col + 1] == WALL && x0 >= (col + 0.5) * tile_size){
        return compute_dist_calculation(li + 1, col + 1, x0, y0, tile_size, radius);
    }
    return 0;
}

static int is_on_wall(const void *ptr, const Map *map, int tile_size) {
    double x0, y0, radius;
    Entity *entity = (Entity *)ptr;
    x0 = entity->x;
    y0 = entity->y;
    radius = tile_size * 0.5;
    return WALL == map->layout[coord_to_case(y0 + radius, tile_size)][coord_to_case(x0 + radius, tile_size)];
}

int check_wall_collide_by_direction(const void *ptr, Direction dir, const Map *map, 
                                    double base_velocity, int tile_size){
    Entity *entity = (Entity *)ptr;
    int code = 0;

    if (is_on_wall(entity, map, tile_size)) {
        return -1;
    }

    switch(dir){
        case DOWN:
            code = check_collide_down(entity, map, base_velocity, tile_size);
            break;
        case UP:
            code = check_collide_up(entity, map, base_velocity, tile_size);
            break;
        case RIGHT:
            code = check_collide_right(entity, map, base_velocity, tile_size);
            break;
        case LEFT:
            code = check_collide_left(entity, map, base_velocity, tile_size);
            break;
        case DOWN_LEFT:
            code = check_collide_down(entity, map, base_velocity, tile_size);
            if (code == 0)
                code = check_collide_left(entity, map, base_velocity, tile_size);
            break;
        case DOWN_RIGHT:
            code = check_collide_down(entity, map, base_velocity, tile_size);
            if (code == 0)
                code = check_collide_right(entity, map, base_velocity, tile_size);
            break;
        case UP_LEFT:
            code = check_collide_up(entity, map, base_velocity, tile_size);
            if (code == 0)
                code = check_collide_left(entity, map, base_velocity, tile_size);
            break;
        case UP_RIGHT:
            code = check_collide_up(entity, map, base_velocity, tile_size);
            if (code == 0)
                code = check_collide_right(entity, map, base_velocity, tile_size);
            break;
        default:
            break;
    }
    return code;
}