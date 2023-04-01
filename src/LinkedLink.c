/*
 * Stealth
 * December 2022 - January 2023
 * BERNIER Valentin - VILAYVANH Mickael
 */

#include "../include/LinkedLink.h"

#include <stdio.h>

static int init_link(void *addr, Link **dest){
    Link *res;
    if (!(res = (Link *)malloc(sizeof(Link)))){
        return -1;
    }
    res->addr = addr;
    res->next = NULL;
    *dest = res;
    return 0;
}

void init_linkedlink(Linkedlink *dest){
    dest->nmemb = 0;
    dest->head = NULL;
}

int add_address(void *addr, Linkedlink *dest){
    Link *add;
    if (init_link(addr, &add) == -1){
        return -1;
    }
    add->next = dest->head;
    dest->head = add;
    dest->nmemb++;
    return 0;
}

static void free_all_link(Link *ptr){
    if (ptr){
        free_all_link(ptr->next);
        free(ptr);
    }
    
}

void free_linkedlink(Linkedlink *ptr){
    free_all_link(ptr->head);
    ptr->nmemb = 0;
    ptr->head = NULL;
}

void print_link(const Link *head){
    if (head){
        printf("%p ->", head);
        print_link(head->next);
    } else {
        printf("NULL\n");
    }
}