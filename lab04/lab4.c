#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

void sig_handler(int);
void* worker (void* arg);

int file_requests = 0;

int main () {
   signal(SIGINT, sig_handler);   
   int status;
   char filename[32];
   
   do {
      puts("Enter a file name:");
      fgets(filename, 32, stdin);
      filename[strcspn(filename, "\n")] = 0;
      file_requests++;
      pthread_t thread1;
      if ((status = pthread_create (&thread1, NULL,  worker, &filename)) != 0) {
         fprintf (stderr, "thread create error %d: %s\n", status, strerror(status));
         exit (1);
      }
   } while(1);
}

void* worker (void* arg) {
	char *val_ptr = (char *) arg;
	
	int time_to_find_file = rand()%5 + 1;
	if (time_to_find_file <= 4) {
	   sleep(1);
	   printf("Found [%s], terminating thread.\n", val_ptr);
	}
	else {
	   int time_to_sleep = rand()%4 + 7;
	   sleep(time_to_sleep);
	   printf("[%s] not found, terminating thread.\n", val_ptr);

	}
	return NULL;
}

void sig_handler(int sig_num) {

    if(sig_num == SIGINT) {
        printf("\nClosing Program. Total number of file requests recieved: [%d].\n", file_requests);
        exit(0);
    }
}
