/*
 * Stealth
 * December 2022 - January 2023
 * BERNIER Valentin - VILAYVANH Mickael
 */

#include "../include/Game.h"
#include "../include/Menu.h"

int main(void) {
    int res;

    srand(time(NULL));

    MLV_create_window("Stealth", NULL, WIN_WIDTH, WIN_HEIGHT);

    while (1) {
        res = main_menu();
        if (res == -1) {
            exit(EXIT_FAILURE);
        }
        if (res == 1) {
            if (game() == -1) {
                exit(EXIT_FAILURE);
            }
        }
        else if (res == 2) {
            display_leaderboards("./data/lb/lb_time.bin", "./data/lb/lb_mana.bin");
        }
        else if (res == 3) {
            break;
        }
    }
    
    MLV_free_window();

    return 0;
}
