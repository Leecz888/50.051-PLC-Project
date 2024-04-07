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
    /*
        Insert timenode based on 30 minute intervals
    */
    time_t currentTime = startTime;
    while (difftime(currentTime, endTime) < 0) {
        /*
            Create a new timeNode
        */
        timeNode *newNode = malloc(sizeof(timeNode));

        /*
            Set the timeNode's time
        */
        newNode->time = malloc(15);
        strftime(newNode->time, 15, "%Y%m%dT%H%M%S", localtime(&currentTime));

        /*
            Set the timeNode's data
        */
        newNode->data = malloc(sizeof(dataNode));
        newNode->data->className = data->className;
        newNode->data->studentID = data->studentID;
        newNode->data->next = NULL;

        /*
            Set the timeNode's next pointer
        */
        newNode->next = NULL;

        /*
            Insert the new timeNode into the linked list
        */
        if (list->head == NULL) {
            list->head = newNode;
        } else {
            timeNode *current = list->head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newNode;
        }
        currentTime += 1800;
        list->size++;
    }
}

/*
    Insert Data into Existing TimeNode
*/
void insertDataIntoTimeNode(timeNode *current, LLNodeData *data) 
{
    /*
        Create a new dataNode
    */
    dataNode *newData = malloc(sizeof(dataNode));
    /*
        Set the dataNode's className and studentID
    */
    newData->className = data->className;
    newData->studentID = data->studentID;
    newData->next = current->data;
    current->data = newData;
}


/*
    Add a new node to the LinkedList
*/
void addNode(timeLinkedList *list, LLNodeData *data) 
{
    /*
       Convert start and end times from the LLNodeData struct to time_t structs
    */
    time_t startTime = convertStringToTime_t(data->timeStart);
    time_t endTime = convertStringToTime_t(data->timeEnd);

    /*
        If linked list is empty, set the new node as the head
    */
    if (list->head == NULL) {
        addLinkedListHead(list, data, startTime, endTime);
        return;
    } 


    timeNode *current = list->head;
    timeNode *prev = NULL;

    /*
        Insert timenode based on 30 minute intervals
    */
    time_t currentTime = startTime;
    while (difftime(currentTime, endTime) < 0) {
        /*
            Traverse the linked list to find the correct position to insert the new node
        */
        while (current != NULL && difftime(currentTime, convertStringToTime_t(current->time)) > 0) {
            prev = current;
            current = current->next;
        }

        /*
            If timenode with starTime equal to currentTime is already in the list, insert data into the existing timenode
        */
        if (current != NULL && difftime(currentTime, convertStringToTime_t(current->time)) == 0) {
            insertDataIntoTimeNode(current, data);
        }

        /*
            If the timeNode does not exist, create a new timeNode and insert it into the linked list
        */
        else {
            /*
                Create a new timeNode
            */
            timeNode *newNode = malloc(sizeof(timeNode));

            /*
                Set the timeNode's time
            */
            newNode->time = malloc(15);
            strftime(newNode->time, 15, "%Y%m%dT%H%M%S", localtime(&currentTime));

            /*
                Set the timeNode's data
            */
            newNode->data = malloc(sizeof(dataNode));
            newNode->data->className = data->className;
            newNode->data->studentID = data->studentID;
            newNode->data->next = NULL;

            /*
                Set the timeNode's next pointer
            */

            newNode->next = current;
            if (prev == NULL) {
                list->head = newNode;
            } else {
                prev->next = newNode;
            }
            prev = newNode;
        }

        currentTime += 1800;
        list->size++;
        
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
        free(temp->time);
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

    /*
        Sample payload
    */
    s1 = (LLNodeData*) malloc(sizeof(LLNodeData));
    s1->studentID = 1005037;
    s1->className = "PLC";
    s1->timeStart = "20240313T103000";
    s1->timeEnd = "20240313T133000";

    s2 = (LLNodeData*) malloc(sizeof(LLNodeData));
    s2->studentID = 1005037;
    s2->className = "DB";
    s2->timeStart = "20240101T133000";
    s2->timeEnd = "20240101T153000";

    s3 = (LLNodeData*) malloc(sizeof(LLNodeData));
    s3->studentID = 1005037;
    s3->className = "Capstone";
    s3->timeStart = "20240313T153000";
    s3->timeEnd = "20240313T173000";

    p3 = (LLNodeData*) malloc(sizeof(LLNodeData));
    p3->studentID = 1005111;
    p3->className = "Capstone";
    p3->timeStart = "20240313T153000";
    p3->timeEnd = "20240313T173000";

    p1 = (LLNodeData*) malloc(sizeof(LLNodeData));
    p1->studentID = 1005111;
    p1->className = "Capstone";
    p1->timeStart = "20240313T153000";
    p1->timeEnd = "20240313T173000";
    
    p3 = (LLNodeData*) malloc(sizeof(LLNodeData));
    p3->studentID = 1005111;
    p3->className = "Capstone";
    p3->timeStart = "20240413T153000";
    p3->timeEnd = "20240413T173000";


    /*
        Main function
    */
    list = createLinkedList();

    addNode(list, s1);
    addNode(list, s2);
    addNode(list, s3);
    addNode(list, p1);
    addNode(list, p3);
    printList(list);
    freeList(list);

    /*
        Free the payloads
    */
    free(s1);
    free(s2);
    free(s3);
    return 0;
}
