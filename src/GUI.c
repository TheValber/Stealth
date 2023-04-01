/*
 * Stealth
 * December 2022 - January 2023
 * BERNIER Valentin - VILAYVANH Mickael
 */

#include "../include/GUI.h"
#include "../include/Util.h"
#include "../include/Spells.h"

static void display_player(const Player *player, int square_size){
    MLV_draw_filled_circle(player->x, player->y, square_size / 2, MLV_rgba(0, 255, 255 * (player->speed > 1), 100 + 155 * player->visible));
    MLV_draw_circle(player->x, player->y, square_size / 2, MLV_COLOR_BLACK);
}

static void display_mana_gauge(const Player *player, const Map * map, int square_size, const Font * font){
    int x, y, width;
    x = (map->width + 1) * square_size;
    y = 2 * square_size;
    width = WIN_WIDTH - x - square_size;
    MLV_draw_filled_rectangle(x, y, width * player->mana / player->mana_max, 30, MLV_COLOR_ROYAL_BLUE);
    MLV_draw_rectangle(x - 1, y, width, 30, MLV_COLOR_BLACK);
    MLV_draw_text_box_with_font(x, y, width, 30, "%d / %d", font->game_gui, 0,
        MLV_rgba(0, 0, 0, 0), MLV_COLOR_BLACK, MLV_rgba(0, 0, 0, 0), MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER,
        player->mana, player->mana_max);
}

static void display_map(const Map * map, int square_size) {
    int i, j;

    for (i = 0; i < map->height; ++i) {
        for (j = 0; j < map->width; ++j) {
            /* Floor or door */
            if (map->layout[i][j] == FLOOR || map->layout[i][j] == DOOR) {
                MLV_draw_rectangle(j * square_size, i * square_size, square_size, square_size, MLV_COLOR_GREY42);
                /* Mana tile */
                if (map->mana_tiles.layout[i][j] == MANA_TILE) {
                    MLV_draw_filled_circle((j + 0.5) * square_size, (i + 0.5) * square_size, square_size / 8, MLV_COLOR_ROYAL_BLUE);
                    MLV_draw_circle((j + 0.5) * square_size, (i + 0.5) * square_size, square_size / 8, MLV_COLOR_BLUE);
                }
            }
            /* Wall */
            else if (map->layout[i][j] == WALL) {
                MLV_draw_filled_rectangle(j * square_size, i * square_size, square_size, square_size, MLV_COLOR_BLACK);
            }
            /* Exit tile */
            else if (map->layout[i][j] == EXIT) {
                MLV_draw_filled_rectangle(j * square_size, i * square_size, square_size, square_size, MLV_rgba(50, 150, 50, 255));
            }
        }
    }
}

static void display_enemies(const Enemies * enemies, int square_size) {
    int i, j;
    for (i = 0; i < NB_ENEMIES; ++i) {
        MLV_draw_filled_circle(enemies->list[i].x, enemies->list[i].y, square_size / 2, MLV_COLOR_RED);
        MLV_draw_circle(enemies->list[i].x, enemies->list[i].y, square_size / 2, MLV_COLOR_BLACK);

        for (j = 0; j < 3; ++j) {
            MLV_draw_circle(enemies->list[i].x, enemies->list[i].y, enemies->list[i].field_of_view * square_size - j, MLV_COLOR_RED);
        }
    }
}

static void display_relics(const Relics * relics, int square_size) {
    int i;
    char text[2];
    for (i = 0; i < NB_RELICS; ++i) {
        MLV_draw_text(relics->list[i].x, relics->list[i].y, text, MLV_COLOR_BLACK);
        MLV_draw_filled_circle(relics->list[i].x, relics->list[i].y, (square_size / (2 + 2 * !relics->list[i].is_present)), MLV_rgba(255, 255, 0, 255 * relics->list[i].is_not_detected));
        MLV_draw_circle(relics->list[i].x, relics->list[i].y, (square_size / (2 + 2 * !relics->list[i].is_present)), MLV_rgba(0, 0, 0, 255 * relics->list[i].is_not_detected));
    }
}

static void display_enemy_panic(const Enemies * enemies, const Map * map, int square_size, const Font * font) {
    int x, y;
    if (!enemies->is_panic_mode) {
        return;
    }
    x = map->width * square_size;
    y = 6 * square_size;
    MLV_draw_text_box_with_font(x, y, WIN_WIDTH - x, 30, "PANIC MODE", font->game_gui, 0,
        MLV_rgba(0, 0, 0, 0), MLV_COLOR_RED, MLV_rgba(0, 0, 0, 0), MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    MLV_draw_text_box_with_font(x, y + 30, WIN_WIDTH - x, 30, "%d", font->game_gui, 0,
        MLV_rgba(0, 0, 0, 0), MLV_COLOR_RED, MLV_rgba(0, 0, 0, 0), MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER,
        (int) enemies->panic_time);

    MLV_draw_filled_rectangle(0, 0, WIN_WIDTH, WIN_HEIGHT, MLV_rgba(255, 0, 0, 50 * abs_double(sin_double(30 - enemies->panic_time))));
}

static void display_quit_info(const Map * map, int square_size, const Font * font) {
    int x, y;
    x = map->width * square_size;
    y = WIN_HEIGHT - 6 * square_size;
    MLV_draw_text_box_with_font(x, y, WIN_WIDTH - x, 30, "Press ESCAPE to quit", font->game_gui, 0,
        MLV_rgba(0, 0, 0, 0), MLV_COLOR_BLACK, MLV_rgba(0, 0, 0, 0), MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
}

void update_gui(const Player *player, const Map * map, const Enemies * enemies, const Relics * relics, const Font * font) {
    int square_size;

    square_size = tile_size(map);

    MLV_draw_filled_rectangle(0, 0, WIN_WIDTH, WIN_HEIGHT, MLV_COLOR_GREY80);

    display_map(map, square_size);
    display_relics(relics, square_size);
    display_player(player, square_size);
    display_enemies(enemies, square_size);
    display_mana_gauge(player, map, square_size, font);
    display_enemy_panic(enemies, map, square_size, font);
    display_quit_info(map, square_size, font);

    MLV_update_window();
}

int tile_size(const Map * map) {
    return min(WIN_WIDTH / map->width, WIN_HEIGHT / map->height);
}

double wait_time_for_FPS(struct timespec * end_time, struct timespec * new_time) {
    double frametime, extratime;

    clock_gettime(CLOCK_REALTIME, new_time);

    frametime = new_time->tv_sec - end_time->tv_sec;
    frametime += (new_time->tv_nsec - end_time->tv_nsec) / 1.0E9;

    extratime = 1.0 / FPS - frametime;

    if (extratime > 0) {
        MLV_wait_milliseconds((int)(extratime * 1000));
    }

    clock_gettime(CLOCK_REALTIME, end_time);

    return frametime + (extratime >= 0) ? extratime : 0;
}

static void display_menu_map(const Map * map, int square_size) {
    int i, j;

    for (i = 0; i < map->height; ++i) {
        for (j = 0; j < map->width; ++j) {
            if (map->layout[i][j] == WALL) {
                MLV_draw_filled_rectangle(j * square_size, i * square_size, square_size, square_size, MLV_COLOR_BLACK);
            }
            else if (map->layout[i][j] == EXIT) {
                MLV_draw_filled_rectangle(j * square_size, i * square_size, square_size, square_size, MLV_rgba(50, 150, 50, 255));
            }
        }
    }
}

static void display_leaderboard(const Leaderboard *lb, int x, int y, int tile_size, const FILE *ptr, const Font *font){
    int i, y0, n;
    char text[100];
    y0 = y + tile_size * 2;
    MLV_draw_rectangle(x, y, 25 * tile_size, tile_size * 2, MLV_COLOR_BLACK);
    MLV_draw_rectangle(x, y0, 25 * tile_size, 10 * tile_size * 2, MLV_COLOR_BLACK);
    if (ptr){
        if (lb->label == MANA){
            strcpy(text, "  NAME  | BY MANA CONSUMPTION  ");
        } else {
            strcpy(text, "  NAME  | BY TIME  ");
        }
        MLV_draw_text_with_font(x + 1, y, text, font->game_gui, MLV_COLOR_RED);
        n = min(lb->nmemb, 10);
        for (i = 0; i < n; i++){
            if (lb->label == MANA){
                sprintf(text, "%s - %d", lb->array[i].name, lb->array[i].mana);
            } else {
                sprintf(text, "%s - %d:%d:%3ld", 
                    lb->array[i].name, lb->array[i].minutes, lb->array[i].seconds,
                    lb->array[i].milliseconds);
            }
            MLV_draw_text_with_font(x + tile_size, y0 + tile_size * 2 * i, text, font->game_gui, MLV_COLOR_BLACK);
        }
    }
}

void display_leaderboards(const char *fpath1, const char *fpath2){
    Leaderboard lb_time;
    Leaderboard lb_mana;
    Font font;
    int square_size, width, height, quit;
    load_font(&font);
    width = 80;
    height = 45;
    quit = 0;
    FILE *fp1 = fopen(fpath1, "rb");
    FILE *fp2 = fopen(fpath2, "rb");
    square_size = min(WIN_WIDTH / width, WIN_HEIGHT / height);
    if (fp1){
        fread(&lb_time, sizeof(Leaderboard), 1, fp1);
        fclose(fp1);
    }
    if (fp2){
        fread(&lb_mana, sizeof(Leaderboard), 1, fp2);
        fclose(fp2);
    }
    MLV_draw_filled_rectangle(0, 0, WIN_WIDTH, WIN_HEIGHT, MLV_COLOR_GREY80);

    MLV_draw_text_with_font((width / 2 - 5) * square_size, 2 * square_size, 
                            "Leaderboard", font.game_gui, MLV_COLOR_BLACK);
    display_leaderboard(&lb_time, square_size * 8, 
                        square_size * (height/ 3 - 5), square_size, fp1, &font);
    display_leaderboard(&lb_mana, square_size * (width / 2 + 5), 
                        square_size * (height/ 3 - 5), square_size, fp2, &font);
    MLV_draw_text_with_font((width / 2 - 5) * square_size, (height - 2) * square_size,
                            "Press ESCAPE to quit", font.game_gui, MLV_COLOR_BLACK);
    MLV_update_window();
    free_font(&font);

    while (!quit){
        MLV_wait_keyboard(NULL, NULL, NULL);
        if (is_escape_pressed())
            quit = 1;
    }
}

void update_menu_gui(const Player * player, const MenuImages * menu_images, const Map * map, const Enemies * enemies, const Relics * relics) {
    int square_size;

    square_size = tile_size(map);

    MLV_draw_filled_rectangle(0, 0, WIN_WIDTH, WIN_HEIGHT, MLV_COLOR_GREY80);

    display_menu_map(map, square_size);
    display_relics(relics, square_size);
    display_enemies(enemies, square_size);

    MLV_draw_image(menu_images->menu_overlay, 0, 0);

    display_player(player, square_size);

    MLV_update_window();
}

void display_end_screen(int status, Font * font) {
    MLV_draw_filled_rectangle(0, 0, WIN_WIDTH, WIN_HEIGHT, MLV_rgba(0, 0, 0, 200));

    if (status) {
        MLV_draw_text_box_with_font(0, 0, WIN_WIDTH, WIN_HEIGHT, "You win", font->end_screen, 0,
            MLV_rgba(0, 0, 0, 0), MLV_COLOR_GREEN, MLV_rgba(0, 0, 0, 0), MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    } else {
        MLV_draw_text_box_with_font(0, 0, WIN_WIDTH, WIN_HEIGHT, "You lose", font->end_screen, 0,
            MLV_rgba(0, 0, 0, 0), MLV_COLOR_RED, MLV_rgba(0, 0, 0, 0), MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    }

    MLV_draw_text_box_with_font(0, WIN_HEIGHT / 2, WIN_WIDTH, WIN_HEIGHT / 2, "Press ESCAPE to quit", font->game_gui, 0,
            MLV_rgba(0, 0, 0, 0), MLV_COLOR_WHITE, MLV_rgba(0, 0, 0, 0), MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);

    MLV_update_window();
}

void ask_name(Font * font, char ** text) {
    MLV_draw_text_box_with_font(0, 0, WIN_WIDTH, WIN_HEIGHT / 3, "Enter your name (Return to validate)", font->game_gui, 0,
            MLV_rgba(0, 0, 0, 0), MLV_COLOR_WHITE, MLV_rgba(0, 0, 0, 0), MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    MLV_wait_input_box_with_font(WIN_WIDTH / 2 - WIN_WIDTH / 8, WIN_HEIGHT / 5, WIN_WIDTH / 4, 50, MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_rgba(0, 0, 0, 0), "", text, font->game_gui);
}
