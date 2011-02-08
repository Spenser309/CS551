/* main.c - A basic simple shell.
 *
 * Creation: 1/27/2011
 *
 * Changelog:
 *    01-27-2011 - Initial Creation
 *    02-04-2011 - Modified to allow ASYNC IO.
 */ 

#define _POSIX_SOURCE // From http://osdir.com/ml/minix3/2011-01/msg00227.html. 

#include <sys/types.h>

#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

#include <pthread.h> // Must compile with -pthread

#define UPDATE  1
#define PASS    0
#define FAIL   -1

#define TRUE  1
#define FALSE 0

volatile static sig_atomic_t prompt_for_quit = 0; // Global var for sigint handler.

char** my_environ;
int my_environ_size;

/* my_getline - Reads a line of text from the file descriptor and returns a pointer
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
 int my_getline(FILE *fd, char** line);

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
int env_write(const char* value);

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

/* exec_profile - Open the PROFILE file and execute scripts.
 * 
 * Variables:
 *    filename - name of PROFILE file to open.
 *
 * Return:
 *    PASS - When file is executed successfully.
 *    FAIL - When file is not executed successfully.
 */
int exec_profile(const char* filename);

/* interpret_line - Interprete a line of code.
 * 
 * Variables:
 *    line - A line of code to execute.
 *
 * Return:
 *    The return code of the various programs with a nonzero return code being an error.
 * 
 */
int interpret_line(char* line);

/* sigint_handler - this is the signal handler for the C-c console input.
 */
void sigint_handler(int signum) {   
   prompt_for_quit = 1;
/*  char response;
   
   fflush(stdin); 
   
   printf("Are you sure(y/n)?\n");
   response = fgetc(stdin);

   if (response == 'y' || response == 'Y') {
      printf("Thank You come again!\n");
      exit(PASS);
   }
   else
   {
	  fflush(stdin); 
      printf("Is that all you got!\n");
   }*/
}


int main(int argc, char **argv) {

   int done = 0;
   int success = FAIL;
   char* line = NULL; /* Define a line buffer. */
   char* prompt = NULL;
   
   sigset_t sigint_set;
   
   /*sigint_set = ~0;*/
   sigfillset(&sigint_set);
   
   struct sigaction sigint_actions;
   
   sigint_actions.sa_handler = sigint_handler;
   sigint_actions.sa_mask    = sigint_set;
   sigint_actions.sa_flags   = 0; 
   
   /* Control Ctl-C signal */
   
   sigaction(SIGINT, &sigint_actions, NULL);
   /*signal(SIGINT, sigint_handler); */
   
   /*  Execute a profile Script */
   /* success = exec_profile("/root/.profile"); */

   
   if(!success) {
      env_write("HOME=/");
      env_write("PROMPT=SETPROMPT-$#");
   }
 
   /* Change to home directory or default. */
   env_read("HOME", &line);
   success = chdir("/");
   free(line); /* Line was allocated on env_read so free here. */


   if(success == 1) {
      printf("FATAL: Could not set home directory\n");
      exit(1);
   }
   
   //fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

           
   do {
      env_read("PROMPT", &prompt);
      printf("%s", prompt);
      fflush(stdout);
      free(prompt); /* Line was allocated by env_read so free here. */
   
      if(prompt_for_quit == 1) {
         printf("Are you sure?(y/n)\n");
         my_getline(stdin, &line);
         
         if(strcmp(line, "n\n") == 0) {
            done = 1;
         }
         free(line);
      } else {
         my_getline(stdin, &line);
         printf("%s",line);
         free(line);
      }
      
   } while(!done);
   
   return 0;
}


/* Helper functions. */

int my_getline(FILE *fd, char** line) {
   
   int lsize = 80; /* initial line length */
   int lindex = 0; /* index */
   
   (*line) = calloc(sizeof(char), lsize); /* Line of text */

   if(*line == NULL) {
      printf("FATAL: Could not allocate memory for the line\n");
      return FAIL;
   }
   
   do { 
      (*line)[lindex] = fgetc(fd);
      
      lindex++; /* Always one ahead */
      
      if (lindex >= lsize) { /* Check if we should allocate a larger line buffer. */
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

//addition
int interpret_line(char *line){ /*
   int i;
   char *cmd = line;
   int index = 0;
   char arguments[5];
   char ret[100];
   
   memset(ret, '\0', 100); //initialising the ret array to all zeros
   
   if(strcmp(line,"exit") == 0) {
      printf("Bye");
      sleep(1);
      exit(PASS); //going by the way we are executing the exit in "ctrl+c" handler
   }
   
   //Step #1:We parse the input and put each word seperated by comma as individual elements an array.
   //first element is the function
   //anything that follows with "-x" is an option
   //everything else is an parameter for the function

   while(*cmd != '\0') { //until we have not reached the end of array
      if(index == 5) //check if the max number of arugments have been reached 
         break;

      if(*cmd == ' ') {
         if(arguments[index] == NULL) { //create memory to store this argument
            arguments[index] = (char *)malloc(sizeof(char) * strlen(ret) + 1);
         } else { //if there was something already.set it to zero
            bzero(arguments[index], strlen(arguments[index]));
         }
         
         strncpy(arguments[index], ret, strlen(ret));
         strncat(arguments[index], "\0", 1);
         bzero(ret, 100);
         index++;
      } else {
         strncat(ret, cmd, 1); //add this to the ret array
      }
      cmd++;
   }
   
   if(ret[0] != '\0') { 
      arguments[index] = (char *)malloc(sizeof(char) * strlen(ret) + 1);
      strncpy(arguments[index], ret, strlen(ret));
      strncat(arguments[index], "\0", 1);
   }

   if(fork() == 0) {
      i = execlp(cmd, my_argv, my_envp);
      if(i < 0) {
         printf("%s: %s\n", cmd, "command not found");
         exit(1);
      }
   } else {
      wait(NULL);
   } */
   
   return 0;
} 


int env_write(const char* value) {
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
   *value = calloc(sizeof(char),20);
   strcpy(*value,"prompt# ");
   return PASS;
}
