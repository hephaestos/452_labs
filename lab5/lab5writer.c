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

int main () {
    signal(SIGINT, sig_handler);
    int shmId;
    char *shmPtr;
    int keyId = 70;
    key_t shmKey;
    char enteredString[16];

    do {
        shmKey = ftok("shmtext", keyId);
        if ((shmId = shmget (shmKey, FOO, 0666|IPC_CREAT)) < 0) {
            perror ("Can't get Shared Memory ID\n");
            exit (1);
        }
        if ((shmPtr = (char*)shmat(shmId, (void*)0, 0)) == (void*) -1) {
            perror ("Can't attach to Shared Memory\n");
            exit (1);
        }
        puts("Enter a String");
        fgets(enteredString, sizeof(enteredString), stdin);
        enteredString[strcspn(enteredString, "\n")] = 0;
        strcpy(shmPtr, enteredString);
        printf("String [%s] written to Shared Memory\n", shmPtr);
        enteredString[0] = '\0';
    } while(1);
}

void sig_handler(int sig_num) {
    if(sig_num == SIGINT) {
        puts("\nClosing Writer Program! Shutting down gracefully.\n");
    exit(0);
    }
}
