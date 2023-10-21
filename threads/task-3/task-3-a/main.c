#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0

typedef struct {
    int intVar;
    char *charPtr;
} Data;

void *threadRoutine(void *arg) {
    Data *data = (Data *) arg;
    if (data == NULL) {
        perror("NEW THREAD | Thread routine argument is NULL\n");

        return NULL;
    }

    printf("NEW THREAD | intVar:  %d\n", data->intVar);
    printf("NEW THREAD | charPtr: %c\n", *(data->charPtr));

    return NULL;
}

int main(int argc, char **argv) {
    Data data;
    data.intVar = 10;
    data.charPtr = (char *) malloc(sizeof(char));
    if (data.charPtr == NULL) {
        perror("MAIN THREAD | malloc() failed");

        return EXIT_FAILURE;
    }
    *(data.charPtr) = 'a';

    pthread_t tid;
    if (pthread_create(&tid, NULL, threadRoutine, (void *) &data) != SUCCESS) {
        perror("MAIN THREAD | pthread_create() failed");

        free(data.charPtr);

        return EXIT_FAILURE;
    }

    if (pthread_join(tid, NULL) != SUCCESS) {
        perror("MAIN THREAD | pthread_join() failed");

        free(data.charPtr);

        return EXIT_FAILURE;
    }

    free(data.charPtr);

    return EXIT_SUCCESS;
}
