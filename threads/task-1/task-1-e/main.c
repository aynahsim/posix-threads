#define _GNU_SOURCE

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SUCCESS 0
#define THREAD_COUNT 5
#define TRUE 1
#define bool int
#define TEN_SECONDS 10

int globalVar = 0;

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
    int localVar = 0;
    static int localStaticVar = 0;
    const int localConstVar = 0;
    
    printThreadInfo("NEW THREAD", getpid(), getppid(), gettid(), pthread_self());

    printf(
        "\t%s%p\n\t%s%p\n\t%s%p\n\t%s%p\n",
        "NEW THREAD | globalVar:      ", &globalVar,
        "NEW THREAD | localVar:       ", &localVar,
        "NEW THREAD | localStaticVar: ", &localStaticVar,
        "NEW THREAD | localConstVar:  ", &localConstVar
    );

    bool *changeVars = (bool *) arg;
    if (changeVars != NULL && *changeVars == TRUE) {
        printf("\t\tNEW THREAD | I am changing variables...\n");

        localVar = 1;
        localStaticVar = 1;
        globalVar = 1;
    }

    printf(
        "\t\t%s%d\n\t\t%s%d\n\t\t%s%d\n",
        "NEW THREAD | New value of globalVar:      ", globalVar,
        "NEW THREAD | New value of localVar:       ", localVar,
        "NEW THREAD | New value of localStaticVar: ", localStaticVar
    );

    sleep(TEN_SECONDS);

    return NULL;
}

int main(int argc, char **argv) { 
    printThreadInfo("MAIN THREAD", getpid(), getppid(), gettid(), pthread_self());

    pthread_t *tids = (pthread_t *) malloc(THREAD_COUNT * sizeof(pthread_t));
    if (tids == NULL) {
        perror("MAIN THREAD | malloc() failed");

        return EXIT_FAILURE;
    }

    bool changeVars = TRUE;
    if (pthread_create(&tids[0], NULL, threadRoutine, &changeVars) != SUCCESS) {
        perror("MAIN THREAD | pthread_create() failed");

        free(tids);

        return EXIT_FAILURE;
    }

    for (int i = 1; i < THREAD_COUNT; ++i) {
        if (pthread_create(&tids[i], NULL, threadRoutine, NULL) != SUCCESS) {
            perror("MAIN THREAD | pthread_create() failed");

            free(tids);

            return EXIT_FAILURE;
        }

        printf("MAIN THREAD | tids[%d]: %lu\n", i, tids[i]);
    }

    printf(
        "MAIN THREAD | Thread #0 is equals to thread #0: %d\n",
        pthread_equal(tids[0], tids[0])
    );
    printf(
        "MAIN THREAD | Thread #0 is equals to thread #1: %d\n",
        pthread_equal(tids[0], tids[1])
    );

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
