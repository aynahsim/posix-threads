#define _GNU_SOURCE

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    printThreadInfo("MAIN THREAD", getpid(), getppid(), gettid(), pthread_self());

    char *result = (char *) malloc((strlen("Hello, World!") + 1) * sizeof(char));
    if (result == NULL) {
        perror("NEW THREAD | malloc() failed");

        return NULL;
    }
    strcpy(result, "Hello, World!");

    return (void *) result;
}

int main(int argc, char **argv) {
    printThreadInfo("MAIN THREAD", getpid(), getppid(), gettid(), pthread_self());

    pthread_t tid;
    if (pthread_create(&tid, NULL, threadRoutine, NULL) != SUCCESS) {
        perror("MAIN THREAD | pthread_create() failed");

        return EXIT_FAILURE;
    }

    void *result = NULL;

    if (pthread_join(tid, &result) != SUCCESS) {
        perror("MAIN THREAD | pthread_join() failed");

        return EXIT_FAILURE;
    }

    if (result != NULL) {
        printf("MAIN THREAD | New thread returned: %s\n", (char *) result);

        free(result);
    }

    return EXIT_SUCCESS;
}
