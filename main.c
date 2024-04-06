#include <stdio.h>
#include <stdlib.h>
/* Additional custom function files*/
#include "parser.h"
#include "eventTranslator.h"

int main(int argc, char **argv)
{
    int i, j;
    ICS ics;

    /* Check for valid file input */
    if (argc == 1)
    {
        fprintf(stderr, "Please input an ICS file.");
        exit(EXIT_FAILURE);
    }

    for (i = 1; i < argc; i++)
    {
        ics = parseFile(argv[i]);

        printf("ICS Filename: %s\n", ics.filename);
        printf("No. Events: %d\n", ics.numEvents);
        for (j = 0; j < ics.numEvents; j++)
        {
            printf("Event %d:\n", j + 1);
            printEvent(ics.events[j]);
            // processEvent function will eventually return a list of LLNodeData structs to be parsed into insert node
            processEvent(ics.events[j]);
            // InsertNode(LLNodeData data) will be called here

        }
        freeICS(&ics);
    }
    return 0;
}
