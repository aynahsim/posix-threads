#define _GNU_SOURCE

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SUCCESS 0

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

    pthread_t tid;
    if (pthread_create(&tid, NULL, threadRoutine, NULL) != SUCCESS) {
        perror("MAIN THREAD | pthread_create() failed");

        return EXIT_FAILURE;
    }

    if (pthread_join(tid, NULL) != SUCCESS) {
        perror("MAIN THREAD | pthread_join() failed");

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
