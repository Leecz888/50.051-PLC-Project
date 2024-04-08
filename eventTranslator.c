#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "parser.h"
#include "eventTranslator.h"
#include "timeManager.h"

typedef struct {
    time_t timeStart;
    time_t timeEnd;
} timings;

int processEvent(Event event, LLNodeData **data, char *studentID);
int processRRule(char *rrule, char *dateStart, char *dateEnd, timings **timingsList);
void getRRuleAttributes(char *rrule, RRuleAttributes *fsm);
int createDateStartList(char *dateStart, char *dateEnd, time_t** dateStartList, RRuleAttributes *fsm);

void initRRuleAttributes(RRuleAttributes *fsm)
{
    fsm->currentState = INITIAL_RRULE_STATE;
    fsm->count = 0;
    fsm->interval = 0;
    fsm->freq = 0;
    fsm->hasByDay = 0;
    fsm->hasUntil = 0;
    fsm->until = NULL;
    fsm->byDay = 0;
    fsm->sizeOfByDay = 0;
}

int processEvent(Event event, LLNodeData **dataList, char *studentID)
{
    int i;
    int LLNodeListSize;
    LLNodeData *data;
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
        int sizeOfTimingsList = processRRule(event.rrule, event.dtstart, event.dtend, &timingsList);
        *dataList = (LLNodeData *)malloc(sizeOfTimingsList * sizeof(LLNodeData));
        for (i = 0; i < sizeOfTimingsList; i++)
        {
            const time_t *timeStart = &timingsList[i].timeStart;
            const time_t *timeEnd = &timingsList[i].timeEnd;
            data->timeStart = processTimeStructToString(timeStart);
            data->timeEnd = processTimeStructToString(timeEnd);
            (*dataList)[i] = *data;
        }
        LLNodeListSize = sizeOfTimingsList;
    }
    return LLNodeListSize;
}

int processRRule(char *rrule, char *dateStart, char *dateEnd, timings **timingsList)
{
    int i;
    int listSize;
    int duration;
    time_t endTime;
    time_t startTime;
    RRuleAttributes fsm;
    time_t* dateStartList;
    timings newTiming;

    initRRuleAttributes(&fsm);
    getRRuleAttributes(rrule, &fsm);
    listSize = createDateStartList(dateStart, dateEnd, &dateStartList, &fsm);
    *timingsList = (timings *)malloc(listSize * sizeof(timings));
    printf("List Size: %d\n", listSize);
    endTime = processStringToTimeStruct(dateEnd);
    startTime = processStringToTimeStruct(dateStart);
    duration = difftime(endTime, startTime);
    if (listSize > 0) {
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
    return listSize;
}

void getRRuleAttributes(char *rrule, RRuleAttributes *fsm)
{
    int i;
    int j;
    char *byDay;
    char *day;
    char *token = strtok(rrule, ";");
    while (token != NULL)
    {
        printf("%s\n", token);
        if (strstr(token, "FREQ") != NULL)
        {
            char *freq = strchr(token, '=') + 1;
            if (strcmp(freq, "DAILY") == 0)
            {
                fsm->freq = 0;
            }
            else if (strcmp(freq, "WEEKLY") == 0)
            {
                fsm->freq = 1;
            }
            else if (strcmp(freq, "MONTHLY") == 0)
            {
                fsm->freq = 2;
            }
            else if (strcmp(freq, "YEARLY") == 0)
            {
                fsm->freq = 3;
            }
        }
        else if (strstr(token, "COUNT") != NULL)
        {
            char *count = strchr(token, '=') + 1;
            fsm->count = atoi(count);
        }
        else if (strstr(token, "INTERVAL") != NULL)
        {
            char *interval = strchr(token, '=') + 1;
            fsm->interval = atoi(interval);
        }
        else if (strstr(token, "UNTIL") != NULL)
        {
            char *until = strchr(token, '=') + 1;
            fsm->hasUntil = 1;
            fsm->until = until;

        }
        else if (strstr(token, "BYDAY") != NULL)
        {
            fsm-> byDay = (int *)malloc(sizeof(int));
            byDay = strchr(token, '=') + 1;
            day = strtok(byDay, ",");
            j = 0;
            while (day != NULL)
            {   
                for (i = 0; i < 7; i++)
                {
                    if (strcmp(day, dayOfWeekStrings[i]) == 0)
                    {
                        fsm->byDay = (int *)realloc(fsm->byDay, (i + 1) * sizeof(int));
                        fsm->byDay[j] = i;
                        printf("Day: %d\n", fsm->byDay[j]);
                        fsm->sizeOfByDay += 1;
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

int createDateStartList(char *dateStart, char *dateEnd, time_t** dateStartList, RRuleAttributes *fsm)
{
    time_t startTime;
    time_t endTime;
    int numDays = 1;
    int i;

    printf("Count: %d\n", fsm->count);
    printf("Interval: %d\n", fsm->interval);
    printf("Freq: %d\n", fsm->freq);
    printf("HasByDay: %d\n", fsm->hasByDay);
    printf("Size of ByDay: %d\n", fsm->sizeOfByDay);
    printf("ByDay: ");
    for (i = 0; i < fsm->sizeOfByDay; i++) {
        printf("%d ", fsm->byDay[i]);
    }
    startTime = processStringToTimeStruct(dateStart);
    endTime = processStringToTimeStruct(dateEnd);


    if (fsm->count == 0 && fsm->interval == 0 && fsm-> hasByDay == 0 && fsm->until == 0) 
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
    else if (fsm -> count == 0 && fsm -> interval == 0 && fsm-> until != 0 && (fsm-> sizeOfByDay == 0 || fsm-> sizeOfByDay == 1)) 
    {
        endTime = processStringToTimeStruct(fsm->until);
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
    else if (fsm-> count != 0 && fsm-> interval == 0 && fsm-> hasByDay == 0 && fsm->until == 0) 
    {
        *dateStartList = (time_t *)malloc(fsm->count * sizeof(time_t));
        (*dateStartList)[0] = startTime;
        for (i = 1; i < fsm->count; i++) {
            struct tm *timeStruct = localtime(&startTime);
            timeStruct->tm_mday += 7;
            startTime = mktime(timeStruct);
             (*dateStartList)[i] = startTime;
        }
        numDays = fsm->count;
    } else if (fsm-> count == 0 && fsm-> interval != 0 && fsm-> hasByDay == 0 && fsm->until == 0) 
    {
        numDays += difftime(endTime, startTime) / (60 * 60 * 24 * 7 * fsm->interval);
        *dateStartList = (time_t *)malloc(numDays * sizeof(time_t));
        (*dateStartList)[0] = startTime;
        for (i = 1; i < numDays; i++) {
            struct tm *timeStruct = localtime(&startTime);
            timeStruct->tm_mday += (7 * fsm->interval);
            startTime = mktime(timeStruct);
            (*dateStartList)[i] = startTime;
        }
    } else if (fsm-> count == 0 && fsm-> interval == 0 && fsm->sizeOfByDay > 0 && fsm->until == 0) 
    {
        printf("size of byday: %d\n", fsm->sizeOfByDay);
        /* print by day */
        for (i = 0; i < fsm->sizeOfByDay; i++) {
            if (fsm->byDay[i] == 1) {
                printf("Day: %s\n", dayOfWeekStrings[i]);
            }
        }
        return 0;
    } 

    else {
        return -1;
    }
    return numDays;
}
