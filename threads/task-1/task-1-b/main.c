#define _GNU_SOURCE

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SUCCESS 0
#define THREAD_COUNT 5

void printThreadInfo(char *threadName, int pid, int ppid, int tid) {
    printf(
        "%s | %s: %d | %s: %d | %s: %d (%lu)\n",
        threadName,
        "Process ID", pid,
        "Parent process ID", ppid,
        "Thread ID", tid
    );
}

void *threadRoutine(void *arg) {
    printThreadInfo("NEW THREAD", getpid(), getppid(), gettid());

    return NULL;
}

int main(int argc, char **argv) { 
    printThreadInfo("MAIN THREAD", getpid(), getppid(), gettid());

    pthread_t *tids = (pthread_t *) malloc(THREAD_COUNT * sizeof(pthread_t));
    if (tids == NULL) {
        perror("MAIN THREAD | malloc() failed");

        return EXIT_FAILURE;
    }

    for (int i = 0; i < THREAD_COUNT; ++i) {
        if (pthread_create(&tids[i], NULL, threadRoutine, NULL) != SUCCESS) {
            perror("MAIN THREAD | pthread_create() failed");

            free(tids);

            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < THREAD_COUNT; ++i) {
        if (pthread_join(tids[i], NULL) != SUCCESS) {
            perror("MAIN THREAD | pthread_join() failed");

            free(tids);

            return EXIT_FAILURE;
        }
    }

    free(tids);

    return EXIT_SUCCESS;
}
