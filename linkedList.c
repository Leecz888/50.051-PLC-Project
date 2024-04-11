#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "linkedList.h"
#include "eventTranslator.h"
#include "timeManager.h"

/*
    Initialize a new LinkedList
*/
timeLinkedList *createLinkedList(void) 
{
    timeLinkedList *list = malloc(sizeof(timeLinkedList));
    list->head = NULL;
    list->size = 0;
    return list;
}

/*
    Add a new node to the LinkedList
*/
void addNode(timeLinkedList *list, LLNodeData *data, time_t startTime, time_t endTime) 
{
    char* timeStr;
    time_t currentTime;
    timeNode *current;
    timeNode *prev;

    printf("startTime: %s\n", ctime(&startTime));
    printf("endTime: %s\n", ctime(&endTime));
    /*
        If linked list is empty, set the new node as the head
    */
    if (list->head == NULL) {
        /*
            Insert timenode based on 30 minute intervals
        */
        time_t currentTime = startTime;
        while (difftime(currentTime, endTime)<0) {
            timeNode *newNode = malloc(sizeof(timeNode));
            timeStr = ctime(&currentTime);
            newNode->time = malloc(strlen(timeStr) + 1);
            newNode->startTime = currentTime;
            strcpy(newNode->time, timeStr);
            
            newNode->data = malloc(sizeof(dataNode));
            newNode->data->className = malloc(strlen(data->className) + 1);
            strcpy(newNode->data->className, data->className);
            newNode->data->studentID = data->studentID;
            newNode->data->next = NULL;

            newNode->next = NULL;

            if (list->head == NULL) {
                list->head = newNode;
            } else {
                timeNode *current = list->head;
                while (current->next != NULL) {
                    current = current->next;
                }
                current->next = newNode;
            }
            list->size++;
            currentTime += 1800;
        }
        return;
    } 

    /* Else, set cur pointer to head to traverse through linked list*/
    current = list->head;
    prev = NULL;
    /*
        Insert timenode based on 30 minute intervals
    */
    currentTime = startTime;
    while (difftime(currentTime, endTime) < 0) {
        /*
            Traverse the linked list to find the correct position to insert the new node
        */
        /* Condition 1: newNode is later than currentNode -> continue traversing */
        while (current != NULL && difftime(currentTime,  current->startTime) > 0) {
            prev = current;
            current = current->next;
        }

        /*
            Condition 2: newNode->startTime == currentTime -> already in the LL -> insert data into the existing timenode
        */
        if (current != NULL && difftime(currentTime, current->startTime) == 0) {
            /*
                Create a new dataNode
            */
            dataNode *newData = malloc(sizeof(dataNode));
            newData->className = malloc(strlen(data->className) + 1);
            strcpy(newData->className, data->className);
            newData->studentID = data->studentID;
            newData->next = malloc(sizeof(dataNode));
            newData->next = current->data;

            /*
                Insert the new dataNode into the timenode
            */
            current->data = newData;

        } else {
            /*
                Create a new timeNode
            */
            timeNode *newNode = malloc(sizeof(timeNode));

            timeStr = ctime(&currentTime);
            newNode->time = malloc(strlen(timeStr) + 1);
            newNode->startTime = currentTime;
            strcpy(newNode->time, timeStr);

            newNode->data = malloc(sizeof(dataNode));
            newNode->data->className = malloc(strlen(data->className) + 1);
            strcpy(newNode->data->className, data->className);
            newNode->data->studentID = data->studentID;
            newNode->data->next = NULL;

            newNode->next = current;
            if (prev == NULL) {
                list->head = newNode;
            } else {
                prev->next = newNode;
            }
            prev = newNode;
            list->size++;
        }
        currentTime += 1800;
    }
}

/*
    Print the linked list
*/
void printList(timeLinkedList *list) 
{
    timeNode *current = list->head;
    dataNode *currentData;
    while (current != NULL) {
        printf("--------------------------------\n");
        printf("Time: %s\n", current->time);
        currentData = current->data;
        while (currentData != NULL) {
            printf("Class: %s\n", currentData->className);
            printf("Student ID: %d\n", currentData->studentID);
            currentData = currentData->next;
        }
        current = current->next;
        printf("--------------------------------\n");

    }
}

/*
    Free the linked list
*/
void freeList(timeLinkedList *list) 
{
    timeNode *current = list->head;
    timeNode *next;
    dataNode *currentData;
    dataNode *nextData;
    while (current != NULL) {
        next = current->next;
        currentData = current->data;
        while (currentData != NULL) {
            nextData = currentData->next;
            free(currentData->className);
            free(currentData);
            currentData = nextData;
        }
        free(current->time);
        free(current);
        current = next;
    }
    free(list);
}
