/*
 * Stealth
 * December 2022 - January 2023
 * BERNIER Valentin - VILAYVANH Mickael
 */

#ifndef _LEADERBOARD_
#define _LEADERBOARD_


#define _POSIX_C_SOURCE 199309L
#include <time.h>
#define MAX_NAME_LENGHT 20
#define MAX_ELEM 100
#include <stdlib.h>
#include <stdio.h>

typedef struct data {
    char name[MAX_NAME_LENGHT + 1];
    int minutes;
    int seconds;
    int mana;
    long milliseconds;
} Data;

typedef enum label {
    TIME,
    MANA
} Label;

typedef struct leaderboard {
    int nmemb;
    Label label;
    Data array[MAX_ELEM];
} Leaderboard;

/**
 * @brief Save score in binary file
 * 
 * @param start time at the start of the game
 * @param end time at the end of the game
 * @param mana_consumed mana consumed by user
 * @param name nickname used
 * @return int Error code
 */
int save_score(const struct timespec *start, const struct timespec *end, int mana_consumed, const char *name);

/**
 * @brief Print in terminal binary file's content
 * 
 * @param name file's path
 */
void print_bin_content(const char *name);

#endif