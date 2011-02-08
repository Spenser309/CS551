/*
 * env.c
 *
 *  Created on: Feb 7, 2011
 *      Author: spenser
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

int env_write(const char* value) {

    my_envp = realloc(my_envp, sizeof(char*)*(num_env+2));
	my_envp[num_env] = malloc(sizeof(char)*strlen(value));
	strcpy(my_envp[num_env], value);

	num_env++;
	my_envp[num_env] = NULL;

	return PASS;
}

int env_read(const char* key, char** value) {

    for(int j = 0; j < num_env; j++) {
    	if(strspn(my_envp[j], key) == strlen(key)) {
    		*value = &my_envp[j][strlen(key)+1];
    	}
    }

	return PASS;
}

int alias_write(const char* value) {

	my_aliases = realloc(my_aliases, sizeof(char*)*(num_aliases+1));
	my_aliases[num_aliases] = malloc(sizeof(char)*strlen(value));

	if(my_aliases == NULL || my_aliases[num_aliases] == NULL) {
		printf("ERROR: Out of Memory.\n");
		exit(EXIT_FAILURE);
	}

	strcpy(my_aliases[num_aliases], value);

	num_aliases++;

	return PASS;
}

int expand_line(char** line) {
    for(int a = 0; a < num_aliases; a++) {

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
