/*
 * Stealth
 * December 2022 - January 2023
 * BERNIER Valentin - VILAYVANH Mickael
 */

#include "../include/Gameplay.h"

static void get_direction(Direction *dir){
    /* To change:
        Instead of calling MLV_get_keyboard_state on selected keys,
        we should create an array for keybinding option and select
        correct keys from there to get the right direction.    
    */
    /* South direction */
    if (MLV_get_keyboard_state(MLV_KEYBOARD_DOWN) == MLV_PRESSED){
        if (MLV_get_keyboard_state(MLV_KEYBOARD_LEFT) == MLV_PRESSED)
            *dir = DOWN_LEFT;
        else if (MLV_get_keyboard_state(MLV_KEYBOARD_RIGHT) == MLV_PRESSED)
            *dir = DOWN_RIGHT;
        else
            *dir = DOWN;
    } 
    /* North direction */
    else if (MLV_get_keyboard_state(MLV_KEYBOARD_UP) == MLV_PRESSED) {
        if (MLV_get_keyboard_state(MLV_KEYBOARD_LEFT) == MLV_PRESSED)
            *dir = UP_LEFT;
        else if (MLV_get_keyboard_state(MLV_KEYBOARD_RIGHT) == MLV_PRESSED)
            *dir = UP_RIGHT;
        else
            *dir = UP;
    } 
    /* West direction */
    else if (MLV_get_keyboard_state(MLV_KEYBOARD_LEFT) == MLV_PRESSED)
        *dir = LEFT;
    /* East direction */
    else if (MLV_get_keyboard_state(MLV_KEYBOARD_RIGHT) == MLV_PRESSED)
        *dir = RIGHT;
    /* Case no legal direction key pressed */
    else
        *dir = NONE;
}

static void skill_activation(Player *player, double *limit, Linkedlink *ptr){
    if (MLV_get_keyboard_state(MLV_KEYBOARD_SPACE) == MLV_PRESSED && player->mana - 2 >= 0){
        turn_off_visibility(player);
        player->mana -= 2;
        player->mana_consumed += 2;
        /* Compute redistribution of mana in mana tiles */
        if (ptr){
            release_mana(ptr);
            release_mana(ptr);
        }
    } else {
        turn_on_visibility(player);
    }
    if (MLV_get_keyboard_state(MLV_KEYBOARD_LSHIFT) == MLV_PRESSED && player->mana - 1 >= 0){
        *limit = 1.2;
        player->mana -= 1;
        player->mana_consumed++;
        /* Compute redistribution of mana in mana tiles */
        if (ptr){
            release_mana(ptr);
        }
    }
}

void player_action(Player *player, const Map *map, int tile_size, Linkedlink *ptr){
    Direction dir;
    double velocity_limit = 0.9;
    /* Get direction from user */
    get_direction(&dir);
    /* Activate skills if requirements are met */
    skill_activation(player, &velocity_limit, ptr);
    /* Move player if input allows it */
    if (map == NULL || !check_wall_collide_by_direction(player, dir, map, V, tile_size)){
        update_player_velocity(player, dir, SPEED_MULTIPLIER, V, velocity_limit);
    } else {
        reset_player_velocity(player, dir);
    }
}

int is_return_pressed() {
    return MLV_get_keyboard_state(MLV_KEYBOARD_RETURN) == MLV_PRESSED;
}

int is_escape_pressed() {
    return MLV_get_keyboard_state(MLV_KEYBOARD_ESCAPE) == MLV_PRESSED;
}
