#ifndef _JPARSE_H
#define _JPARSE_H

#define MAX_LENGTH 500


/* al the values parsed form the json */
extern char parsed[MAX_LENGTH][MAX_LENGTH];

int parse_json(char *json, char parsed[][MAX_LENGTH]);

#endif /* jparse.h */
