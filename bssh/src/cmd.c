/*
 * cmd.c
 *
 *  Created on: Feb 7, 2011
 *      Author: spenser
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <sys/wait.h>

#include "cmd.h"
#include "env.h"

cmd* cmd_init(void) {
   cmd* command = malloc(sizeof(cmd));

   command->argv = malloc(sizeof(char*));

   command->fd_in = STDIN_FILENO;
   command->fd_out = STDOUT_FILENO;

   command->next = NULL;

   return command;
}

cmd* cmd_add(cmd* head) {

   while(head->next != NULL) {
      head = (cmd*) head->next;
   }

   head->next = (struct cmd*) cmd_init();

   return (cmd*) head->next;
}

void cmd_free(cmd* head) {

   cmd* next_head = NULL;

   while(head != NULL) {
      free(head->argv);
      next_head = (cmd*) head->next;
      free(head);
      head = (cmd*) next_head;
   }
}

int cmd_exec(cmd* head) {
   char* path = NULL;
   char* token = NULL;
   char* try = NULL;


   while(head != NULL) {

      head->pid = fork();

      if(head->pid == 0) {
         // Child
         // Substitute STDIN if necessary
         if(head->fd_in != STDIN_FILENO) {
         	close(STDIN_FILENO);
            dup(head->fd_in);
            close(head->fd_in);
         }
         // Substitute STDOUT if necessary
         if(head->fd_out != STDOUT_FILENO) {
            close(STDOUT_FILENO);
            dup(head->fd_out);
            close(head->fd_out);
         }

         if(head->argv[0][0] == '/') { // If full path then execute this way.
        	 execve(head->argv[0], head->argv, my_envp);
         } else {
        	 env_read("PATH", &path);
        	 token = strtok(path, ":");
        	 while(token != NULL) {
        		// printf("Trying %s\n", token);
        		 try = malloc(sizeof(char)*(strlen(token)+strlen(head->argv[0])));
        		 strcat(try, token);
        		 strcat(try, "/");
        		 strcat(try, head->argv[0]);
        		 execve(try, head->argv, my_envp);
        		 token = strtok(NULL, ":");
        		 free(try);
        	 }
         }

         perror("execve");
         // printf("Failed to execute\n");
         exit(EXIT_FAILURE);
      } else {
        // Parent

    	if(head->fd_in != STDIN_FILENO)
    		close(head->fd_in);
        if(head->fd_out != STDOUT_FILENO)
        	close(head->fd_out);

        wait(NULL);
        head = (cmd*) head->next;
      }
   }

   return 0;
}
