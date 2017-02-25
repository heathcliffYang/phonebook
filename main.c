#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include IMPL

#ifdef OPT
#define OUT_FILE "opt.txt"
#else
#define OUT_FILE "orig.txt"
#endif

#define DICT_FILE "./dictionary/all-names.txt"

#if defined(HASH)||defined(SOUNDEX)
#define TABLE_SIZE 3000
#endif

static double diff_in_second(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}

int main(int argc, char *argv[])
{
    FILE *fp;
    int i = 0;
    char line[MAX_LAST_NAME_SIZE];
    struct timespec start, end;
    double cpu_time1, cpu_time2;
#if defined(HASH)
    unsigned int hash_index = 0;
#elif defined(SOUNDEX)
    int hash_index = 0;
    int hashtag = 0;
#endif
#if defined(DISTRIBUTION)
    unsigned int hash_layer[TABLE_SIZE]= {};
#endif
    /* check file opening */
    fp = fopen(DICT_FILE, "r");
    if (fp == NULL) {
        printf("cannot open the file\n");
        return -1;
    }

    /* build the entry */
    printf("size of entry : %lu bytes\n", sizeof(entry));

#if defined(HASH)||defined(SOUNDEX)
    entry *pHead, e[TABLE_SIZE];
    pHead = e;
    for (i=0; i<TABLE_SIZE; i++) {
        e[i].pNext=NULL;
        e[i].pDetail=NULL;
    }
    i=0;
#else
    entry *pHead, *e;
    pHead = (entry *) malloc(sizeof(entry));
    e = pHead;
    e->pNext=NULL;
#endif

#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    clock_gettime(CLOCK_REALTIME, &start);
    while (fgets(line, sizeof(line), fp)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
#if defined(HASH)
        hash_index = hashfunction(line)%TABLE_SIZE;
        append(line, &e[hash_index]);
#elif defined(SOUNDEX)
        hashtag = soundex(line);
        hash_index = hashtag%TABLE_SIZE;
        append(line, &e[hash_index], hashtag);
#else
        e = append(line, e);
#endif
#if defined(DISTRIBUTION)
        hash_layer[hash_index]++;
#endif
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);

    /* close file as soon as possible */
    fclose(fp);

#if defined(HASH)||defined(SOUNDEX)

#else
    e = pHead;
#endif

    /* the givn last name to find */
    char input[MAX_LAST_NAME_SIZE] = "abby"; //zyxel

#if defined(HASH)||defined(SOUNDEX)

#else
    e = pHead;
#endif

#if defined(SOUNDEX)
#else
    assert(findName(input, e) &&
           "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input, e)->lastName, "abby"));
#endif

#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    /* compute the execution time */
    clock_gettime(CLOCK_REALTIME, &start);
    findName(input, e);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time2 = diff_in_second(start, end);

    FILE *output = fopen(OUT_FILE, "a");
    fprintf(output, "append() findName() %lf %lf\n", cpu_time1, cpu_time2);
    fclose(output);

    printf("execution time of append() : %lf sec\n", cpu_time1);
    printf("execution time of findName() : %lf sec\n", cpu_time2);

    /* print similar answer */
#if defined(SIMILAR)
    entry *similar = findName(input, e);
    entry *tmp = similar;
    printf("Similar answer:\n");
    while(tmp!=NULL) {
        printf("%s, %d\n",tmp->lastName,tmp->hashtag);
        tmp = tmp->pNext;
    }
#endif


#if defined(DISTRIBUTION)
    FILE *distribution = fopen("eachDistribute.txt","a");
    for (i=0; i<TABLE_SIZE; i++) {
        fprintf(distribution,"%d %d\n", i, hash_layer[i]);
    }
    fclose(distribution);
#endif

#if defined(HASH)
#elif defined(SIMILAR)
    if(similar->pNext) free(similar->pNext);
    free(similar);
#else
    if (pHead->pNext) free(pHead->pNext);
    free(pHead);
#endif

    return 0;
}
