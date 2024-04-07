#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "timeManager.h"

void freeIndividualTimeStrings(char *year, char *month, char *day, char *hour, char *minute, char *second);

time_t processStringToTimeStruct(char *timeString) 
{
        // convert the dateStart and dateEnd to a time_t struct
    char* dateStartToken;
    // If the time string contains TZID, check after the first :
    if (strstr(timeString, "TZID") != NULL) {
        dateStartToken = strchr(timeString, ':') + 1;
    } else {
        dateStartToken = timeString;
    }
    struct tm tm = {0};
    if (sscanf(dateStartToken, "%4d%2d%2dT%2d%2d%2d", 
               &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
               &tm.tm_hour, &tm.tm_min, &tm.tm_sec) != 6) {
        // Parsing failed
        return -1; // or handle error as needed
    }

    tm.tm_year -= 1900;  // Adjust for year (years since 1900)
    tm.tm_mon -= 1;      // Adjust for month (0-based index)

    time_t time = mktime(&tm);
    printf("Time: %s", ctime(&time));
    if (time == -1) {
        // Conversion failed
        return -1; // or handle error as needed
    }

    return time;
}

/* Convert time_t back to string YYYYMMDDTHHMMSS */
char *processTimeStructToString(const time_t *time) {
    struct tm *timeStruct = localtime(time);
    char *timeString = (char *)malloc(15);
    sprintf(timeString, "%d%02d%02dT%02d%02d%02d", timeStruct->tm_year + 1900, timeStruct->tm_mon + 1, timeStruct->tm_mday, timeStruct->tm_hour, timeStruct->tm_min, timeStruct->tm_sec);
    return timeString;
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
