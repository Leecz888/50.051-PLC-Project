#include <stdio.h>
#include <stdlib.h>
/* Additional custom function files*/
#include "parser.h"

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
        }
        freeICS(&ics);
    }
    return 0;
}
