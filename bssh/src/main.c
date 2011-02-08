/* main.c - A basic simple shell.
 *
 * Creation: 1/27/2011
 *
 * Changelog:
 *    01-27-2011 - Initial Creation.
 *    02-06-2011 - Rewrite based on select.
 *    02-07-2011 - Implement processes.
 *    02-08-2011 - Feature Complete.
 */

#define _POSIX_SOURCE // From http://osdir.com/ml/minix3/2011-01/msg00227.html. 

#include <sys/types.h>
#include <sys/select.h>
#include <sys/wait.h>

#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

#include "cmd.h"
#include "env.h"

#define TRUE  (0)
#define FALSE (-1)

#define PASS (0)
#define FAIL (-1)

/* my_getline - Reads a line of text from the file descriptor and returns a pointer
 *    to the line with the trailing '\n' character.
 *
 * Variables:
 *    fd - file descriptor to read from.
 *    line - pointer to a line of text.  It will be allocated by this function but
 *       should be freed by the user.
 *
 * Return:
 *    PASS - On success.
 *    FAIL - On failure to allocate necessary memory.
 */
int my_getline(FILE *fd, char** line);

/* interpret_line - Interpret a line of code.
 *
 * Variables:
 *    line - A line of code to execute.
 *
 * Return:
 *    The return code of the various programs with a nonzero return code being an error.
 *
 */
int interpret_line(char* line);

/* prompt_for_quit - altered by signal handler for quitting the program.
 */
volatile static sig_atomic_t prompt_for_quit = 0;

/* sigint_handler - this is the signal handler for the C-c console input.
 */
void sigint_handler(int signum) {
   prompt_for_quit = ! prompt_for_quit; // Ask to quit.
}

/* setup_sighandler - setup SIGINT signal handler.
 *
 * Variables:
 *
 * Return:
 *    PASS - successfully installed.
 *    FAIL - failed to install.
 */
int setup_sighandler() {
   sigset_t sigint_set;
   
   sigfillset(&sigint_set);
   sigdelset(&sigint_set, SIGINT);
   
   struct sigaction sigint_actions;
   
   sigint_actions.sa_handler = sigint_handler;
   sigint_actions.sa_mask    = sigint_set;
   sigint_actions.sa_flags   = 0; 
   
   /* Control Ctl-C signal */
   sigaction(SIGINT, &sigint_actions, NULL);
   
   return 0;
}

int main() {
   fd_set fdset;
   char *line;

   FD_ZERO(&fdset);
   FD_SET(STDIN_FILENO, &fdset);
   
   env_write("PROMPT=PROMPT# ");
   env_write("HOME=/root/");
   env_write("PATH=/usr/bin:/bin");

   setup_sighandler();
   
   for(;;) {

	  env_read("PROMPT", &line);
      printf("%s", line);
      fflush(stdout);
      
      // select returns if input is available or a signal occured.
      select(1, &fdset, NULL, NULL, NULL);
      
      if( prompt_for_quit == 1 ) {
         printf("Are you sure?(y/n)\n");
         
         if(fgetc(stdin) == 'y') {
            break;
         } else {
            prompt_for_quit = 0; // reset prompt.
            while(fgetc(stdin) != '\n'); // Because we are in canonical input mode this will clear the buffer.
            continue;
         }
      }
      
      my_getline(stdin, &line);
      expand_line(&line);
      interpret_line(line);


      free(line);
   }
   
   env_free();
   alias_free();

   // printf("Thank you for using the basic simple shell!\n");
   
   return 0;
}

/* split_line - Splits a string into an array of elements without whitespace. 
 *
 * Variables:
 *    line - string of text to be split.
 *    elements - array of elements to store the result in.
 *
 * Return:
 *    number of elements found in the string.
 */
char** split_line(char* line, int* num_elements) {

   char** elements = NULL;
   
   int    element = 0;
   int    start = 0;
   
   int double_open = 0; // open and closed status.
   int single_open = 0; // open and closed status.
   
   for(int i=0; i < strlen(line); i++) {
      
      if( line[i] == '\"') {
         double_open = ! double_open;
      }
      
      if( line[i] == '\'') {
         single_open = ! single_open;
      }
      
      if( double_open == 0 && single_open == 0 && 
         (line[i] == ' ' || line[i] == '\n' || line[i] == '\t')) {

         elements = realloc(elements, sizeof(char*)*(element+1));
         elements[element] = malloc(sizeof(char)*(i-start+1));
         
         if(elements == NULL || elements[element] == NULL) {
            printf("ERROR: Out of memory\n");
            exit(EXIT_FAILURE);
         }
         
         memcpy(&elements[element][0], &line[start], i-start);
         elements[element][i-start] = '\0';
         
         while(i < strlen(line) && 
               (line[i] == ' ' || line[i] == '\n' || line[i] == '\t') ) {
            i++; // Fast forward to the next non-whitespace character.
         }
         
         start = i;
         element++;
      }
   }
   
   *num_elements = element;
   
   // Good debugging info
   /* for(int j=0; j < element; j++) {
      printf("elements[%i] = \"%s\"\n", j, elements[j]);
   }*/

   return elements;
}

int interpret_line(char* line) {

   cmd* commands_head = cmd_init();
   cmd* command = commands_head;
   
   int num_elements;
   char** elements = split_line(line, &num_elements);
   
   int element  = 0;
   int variable = 0;

   int fds[2] = {STDIN_FILENO, STDOUT_FILENO};

   while(element < num_elements) {
      
      while( element < num_elements && strcmp(elements[element], "|") 
                                    && strcmp(elements[element], "<") 
                                    && strcmp(elements[element], ">")  ) {
         command->argv = realloc(command->argv, sizeof(char*)*(variable+1));
         command->argv[variable] = elements[element];

         element++;
         variable++;
      }

      /*for(int j=0; j < variable; j++) {
         printf("argv[%i] = %s\n", j, command->argv[j]);
      }*/

      if(element < num_elements && strcmp(elements[element], ">") == 0) {
         printf("Redirect STDOUT\n");
         element++;
         
         if(element < num_elements) {
         command->fd_out = open(elements[element], O_WRONLY);

         element++;
         } else {
            printf("ERROR: missing filename\n");
            exit(EXIT_FAILURE);
         }
      }
      
      if(element < num_elements && strcmp(elements[element], "<") == 0) {
         printf("Redirect STDIN\n");
         element++;
         
         if(element < num_elements) {
         command->fd_in = open(elements[element], O_RDONLY);
         element++;
         } else {
            printf("ERROR: missing filename\n");
            exit(EXIT_FAILURE);
         }
      }
      
      if(element < num_elements && strcmp(elements[element], "|") == 0) {

         pipe(fds);
         command->fd_out = fds[1];
         command = cmd_add(commands_head);
         command->fd_in = fds[0];

         element++;
         variable = 0;
      }
   }

   if( strcmp(commands_head->argv[0], "exit") == 0) {
      exit(EXIT_SUCCESS);
      //return EXIT;
   } else if(strcmp(commands_head->argv[0], "export") == 0) {
      printf("Writing env var\n");
      env_write(commands_head->argv[1]);
   } else if(strcmp(commands_head->argv[0], "alias") == 0) {
      printf("Writing alias\n");
      alias_write(commands_head->argv[1]);
   } else {
      cmd_exec(commands_head);
   }

   for(int k = 0; k < num_elements; k++) {
      free(elements[k]);
   }
   free(elements);

   cmd_free(commands_head);
   
   return 0;
}

int my_getline(FILE *fd, char** line) {
   
   int lsize = 80; /* initial line length */
   int lindex = 0; /* index */
   
   (*line) = calloc(sizeof(char), lsize); /* Line of text */

   if(*line == NULL) {
      printf("FATAL: Could not allocate memory for the line\n");
      return -1;
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
            return -1;
         }
      }
   } while ((*line)[lindex-1] != '\n');
   
   return 0;
}

