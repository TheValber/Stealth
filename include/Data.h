/*
 * Stealth
 * December 2022 - January 2023
 * BERNIER Valentin - VILAYVANH Mickael
 */

#ifndef __DATA__
#define __DATA__

#include <MLV/MLV_all.h>

typedef struct {
    MLV_Image * menu_overlay;
} MenuImages;

typedef struct {
    MLV_Font * game_gui;
    MLV_Font * end_screen;
} Font;

/**
 * @brief Load all images for the menu
 * 
 * @param menu_image Pointer to menu_image that stock all images
 */
void load_menu_images(MenuImages * menu_images);

/**
 * @brief Free memory allocated for all images.
 * 
 * @param menu_images Pointer to menu_image that stock all images
 */
void free_menu_images(MenuImages * menu_images);

/**
 * @brief Load all fonts
 * 
 * @param font Pointer to font that stock all fonts
 */
void load_font(Font * font);

/**
 * @brief Free memory allocated for all fonts
 * 
 * @param font Pointer to font that stock all fonts
 */
void free_font(Font * font);

#endif
