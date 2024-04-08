#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
/* additional imports */
#include "csvConverter.h"
#include "linkedList.h"
#include "timeManager.h"

/* Placeholder constant for number of ICS files */
#define NUM_FILES 2
#define MAX_LENGTH 100

void printRowData(char rowData[NUM_FILES][MAX_LENGTH]) {
    for (int i = 0; i < NUM_FILES; i++) {
        printf("rowData[%d]: %s\n", i, rowData[i]);
    }
}

void writeArrayToCSV(FILE* file, const char (*array)[MAX_LENGTH], const time_t startTime, const time_t* endTime, size_t size) {
    size_t i;
    char* startDate;
    char* endDate;

    /* write start date to csv */
    startDate = ctime(&startTime);    
    startDate[strlen(startDate)-1] = '\0';
    fprintf(file, "%s,", startDate);
    
    /* write end date to csv */
    endDate = ctime(endTime);
    endDate[strlen(endDate)-1] = '\0';
    fprintf(file, "%s,", endDate);

    /* write occupied classes to csv */
    for (i=0; i<size; i++) {
        fprintf(file, "%s", array[i]);
        /* only add comma if element is not the last */
        if (i != size-1) {
            fprintf(file, ",");
        }
    }
    /* move file pointer to the next row */
    fprintf(file, "\n");
}

void writeFreetoCSV(FILE* file, const time_t* startTime, const time_t endTime, size_t size) {
    size_t i;
    char* startDate;
    char* endDate;

    /* write start date to csv */
    startDate = ctime(startTime);    
    startDate[strlen(startDate)-1] = '\0';
    fprintf(file, "%s,", startDate);
    
    /* write end date to csv */
    endDate = ctime(&endTime);
    endDate[strlen(endDate)-1] = '\0';
    fprintf(file, "%s,", endDate);

    /* write FREE to csv */
    for (i=0; i<size; i++) {
        fprintf(file, "FREE");
        /* only add comma if element is not the last */
        if (i != size-1) {
            fprintf(file, ",");
        }
    }
    /* move file pointer to the next row */
    fprintf(file, "\n");
}


/*
    Process a single TimeNode
*/
void process_timenode(timeNode* node, FILE* output, time_t* endTime, char** studentIDS, int numberOfStudents) {
    char rowData[NUM_FILES][MAX_LENGTH];
    time_t currentTime = node->startTime;
    dataNode* data = node->data;
    printf("Current: %s", ctime(&currentTime));
    printf("End: %s", ctime(endTime));
    
    /* Populate rowData array with empty strings*/
    for (int i = 0; i < NUM_FILES; i++) {
        strncpy(rowData[i], "", MAX_LENGTH);
    }

    /* Check if start time corresponds to end time*/
    if (difftime(currentTime, *endTime) > 0) {
        printf("writing free\n");
        /* write to CSV as 'FREE' time if there's a time gap*/
        writeFreetoCSV(output, endTime, currentTime, NUM_FILES);
        /* set new endTime pointer */
        *endTime = currentTime;
    }

    /* Else, timeslot is occupied by someone */
    if (difftime(currentTime, *endTime) == 0) {
        printf("writing occupied\n");
        /* set new endTime pointer */
        *endTime = currentTime + 1800;
        
        /* traverse data node to populate rowData*/
        /* Print all attributes in data */
        
        // while (data != NULL) {
        //     printf("Student ID: %d\n", data->studentID);
        //     printf("Class Name: %s\n", data->className);

        //     /* append class name to array based on column index (id) */
        //     strncpy(rowData[data->studentID], data->className, strlen(data->className));
        //     data = data->next;
        // }
        while (data != NULL) {

            for (int i = 0; i < numberOfStudents; i++) {
                printf("Student ID: %d\n", studentIDS[i]);
                if (data->studentID == atoi(studentIDS[i])) {

                    strncpy(rowData[i], data->className, strlen(data->className));
                    break;
                }
            }
            data = data->next;
        }

        printRowData(rowData);
        printf("Current: %s", ctime(&currentTime));
        printf("New End: %s", ctime(endTime));
        writeArrayToCSV(output, rowData, currentTime, endTime, NUM_FILES);
        printf("-----\n");
    }
    return;
}

void processLinkedList(timeLinkedList *list, char **studentIDS, int numberOfStudents) {
    time_t* endTime = malloc(sizeof(time_t));
    
    /* set node head and end time of first timeNode */
    timeNode *current = list->head;
    *endTime = current->startTime;

    /* Open the output CSV file */
    FILE *output = fopen("output.csv", "w");
    
    /* Check for invalid CSV file opening */
    if (output == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

/* Form a string to formulate the rows in the csv */
    char* fixedHeader = "Start,End,";
    char* rowHeader = malloc(strlen(fixedHeader) + 1);
    strcpy(rowHeader, fixedHeader);
    for (int i = 0; i < numberOfStudents; i++) {
        rowHeader = realloc(rowHeader, strlen(rowHeader) + strlen(studentIDS[i]) + 1);
        strcat(rowHeader, studentIDS[i]);
        if (i != numberOfStudents - 1) {
            rowHeader = realloc(rowHeader, strlen(rowHeader) + 1);
            strcat(rowHeader, ",");
        }
    }
    printf("Row Header: %s\n", rowHeader);
    /* TODO: Write column names to CSV */
    fprintf(output, "%s\n", rowHeader);
    /* Write studentIDs to CSV */


    /* Loop through linked list until NULL 
    */
    while (current != NULL) {
        process_timenode(current, output, endTime, studentIDS, numberOfStudents);
        current = current->next;
    }
    
    /* Close output CSV file */
    fclose(output);
    free(endTime);
}

/*
int main(int argc, char **argv)
{
    LLNodeData* s1;
    LLNodeData* s2;
    LLNodeData* s3;
    LLNodeData* p1;
    LLNodeData* p3;
    timeLinkedList *list;

    s1 = (LLNodeData*) malloc(sizeof(LLNodeData));
    s1->studentID = 0;
    s1->className = "PLC";
    s1->timeStart = "20240313T103000";
    s1->timeEnd = "20240313T133000";

    p1 = (LLNodeData*) malloc(sizeof(LLNodeData));
    p1->studentID = 1;
    p1->className = "PLC";
    p1->timeStart = "20240313T103000";
    p1->timeEnd = "20240313T133000";

    s2 = (LLNodeData*) malloc(sizeof(LLNodeData));
    s2->studentID = 0;
    s2->className = "DB";
    s2->timeStart = "20240313T153000";
    s2->timeEnd = "20240313T183000";

    s3 = (LLNodeData*) malloc(sizeof(LLNodeData));
    s3->studentID = 0;
    s3->className = "HASS";
    s3->timeStart = "20240301T153000";
    s3->timeEnd = "20240301T183000";

    list = createLinkedList();
    addNode(list, s1, convertStringToTime_t(s1->timeStart), convertStringToTime_t(s1->timeEnd));
    addNode(list, p1, convertStringToTime_t(p1->timeStart), convertStringToTime_t(p1->timeEnd));
    addNode(list, s2, convertStringToTime_t(s2->timeStart), convertStringToTime_t(s2->timeEnd));
    addNode(list, s3, convertStringToTime_t(s3->timeStart), convertStringToTime_t(s3->timeEnd));
    printList(list);
    printf("Size: %d\n", list->size);

    process_linked_list(list);
    freeList(list);

    free(s3);
    free(s2);
    free(p1);
    free(s1);

    return 0;
}
*/