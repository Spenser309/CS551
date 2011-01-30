/* main.c - A basic simple shell.
 *
 * Creation: 1/27/2011
 *
 * Changelog:
 *    01-27-2011 - Initial Creation
 *
 */ 
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define UPDATE  1
#define PASS    0
#define FAIL   -1

#define TRUE  1
#define FALSE 0

/* env_entry is an entry in the environment array.   
 */
typedef struct {
   char* key;   // Variable name
   char* value; // variable value
} env_entry;

env_entry *env; // envrionmental variables.

/* getline - Reads a line of text from the file descriptor and returns a pointer
 *    to the line with the trailing '\n' character.
 *
 * Variables:
 *    fd - file descriptor to read from.  Use STDIN_FILENO for stdin.
 *    line - pointer to a line of text.  It will be allocated by this function but
 *       should be freed by the user.
 *  
 * Return:
 *    PASS - On success.
 *    FAIL - On failure to allocate neccessary memory.
 */
int getline(FILE *fd, char** line);

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
int env_write(const char* key, const char* value);

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
int env_read(const char* key, char** value);

/* exec_profile - 
 */
int exec_profile(const char* filename);

/* interpret_line - 
 */
int interpret_line(char* line);

/* sigint_handdler - this is the signal handler for the C-c console input.
 */
void sigint_handler() {
       sigset_t mask_set;	/* used to set a signal masking set. */
    sigset_t old_set;	/* used to store the old mask set.   */
    
   signal(SIGINT, sigint_handler);
   /* mask any further signals while we're inside the handler. */
   sigfillset(&mask_set);
   sigprocmask(SIG_SETMASK, &mask_set, &old_set);
   
   char response;
   
   //while(fgetc(stdin) != EOF); // Flush the input buffer.
   
   printf("Are you sure(y/n)?\n");
   response = fgetc(stdin);

   
   if (response == 'y' || response == 'Y') {
      printf("Thank You come again!\n");
      exit(PASS);
   }

}


int main(int argc, char **argv) {

   int done = 0;
   int success = FAIL;
   char* homedir;
   char* line = NULL; // Define a line buffer.
   
   // Control Ctl-C signal
   signal(SIGINT, sigint_handler);
   
   // Execute a profile Script
   //success = exec_profile("/root/.profile");

   
   if(!success) {
      env_write("HOME","/");
      env_write("PROMPT","SETPROMPT-$#");
   }
 
   // Change to home directory or default.
   env_read("HOME", &line);
   success = chdir("/");
   free(line); // Line was allocated on env_read so free here.


   if(success == 1) {
      printf("FATAL: Could not set home directory\n");
      exit(1);
   }
            
   do {
      // Retrieve and print the prompt
      env_read("PROMPT", &line);
      printf("%s", line);
      free(line); // Line was allocated with env_read so free here.

      // Get the users command.
      getline(stdin, &line);
      printf("%s", line);
      // interpret_line(line);
      free(line); // Line was allocated on getline so free here.
   } while(!done);
   
   return 0;
}


// Helper functions.

int getline(FILE *fd, char** line) {
 
   int lsize = 80; // initial line length
   int lindex = 0; // index
   
   (*line) = malloc(sizeof(char)*lsize); // Line of text

   if(*line == NULL) {
      printf("FATAL: Could not allocate memory for the line\n");
      return FAIL;
   }
   
   do { 
      (*line)[lindex] = fgetc(fd);
      
      lindex++; // Always one ahead
      
      if (lindex >= lsize) { // Check if we should allocate a larger line buffer.
         lsize += 10;
         (*line) = realloc(*line, sizeof(char) * lsize);
         
         if( *line == NULL) {
            printf("FATAL: Could not reallocate memory for the line\n");
            free(*line);
            return FAIL;
         }
      }
   } while ((*line)[lindex-1] != '\n');
   
   return PASS;
}

int env_write(const char* key, const char* value) {
   static int env_index = 0;
   /*
   int i;
   
   for(i = 0; i < env_index; i++) {
      if(strcmp(env[i].key, key)) {
         env[i].value = value;
         return UPDATE;
      } 
   }
   
   env_index++;
   
   // Allocate some more memory
   // Add the entry.
   
   env[env_index].key = key;
   env[env_index].value = value; 
   */
   return PASS;
}

int env_read(const char* key, char** value) {
   *value = malloc(sizeof(char)*20);
   strcpy(*value,"prompt# ");
   return PASS;
}
