/*
 * Stealth
 * December 2022 - January 2023
 * BERNIER Valentin - VILAYVANH Mickael
 */

#ifndef _LINKEDLINK_
#define _LINKEDLINK_

#include <stdlib.h>

typedef struct link {
    struct link *next;
    void *addr;
} Link;

typedef struct linkedlink {
    int nmemb;
    Link *head;
} Linkedlink;

/**
 * @brief Initializes a linkedlink by setting head to null
 * and number of member to 0.
 * 
 * @param dest Pointer to destination.
 */
void init_linkedlink(Linkedlink *dest);

/**
 * @brief Add a pointer's adress in linkedlink's value
 * 
 * @param addr address to add.
 * @param dest Pointer to destination.
 * @return int Error code.
 */
int add_address(void *addr, Linkedlink *dest);

/**
 * @brief Remove an element from linkedlink. This element's index
 * is specified in parameter and has to be between number of linkedlink member
 * and 0. Else no element are removed.
 * 
 * @param index element's index.
 * @param ptr Pointer to destination.
 */
void remove_link(int index, Linkedlink *ptr);

/**
 * @brief Print on terminal the linkedlink by using
 * head's field.
 * 
 * @param head pointer to linkedlink's head
 */
void print_link(const Link *head);

/**
 * @brief Free a linkedlink.
 * 
 * @param ptr Pointer to a linkedlink.
 */
void free_linkedlink(Linkedlink *ptr);

#endif