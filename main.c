#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Additional custom function files*/
#include "parser.h"
#include "eventTranslator.h"
#include "linkedList.h"
#include "csvConverter.h"

int main(int argc, char **argv)
{
    int i, j, k;
    ICS ics;
    char **studentIDS;
    char* studentID;
    timeLinkedList *list;
    int numberOfNodes;
    int validStudentID;
    /* Check for valid file input */
    if (argc == 1)
    {
        fprintf(stderr, "Please input an ICS file.");
        exit(EXIT_FAILURE);
    }
    studentIDS = (char **)malloc(sizeof(char *) * (argc - 1));
    /* Initialise linkedlist */
    list = createLinkedList();
    for (i = 1; i < argc; i++)
    {
        validStudentID = 0;
        studentID = (char *)malloc(sizeof(char) * 20);
        ics = parseFile(argv[i]);

        do {
            printf("ICS Filename: %s\n", ics.filename);
            /* Ask user for student name corresponding to ics file */
            printf("Enter the ID of the student that uses this ics file: ");

            scanf("%s", studentID);
            if (atoi(studentID) == 0)
            {
                printf("Please enter a valid student ID: ");
                continue;
            }
            if (i-1 > 0)
            {
                int foundMatchingID;
                foundMatchingID = 0;
                for (j = 0; j < i-1; j++)
                {
                    if (strcmp(studentID, studentIDS[j]) == 0)
                    {
                        printf("Student ID already exists. Please enter a different student ID: ");
                        foundMatchingID = 1;
                        validStudentID = 0;
                        continue;
                    }
                }
                if (foundMatchingID == 1) {
                    continue;
                }
                validStudentID = 1;
            } 
            validStudentID = 1;
        } while(!validStudentID);


        studentIDS[i - 1] = studentID;
        printf("No. Events: %d\n", ics.numEvents);
        for (j = 0; j < ics.numEvents; j++)
        {
           
            LLNodeData *data;
            printf("Event %d:\n", j + 1);
            printEvent(ics.events[j]);
            numberOfNodes = processEvent(ics.events[j], &data, studentID);
            printf("Number of Nodes: %d\n", numberOfNodes);
            for (k = 0; k < numberOfNodes; k++)
            {
                printf("Student Name: %d\n", data[k].studentID);
                printf("Class Name: %s\n", data[k].className);
                printf("Time Start: %s\n", data[k].timeStart);
                printf("Time End: %s\n", data[k].timeEnd);
                addNode(list, data, convertStringToTime_t(data[k].timeStart), convertStringToTime_t(data[k].timeEnd));
            }
            printf("list size: %d\n", list->size);


        }
        /* print all the student names */
    }
    processLinkedList(list, studentIDS, argc - 1);
    freeList(list);
    return 0;
}
