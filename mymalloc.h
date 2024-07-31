/***** MYMALLOC FUNCTION USING BEST FIT ******/
#include<stddef.h>
#include<stdio.h>
#define FREE 1
#define ALLOCATED 0
#define MEMSIZE 25000

char MEMORY[MEMSIZE];//create byte addressed array

struct metaData{
    int STATUS;
    size_t SIZE;
    struct metaData * NEXT;
};

struct metaData * initializeMemory(void *startPtr, size_t sizeMem);
struct metaData * newMeta(struct metaData *startPtr, size_t sizeMem, struct metaData *nextPtr);
struct metaData * searchBestFit(size_t targetSize);
void printMemory(struct metaData *ptr);
void * MyMalloc(size_t givenSize);
struct metaData * preMeta(void *locAddress);
void MyFree(void *givenAddress);