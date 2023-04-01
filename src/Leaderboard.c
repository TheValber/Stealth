/*
 * Stealth
 * December 2022 - January 2023
 * BERNIER Valentin - VILAYVANH Mickael
 */

#include "../include/Leaderboard.h"

#include <string.h>

int init_leaderboard(const char *name, Leaderboard **dest){
    FILE *in;
    Leaderboard *res = (Leaderboard *)malloc(sizeof(Leaderboard));
    if (!res){
        return -1;
    }
    in = fopen(name, "rb");
    /* if non existant, we create this file and initialize for the first time */
    if (!in){
        in = fopen(name, "wb");
        res->nmemb = 0;
    } else {
        /* If there is data in file */
        fread(res, sizeof(Leaderboard), 1, in);
    }
    fclose(in);
    /* Assigning label */
    if (strcmp(name, "./data/lb/lb_time.bin") == 0){
        res->label = TIME;
    } else if (strcmp(name, "./data/lb/lb_mana.bin") == 0) {
        res->label = MANA;
    } else {
        /* Illegal name if other than these two */
        free(res);
        return -1;
    }
    *dest = res;
    return 0;
}

int mana_consumption_cmp(const void *ptr1, const void *ptr2){
    Data *tmp1 = (Data *)ptr1;
    Data *tmp2 = (Data *)ptr2;
    return tmp1->mana - tmp2->mana;
}

int time_cmp(const void *ptr1, const void *ptr2){
    Data *tmp1 = (Data *)ptr1;
    Data *tmp2 = (Data *)ptr2;
    int time1 = tmp1->minutes * 60 * 1000 + tmp1->seconds * 1000 + tmp1->milliseconds;
    int time2 = tmp2->minutes * 60 * 1000 + tmp2->seconds * 1000 + tmp2->milliseconds;
    return time1 - time2;
}

static int write_score_by_label(const char *name, Leaderboard *ptr){
    FILE *in = fopen(name, "wb");
    if (!in){
        return -1;
    }
    /* If max capacity is reached, we erase worst score */
    if (ptr->nmemb < MAX_ELEM - 1)
        ptr->nmemb++;
    /* Before writing, we sort leaderboard's array */
    if (ptr->label == MANA){
        qsort(ptr->array, ptr->nmemb, sizeof(Data), mana_consumption_cmp);
    } else {
        qsort(ptr->array, ptr->nmemb, sizeof(Data), time_cmp);
    }
    /* Writing state of leaderboard */
    fwrite(ptr, sizeof(Leaderboard), 1, in);
    fclose(in);
    return 0;
}

static void add_score(const Data *data, Leaderboard *dest){
    strcpy(dest->array[dest->nmemb].name, data->name);
    dest->array[dest->nmemb].mana = data->mana;
    dest->array[dest->nmemb].minutes = data->minutes;
    dest->array[dest->nmemb].seconds = data->seconds;
    dest->array[dest->nmemb].milliseconds = data->milliseconds;
}

int write_score(Leaderboard *lb_time, Leaderboard *lb_mana, const Data *score){
    add_score(score, lb_time);
    add_score(score, lb_mana);
    if (write_score_by_label("./data/lb/lb_mana.bin", lb_mana) == -1){
        return -1;
    }
    if (write_score_by_label("./data/lb/lb_time.bin", lb_time) == -1){
        return -1;
    }
    return 0;
}

int save_score(const struct timespec *start, const struct timespec *end, int mana_consumed, const char *name){
    Leaderboard *lb_time;
    Leaderboard *lb_mana;
    Data data;
    if (init_leaderboard("./data/lb/lb_time.bin", &lb_time) == -1){
        return -1;
    }
    if (init_leaderboard("./data/lb/lb_mana.bin", &lb_mana) == -1){
        return -1;
    }
    strcpy(data.name, name);
    data.mana = mana_consumed;
    data.minutes = (end->tv_sec - start->tv_sec) / 60;
    data.seconds = (end->tv_sec - start->tv_sec) % 60;
    data.milliseconds = (end->tv_nsec - start->tv_nsec) / 1000000;
    if (write_score(lb_time, lb_mana, &data) == -1){
        return -1;
    }
    free(lb_time);
    free(lb_mana);
    return 0;
}


void print_bin_content(const char *name){
    int i;
    Leaderboard tmp;
    FILE *in = fopen(name, "rb");
    if (!in){
        return;
    }
    fread(&tmp, sizeof(Leaderboard), 1, in);
    fclose(in);
    for (i = 0; i < tmp.nmemb; i++){
        if (tmp.label == MANA){
            printf("NAME: %s, MANA CONS: %d\n", 
                tmp.array[i].name, tmp.array[i].mana);
        } else {
            printf("NAME : %s, TIME: %d:%d:%3ld\n", 
                tmp.array[i].name, tmp.array[i].minutes, 
                tmp.array[i].seconds, tmp.array[i].milliseconds);
        }
    }
}
