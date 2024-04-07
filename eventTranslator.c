#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "parser.h"
#include "eventTranslator.h"
#include "timeManager.h"

void processEvent(Event event);
void processRRule(char *rrule, char* dateStart, char* dateEnd);
void getRRuleAttributes(char *rrule, RRuleFSM *fsm);

void initRRuleFSM(RRuleFSM *fsm) 
{
    fsm->currentState = INITIAL_RRULE_STATE;
}

void processEvent(Event event) 
{
    /* Initialise an empty list to append LLNodeData structs to */
    LLNodeData *data = (LLNodeData *)malloc(sizeof(LLNodeData));
    if (event.summary != NULL) {
        data->className = event.summary;
    }
    if (event.description != NULL) {
        data->studentID = atoi(event.description);
    }
    if (event.rrule != NULL) {
        processRRule(event.rrule, event.dtstart, event.dtend);
    }
}

/* TODO: Fix parsing bugs */
void processRRule(char *rrule, char* dateStart, char* dateEnd) 
{
    int i;
    time_t startTime;
    RRuleFSM fsm;

    initRRuleFSM(&fsm);
    /* Find the FREQ token in the RRule */
    getRRuleAttributes(rrule, &fsm);
    /* Print out all the attributes */
    printf("Frequency: %d\n", fsm.freq);
    printf("Count: %d\n", fsm.count);
    printf("Interval: %d\n", fsm.interval);
    for (i = 0; i < 7; i++) {
        printf("ByDay: %d\n", fsm.byDay[i]);
    }
    startTime = processStringToTimeStruct(dateStart);
    printTime(&startTime);
}

void getRRuleAttributes(char *rrule, RRuleFSM *fsm) 
{   
    int i;
    int j;
    char *day;
    char *token = strtok(rrule, ";");
    /* Print out all the tokens */
    while (token != NULL) {
        printf("%s\n", token);
        token = strtok(NULL, ";");
    }
    while (token != NULL) {
        if (strcmp(token, "FREQ") == 0) {
            /* Get the value after = */
            token = strtok(NULL, "=");
            if (strcmp(token, "DAILY") == 0) {
                fsm->freq = 1;
            }
            else if (strcmp(token, "WEEKLY") == 0) {
                fsm->freq = 2;
            }
            else if (strcmp(token, "MONTHLY") == 0) {
                fsm->freq = 3;
            }
            else if (strcmp(token, "YEARLY") == 0) {
                fsm->freq = 4;
            }
        }
        else if (strcmp(token, "COUNT") == 0) {
            /* Get the value after */
            token = strtok(NULL, "=");
            fsm->count = atoi(token);
        }
        else if (strcmp(token, "INTERVAL") == 0) {
            /* Get the value after */
            token = strtok(NULL, "=");
            fsm->interval = atoi(token);
        }
        else if (strcmp(token, "BYDAY") == 0) {
            /* Get the value after */
            token = strtok(NULL, "=");
            day = strtok(token, ",");
            while (day != NULL) {
                j = 0;
                for (i = 0; i < 7; i++) {
                    if (strcmp(day, dayOfWeekStrings[i]) == 0) {
                        /* Assign enum DayOfweek to byDay */
                        fsm->byDay[j] = i;
                        j++;
                    }
                }
            }
        }
        token = strtok(NULL, ";");
    }
}
