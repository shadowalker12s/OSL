#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_KEY 1234 // Shared memory key
#define SHM_SIZE 1024 // Shared memory size

int main() {
    // Create a shared memory segment
    int shmid = shmget(SHM_KEY, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Attach to the shared memory segment
    char *shm = (char *)shmat(shmid, NULL, 0);
    if (shm == (char *)-1) {
        perror("shmat");
        exit(1);
    }

    // Write a message to the shared memory segment
    strcpy(shm, "Hello from the server!");

    // Detach from the shared memory segment
    shmdt(shm);

    printf("Server: Message written to shared memory.\n");

    return 0;
}
