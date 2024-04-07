#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "timeManager.h"

void freeIndividualTimeStrings(char *year, char *month, char *day, char *hour, char *minute, char *second);

time_t processStringToTimeStruct(char *timeString) {
    char *dateStartToken;
    char *year;
    char *month;
    char *day;
    char *time;
    char *hour;
    char *minute;
    char *second;
    struct tm strDtStart;
    time_t t;
    /* convert the dateStart and dateEnd to a time_t struct */
    /* Start only after the first : */
    dateStartToken = strchr(timeString, ':') + 1;
    /* The year is the first 4 characters */
    year = (char *)malloc(4);
    strncpy(year, dateStartToken, 4);
    /* The month is the next 2 characters */
    month = (char *)malloc(2);
    strncpy(month, dateStartToken + 4, 2);
    /* The day is the next 2 characters */
    day = (char *)malloc(2);
    strncpy(day, dateStartToken + 6, 2);
    /* The time is the next 6 characters */
    time = (char *)malloc(6);
    strncpy(time, dateStartToken + 9, 6);
    /* Separate the time into hour, minute, second */
    hour = (char *)malloc(2);
    strncpy(hour, time, 2);
    minute = (char *)malloc(2);
    strncpy(minute, time + 2, 2);
    second = (char *)malloc(2);
    strncpy(second, time + 4, 2);
    /* create time_t with the above values */
    strDtStart.tm_year = atoi(year) - 1900;
    strDtStart.tm_mon = atoi(month) - 1;
    strDtStart.tm_mday = atoi(day);
    strDtStart.tm_hour = atoi(hour);
    strDtStart.tm_min = atoi(minute);
    strDtStart.tm_sec = atoi(second);
    t = mktime(&strDtStart);
    freeIndividualTimeStrings(year, month, day, hour, minute, second);
    return t;
}

/*
    Converts a string to a time_t
*/
time_t convertStringToTime_t(char *timeString)
{
    struct tm tm = {0};
    if (sscanf(timeString, "%4d%2d%2dT%2d%2d%2d", 
               &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
               &tm.tm_hour, &tm.tm_min, &tm.tm_sec) != 6) {
        // Parsing failed
        return -1; // or handle error as needed
    }

    tm.tm_year -= 1900;  // Adjust for year (years since 1900)
    tm.tm_mon -= 1;      // Adjust for month (0-based index)

    time_t time = mktime(&tm);
    if (time == -1) {
        // Conversion failed
        return -1; // or handle error as needed
    }

    return time;
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
