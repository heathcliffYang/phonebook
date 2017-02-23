#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "phonebook_opt.h"

#define TABLE_SIZE 30000

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
        if (e->pNext == NULL) {
            e->pNext = (entry *) malloc(sizeof(entry));
        } else {
            entry *tmp = e->pNext;
            e->pNext = (entry *) malloc(sizeof(entry));
            e->pNext->pNext = tmp;
        }
        e=e->pNext;
    }
    strcpy(e->lastName, lastName);

    return NULL;
}

unsigned int hashfunction(char *str)
{
#if defined(BKDR)
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;

    while (*str) {
        hash = hash * seed + (*str++);
    }
#elif defined(SDBM)
    unsigned int hash = 0;

    while (*str) {
        // equivalent to: hash = 65599*hash + (*str++);
        hash = (*str++) + (hash << 6) + (hash << 16) - hash;
    }
#elif defined(RS)
    unsigned int b = 378551;
    unsigned int a = 63689;
    unsigned int hash = 0;

    while (*str) {
        hash = hash * a + (*str++);
        a *= b;
    }
#elif defined(JS)
    unsigned int hash = 1315423911;

    while (*str) {
        hash ^= ((hash << 5) + (*str++) + (hash >> 2));
    }
#elif defined(PJW)
    unsigned int BitsInUnignedInt = (unsigned int)(sizeof(unsigned int) * 8);
    unsigned int ThreeQuarters    = (unsigned int)((BitsInUnignedInt  * 3) / 4);
    unsigned int OneEighth        = (unsigned int)(BitsInUnignedInt / 8);
    unsigned int HighBits         = (unsigned int)(0xFFFFFFFF) << (BitsInUnignedInt - OneEighth);
    unsigned int hash             = 0;
    unsigned int test             = 0;

    while (*str) {
        hash = (hash << OneEighth) + (*str++);
        if ((test = hash & HighBits) != 0) {
            hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }
#elif defined(ELF)
    unsigned int hash = 0;
    unsigned int x    = 0;

    while (*str) {
        hash = (hash << 4) + (*str++);
        if ((x = hash & 0xF0000000L) != 0) {
            hash ^= (x >> 24);
            hash &= ~x;
        }
    }
#elif defined(DJB)
    unsigned int hash = 5381;

    while (*str) {
        hash += (hash << 5) + (*str++);
    }
#elif defined(AP)
    unsigned int hash = 0;
    int i;

    for (i=0; *str; i++) {
        if ((i & 1) == 0) {
            hash ^= ((hash << 7) ^ (*str++) ^ (hash >> 3));
        } else {
            hash ^= (~((hash << 11) ^ (*str++) ^ (hash >> 5)));
        }
    }
#endif

    return (hash & 0x7FFFFFFF);
}

