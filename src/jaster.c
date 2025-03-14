#include <stdio.h>
#include <ctype.h>
#include "jaster.h"


/*static functions prototype*/
static int parse_object(char **json, char parsed[][MAX_LENGTH], int *step);
static int parse_string(char **json, char parsed[][MAX_LENGTH], int *step);
static int parse_array(char **json, char parsed[][MAX_LENGTH], int *step);
static int parse_literal(char **json, char parsed[][MAX_LENGTH], int *step);
static int parse_number(char **json, char parsed[][MAX_LENGTH], int *step);

int parse_json(char *json, char parsed[][MAX_LENGTH])
{
	int step =  0;
	while(*json != '\0') {
		if(*json == ',' || *json == ' ' || *json == '\n' ||
				*json == '\t' || *json == '\r' || *json == ':'){
			json++;
			continue;
		}


		switch(*json){
		case '{':
			parse_object(&json,parsed,&step);
			break;
		case '"':
			/*string*/

			parse_string(&json,parsed,&step);
			break;
		case '[':
			/*array */
			parse_array(&json, parsed,&step);	
			break;
		default:
			if(isalpha(*json))
				parse_literal(&json,parsed,&step);
			else if(isdigit(*json) || *json == '-')
				parse_number(&json,parsed,&step);

			break;
		}

		json++;	
	
	}
	return step;
}

static int parse_object(char **json, char parsed[][MAX_LENGTH], int *step)
{
	(*json)++;
	while(**json != '}' && **json != '\0'){
		if(**json == ',' || **json == ' ' || **json == '\n' ||
			       	**json == '\t' || **json == '\r' || **json == ':'){
			(*json)++;
			continue;
		}


		switch(**json){
		case '{':
			parse_object(json,parsed,step);
			break;
		case '"':
			/*string*/
			parse_string(json,parsed,step);
			break;
		case '[':
			/*array */
			parse_array(json,parsed,step);
			break;
		default:
			if(isalpha(**json))
				parse_literal(json,parsed,step);
			else if(isdigit(**json) || **json == '-')
				parse_number(json,parsed,step);

			break;
		}
	if(**json != '}')		
		(*json)++;
	}

	return 0;
}


static int parse_string(char **json, char parsed[][MAX_LENGTH], int *step)
{
	(*json)++;
	int counter = 0;
	while(**json != '"'){
		parsed[*step][counter] = **json;
		counter++;
		(*json)++;
	}

	(*step)++;
	return 0;
}

static int parse_array(char **json, char parsed[][MAX_LENGTH], int *step)
{
	(*json)++;
	while(**json != ']') {
		if(**json == ',' || **json == ' '){
			(*json)++;
			continue;
		}

		switch(**json){
		case '{':
			parse_object(json,parsed,step);
			(*json)++;
			break;
		case '"':
			/*string*/
			parse_string(json,parsed,step);
			break;
		case '[':
			/*array */
			parse_array(json,parsed,step);
			break;
		default:
			if(isalpha(**json))
				parse_literal(json,parsed,step);
			else if(isdigit(**json) || **json == '-')
				parse_number(json,parsed,step);

			break;
		}

		(*json)++;
	}
	return 0;
}

static int parse_literal(char **json,  char parsed[][MAX_LENGTH], int *step)
{
	int counter = 0;
	while(**json != ',' && **json != ' ' && **json != '}'){
		parsed[*step][counter] = **json;
		counter++;
		(*json)++;
	}
	(*step)++;
	return 0;
}

static int parse_number(char **json,  char parsed[][MAX_LENGTH], int *step)
{
	int counter = 0;
	while(**json != ',' && **json != ' ' && **json != '}'){
		parsed[*step][counter] = **json;
		counter++;
		(*json)++;
	}
	(*step)++;
	return 0;
}
