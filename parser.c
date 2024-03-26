#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_LENGTH 1024

/*
    Define the FSM states.
    - INITIAL_STATE: The starting state
    - BEGIN_VCALENDAR_STATE: Marks the beginning of an ICS file
    - BEGIN_VEVENT_STATE: Marks the beginning of a calendar event
    - END_VEVENT_STATE: Marks the end of a calendar event
    - ERROR_STATE: Error state which indicates an error in the ICS file format
 */
typedef enum
{
    INITIAL_STATE,
    BEGIN_VCALENDAR_STATE,
    BEGIN_VEVENT_STATE,
    END_VEVENT_STATE,
    END_VCALENDAR_STATE,
    ERROR_STATE
} State;

/*
    Define an Event object
    - description: Contains the description of the event
    - summary: Contains the event summary
    - dtstart: Contains the starting date and time of the event
    - dtend: Contains the ending date and time of the event
    - rrule: Contains the recurrence rule of the calendar event
*/
typedef struct
{
    char *description;
    char *summary;
    char *dtstart;
    char *dtend;
    char *rrule;
} Event;

/*
    Define the FSM struct
    - currentState: Contains information about the current state of the FSM
    - events: Stores all the events observed in the ICS file
*/
typedef struct
{
    State currentState;
    Event *events;
    int numEvents;
} FSM;

/*
    Initialize the FSM
*/
void initFSM(FSM *fsm)
{
    fsm->currentState = INITIAL_STATE;
    fsm->events = NULL; 
    fsm->numEvents = 0;
}

/*
    Process a line in the ICS file and update the FSM
*/
void processLine(FSM *fsm, char *line)
{
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
        }
        else
        {
            /* TODO: It should modify or create a new Event */
            fprintf(stderr, "TODO not implemented");
            exit(EXIT_FAILURE);
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
void parseFile(char *filename)
{
    char line[MAX_LINE_LENGTH];
    FSM fsm;
    FILE * file;
    int length;
    int line_number = 0;
    initFSM(&fsm);

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
        if(line[length - 1] == '\n') {
            line[length - 1] = '\0'; 
        }
        
        processLine(&fsm, line);

        /* If currentState is ERROR_STATE, throw an error and print the last line that was processed */
        if (fsm.currentState == ERROR_STATE)
        {
            fprintf(stderr, "Improper ICS file format. BEGIN:VCALENDAR not found.");
            exit(EXIT_FAILURE);
        }

        line_number++;
    }

    /* If the final state is not END_VCALENDAR_STATE, it meanas END:VCALENDAR is missing in the ics file */
    if (fsm.currentState != END_VCALENDAR_STATE)
    {
        fprintf(stderr, "Improper ICS file format. END:VEVENT or END:VCALENDAR not found.");
        exit(EXIT_FAILURE);
    }

    fclose(file);
}

int main(int argc, char **argv)
{
    int i;

    if (argc == 1)
    {
        fprintf(stderr, "Please input an ICS file.");
        exit(EXIT_FAILURE);
    }

    for (i = 1; i < argc; i++)
    {
        parseFile(argv[i]);
    }

    return 0;
}
