#include "eventTranslator.h"
#include "timeManager.h"

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct timeNode timeNode;
typedef struct dataNode dataNode;
typedef struct timeLinkedList timeLinkedList;

/*
    Define the TimeNode struct
    - time: A string containing the date and time of the class in the format "YYYYMMDDTHHMMSS"
    - next: A pointer to the next TimeNode in the list (NULL if it is the last node)
    - data: A pointer to the DataNode containing the class and student information
 */
struct timeNode
{
    char *time;
    time_t startTime;
    timeNode *next;
    dataNode *data;
};

/*
    Define the DataNode struct containing
    - className: The name of the class
    - studentName: The name of the student
    - next: A pointer to the next DataNode in the list (NULL if it is the last node)
 */
struct dataNode 
{
    char *className;
    int studentID;
    dataNode *next;
};

/*
    Define the LinkedList struct, where TimeNodes are stored in chronological order
 */
struct timeLinkedList
{
    timeNode *head;
    int size;
};

/*
    Function prototypes
 */
void addLinkedListHead(timeLinkedList *list, LLNodeData *data, time_t startTime, time_t endTime);
timeLinkedList *createLinkedList(void);
void printList(timeLinkedList *list);
void freeList(timeLinkedList *list);
void addNode(timeLinkedList *list, LLNodeData *data, time_t startTime, time_t endTime);
#endif
