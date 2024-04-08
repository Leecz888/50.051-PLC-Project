#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "parser.h"
#include "eventTranslator.h"
#include "timeManager.h"

/* 
    Define enum for frequency types
    - DAILY: Daily recurrence
    - WEEKLY: Weekly recurrence
    - MONTHLY: Monthly recurrence
    - YEARLY: Yearly recurrence
*/
typedef enum {
    DAILY,
    WEEKLY,
    MONTHLY,
    YEARLY
} freqType;

/* 
    Define enum for day of week
    - SU: Sunday
    - MO: Monday
    - TU: Tuesday
    - WE: Wednesday
    - TH: Thursday
    - FR: Friday
    - SA: Saturday
*/
static const char *dayOfWeekStrings[] = {
    "SU",
    "MO",
    "TU",
    "WE",
    "TH",
    "FR",
    "SA"
};

/* 
    Define struct for RRule attributes
    - count: Number of occurrences
    - interval: Interval between occurrences
    - freq: Frequency of recurrence
    - hasByDay: Flag to check if BYDAY is present
    - hasUntil: Flag to check if UNTIL is present
    - until: End date of recurrence
    - byDay: Array of days of the week
    - sizeOfByDay: Size of the byDay array
*/
typedef struct {
    int count;
    int interval;
    int freq;
    int hasByDay;
    int hasUntil;
    char* until;
    int* byDay;
    int sizeOfByDay;
} RRuleAttributes;

/* 
    Define struct for timings
    - timeStart: Start time of the class
    - timeEnd: End time of the class
*/
typedef struct {
    time_t timeStart;
    time_t timeEnd;
} timings;

int processEvent(Event event, LLNodeData **data, char *studentID);
int processRRule(char *rrule, char *dateStart, char *dateEnd, timings **timingsList);
void getRRuleAttributes(char *rrule, RRuleAttributes *fsm);
int createDateStartList(char *dateStart, char *dateEnd, time_t** dateStartList, RRuleAttributes *fsm);

/* 
    Initialse Rule Attributes
*/
void initRRuleAttributes(RRuleAttributes *rules)
{
    rules->count = 0;
    rules->interval = 0;
    rules->freq = 0;
    rules->hasByDay = 0;
    rules->hasUntil = 0;
    rules->until = NULL;
    rules->byDay = 0;
    rules->sizeOfByDay = 0;
}

/* 
    Process the event and return the number of LLNodeData elements
*/
int processEvent(Event event, LLNodeData **dataList, char *studentID)
{
    int i;
    int LLNodeListSize;
    LLNodeData *data;
    /* Allocate size for a single LLNode */
    data = (LLNodeData *)malloc(sizeof(LLNodeData));
    LLNodeListSize = 0;
    if (event.summary != NULL)
    {
        data->className = event.summary;
    }
    data->studentID = atoi(studentID);
    if (event.rrule != NULL)
    {
        timings *timingsList;
        /* return a list of start timings generated from the RRULE */
        int sizeOfTimingsList = processRRule(event.rrule, event.dtstart, event.dtend, &timingsList);
        /* Malloc the dataList to the number of LLNodes that corresponds to the number of timings */
        *dataList = (LLNodeData *)malloc(sizeOfTimingsList * sizeof(LLNodeData));
        for (i = 0; i < sizeOfTimingsList; i++)
        {
            const time_t *timeStart = &timingsList[i].timeStart;
            const time_t *timeEnd = &timingsList[i].timeEnd;
            data->timeStart = processTimeStructToString(timeStart);
            data->timeEnd = processTimeStructToString(timeEnd);
            /* Add LLNode data to the list */
            (*dataList)[i] = *data;
        }
        LLNodeListSize = sizeOfTimingsList;
    }
    /* Return the total number of nodes generated */
    return LLNodeListSize;
}

/*
    Process the RRULE in the event and return the number of timings generated
*/
int processRRule(char *rrule, char *dateStart, char *dateEnd, timings **timingsList)
{
    int i;
    int listSize;
    int duration;
    time_t endTime;
    time_t startTime;
    RRuleAttributes rules;
    time_t* dateStartList;
    timings newTiming;
    initRRuleAttributes(&rules);
    /* Get the RRule attributes from the RRule String */
    getRRuleAttributes(rrule, &rules);
    /* Create a list of start timings based on the RRule attributes */
    listSize = createDateStartList(dateStart, dateEnd, &dateStartList, &rules);
    /* Malloc the timingsList to the number of timings */
    *timingsList = (timings *)malloc(listSize * sizeof(timings));
    endTime = processStringToTimeStruct(dateEnd);
    startTime = processStringToTimeStruct(dateStart);
    /* Get the duration for a single session of the event */
    duration = difftime(endTime, startTime);
    if (listSize > 0) {
        /* Add the timeStart timeEnd pair to the timingsList. Each pair corresponds to a session */
        for (i = 0; i < listSize; i++) {
            newTiming.timeStart = dateStartList[i];
            newTiming.timeEnd = dateStartList[i] + duration;
            (*timingsList)[i] = newTiming;
        }
    }
    else 
    {
        printf("Case not handled yet\n"); 
    }
    /* Return the total number of sessions generated from the RRule */
    return listSize;
}

/* 
    Get the RRule attributes from the RRule string
*/
void getRRuleAttributes(char *rrule, RRuleAttributes *rules)
{
    int i;
    int j;
    char *byDay;
    char *day;
    /* Get each RRule using ; as a delimiter */
    char *token = strtok(rrule, ";");
    while (token != NULL)
    {
        /* Check if the RRule contains FREQ */
        if (strstr(token, "FREQ") != NULL)
        {
            char *freq = strchr(token, '=') + 1;
            if (strcmp(freq, "DAILY") == 0)
            {
                rules->freq = 0;
            }
            else if (strcmp(freq, "WEEKLY") == 0)
            {
                rules->freq = 1;
            }
            else if (strcmp(freq, "MONTHLY") == 0)
            {
                rules->freq = 2;
            }
            else if (strcmp(freq, "YEARLY") == 0)
            {
                rules->freq = 3;
            }
        }
        /* Check if the RRule contains COUNT */
        else if (strstr(token, "COUNT") != NULL)
        {
            char *count = strchr(token, '=') + 1;
            rules->count = atoi(count);
        }
        /* Check if the RRule contains INTERVAL */
        else if (strstr(token, "INTERVAL") != NULL)
        {
            char *interval = strchr(token, '=') + 1;
            rules->interval = atoi(interval);
        }
        /* Check if the RRule contains UNTIL */
        else if (strstr(token, "UNTIL") != NULL)
        {
            char *until = strchr(token, '=') + 1;
            rules->hasUntil = 1;
            rules->until = until;

        }
        /* Check if the RRule contains BYDAY */
        else if (strstr(token, "BYDAY") != NULL)
        {
            rules-> byDay = (int *)malloc(sizeof(int));
            byDay = strchr(token, '=') + 1;
            day = strtok(byDay, ",");
            j = 0;
            while (day != NULL)
            {   
                for (i = 0; i < 7; i++)
                {
                    if (strcmp(day, dayOfWeekStrings[i]) == 0)
                    {
                        rules->byDay = (int *)realloc(rules->byDay, (i + 1) * sizeof(int));
                        rules->byDay[j] = i;
                        printf("Day: %d\n", rules->byDay[j]);
                        rules->sizeOfByDay += 1;
                        j++;
                        break;
                    }
                }
                day = strtok(NULL, ",");
            }
        }
        token = strtok(NULL, ";");
    }
}

/*
    Create a list of start timings based on the RRule attributes
*/
int createDateStartList(char *dateStart, char *dateEnd, time_t** dateStartList, RRuleAttributes *rules)
{
    time_t startTime;
    time_t endTime;
    int numDays = 1;
    int i;
    startTime = processStringToTimeStruct(dateStart);
    endTime = processStringToTimeStruct(dateEnd);


    if (rules->count == 0 && rules->interval == 0 && rules-> hasByDay == 0 && rules->until == 0) 
    {
        *dateStartList = (time_t *)malloc(sizeof(time_t));
        (*dateStartList)[0] = startTime;
        /* Add time until endTime */
        numDays += difftime(endTime, startTime) / (60 * 60 * 24 * 7);
        *dateStartList = (time_t *)realloc(*dateStartList, numDays * sizeof(time_t));
        for (i = 1; i < numDays; i++) {
            struct tm *timeStruct = localtime(&startTime);
            timeStruct->tm_mday += 7;
            startTime = mktime(timeStruct);
            (*dateStartList)[i] = startTime;
        }
    }
    else if (rules -> count == 0 && rules -> interval == 0 && rules-> until != 0 && (rules-> sizeOfByDay == 0 || rules-> sizeOfByDay == 1)) 
    {
        endTime = processStringToTimeStruct(rules->until);
        *dateStartList = (time_t *)malloc(sizeof(time_t));
        (*dateStartList)[0] = startTime;
        numDays += difftime(endTime, startTime) / (60 * 60 * 24 * 7);
        *dateStartList = (time_t *)realloc(*dateStartList, numDays * sizeof(time_t));
        for (i = 1; i < numDays; i++) {
            struct tm *timeStruct = localtime(&startTime);
            timeStruct->tm_mday += 7;
            startTime = mktime(timeStruct);
            (*dateStartList)[i] = startTime;
        }
    }
    else if (rules-> count != 0 && rules-> interval == 0 && rules-> hasByDay == 0 && rules->until == 0) 
    {
        *dateStartList = (time_t *)malloc(rules->count * sizeof(time_t));
        (*dateStartList)[0] = startTime;
        for (i = 1; i < rules->count; i++) {
            struct tm *timeStruct = localtime(&startTime);
            timeStruct->tm_mday += 7;
            startTime = mktime(timeStruct);
             (*dateStartList)[i] = startTime;
        }
        numDays = rules->count;
    } else if (rules-> count == 0 && rules-> interval != 0 && rules-> hasByDay == 0 && rules->until == 0) 
    {
        numDays += difftime(endTime, startTime) / (60 * 60 * 24 * 7 * rules->interval);
        *dateStartList = (time_t *)malloc(numDays * sizeof(time_t));
        (*dateStartList)[0] = startTime;
        for (i = 1; i < numDays; i++) {
            struct tm *timeStruct = localtime(&startTime);
            timeStruct->tm_mday += (7 * rules->interval);
            startTime = mktime(timeStruct);
            (*dateStartList)[i] = startTime;
        }
    } 
    else {
        return -1;
    }
    return numDays;
}
