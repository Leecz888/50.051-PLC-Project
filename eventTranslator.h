
#ifndef EVENT_TRANSLATOR_H
#define EVENT_TRANSLATOR_H

#include "parser.h"
#include <time.h>

/*
    Define the Intermediate LLNodeData struct to be parsed into addNode containing
    - timeStart: The start time of the class
    - timeEnd: The end time of the class
    - studentID: The ID of the student
    - className: The name of the class
*/
typedef struct {
    char *timeStart;
    char *timeEnd;
    int studentID;
    char *className;
} LLNodeData;

/*
    Function prototypes
 */
int processEvent(Event event, LLNodeData **data, char* studentID);

#endif
