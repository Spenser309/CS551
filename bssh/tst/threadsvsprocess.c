#define _POSIX_SOURCE // From http://osdir.com/ml/minix3/2011-01/msg00227.html. 

#include <sys/wait.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

void do_work(void);

int main(int argc, char* argv[]) {

	if(strcmp(argv[1], "threads") == 0) {
		printf("Running with threads\n");
      //pthread_init();
      
		pthread_t t1, t2;
		
		pthread_create(&t1, NULL, (void*) do_work, NULL);
		pthread_create(&t2, NULL, (void*) do_work, NULL);

		pthread_join(t1, NULL);
		pthread_join(t2, NULL);

		return 0;

	} else {
		printf("Running with processes\n");

		if(fork() != 0) {
			do_work();
			exit(0);
		}

		if(fork() != 0) {
			do_work();
			exit(0);
		}

		wait(NULL);
		wait(NULL);

		return 0;
	}
}

void do_work(void){

	double number = 9948943;
   int i = 0;
   
	for( i = 0; i < 100000000; i++ ) {
		number = number / i;
	}

}
