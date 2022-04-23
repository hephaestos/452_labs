#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>

#define SIZE 16

int main (int argc, char* argv[])
{
   int status;
   long int i, temp, *shmPtr;
   int shmId, semId;
   pid_t pid;

   // get value of loop variable (from command-line argument)
   long int loop = atoi(argv[1]);

   struct sembuf sem_wait = { 0, -1, SEM_UNDO };
   struct sembuf sem_signal = { 0, +1, SEM_UNDO };

   if ((semId = semget (IPC_PRIVATE, 1, 00600)) < 0) {
      perror("failed to create semaphore\n");
      exit(1);
   }

   if (semctl (semId, 0, SETVAL, 1) < 0) {
      perror("failed to initialize the semaphore\n");
      exit(1);
   }

   if ((shmId = shmget (IPC_PRIVATE, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
      perror ("i can't get no..\n");
      exit (1);
   }
   if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
      perror ("can't attach\n");
      exit (1);
   }

   shmPtr[0] = 0;
   shmPtr[1] = 1;

   if (!(pid = fork())) { // child
      for (i=0; i<loop; i++) {
               if(semop(semId, &sem_wait, 1) < 0){
                  perror("semop wait failed\n");
                  exit(1);
               }
               // swap the contents of shmPtr[0] and shmPtr[1]
               temp = shmPtr[0];
               shmPtr[0] = shmPtr[1];
               shmPtr[1] = temp;
               if(semop(semId, &sem_signal, 1) < 0){
                  perror("semop signal failed\n");
                  exit(1);
               }
      }
      if (shmdt (shmPtr) < 0) {
         perror ("just can't let go\n");
         exit (1);
      }
      exit(0);
   }
   else // parent
      for (i=0; i<loop; i++) {
               if(semop(semId, &sem_wait, 1) < 0){
                  perror("semop wait failed\n");
                  exit(1);
               }
               // swap the contents of shmPtr[1] and shmPtr[0]
               temp = shmPtr[0];
               shmPtr[0] = shmPtr[1];
               shmPtr[1] = temp;
               if(semop(semId, &sem_signal, 1) < 0){
                  perror("semop signal failed\n");
                  exit(1);
               }
      }

   wait (&status);
   printf ("values: %li\t%li\n", shmPtr[0], shmPtr[1]);

   if (shmdt (shmPtr) < 0) {
      perror ("just can't let go\n");
      exit (1);
   }
   if (shmctl (shmId, IPC_RMID, 0) < 0) {
      perror ("can't deallocate\n");
      exit(1);
   }

   if (semctl (semId, 0, IPC_RMID) < 0) {
      perror("failed to remove semaphore\n");
      exit(1);
   }

   return 0;
}
