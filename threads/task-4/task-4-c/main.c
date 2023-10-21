#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define EXECUTE 1
#define FIVE_SECONDS 5
#define SUCCESS 0
#define TRUE 1

void cleanup(void *arg) {
    char *resources = (char *) arg;
    if (resources != NULL) {
        free(resources);
    }
}

void *threadRoutine(void *arg) {
    char *string = (char *) malloc((strlen("Hello, World!") + 1) * sizeof(char));
    if (string == NULL) {
        perror("NEW THREAD | malloc() failed");

        return NULL;
    }
    strcpy(string, "Hello, World!");

    pthread_cleanup_push(cleanup, (void *) string);
    while (TRUE) {
        printf("NEW THREAD | %s\n", string);
    }
    pthread_cleanup_pop(EXECUTE);

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
