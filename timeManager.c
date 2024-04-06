#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "timeManager.h"

void freeIndividualTimeStrings(char *year, char *month, char *day, char *hour, char *minute, char *second);

time_t processStringToTimeStruct(char *timeString) {
        // convert the dateStart and dateEnd to a time_t struct
    // Start only after the first :
    char* dateStartToken = strchr(timeString, ':') + 1;
    // The year is the first 4 characters
    char *year = (char *)malloc(4);
    strncpy(year, dateStartToken, 4);
    // The month is the next 2 characters
    char *month = (char *)malloc(2);
    strncpy(month, dateStartToken + 4, 2);
    // The day is the next 2 characters
    char *day = (char *)malloc(2);
    strncpy(day, dateStartToken + 6, 2);
    // The time is the next 6 characters
    char *time = (char *)malloc(6);
    strncpy(time, dateStartToken + 9, 6);
    // Separate the time into hour, minute, second
    char *hour = (char *)malloc(2);
    strncpy(hour, time, 2);
    char *minute = (char *)malloc(2);
    strncpy(minute, time + 2, 2);
    char *second = (char *)malloc(2);
    strncpy(second, time + 4, 2);
    // create time_t with the above values
    struct tm strDtStart = {
        .tm_year = atoi(year) - 1900,
        .tm_mon = atoi(month) - 1,
        .tm_mday = atoi(day),
        .tm_hour = atoi(hour),
        .tm_min = atoi(minute),
        .tm_sec = atoi(second)
    };
    time_t t = mktime(&strDtStart);
    freeIndividualTimeStrings(year, month, day, hour, minute, second);
    return t;
}

void freeIndividualTimeStrings(char *year, char *month, char *day, char *hour, char *minute, char *second) {
    free(year);
    free(month);
    free(day);
    free(hour);
    free(minute);
    free(second);
}

void printTime(const time_t *time) {

    printf("Time: %s", ctime(time));
}
