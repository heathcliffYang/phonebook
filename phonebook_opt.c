#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "phonebook_opt.h"

#define TABLE_SIZE 20000

/* FILL YOUR OWN IMPLEMENTATION HERE! */
entry *findName(char lastName[], entry e[])
{
    entry *tmp = &e[hashfunction(lastName)%TABLE_SIZE];
    while(tmp != NULL) {
        if(strcasecmp(lastName, tmp->lastName) == 0)
            return tmp;
        tmp = tmp->pNext;
    }
    return NULL;
}

entry *append(char lastName[], entry *e)
{
    if (e->lastName[0] != 0) {
        e->pNext = (entry *) malloc(sizeof(entry));
        e = e->pNext;
    }
    strcpy(e->lastName, lastName);
    e->pNext = NULL;

    return NULL;
}

unsigned int hashfunction(char *str) // BKDR
{
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;

    while (*str) {
        hash = hash * seed + (*str++);
    }

    return (hash & 0x7FFFFFFF);
}
