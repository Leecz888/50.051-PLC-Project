#ifndef PARSER_H
#define PARSER_H

#define MAX_LINE_LENGTH 1024

/*
    Define the FSM states.
    - INITIAL_STATE: The starting state
    - BEGIN_VCALENDAR_STATE: Marks the beginning of an ICS file
    - BEGIN_VEVENT_STATE: Marks the beginning of a calendar event
    - END_VEVENT_STATE: Marks the end of a calendar event
    - ERROR_STATE: Error state which indicates an error in the ICS file format
 */
typedef enum {
    INITIAL_STATE,
    BEGIN_VCALENDAR_STATE,
    BEGIN_VEVENT_STATE,
    END_VEVENT_STATE,
    END_VCALENDAR_STATE,
    ERROR_STATE
} State;

/*
    Define an Event struct
    - description: Contains the description of the event
    - summary: Contains the event summary
    - dtstart: Contains the starting date and time of the event
    - dtend: Contains the ending date and time of the event
    - rrule: Contains the recurrence rule of the calendar event
*/
typedef struct {
    char *description;
    char *summary;
    char *dtstart;
    char *dtend;
    char *rrule;
} Event;

/*
    Define an ICS struct
    - events: Stores all the events observed in the ICS file
    - numEvents: Stores the number of events observed in the ICS file
    - filename: Stores the name of the ICS file
*/
typedef struct {
    Event *events;
    int numEvents;
    char *filename;
} ICS;

/*
    Define the FSM struct
    - currentState: Contains information about the current state of the FSM
    - ics: Stores the ICS Struct
    - currentEvent: Stores the current Event being processed 
*/
typedef struct {
    State currentState;
    ICS ics;
    Event currentEvent;
} FSM;

/* Functions */
char *strdup(const char *s);
Event createEvent(void);
void initFSM(FSM *fsm, char *filename);
void addEvent(FSM *fsm);
void processLine(FSM *fsm, char *line);
ICS parseFile(char *filename);
void printEvent(Event event);
void freeICS(ICS *ics); 

#endif
