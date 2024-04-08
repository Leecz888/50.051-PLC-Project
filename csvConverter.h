/* additional imports */
#include "linkedList.h"
#include "timeManager.h"

#ifndef CSV_CONVERTER_H
#define CSV_CONVERTER_H

/* Placeholder constant for number of ICS files */
#define NUM_FILES 2
#define MAX_LENGTH 100

/*
    Function prototypes
 */
void printRowData(char** array, int size);
void writeArrayToCSV(FILE* file, char** array, const time_t startTime, const time_t* endTime, size_t size);
void writeFreetoCSV(FILE* file, const time_t* startTime, const time_t endTime, size_t size);
void process_timenode(timeNode* node, FILE* output, time_t* endTime, char** studentID, int numberOfStudents);
void processLinkedList(timeLinkedList* list, char** studentIDS, int numberOfStudents);

#endif
