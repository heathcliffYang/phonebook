#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "phonebook_opt.h"

#define TABLE_SIZE 3000

/* FILL YOUR OWN IMPLEMENTATION HERE! */
entry *findName(char lastName[], entry e[])
{
#if defined(HASH)
    entry *tmp = &e[hashfunction(lastName)%TABLE_SIZE];
    while(tmp != NULL) {
        if(strcasecmp(lastName, tmp->lastName) == 0)
            return tmp;
        tmp = tmp->pNext;
    }
#elif defined(SOUNDEX)
    int hashtag = soundex(lastName);
    entry *tmp = &e[soundex(lastName)%TABLE_SIZE];
    entry *similar = (entry *) malloc(sizeof(entry));
    similar->pDetail=NULL;
    similar->pNext=NULL;
    similar->hashtag=0;
    while(tmp != NULL) {
        if(strcasecmp(lastName, tmp->lastName) == 0) {
            if(similar) free(similar);
            return tmp;
        }
        if(hashtag == tmp->hashtag) {
            strcpy(similar->lastName,tmp->lastName);
            similar->pDetail = tmp->pDetail;
            similar->hashtag = hashtag;
            similar->pNext = (entry *) malloc(sizeof(entry));
            similar = similar->pNext;
            similar->pDetail=NULL;
            similar->pNext=NULL;
            similar->hashtag=0;

        }
        tmp = tmp->pNext;
    }
    if(similar->hashtag!=0)
        return similar;
    else
#endif
    return NULL;
}

#if defined(HASH)
entry *append(char lastName[], entry *e)
#elif defined(SOUNDEX)
entry *append(char lastName[], entry *e, int hashtag)
#endif
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
#if defined(SOUNDEX)
    e->hashtag=hashtag;
#endif

    return NULL;
}

#if defined(HASH)
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

#elif defined(SOUNDEX)
int soundex(char str[])
{
    /*abcdefghijklmnopqrstuvwxyz*/
    int soundexTable[27]= {0,1,2,3,0,1,2,7,0,2,2,4,5,5,0,1,2,6,2,3,0,1,7,2,0,2};
    int soundex[20]= {};
    int numVer[20]= {};
    int i = 0, j=0;
    int hashtag = 0;
    int count = 0;

    for(i=0; str[i]!=0; i++) {
        j = str[i];
        if(j<97)
            j -=65;
        else
            j -=97;
        numVer[i] = soundexTable[j];
        count++;
    }

    if (count ==0)
        return 0;

    j=0;
    for(i=0; i<count; i++) {
        soundex[j]=numVer[i];
        j+=(numVer[i]!=numVer[i+1])||(numVer[i]==0&&numVer[i+1]==0); // not equal
        (numVer[i+1]==0)&&(((numVer[i]==numVer[i+2])&&(numVer[i+1]=numVer[i+2]))||((numVer[i]!=numVer[i+2])&&(i++))); // x0x
        i+=((numVer[i+1]==7)&&(numVer[i]==numVer[i+2]))*2+((numVer[i+1]==7)&&(numVer[i]!=numVer[i+2]));

    }

    hashtag += (((((int) str[0])-97)<0)*(((int)str[0])-65)+((((int) str[0])-97)>=0)*(((int)str[0])-97))*1000;
    hashtag += soundex[1]*100;
    hashtag += soundex[2]*10;
    hashtag += soundex[3];
    return hashtag;
}
#endif
