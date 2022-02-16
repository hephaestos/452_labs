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
    char *shmTurnPtr;
    int keyId = 70;
    key_t shmKey = ftok("shmtext", keyId);
    char enteredString[MAXMESSAGESIZE];
    int limiter = sizeof(int);

    //printf("Limit Size = %d\n",limiter);

    if ((shmId = shmget (shmKey, FOO, 0666|IPC_CREAT)) < 0) {
        perror ("Can't get Shared Memory ID\n");
        exit (1);
    }
    if ((shmTurnPtr = (char*)shmat(shmId, (void*)0, 0)) == (void*) -1) {
        perror ("Can't attach to Shared Memory\n");
        exit (1);
    }
    
    shmPtr = shmTurnPtr + sizeof(int);
    *shmTurnPtr = 0;

    do {
        puts("Enter a String");
        fgets(enteredString, MAXMESSAGESIZE, stdin);
        enteredString[strcspn(enteredString, "\n")] = 0;
        strcpy(shmPtr, enteredString);
        printf("Writer turn [%d]\n", *shmTurnPtr+1);
        printf("String [%s] written to Shared Memory\n", shmPtr);
        shmPtr = shmPtr + MAXMESSAGESIZE; //incrementing the shared address by size of maxsizse
        limiter = limiter + MAXMESSAGESIZE;
        if (*shmTurnPtr >= 126) {
            *shmTurnPtr = 0;
        }
        // printf("LIMIT: [%d]\n", limiter);
        // if (limiter >= 4095) {
        //     printf("limit hit!\n");
        //     limiter = sizeof(int);
        //     shmPtr = shmTurnPtr + sizeof(int);
        // }
        *shmTurnPtr =  *shmTurnPtr + 1; //incrementing the turn int by 1
    } while(1);
}

void sig_handler(int sig_num) {
    if(sig_num == SIGINT) {
        puts("\nClosing Writer Program! Shutting down gracefully.\n");
    exit(0);
    }
}
