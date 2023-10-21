#define _GNU_SOURCE

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SUCCESS 0
#define TRUE 1

void printThreadInfo(char *threadName, int pid, int ppid, int tid, pthread_t pthreadId) {
    printf(
        "%s | %s: %d | %s: %d | %s: %d (%lu)\n",
        threadName,
        "Process ID", pid,
        "Parent process ID", ppid,
        "Thread ID", tid, pthreadId
    );
}

void *threadRoutine(void *arg) {
    printThreadInfo("NEW THREAD", getpid(), getppid(), gettid(), pthread_self());

    return NULL;
}

int main(int argc, char **argv) {
    printThreadInfo("MAIN THREAD", getpid(), getppid(), gettid(), pthread_self());

    pthread_attr_t attr;
    if (pthread_attr_init(&attr) != SUCCESS) {
        perror("MAIN THREAD | pthread_attr_init() failed");

        return EXIT_FAILURE;
    }
    if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != SUCCESS) {
        perror("MAIN THREAD | pthread_attr_setdetachstate() failed");

        if (pthread_attr_destroy(&attr) != SUCCESS) {
            perror("MAIN THREAD | pthread_destroy() failed");
        }

        return EXIT_FAILURE;
    }

    pthread_t tid;
    while (TRUE) {
        if (pthread_create(&tid, &attr, threadRoutine, NULL) != SUCCESS) {
            perror("MAIN THREAD | pthread_create() failed");

            if (pthread_attr_destroy(&attr) != SUCCESS) {
                perror("MAIN THREAD | pthread_destroy() failed");
            }

            return EXIT_FAILURE;
        }
    }

    if (pthread_attr_destroy(&attr) != SUCCESS) {
        perror("MAIN THREAD | pthread_destroy() failed");
    }

    return EXIT_SUCCESS;
}
