#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jaster.h"

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
		"  \"id\": 456,"
		"  \"username\": \"complex_user\","
		"  \"active\": false,"
		"  \"score\": 1500.75,"
		"  \"profile\": {"
		"    \"first_name\": \"Alice\","
		"    \"last_name\": \"Smith\","
		"    \"age\": 28,"
		"    \"contact\": {"
		"      \"email\": \"alice.smith@example.com\","
		"      \"phone\": \"123-456-7890\""
		"    }"
		"  },"
		"  \"roles\": ["
		"    \"admin\","
		"    \"developer\","
		"    \"tester\""
		"  ],"
		"  \"settings\": {"
		"    \"theme\": \"dark\","
		"    \"language\": \"en-US\","
		"    \"notifications\": {"
		"      \"email\": true,"
		"      \"sms\": false,"
		"      \"push\": true"
		"    }"
		"  },"
		"  \"projects\": ["
		"    {"
		"      \"name\": \"Project Alpha\","
		"      \"status\": \"active\","
		"      \"budget\": 100000.00"
		"    },"
		"    {"
		"      \"name\": \"Project Beta\","
		"      \"status\": \"completed\","
		"      \"budget\": 75000.50"
		"    }"
		"  ],"
		"  \"last_login\": null"
		"}";

	struct object user = {0};

	int steps = parse_json(json, parsed);
	
	jenerator(steps,parsed,jen_str);	
	for(int i = 0; i < steps; i++)
		printf("%s -> %s\n",parsed[i],jen_str[i]);


	return 0;

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
