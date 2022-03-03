#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>

int main() {
    int semId;

    if ((semId = semget (IPC_PRIVATE, 1, 00600)) < 0) {
      perror("failed to create semaphore\n");
      exit(1);
    }

    for(int val = 1; 1; ++val) {
        printf("Setting semaphore to value %d\n", val);
        if (semctl (semId, 0, SETVAL, val) < 0) {
          printf("failed to set the semaphore to value %d\n", val);
          exit(1);
        }
    }
}
