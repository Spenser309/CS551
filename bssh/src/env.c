/*
 * env.c
 *
 *  Created on: Feb 7, 2011
 *      Author: Spenser Gilliland
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "env.h"


char** my_envp = NULL;
char** my_aliases = NULL;

int num_env   = 0;
int num_aliases = 0;

#define PASS (0)
#define FAIL (-1)

int env_write(char* value) {

    int key_size = strcspn(value,"=");
	char* key = malloc(sizeof(char)*(key_size+1));
	strncpy(key, value, key_size);
	key[key_size] = '\0';

	int found = 0;

	for(int j=0; j < num_env; j++) {
		if(strspn(my_envp[j], key) == strlen(key)) {
			//printf("Updating Entry\n");
			my_envp[j] = realloc(my_envp[j], sizeof(char)*(strlen(value)+1));
			strcpy(my_envp[j], value);
			found = 1;
		}
	}

	if(!found) {
		//printf("Adding Entry\n");
		my_envp = realloc(my_envp, sizeof(char*)*(num_env+2));
		my_envp[num_env] = malloc(sizeof(char)*(strlen(value)+1));
		strcpy(my_envp[num_env], value);
		my_envp[num_env+1] = NULL;

		if(my_envp == NULL || my_envp[num_env] == NULL) {
			printf("ERROR: Out of Memory.\n");
			exit(EXIT_FAILURE);
		}

		num_env++;
	}

	free(key);

	return PASS;
}

int env_read(char* key, char** value) {

    for(int j = 0; j < num_env; j++) {
    	if(strspn(my_envp[j], key) == strlen(key)) {
    		*value = &my_envp[j][strlen(key)+1];
    	}
    }

	return PASS;
}

int alias_write(char* value) {
	int key_size = strcspn(value,"=");
	char* key = malloc(sizeof(char)*(key_size+1));
	strncpy(key, value, key_size);
	key[key_size] = '\0';

	// printf("extracted key = \"%s\" from value = \"%s\"\n", key, value);

	int found = 0;

	for(int j=0; j < num_aliases; j++) {
		if(strspn(my_aliases[j], key) == strlen(key)) {
			//printf("Updating Entry\n");
			my_aliases[j] = realloc(my_aliases[j], sizeof(char)*(strlen(value)+1));
			strcpy(my_aliases[j], value);
			found = 1;
		}
	}

	if(!found) {
		my_aliases = realloc(my_aliases, sizeof(char*)*(num_aliases+1));
		my_aliases[num_aliases] = malloc(sizeof(char)*(strlen(value)+1));

		if(my_aliases == NULL || my_aliases[num_aliases] == NULL) {
			printf("ERROR: Out of Memory.\n");
			exit(EXIT_FAILURE);
		}

		strcpy(my_aliases[num_aliases], value);

		num_aliases++;
	}
	free(key);

	return PASS;
}

int expand_aliases(char** line) {
	char* token = NULL;
	char* value = NULL;
	char* found = NULL;
	char* newline = NULL;
	int equal_loc = 0;
	int newline_size = 0;

    for(int a = 0; a < num_aliases; a++) {

    	equal_loc = strcspn(my_aliases[a],"=");
    	token = malloc(sizeof(char)*(equal_loc+1));
    	strncpy(token, my_aliases[a], equal_loc);
    	token[equal_loc] = '\0';

    	found = strstr(*line, token);

    	if(found != NULL) {
    		//printf("Found string in command updating command\n");
    		value = &my_aliases[a][equal_loc+1];
    		newline_size = strlen(*line)+strlen(value)-strlen(token);
    		newline = malloc(sizeof(char)*(newline_size+1));

    		memcpy(newline, *line, (int) (found - *line));
    		newline[(int)(found - *line)] = '\0';

    		strcat(newline, value);
    		found += strlen(token);
    		strcat(newline, found);

    		//printf("Original Line: %s\n", *line);
    		free(*line);

    		*line = newline;
    		//printf("Altered Line: %s\n", *line);
    	}

    	free(token);
    }


	return PASS;
}

int expand_env(char** line) {
	char* token = NULL;
	char* value = NULL;
	char* found = NULL;
	char* newline = NULL;
	int equal_loc = 0;
	int newline_size = 0;

    for(int a = 0; a < num_env; a++) {
    	equal_loc = strcspn(my_envp[a],"=");
    	token = malloc(sizeof(char)*(equal_loc+2));
    	token[0] = '$';
    	strncpy(&token[1], my_envp[a], equal_loc);

    	token[equal_loc+1] = '\0';

    	found = strstr(*line, token);

    	if(found != NULL) {
    		//printf("Found string in command updating command\n");
    		value = &my_envp[a][equal_loc+1];
    		newline_size = strlen(*line)+strlen(value)-strlen(token);
    		newline = malloc(sizeof(char)*(newline_size+1));

    		memcpy(newline, *line, (int) (found - *line));
    		newline[(int)(found - *line)] = '\0';

    		strcat(newline, value);
    		found += strlen(token);
    		strcat(newline, found);

    		//printf("Original Line: %s\n", *line);
    		free(*line);

    		*line = newline;
    		//printf("Altered Line: %s\n", *line);
    	}

    	free(token);
    }

	return PASS;
}

void env_free(void) {
	for(int i = 0; i < num_env; i++) {
		free(my_envp[i]);
	}
	free(my_envp);
}

void alias_free(void) {
	for(int i=0; i < num_aliases; i++) {
		free(my_aliases[i]);
	}
	free(my_aliases);
}
