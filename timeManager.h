#ifndef TIME_STRING_PARSER_H
#define TIME_STRING_PARSER_H

time_t processStringToTimeStruct(char* timeString);
time_t convertStringToTime_t(char *timeString);
void printTime(const time_t *time);
char *processTimeStructToString(const time_t *time);
void freeIndividualTimeStrings(char *year, char *month, char *day, char *hour, char *minute, char *second);

#endif
