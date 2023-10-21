#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FIVE_SECONDS 5
#define SUCCESS 0
#define TRUE 1

void *threadRoutine(void *arg) {
    int localVar = 0;

    while (TRUE) {
        ++localVar;
        pthread_testcancel();
    }

    printf("NEW THREAD | The end of the thread routine\n");

    return NULL;
}

int main(int argc, char **argv) {
    pthread_t tid;
    if (pthread_create(&tid, NULL, threadRoutine, NULL) != SUCCESS) {
        perror("MAIN THREAD | pthread_create() failed");

        return EXIT_FAILURE;
    }

    sleep(FIVE_SECONDS);

    if (pthread_cancel(tid) != SUCCESS) {
        perror("MAIN THREAD | pthread_cancel() failed");

        return EXIT_FAILURE;
    }
    printf("MAIN THREAD | Thread %lu canceled\n", tid);

    if (pthread_join(tid, NULL) != SUCCESS) {
        perror("MAIN THREAD | pthread_join() failed");

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
