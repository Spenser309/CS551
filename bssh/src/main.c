#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define PASS 0
#define FAIL 1

int getline(FILE *fd, char* line);

int env_write(const char* key, const char* value);
int env_read(const char* key, char* value);

typedef struct {
   char* key;
   char* value;
} env_entry;

env_entry *env; // Envrionmental variables.

int exec_profile(const char* filename);
int interpret_line(char* line);

void sigint_handler() {

   char response;

   printf("Are you sure?");
   scanf("%c", &response);
   
   if (response == 'y' || response == 'Y') {
      printf("Thank You come again!\n");
      exit(PASS);
   }   
   
   signal(SIGINT, sigint_handler);
}

int main(int argc, char **argv) {

   int success = FAIL;
   char* homedir;
   char* line; // Define a line buffer.
   
   // Control Ctl-C signal
   signal(SIGINT, sigint_handler);
   
   // Execute a profile Script
   //success = exec_profile("/root/.profile");

   if(!success) {
      env_write("HOME","/");
      env_write("PROMPT","#");
   }

   // Change to home directory or default.
   env_read("HOME", homedir);
   success = chdir(homedir);
   
   if(!success) {
      printf("FATAL: Could not set home directory\n");
      exit(1);
   }
   
   do {
     getline((FILE*) STDIN_FILENO, line);
     interpret_line(line); 
   } while(!exit);
   
   return 0;
}

int getline(FILE *fd, char* line) {
 
   int lsize = 80; // initial line length
   int lindex = 0; // index
   
   line = malloc(sizeof(char)*lsize); // Line of text 

   if(line == NULL) {
      printf("FATAL: Could not allocate memory for the line\n"); 
      return FAIL;
   }
   
   while(line[lindex] = getc(fd) == '\n') { 
      lindex++; // Always one ahead
      
      if (lindex >= lsize) { // Check if we should allocate a larger line buffer.
      
         lsize += 10;
         line = realloc(line, sizeof(char) * lsize);
         
         if( line == NULL) {
            printf("FATAL: Could not reallocate memory for the line\n");
            return FAIL;
         }
      }
   }
   
   return PASS;
}
