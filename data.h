#ifndef _DATA_H
#define _DATA_H


#include "jaster.h" /* for MAX_STRING_LENGTH */


struct Profile {
	char first_name[MAX_STRING_LENGTH];
	char last_name[MAX_STRING_LENGTH];
	long age;
	struct Contact contact;
};

struct Settings {
	char theme[MAX_STRING_LENGTH];
	char language[MAX_STRING_LENGTH];
	struct Notifications notifications;
};

struct Contact {
	char email[MAX_STRING_LENGTH];
	char phone[MAX_STRING_LENGTH];
};

struct Notifications {
	int email;
	int sms;
	int push;
};

struct Object {
	long id;
	char username[MAX_STRING_LENGTH];
	int active;
	double score;
	struct Profile profile;
	char roles[3];
	struct Settings settings;
	char name[MAX_STRING_LENGTH];
	char status[MAX_STRING_LENGTH];
	double budget;
	char name[MAX_STRING_LENGTH];
	char status[MAX_STRING_LENGTH];
	double budget;
	char last_login[MAX_STRING_LENGTH];
