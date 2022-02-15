#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define FOO 4096
#define MAXMESSAGESIZE 16

void sig_handler(int);

int main() {
	signal(SIGINT, sig_handler);
    int shmId;
    char *shmPtr;
    char *turnPtr;
    int keyId = 70;
	key_t shmKey = ftok("shmtext", keyId);
	char readString[MAXMESSAGESIZE];

    if ((shmId = shmget(shmKey, FOO, 0666)) < 0) {
		perror ("Can't get Shared Memory ID\n");
		exit(1);
	}
    if ((turnPtr = shmat(shmId, NULL, 0)) == (char*) -1) {
		perror ("Can't attach to Shared Memory\n");
        exit (1);
    }
    shmPtr = turnPtr + sizeof(int);

	do {
        //if() //check if the turn pointer is the shared address we need
        strcpy(readString, shmPtr);
		printf("String [%s] read from Shared Memory\n", readString);
        shmPtr = shmPtr + MAXMESSAGESIZE;
        //sleep(2);
        //}
	} while(1);

	return 0;
}

void sig_handler(int sig_num) {
    if(sig_num == SIGINT) {
        puts("\nClosing Reader Program! Shutting down gracefully.\n");
    exit(0);
    }
}
