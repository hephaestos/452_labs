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

int main () {
    signal(SIGINT, sig_handler);
    int shmId;
    char *shmPtr;
    char *turnPtr;
    int keyId = 70;
    key_t shmKey = ftok("shmtext", keyId);
    char enteredString[MAXMESSAGESIZE];

    if ((shmId = shmget (shmKey, FOO, 0666|IPC_CREAT)) < 0) {
        perror ("Can't get Shared Memory ID\n");
        exit (1);
    }
    if ((turnPtr = (char*)shmat(shmId, (void*)0, 0)) == (void*) -1) {
        perror ("Can't attach to Shared Memory\n");
        exit (1);
    }
    shmPtr = turnPtr + sizeof(int);

    do {
        puts("Enter a String");
        fgets(enteredString, MAXMESSAGESIZE, stdin);
        enteredString[strcspn(enteredString, "\n")] = 0;
        strcpy(shmPtr, enteredString);
        printf("String [%s] written to Shared Memory\n", shmPtr);
        shmPtr = shmPtr + MAXMESSAGESIZE; //incrementing the shared address by size of maxsizse
        *turnPtr = *turnPtr + 1; //incrementing the turn int by 1
    } while(1);
}

void sig_handler(int sig_num) {
    if(sig_num == SIGINT) {
        puts("\nClosing Writer Program! Shutting down gracefully.\n");
    exit(0);
    }
}
