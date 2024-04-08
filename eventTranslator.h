
#ifndef EVENT_TRANSLATOR_H
#define EVENT_TRANSLATOR_H

#include "parser.h"
#include <time.h>

typedef struct {
    char *timeStart;
    char *timeEnd;
    int studentID;
    char *className;
} LLNodeData;

int processEvent(Event event, LLNodeData **data, char* studentID);

#endif
