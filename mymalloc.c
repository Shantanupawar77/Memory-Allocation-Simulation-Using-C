#include <stdio.h>
#include<stddef.h>
#include "mymalloc.h"

struct metaData *metaList = NULL;

/* This is for malloc first use, just initialize the first node of metaData linked list*/
struct metaData *initializeMemory(void *startPtr, size_t sizeMem)
{
    struct metaData *tmp = (struct metaData *)startPtr;
    tmp->STATUS = FREE;
    tmp->NEXT   = NULL;
    tmp->SIZE   = sizeMem - sizeof(struct metaData);
    return tmp;
}
/* Create a new node and add it before node which point "nextptr" */
struct metaData *newMeta(struct metaData *startPtr, size_t sizeMem, struct metaData *nextPtr)
{
    struct metaData *tmp = startPtr;
    tmp->STATUS = FREE;
    tmp->NEXT   = nextPtr;
    tmp->SIZE   = sizeMem - sizeof(struct metaData);
    return tmp;
}
/*Find the best fited node in the linkedlist and returns it's pointer*/
struct metaData *searchBestFit(size_t targetSize)
{
    struct metaData *tmp = metaList;
    struct metaData *min = NULL;
    while (tmp)
    {
        if (min == NULL && (tmp->SIZE) >= targetSize && (tmp->STATUS) == FREE)
            min = tmp;
        else if (min && (tmp->SIZE) >= targetSize && (tmp->SIZE) < (min->SIZE) && (tmp->STATUS) == FREE)
        {
        	min = tmp;
		}
            
        tmp = tmp->NEXT;
    }
    return min;
}
/* Just for testing purposes, print all nodes in the linked list*/
void printMemory(struct metaData *ptr)
{
	//if metaList is empty,create a new node and init all memory to it
	if (!metaList) 
    {
        metaList = initializeMemory((void *)MEMORY, MEMSIZE);
		ptr=metaList;
    }
    while (ptr)
    {
        printf("\t\t\t\t\t[PTR=%p]\t[STARTPTR=%p]\t[SIZE=%ld]\t[STATUS=%s]\t[NEXTPTR=%p]\t\n", ptr, ptr + 1, ptr->SIZE, (ptr->STATUS == 0) ? "ALLOC" : "FREE", ptr->NEXT);
        ptr = ptr->NEXT;
    }
    printf("\n\n");
    return;
}

void *MyMalloc(size_t givenSize)
{
    //if metaList is empty,create a new node and init all memory to it
    if (!metaList) 
    {
        metaList = initializeMemory((void *)MEMORY, MEMSIZE);
    }
    struct metaData *ptr;
    ptr = searchBestFit(givenSize + sizeof(struct metaData));
    //if there is a fitted slot for the given node setup it for use otherwise return NULL
    if (ptr)
    {
        char *tmp;
        tmp = (char *)ptr;
        tmp = tmp + (int)sizeof(struct metaData) + (int)givenSize;
        ptr->NEXT   = newMeta((struct metaData *)tmp, ptr->SIZE - givenSize, ptr->NEXT);
        ptr->SIZE   = givenSize;
        ptr->STATUS = ALLOCATED;
        return ptr + 1;
    }
    else
    {
        fprintf(stderr, "\t\t\t\t\tError ! Out Of Memory \n\n");
        return NULL;
    }
}
/* Sreach for a node,which it's nextnode has "locAddress" and return it*/
struct metaData *preMeta(void *locAddress)
{
    struct metaData *tmp    = metaList;
    struct metaData *preTmp = metaList;
    while (tmp)
    {
        if (tmp + 1 == locAddress)
        {
            return preTmp;
        }
        preTmp = tmp;
        tmp = tmp->NEXT;
    }
    return NULL;
}

void MyFree(void *givenAddress)
{
    //if the first node has that "givenAddress" just free it 
    if (givenAddress == metaList + 1)
    {
        metaList->STATUS = FREE;
        if (metaList->NEXT != NULL)
        {
            //if second node is also a free node mearge them together
            if (metaList->NEXT->STATUS == FREE)
            {
                struct metaData *tmp;
                tmp = metaList;
                size_t newSize = tmp->SIZE + tmp->NEXT->SIZE + sizeof(struct metaData);
                tmp->SIZE = newSize;
                tmp->NEXT = tmp->NEXT->NEXT;
            }
        }
    }
    else
    {
        struct metaData *preMetaAddress = preMeta(givenAddress);
        //if there is a pointer called "givenAddress" make it free and mareage with near free slots
        if (preMetaAddress)
        {
            preMetaAddress->NEXT->STATUS = FREE;
            //if node is last one 
            if (preMetaAddress->NEXT->NEXT == NULL)
            {
                if (preMetaAddress->STATUS == FREE)
                {
                    preMetaAddress->SIZE = preMetaAddress->SIZE + preMetaAddress->NEXT->SIZE + sizeof(struct metaData);
                    preMetaAddress->NEXT = preMetaAddress->NEXT->NEXT;
                }
            }
            else
            {
                // prevNode and nextNode both are empty
                if (preMetaAddress->STATUS == FREE && preMetaAddress->NEXT->NEXT->STATUS == FREE)
                {
                    preMetaAddress->SIZE = preMetaAddress->SIZE + preMetaAddress->NEXT->SIZE + preMetaAddress->NEXT->NEXT->SIZE + sizeof(struct metaData) * 2;
                    preMetaAddress->NEXT = preMetaAddress->NEXT->NEXT->NEXT;
                }
                //only prevNode is  empty
                else if (preMetaAddress->STATUS == FREE)
                {
                    preMetaAddress->SIZE = preMetaAddress->SIZE + preMetaAddress->NEXT->SIZE + sizeof(struct metaData);
                    preMetaAddress->NEXT = preMetaAddress->NEXT->NEXT;
                }
                //only nextNode is empty
                else if (preMetaAddress->NEXT->NEXT->STATUS == FREE)
                {
                    preMetaAddress->NEXT->SIZE = preMetaAddress->NEXT->SIZE + preMetaAddress->NEXT->NEXT->SIZE + sizeof(struct metaData);
                    preMetaAddress->NEXT->NEXT = preMetaAddress->NEXT->NEXT->NEXT;
                }
            }
        }
        else
        {
            fprintf(stderr, "Error ! Enter a Valid Pointer");
        }
    }
    return;
}
