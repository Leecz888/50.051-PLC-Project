/* additional imports */
#include "linkedList.h"
#include "timeManager.h"

#ifndef CSV_CONVERTER_H
#define CSV_CONVERTER_H

/* Placeholder string length to write to CSV */
#define MAX_LENGTH 100

/*
    Function prototypes
 */
void printRowData(char** array, int size);
void writeArrayToCSV(FILE* file, char** array, const time_t startTime, const time_t* endTime, size_t size);
void writeFreetoCSV(FILE* file, const time_t* startTime, const time_t endTime, size_t size);
void processTimenode(timeNode* node, FILE* output, time_t* endTime, char** studentID, int numberOfStudents);
void processLinkedList(timeLinkedList* list, char** studentIDS, int numberOfStudents);

#endif
