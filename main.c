#include <stdio.h>
#include <stdlib.h>
/* Additional custom function files*/
#include "parser.h"
#include "eventTranslator.h"

int main(int argc, char **argv)
{
    int i, j;
    ICS ics;
    char **studentNames;
    /* Check for valid file input */
    if (argc == 1)
    {
        fprintf(stderr, "Please input an ICS file.");
        exit(EXIT_FAILURE);
    }
    studentNames = (char **)malloc(sizeof(char *) * (argc - 1));
    for (i = 1; i < argc; i++)
    {
        char* studentName = (char *)malloc(sizeof(char) * 20);
        ics = parseFile(argv[i]);
        

        printf("ICS Filename: %s\n", ics.filename);
         /* Ask user for student name corresponding to ics file */
        printf("Enter the name of the student that uses this ics file: ");
        scanf("%s", studentName);
        studentNames[i - 1] = studentName;
        printf("No. Events: %d\n", ics.numEvents);
        for (j = 0; j < ics.numEvents; j++)
        {
           
            LLNodeData *data;
            printf("Event %d:\n", j + 1);
            printEvent(ics.events[j]);
            // processEvent function will eventually return a list of LLNodeData structs to be parsed into insert node
            int numberOfNodes = processEvent(ics.events[j], &data);
            for (int k = 0; k < numberOfNodes; k++)
            {
                printf("Student Name: %s\n", data[k].studentName);
                printf("Class Name: %s\n", data[k].className);
                printf("Time Start: %s\n", data[k].timeStart);
                printf("Time End: %s\n", data[k].timeEnd);
            }
            // InsertNode(LLNodeData data) will be called here

        }
        /* print all the student names */
        for (int i = 0; i < argc - 1; i++)
        {
            printf("Student Name: %s\n", studentNames[i]);
        }
        freeICS(&ics);
    }
    return 0;
}
