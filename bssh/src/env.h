/*
 * env.h
 *
 *  Created on: Feb 7, 2011
 *      Author: Spenser Gilliland
 */

#ifndef ENV_H_
#define ENV_H_

extern char** my_envp;
extern char** my_aliases;

/* env_write - updates or adds a value to the environment with the value specified.
 *
 * Variables:
 *    key - The name of the variable to be updated or added.
 *    value - The value that the variable should have.
 *
 * Return:
 *    PASS - when added to env and allocated all memory successfully.
 *    FAIL - on failure to allocate memory
 *    UPDATE - when updated entry to env and allocated all memory successfully.
 */
int env_write(char* value);

/* env_read - reads a value from the environment.
 *
 * Variables:
 *    key - The name of the variable to be read.
 *    value - The value of the variable.
 *
 * Return:
 *    PASS - when variable was found
 *    FAIL - When variable was not found.
 */
int env_read(char* key, char** value);

/* alias_write - add alias to environment.
 *
 * Variables:
 *
 *
 * Return:
 *
 */
int alias_write(char* value);

/* expand_aliases - expand all aliases.
 *
 * Variables:
 *    line - Line of text to expand
 *
 * Return:
 *    PASS - successfully expanded.
 *    FAIL - failure to expand.
 */
int expand_aliases(char** line);

/* expand_env - expand all environmental variables.
 *
 * Variables:
 *    line - Line of text to expand
 *
 * Return:
 *    PASS - successfully expanded.
 *    FAIL - failure to expand.
 */
int expand_env(char** line);

/* env_free - free all env vars.
 *
 * Variables:
 *
 * Return:
 *
 */
void env_free(void);

/* alias_free - free all aliases.
 *
 * Variables:
 *
 * Return:
 *
 */
void alias_free(void);

#endif /* ENV_H_ */
