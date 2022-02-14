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

void sig_handler(int);

int main() {
	signal(SIGINT, sig_handler);
    int shmId;
    char *shmPtr;
    int keyId = 70;
	key_t shmKey;
	char readString[16];

	do {
		shmKey = ftok("shmtext", keyId);
		sleep(1);
		if ((shmId = shmget(shmKey, FOO, 0666)) < 0) {
			perror ("Can't get Shared Memory ID\n");
			exit(1);
		}
		if ((shmPtr = shmat(shmId, NULL, 0)) == (char*) -1) {
			perror ("Can't attach to Shared Memory\n");
            exit (1);
		}
		if (strcmp(readString, shmPtr) != 0) {
			printf("String [%s] read from Shared Memory\n", shmPtr);
		}
		strcpy(readString, shmPtr);
	} while(1);

	return 0;
}

void sig_handler(int sig_num) {
    if(sig_num == SIGINT) {
        puts("\nClosing Reader Program! Shutting down gracefully.\n");
    exit(0);
    }
}