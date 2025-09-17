#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

struct shared_data {
    int x;
    int child_done;
};

int main() {
    key_t key;
    int shmid;
    struct shared_data *shared;

    key = ftok(".", 1);
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    shmid = shmget(key, sizeof(struct shared_data), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    shared = (struct shared_data*) shmat(shmid, NULL, 0);
    if (shared == (void*) -1) {
        perror("shmat");
        exit(1);
    }

    shared->x = 19530;
    shared->child_done = 0;

    printf("x = %d\n", shared->x);

    for (int i = 1; i <= 5; i++) {
        printf("ITERATION %d\n", i);

        shared->child_done = 0;
        fflush(stdout);

        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(1);
        }

        if (pid == 0) {
            shared->x -= 5;
            printf("Child : x = %d\n", shared->x);
            shared->child_done = 1;
            shmdt(shared);
            exit(0);
        } else {
            while (shared->child_done == 0) {
                usleep(1000);
            }

            shared->x /= 5;
            printf("Parent : x = %d\n", shared->x);

            int status;
            wait(&status);
        }
    }

    shmdt(shared);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}