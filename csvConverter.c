#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
/* additional imports */
#include "csvConverter.h"
#include "linkedList.h"
#include "timeManager.h"

/* Placeholder string length to write to CSV */
#define MAX_LENGTH 100

/*
    Helper function to print each row of rowData
*/
void printRowData(char** array, int size) {
    int i;
    for (i = 0; i < size; i++) {
        printf("rowData[%d]: %s\n", i, array[i]);
    }
}

/*
    Writes each occupied row / timeslot to the CSV file
*/
void writeArrayToCSV(FILE* file, char** array, const time_t startTime, const time_t* endTime, size_t size) {
    size_t i;
    char* startDate;
    char* endDate;

    /* Write start date to csv */
    startDate = ctime(&startTime);    
    startDate[strlen(startDate)-1] = '\0';
    fprintf(file, "%s,", startDate);
    
    /* Write end date to csv */
    endDate = ctime(endTime);
    endDate[strlen(endDate)-1] = '\0';
    fprintf(file, "%s,", endDate);

    /* Write occupied classes to csv */
    for (i=0; i<size; i++) {
        fprintf(file, "%s", array[i]);
        /* Only add comma if element is not the last */
        if (i != size-1) {
            fprintf(file, ",");
        }
    }
    /* Move file pointer to the next row */
    fprintf(file, "\n");
}

/*
    Writes 'FREE' to denote all members having free timeslots to the CSV file
*/
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
void processTimenode(timeNode* node, FILE* output, time_t* endTime, char** studentIDS, int numberOfStudents) {
    int i;
    char** rowData;
    time_t currentTime;
    dataNode* data;
    rowData = malloc(numberOfStudents * sizeof(char*));
    for (i = 0; i < numberOfStudents; i++) {
        rowData[i] = malloc(MAX_LENGTH * sizeof(char));
    }
    currentTime = node->startTime;
    data = node->data;
    
    /* Populate rowData array with empty strings*/
    
    for (i = 0; i < numberOfStudents; i++) {
        strncpy(rowData[i], "", MAX_LENGTH);
    }

    /* Check if start time corresponds to end time*/
    if (difftime(currentTime, *endTime) > 0) {
        /* write to CSV as 'FREE' time if there's a time gap*/
        writeFreetoCSV(output, endTime, currentTime, numberOfStudents);
        /* set new endTime pointer */
        *endTime = currentTime;
    }

    /* Else, timeslot is occupied by someone */
    if (difftime(currentTime, *endTime) == 0) {
        /* Set new endTime pointer */
        *endTime = currentTime + 1800;
        
        /* Traverse data node to populate rowData*/
        /* Print all attributes in data */
        while (data != NULL) {
            for (i = 0; i < numberOfStudents; i++) {
                if (data->studentID == atoi(studentIDS[i])) {
                    strncpy(rowData[i], data->className, strlen(data->className));
                    break;
                }
            }
            data = data->next;
        }
        /* printRowData(rowData, numberOfStudents); */
        writeArrayToCSV(output, rowData, currentTime, endTime, numberOfStudents);
        /* free row data */
        for (i = 0; i < numberOfStudents; i++) {
            free(rowData[i]);
        }
        free(rowData);
    }
    return;
}

void processLinkedList(timeLinkedList *list, char **studentIDS, int numberOfStudents) {
    int i;
    char* fixedHeader;
    char* rowHeader;
    FILE *output;
    time_t* endTime = malloc(sizeof(time_t));
    
    /* set node head and end time of first timeNode */
    timeNode *current = list->head;
    *endTime = current->startTime;

    /* Open the output CSV file */
    output = fopen("output/output.csv", "w");
    
    /* Check for invalid CSV file opening */
    if (output == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

/* Form a string to formulate the rows in the csv: 'Start, End, StudentID1, StudentID2...' */
    fixedHeader = "Start,End,";
    rowHeader = malloc(strlen(fixedHeader) + 1);
    strcpy(rowHeader, fixedHeader);
    for (i = 0; i < numberOfStudents; i++) {
        rowHeader = realloc(rowHeader, strlen(rowHeader) + strlen(studentIDS[i]) + 1);
        strcat(rowHeader, studentIDS[i]);
        if (i != numberOfStudents - 1) {
            rowHeader = realloc(rowHeader, strlen(rowHeader) + 1);
            strcat(rowHeader, ",");
        }
    }
    /* Write column names to CSV */
    fprintf(output, "%s\n", rowHeader);


    /* Loop through linked list until NULL */
    while (current != NULL) {
        processTimenode(current, output, endTime, studentIDS, numberOfStudents);
        current = current->next;
    }
    
    /* Close output CSV file */
    fclose(output);
    free(endTime);
}
