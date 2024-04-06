
#ifndef EVENT_TRANSLATOR_H
#define EVENT_TRANSLATOR_H

#include "parser.h"
#include <time.h>

typedef enum {
    INITIAL_RRULE_STATE,
    CHECKING_FREQUENCY,
    CHECKING_COUNT,
    CHECKING_INTERVAL,
    CHECKING_BYDAY
} eventProcessingState;

/* Create enum for days of the week */
typedef enum {
    SU,
    MO,
    TU,
    WE,
    TH,
    FR,
    SA
} dayOfWeek;

static const char *dayOfWeekStrings[] = {
    "SU",
    "MO",
    "TU",
    "WE",
    "TH",
    "FR",
    "SA"
};

typedef struct {
    char *timeStart;
    char *timeEnd;
    int studentID;
    char *className;
} LLNodeData;

typedef struct {
    eventProcessingState currentState;
    int count;
    int interval;
    int freq;
    dayOfWeek byDay[7];
} RRuleFSM;

void processEvent(Event event);

#endif
