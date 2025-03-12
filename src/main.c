#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jparse.h"

struct profile {
	char first_name[MAX_LENGTH];
	char last_name[MAX_LENGTH];
	int age;
};

struct object{
	int id;
	char  username[MAX_LENGTH];
	int active;
	double score;
	struct profile profile;
};


int main(void)
{
	char *json = 
"{"
"  \"id\": 123,"
"  \"username\": \"simple_user\","
"  \"active\": true,"
"  \"score\": 99.5,"
"  \"profile\": {"
"    \"first_name\": \"John\","
"    \"last_name\": \"Doe\","
"    \"age\": 30"
"  }"
"}";

	struct object user = {0};

	char parsed[MAX_LENGTH][MAX_LENGTH];
	for(int i = 0;i < MAX_LENGTH; i++)
		memset(parsed[i],0,MAX_LENGTH);

	int steps = parse_json(json, parsed);

	char *endptr;
	long l = strtol(parsed[1],&endptr,10);

	if(*endptr == '\0')
		user.id  = atoi(parsed[1]); 

	strncpy(user.username,parsed[3],strlen(parsed[3])); 
	user.active  = strncmp(parsed[5],"true",strlen(parsed[5])) == 0 ? 1 : 0; 

	double  d = strtod(parsed[7],&endptr); 
	if(*endptr == '\0')
		user.score = d;

	strncpy(user.profile.first_name,parsed[10],strlen(parsed[9])); 
	strncpy(user.profile.last_name,parsed[12],strlen(parsed[11])); 
	
	l = strtol(parsed[14],&endptr,10);

	if(*endptr == '\0')
		user.profile.age  = l; 


	// Print to confirm it worked
    printf("User:\n");
    printf("  id: %d\n", user.id);
    printf("  username: %s\n", user.username);
    printf("  active: %s\n", user.active ? "true" : "false");
    printf("  score: %.2f\n", user.score);
    printf("  profile:\n");
    printf("    first_name: %s\n", user.profile.first_name);
    printf("    last_name: %s\n", user.profile.last_name);
    printf("    age: %d\n", user.profile.age);
	return 0;
}
