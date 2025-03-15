#ifndef _JASTER_H
#define _JASTER_H

#define DATA "data.h"
#define Head "#ifndef _DATA_H\n#define _DATA_H\n\n\n#include \"jaster.h\" /* for MAX_STRING_LENGTH */\n\n\n"
#define CODE_STRCT "struct "


#define MAX_LENGTH 500
#define MAX_STRING_LENGTH 120



#define OBJ 'o'
#define KEY 'k'
#define VALUE 'v'
#define ARRAY 'a'
#define EL 'e'
#define LITERAL 'l'
#define NUM 'n'
#define OBJk "ko"
#define ARRk "ka"
#define END 'x'

/* al the values parsed form the json */
extern char parsed[MAX_LENGTH][MAX_LENGTH];

/* a schema from the json to use to auto generate struct */
extern char jen_str[MAX_LENGTH][5];

int parse_json(char *json, char parsed[][MAX_LENGTH]);
int jenerator(int steps, char parsed[][MAX_LENGTH],char jen_str[][5]);

#endif /* jaster.h */
