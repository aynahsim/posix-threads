#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>

#define SUCCESS 0

#define ONE_SECOND 1
#define FIVE_SECONDS 5

void printThreadInfo(char *threadName, int pid, int ppid, int tid, pthread_t pthreadId) {
    printf(
        "%s | %s: %d | %s: %d | %s: %d (%lu)\n",
        threadName,
        "Process ID", pid,
        "Parent process ID", ppid,
        "Thread ID", tid, pthreadId
    );
}

void setSigintHandler(char *threadName, void (*sigintHandler)(int)) {
    int errNum = SUCCESS;

    sigset_t sigset;
    if ((errNum = sigemptyset(&sigset)) != SUCCESS) {
        printf("%s | sigemptyset() failed: %s\n", threadName, strerror(errNum));
        exit(EXIT_FAILURE);
    }
    if ((errNum = sigaddset(&sigset, SIGINT)) != SUCCESS) {
        printf("%s | sigaddset() failed: %s\n", threadName, strerror(errNum));
        exit(EXIT_FAILURE);
    }

    struct sigaction sa;
    sa.sa_mask = sigset;
    sa.sa_handler = sigintHandler;
    if ((errNum = sigaction(SIGINT, &sa, NULL)) != SUCCESS) {
        printf("%s | sigaction() failed: %s\n", threadName, strerror(errNum));
        exit(EXIT_FAILURE);
    }
}

void handleSigint1(int sigNum) {
    printf("FIRST HANDLER | SIGINT (%d) detected\n", sigNum);
}

void *threadRoutine1(void *arg) {
    printThreadInfo("FIRST THREAD", getpid(), getppid(), gettid(), pthread_self());

    setSigintHandler("FIRST THREAD", handleSigint1);

    sleep(FIVE_SECONDS);

    return NULL;
}

void handleSigint2(int sigNum) {
    printf("SECOND HANDLER | SIGINT (%d) detected\n", sigNum);
}

void *threadRoutine2(void *args) {
    printThreadInfo("SECOND THREAD", getpid(), getppid(), gettid(), pthread_self());

    setSigintHandler("SECOND THREAD", handleSigint2);
    
    sleep(FIVE_SECONDS);

    return NULL;
}

int main(int argc, char **argv) {
    printThreadInfo("MAIN THREAD", getpid(), getppid(), gettid(), pthread_self());

    // -------------------- CREATING THREADS --------------------

    pthread_t thread1;
    if (pthread_create(&thread1, NULL, threadRoutine1, NULL) != SUCCESS) {
        perror("MAIN THREAD | pthread_create() failed on thread #1");
        return EXIT_FAILURE;
    }

    pthread_t thread2;
    if (pthread_create(&thread2, NULL, threadRoutine2, NULL) != SUCCESS) {
        perror("MAIN THREAD | pthread_create() failed on thread #2");
        return EXIT_FAILURE;
    }

    sleep(ONE_SECOND);

    // -------------------- SENDING SIGNALS --------------------

    if (pthread_kill(thread1, SIGINT) != SUCCESS) {
        perror("MAIN THREAD | pthread_kill() failed on thread #1");
        return EXIT_FAILURE;
    }

    if (pthread_kill(thread2, SIGINT) != SUCCESS) {
        perror("MAIN THREAD | pthread_kill() failed on thread #2");
        return EXIT_FAILURE;
    }

    // -------------------- JOINING THREADS --------------------

    if (pthread_join(thread1, NULL) != SUCCESS) {
        perror("MAIN THREAD | pthread_join() failed on thread #1");
        return EXIT_FAILURE;
    }

    if (pthread_join(thread2, NULL) != SUCCESS) {
        perror("MAIN THREAD | pthread_join() failed on thread #2");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
