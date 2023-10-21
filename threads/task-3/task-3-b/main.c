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

    free(data->charPtr);
    free(data);

    return NULL;
}

int main(int argc, char **argv) {
    Data *data = (Data *) malloc(sizeof(Data));
    if (data == NULL) {
        perror("MAIN THREAD | malloc() failed");

        return EXIT_FAILURE;
    }

    data->charPtr = (char *) malloc(sizeof(char));
    if (data->charPtr == NULL) {
        perror("MAIN THREAD | malloc() failed");

        free(data);

        return EXIT_FAILURE;
    }

    data->intVar = 10;
    *(data->charPtr) = 'a';

    pthread_attr_t attr;
    if (pthread_attr_init(&attr) != SUCCESS) {
        perror("MAIN THREAD | pthread_attr_init() failed");

        free(data->charPtr);
        free(data);

        return EXIT_FAILURE;
    }
    if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != SUCCESS) {
        perror("MAIN THREAD | pthread_attr_setdetachstate() failed");

        free(data->charPtr);
        free(data);

        if (pthread_attr_destroy(&attr) != SUCCESS) {
            perror("MAIN THREAD | pthread_destroy() failed");
        }

        return EXIT_FAILURE;
    }

    pthread_t tid;
    if (pthread_create(&tid, &attr, threadRoutine, (void *) data) != SUCCESS) {
        perror("MAIN THREAD | pthread_create() failed");

        free(data->charPtr);
        free(data);

        if (pthread_attr_destroy(&attr) != SUCCESS) {
            perror("MAIN THREAD | pthread_destroy() failed");
        }

        return EXIT_FAILURE;
    }

    if (pthread_attr_destroy(&attr) != SUCCESS) {
        perror("MAIN THREAD | pthread_attr_destroy() failed");

        free(data->charPtr);
        free(data);

        return EXIT_FAILURE;
    }

    pthread_exit(NULL);

    return EXIT_SUCCESS;
}
