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

int shmId;
char *shmTurnPtr;

int main() {
	signal(SIGINT, sig_handler);
    char *shmPtr;
    int keyId = 70;
	key_t shmKey = ftok("shmtext", keyId);
	char readString[MAXMESSAGESIZE];
    int turn = 1;
    int limiter = sizeof(int);

    if ((shmId = shmget(shmKey, FOO, 0666)) < 0) {
		perror ("Can't get Shared Memory ID\n");
		exit(1);
	}
    if ((shmTurnPtr = shmat(shmId, NULL, 0)) == (char*) -1) {
		perror ("Can't attach to Shared Memory\n");
        exit (1);
    }
    shmPtr = shmTurnPtr + sizeof(int);

	do {
        if(*shmTurnPtr == turn) { //check if turns match
            printf("Writer turn [%d] - Reader turn[%d]\n", *shmTurnPtr, turn);
            strcpy(readString, shmPtr);
		    printf("String [%s] read from Shared Memory\n", readString);
            shmPtr = shmPtr + MAXMESSAGESIZE;
            limiter = limiter + MAXMESSAGESIZE;
            if (turn >= 127) {
                turn = 0;
            }
            printf("LIMIT: [%d]\n", limiter);
            if (limiter >= 4096) {
                printf("limit hit!\n");
                limiter = sizeof(int);
                shmPtr = shmTurnPtr + sizeof(int);
            }
            turn++;          
            //sleep(1);
        }
	} while(1);
	return 0;
}

void sig_handler(int sig_num) {
    if(sig_num == SIGINT) {
        puts("\nClosing Reader Program! Shutting down gracefully.\n");
        if (shmdt (shmTurnPtr) < 0) { //dettaching shared memory
            perror ("Can't deattach to Shared Memory\n");
            exit (1);
        }
        exit(0);
    }
}
