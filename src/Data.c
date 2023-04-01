/*
 * Stealth
 * December 2022 - January 2023
 * BERNIER Valentin - VILAYVANH Mickael
 */

#include "../include/Data.h"

void load_menu_images(MenuImages * menu_images) {
    menu_images->menu_overlay = MLV_load_image("./data/menu/menu.png");
}

void free_menu_images(MenuImages * menu_images) {
    MLV_free_image(menu_images->menu_overlay);
}

void load_font(Font * font) {
    font->game_gui = MLV_load_font("./data/font/Welbut.ttf", 20);
    font->end_screen = MLV_load_font("./data/font/Welbut.ttf", 100);
}

void free_font(Font * font) {
    MLV_free_font(font->game_gui);
    MLV_free_font(font->end_screen);
}
