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
    Create LinkedList head element
*/
void addLinkedListHead(timeLinkedList *list, LLNodeData *data, time_t startTime, time_t endTime) 
{
    char* timeStr;
    /*
        Insert timenode based on 30 minute intervals
    */
    time_t currentTime = startTime;
    while (difftime(currentTime, endTime)<0) {
        timeNode *newNode = malloc(sizeof(timeNode));
        timeStr = ctime(&currentTime);
        newNode->time = malloc(strlen(timeStr) + 1);
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
}

/*
    Print the linked list
*/
void printList(timeLinkedList *list) 
{
    dataNode *currentData;
    timeNode *current = list->head;
    while (current != NULL) {
        printf("--------------------------------\n");
        printf("Time: %s\n", current->time);
        currentData = current->data;
        while (currentData != NULL) {
            printf("Class: %s\n", currentData->className);
            printf("Student ID: %d\n", currentData->studentID);
            currentData = currentData->next;
        }
        printf("--------------------------------\n");
        current = current->next;
    }
}


/*
    Free the linked list
*/
void freeList(timeLinkedList *list) 
{
    dataNode *currentData;
    dataNode *tempData;
    timeNode *current = list->head;
    timeNode *temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        currentData = temp->data;
        while (currentData != NULL) {
            tempData = currentData;
            currentData = currentData->next;
            free(tempData);
        }
        free(temp);
    }
    free(list);
}

int main(int argc, char **argv)
{
    LLNodeData* s1;
    LLNodeData* s2;
    LLNodeData* s3;
    LLNodeData* p1;
    LLNodeData* p3;
    timeLinkedList *list;

    s1 = (LLNodeData*) malloc(sizeof(LLNodeData));
    s1->studentID = 1005037;
    s1->className = "PLC";
    s1->timeStart = "20240313T103000";
    s1->timeEnd = "20240313T133000";

    list = createLinkedList();
    addLinkedListHead(list, s1, convertStringToTime_t(s1->timeStart), convertStringToTime_t(s1->timeEnd));
    printf("Size after addNode 1: %d\n", list->size);
    printList(list);
    freeList(list);

    free(s1);

    return 0;
}
