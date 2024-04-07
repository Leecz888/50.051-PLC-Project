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

int processEvent(Event event, LLNodeData **data);
int processRRule(char *rrule, char *dateStart, char *dateEnd, timings **timingsList);
void getRRuleAttributes(char *rrule, RRuleAttributes *fsm);
int createDateStartList(char *dateStart, char *dateEnd, time_t** dateStartList, RRuleAttributes *fsm);

void initRRuleAttributes(RRuleAttributes *fsm)
{
    fsm->currentState = INITIAL_RRULE_STATE;
    fsm->count = 0;
    fsm->interval = 0;
    fsm->freq = time(0);
    fsm->hasByDay = 0;
    fsm->hasUntil = 0;
    fsm->until = NULL;
    fsm->byDay = 0;
    fsm->sizeOfByDay = 0;
}

int processEvent(Event event, LLNodeData **dataList)
{
    *dataList = (LLNodeData *)malloc(sizeof(LLNodeData));
    // Initialise an empty list to append LLNodeData structs to
    LLNodeData *data = (LLNodeData *)malloc(sizeof(LLNodeData));
    int LLNodeListSize = 0;
    if (event.summary != NULL)
    {
        data->className = event.summary;
    }
    if (event.description != NULL)
    {
        data->studentName = event.description;
    }
    if (event.rrule != NULL)
    {
        timings *timingsList;
        int sizeOfTimingsList = processRRule(event.rrule, event.dtstart, event.dtend, &timingsList);
        for (int i = 0; i < sizeOfTimingsList; i++)
        {
            /* Convert timeStart to string */
            const time_t *timeStart = &timingsList[i].timeStart;
            const time_t *timeEnd = &timingsList[i].timeEnd;
            data->timeStart = processTimeStructToString(timeStart);
            /* Convert timeEnd to string */
            data->timeEnd = processTimeStructToString(timeEnd);
            *dataList = (LLNodeData *)realloc(*dataList, i + 1 * sizeof(LLNodeData));
            (*dataList)[i] = *data;
        }
        LLNodeListSize = sizeOfTimingsList;
    }
    return LLNodeListSize;
}

// TODO: Fix parsing bugs
int processRRule(char *rrule, char *dateStart, char *dateEnd, timings **timingsList)
{
    RRuleAttributes fsm;
    time_t* dateStartList;

    initRRuleAttributes(&fsm);
    // Find the FREQ  token in the RRule
    getRRuleAttributes(rrule, &fsm);
    *timingsList = (timings *)malloc(sizeof(timings));
    // Print out all the attributes
    int listSize = createDateStartList(dateStart, dateEnd, &dateStartList, &fsm);
    printf("List Size: %d\n", listSize);
    // Get duration between dateStart and dateEnd
    time_t endTime = processStringToTimeStruct(dateEnd);
    time_t startTime = processStringToTimeStruct(dateStart);
    // Get the duration between the start and end time
    int duration = difftime(endTime, startTime);
    if (listSize > 0) {
        for (int i = 0; i < listSize; i++) {
            timings newTiming = {dateStartList[i], dateStartList[i] + duration};
            *timingsList = (timings *)realloc(*timingsList, i+1 * sizeof(timings));
            (*timingsList)[i] = newTiming;
        }
    } else {
        printf("Case not handled yet\n"); 
    }
    return listSize;

}

void getRRuleAttributes(char *rrule, RRuleAttributes *fsm)
{
    // Get all the tokens delimited by ;
    char *token = strtok(rrule, ";");
    while (token != NULL)
    {
        printf("%s\n", token);
        // Check if the token contains FREQ
        if (strstr(token, "FREQ") != NULL)
        {
            // Get the frequency value
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
            // Get the count value
            char *count = strchr(token, '=') + 1;
            fsm->count = atoi(count);
        }
        else if (strstr(token, "INTERVAL") != NULL)
        {
            // Get the interval value
            char *interval = strchr(token, '=') + 1;
            fsm->interval = atoi(interval);
        }
        else if (strstr(token, "UNTIL") != NULL)
        {
            // Get the until value
            char *until = strchr(token, '=') + 1;
            fsm->hasUntil = 1;
            /* Allocate size until to fsm->until */
            fsm->until = until;

        }
        else if (strstr(token, "BYDAY") != NULL)
        {
            fsm-> byDay = (int *)malloc(sizeof(int));
            // Get the byday value
            char *byDay = strchr(token, '=') + 1;
            // Delimit by commas
            char *day = strtok(byDay, ",");
            int j = 0;
            // Iterate through all the days
            while (day != NULL)
            {   
                // Check which day it is and set the corresponding dayOfWeek value
                for (int i = 0; i < 7; i++)
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
    int numDays = 1;
    /* Print FSM */
    printf("Count: %d\n", fsm->count);
    printf("Interval: %d\n", fsm->interval);
    printf("Freq: %d\n", fsm->freq);
    printf("HasByDay: %d\n", fsm->hasByDay);
    printf("Size of ByDay: %d\n", fsm->sizeOfByDay);
    printf("ByDay: ");
    for (int i = 0; i < fsm->sizeOfByDay; i++) {
        printf("%d ", fsm->byDay[i]);
    }
    /* print date*/
    // /* Print all fsm attributes*/
    time_t startTime = processStringToTimeStruct(dateStart);
    time_t endTime = processStringToTimeStruct(dateEnd);


    /* FREQ only */
    if (fsm->count == 0 && fsm->interval == 0 && fsm-> hasByDay == 0 && fsm->until == 0) 
    {
        *dateStartList = (time_t *)malloc(sizeof(time_t));
        (*dateStartList)[0] = startTime;
        /* Add time until endTime */
        numDays += difftime(endTime, startTime) / (60 * 60 * 24 * 7);
        *dateStartList = (time_t *)realloc(*dateStartList, numDays * sizeof(time_t));
        for (int i = 1; i < numDays; i++) {
            struct tm *timeStruct = localtime(&startTime);
            timeStruct->tm_mday += 7;
            startTime = mktime(timeStruct);
            (*dateStartList)[i] = startTime;
        }
    }     
    // else if ((fsm-> count == 0 && fsm-> interval == 0 && fsm->until != NULL)) 
    // {
    //     /* print length size of until */
    //     printf("Length of until: %ld\n", strlen(fsm->until));
    //     char* until = fsm->until;
    //     time_t endTime = processStringToTimeStruct(until);
    //     printTime(&endTime); 
    //     *dateStartList = (time_t *)malloc(sizeof(time_t));
    //     (*dateStartList)[0] = startTime;
    //     /* Add time until endTime */
    //     numDays += difftime(endTime, startTime) / (60 * 60 * 24 * 7);
    //     *dateStartList = (time_t *)realloc(*dateStartList, numDays * sizeof(time_t));
    //     for (int i = 1; i < numDays; i++) {
    //         struct tm *timeStruct = localtime(&startTime);
    //         timeStruct->tm_mday += 7;
    //         startTime = mktime(timeStruct);
    //         (*dateStartList)[i] = startTime;
    //     }
    // }
    /* FREQ and COUNT */
    else if (fsm-> count != 0 && fsm-> interval == 0 && fsm-> hasByDay == 0 && fsm->until == 0) 
    {
        *dateStartList = (time_t *)malloc(fsm->count * sizeof(time_t));
        (*dateStartList)[0] = startTime;
        for (int i = 1; i < fsm->count; i++) {
            struct tm *timeStruct = localtime(&startTime);
            timeStruct->tm_mday += 7;
            startTime = mktime(timeStruct);
             (*dateStartList)[i] = startTime;
        }
        numDays = fsm->count;
   /* FREQ and INTERVAL */
    } else if (fsm-> count == 0 && fsm-> interval != 0 && fsm-> hasByDay == 0 && fsm->until == 0) 
    {
        numDays += difftime(endTime, startTime) / (60 * 60 * 24 * 7 * fsm->interval);
        *dateStartList = (time_t *)malloc(numDays * sizeof(time_t));
        (*dateStartList)[0] = startTime;
        for (int i = 1; i < numDays; i++) {
            struct tm *timeStruct = localtime(&startTime);
            timeStruct->tm_mday += (7 * fsm->interval);
            startTime = mktime(timeStruct);
            (*dateStartList)[i] = startTime;
        }
    /* FREQ and BYDAY */
    } else if (fsm-> count == 0 && fsm-> interval == 0 && fsm->sizeOfByDay > 0 && fsm->until == 0) 
    {
        printf("size of byday: %d\n", fsm->sizeOfByDay);
        /* print by day */
        for (int i = 0; i < fsm->sizeOfByDay; i++) {
            if (fsm->byDay[i] == 1) {
                printf("Day: %s\n", dayOfWeekStrings[i]);
            }
        }
        return 0;
    // TODO: FIX UNTIL BUG
    /* FREQ and UNTIL or FREQ and UNTIL and ByDaySize = 1  */
    } 
    // else if ((fsm-> count == 0 && fsm-> interval == 0 && (fsm->sizeOfByDay == 1 || fsm->sizeOfByDay == 0 ))) 
    // {
    //     printf("In here"); 
    //     *dateStartList = (time_t *)malloc(sizeof(time_t));
    //     (*dateStartList)[0] = startTime;
    //     /* Add time until endTime */
    //     numDays += difftime(endTime, startTime) / (60 * 60 * 24 * 7);
    //     *dateStartList = (time_t *)realloc(*dateStartList, numDays * sizeof(time_t));
    //     for (int i = 1; i < numDays; i++) {
    //         struct tm *timeStruct = localtime(&startTime);
    //         timeStruct->tm_mday += 7;
    //         startTime = mktime(timeStruct);
    //         (*dateStartList)[i] = startTime;
    //     } 
    // }
    else {
        return -1;
    }
    // printf("Num Days: %d\n", numDays);
    return numDays;
}

// int testEventTranslator()
// {
//     Event event = {
//         .summary = "Test Event",
//         .description = "This is a test event",
//         .dtstart = "20210901T000000",
//         .dtend = "20210901T010000",
//         .rrule = "FREQ=WEEKLY;COUNT=5;INTERVAL=2;BYDAY=MO,WE,FR"
//     };
//     processEvent(event);


//     return 0;
// }