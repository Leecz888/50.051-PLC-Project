
#ifndef EVENT_TRANSLATOR_H
#define EVENT_TRANSLATOR_H

#include "parser.h"
#include <time.h>

/*
    Create enum for the state of the event processing
*/ 
typedef enum {
    INITIAL_RRULE_STATE,
    RRULE_FREQ,
    RRULE_UNTIL,
    RRULE_BYDAY,
    RRULE_COUNT,
    RRULE_INTERVAL
} eventProcessState;

typedef enum {
    DAILY,
    WEEKLY,
    MONTHLY,
    YEARLY
} freqType;

/* 
    Create enum for days of the week 
*/
typedef enum {
    SU,
    MO,
    TU,
    WE,
    TH,
    FR,
    SA
} dayOfWeek;

typedef struct {
    char *timeStart;
    char *timeEnd;
    int studentID;
    char *className;
} LLNodeData;

typedef struct {
    eventProcessState currentState;
    int count;
    int interval;
    int freq;
    int hasByDay;
    int hasUntil;
    char* until;
    int* byDay;
    int sizeOfByDay;
} RRuleAttributes;

extern const char *dayOfWeekStrings[];
int processEvent(Event event, LLNodeData **data, char* studentID);

#endif
