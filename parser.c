#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

char *strdup(const char *s) {
    size_t size = strlen(s) + 1;
    char *p = malloc(size);
    if (p != NULL) {
        memcpy(p, s, size);
    }
    return p;
}

/*
    Creates a new Event struct
*/
Event createEvent(void)
{
    Event event;
    /* Initialize event properties to NULL */
    event.description = NULL;
    event.summary = NULL;
    event.dtstart = NULL;
    event.dtend = NULL;
    event.rrule = NULL;
    return event;
}

/*
    Initialize the FSM
*/
void initFSM(FSM *fsm, char *filename)
{
    fsm->currentState = INITIAL_STATE;
    fsm->ics.events = NULL;
    fsm->ics.numEvents = 0;
    fsm->ics.filename = filename;
    fsm->currentEvent = createEvent();
}

/*
    Adds a new event into the events list in the FSM's ICS struct
*/
void addEvent(FSM *fsm)
{
    /* Allocate memory for events list if it's the first event */
    if (fsm->ics.numEvents == 0)
    {
        fsm->ics.events = (Event *)malloc(sizeof(Event));
        if (fsm->ics.events == NULL)
        {
            fprintf(stderr, "Memory allocation for new Event failed.\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        /* Resize events list to accommodate the new event */
        fsm->ics.events = (Event *)realloc(fsm->ics.events, (fsm->ics.numEvents + 1) * sizeof(Event));
        if (fsm->ics.events == NULL)
        {
            fprintf(stderr, "Memory reallocation for new Event failed.\n");
            exit(EXIT_FAILURE);
        }
    }
    /* Add the new event to the events list */
    fsm->ics.events[fsm->ics.numEvents] = fsm->currentEvent;
    fsm->ics.numEvents += 1;
}

/*
    Process a line in the ICS file and update the FSM
*/
void processLine(FSM *fsm, char *line)
{
    printf("Current State: %d\n", fsm->currentState);
    printf("Current Line: %s\n", line);
    switch (fsm->currentState)
    {
    case INITIAL_STATE:
        if (strcmp(line, "BEGIN:VCALENDAR") == 0)
        {
            printf("Moving to BEGIN_VCALENDAR_STATE\n");
            fsm->currentState = BEGIN_VCALENDAR_STATE;
        }
        else
        {
            printf("Moving to ERROR_STATE\n");
            fsm->currentState = ERROR_STATE;
        }
        break;
    case BEGIN_VCALENDAR_STATE:
        if (strcmp(line, "BEGIN:VEVENT") == 0)
        {
            printf("Moving to BEGIN_VEVENT_STATE\n");
            fsm->currentState = BEGIN_VEVENT_STATE;
            fsm->currentEvent = createEvent();
        }
        else if (strcmp(line, "END:VCALENDAR") == 0)
        {
            printf("Moving to END_VCALENDAR_STATE\n");
            fsm->currentState = END_VCALENDAR_STATE;
        }
        break;
    case BEGIN_VEVENT_STATE:
        if (strcmp(line, "END:VEVENT") == 0)
        {
            printf("Moving to END_VEVENT_STATE\n");
            fsm->currentState = END_VEVENT_STATE;
            addEvent(fsm);
        }
        else if (strncmp(line, "DESCRIPTION:", 12) == 0)
        {
            fsm->currentEvent.description = strdup(line + 12);
        }
        else if (strncmp(line, "SUMMARY:", 8) == 0)
        {
            fsm->currentEvent.summary = strdup(line + 8);
        }
        else if (strncmp(line, "DTSTART;", 8) == 0 || strncmp(line, "DTSTART:", 8) == 0)
        {   
            fsm->currentEvent.dtstart = strdup(line + 8);
        }
        else if (strncmp(line, "DTEND;", 6) == 0|| strncmp(line, "DTEND:", 6) == 0)
        {
            fsm->currentEvent.dtend = strdup(line + 6);
        }
        else if (strncmp(line, "RRULE:", 6) == 0)
        {
            fsm->currentEvent.rrule = strdup(line + 6);
        }
        break;
    case END_VEVENT_STATE:
        if (strcmp(line, "BEGIN:VEVENT") == 0)
        {
            printf("Moving to BEGIN_VEVENT_STATE\n");
            fsm->currentState = BEGIN_VEVENT_STATE;
        }
        else if (strcmp(line, "END:VCALENDAR") == 0)
        {
            printf("Moving to END_VCALENDAR_STATE\n");
            fsm->currentState = END_VCALENDAR_STATE;
        }
        break;
    default:
        break;
    }
}

/*
    Parses the ICS file
*/
ICS parseFile(char *filename)
{
    char line[MAX_LINE_LENGTH];
    FSM fsm;
    FILE *file;
    int length;
    initFSM(&fsm, filename);
 
    /* Open the ICS file */
    file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    /* Read each line in the file */
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL)
    {
        /* Replace the new line character at the end of each line with null terminator for strcmp to work properly in processLine */
        length = strlen(line);
        if (line[length - 1] == '\n')
        {
            line[length - 1] = '\0';
        }
        
        /* For MacOS/Linux */
        if(line[length - 2] == '\r')
        {
            line[length - 2] = '\0'; 
        }

        processLine(&fsm, line);

        /* If currentState is ERROR_STATE, throws an error */
        if (fsm.currentState == ERROR_STATE)
        {
            fprintf(stderr, "Improper ICS file format. BEGIN:VCALENDAR not found.");
            exit(EXIT_FAILURE);
        }
    }

    /* If the final state is not END_VCALENDAR_STATE, it meanas END:VCALENDAR is missing in the ics file */
    if (fsm.currentState != END_VCALENDAR_STATE)
    {
        fprintf(stderr, "Improper ICS file format. END:VEVENT or END:VCALENDAR not found.");
        exit(EXIT_FAILURE);
    }

    fclose(file);
    return fsm.ics;
}

/*
    Print details of an event
*/
void printEvent(Event event)
{
    printf("Description: %s\n", event.description);
    printf("Summary: %s\n", event.summary);
    printf("Start Date: %s\n", event.dtstart);
    printf("End Date: %s\n", event.dtend);
    printf("RRULE: %s\n", event.rrule);
    printf("\n");
}

/*
    Frees memory allocated for an ICS struct
*/
void freeICS(ICS *ics)
{
    if (ics->events == NULL)
    {
        return;
    }
    
    free(ics->events);
}

