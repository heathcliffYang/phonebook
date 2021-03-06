#ifndef _PHONEBOOK_H
#define _PHONEBOOK_H

#define MAX_LAST_NAME_SIZE 16



/* TODO: After modifying the original version, uncomment the following
 * line to set OPT properly */
#define OPT 1

typedef struct __PHONE_BOOK_ENTRY {
    char lastName[MAX_LAST_NAME_SIZE];
    struct __PHONE_BOOK_ENTRY *pNext;
    struct __PHONE_BOOK_DETAIL *pDetail;
    int hashtag;
} entry;

typedef struct __PHONE_BOOK_DETAIL {
    char firstName[16];
    char email[16];
    char phone[10];
    char cell[10];
    char addr1[16];
    char addr2[16];
    char city[16];
    char state[2];
    char zip[5];

} detail;

entry *findName(char lastName[], entry *pHead);

#if defined(HASH)
entry *append(char lastName[], entry *e);
#elif defined(SOUNDEX)
entry *append(char lastName[], entry *e, int hashtag);
int soundex(char str[]);
#endif
unsigned int hashfunction(char *str);
#endif
