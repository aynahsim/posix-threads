#define _GNU_SOURCE

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

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

void handleSigint(int sigNum) {
    printf("SIGINT HANDLER | SIGINT (%d) detected\n", sigNum);
}

void *blockingThreadRoutine(void *arg) {
    printThreadInfo("BLOCKING THREAD", getpid(), getppid(), gettid(), pthread_self());

    sigset_t sigset;
    if (sigfillset(&sigset) != SUCCESS) {
        perror("BLOCKING THREAD | sigfillset() failed");
        exit(EXIT_FAILURE);
    }
    if (pthread_sigmask(SIG_BLOCK, &sigset, NULL) != SUCCESS) {
        perror("BLOCKING THREAD | pthread_sigmask() failed");
        exit(EXIT_FAILURE);
    }

    sleep(FIVE_SECONDS);

    return NULL;
}

void *sigintThreadRoutine(void *arg) {
    printThreadInfo("SIGINT THREAD", getpid(), getppid(), gettid(), pthread_self());

    sigset_t sigset;
    if (sigemptyset(&sigset) != SUCCESS) {
        perror("SIGINT THREAD | sigemptyset() failed");
        exit(EXIT_FAILURE);
    }
    if (sigaddset(&sigset, SIGINT) != SUCCESS) {
        perror("SIGINT THREAD | sigaddset() failed");
        exit(EXIT_FAILURE);
    }

    struct sigaction sa;
    sa.sa_handler = handleSigint;
    sa.sa_mask = sigset;
    if (sigaction(SIGINT, &sa, NULL) != SUCCESS) {
        perror("SIGINT THREAD | sigaction() failed");
        exit(EXIT_FAILURE);
    }

    sleep(FIVE_SECONDS);
    
    return NULL;
}

void *sigquitThreadRoutine(void *arg) {
    printThreadInfo("SIGQUIT THREAD", getpid(), getppid(), gettid(), pthread_self());

    sigset_t sigset;
    if (sigemptyset(&sigset) != SUCCESS) {
        perror("SIGQUIT THREAD | sigemptyset() failed");
        exit(EXIT_FAILURE);
    }
    if (sigaddset(&sigset, SIGQUIT) != SUCCESS) {
        perror("SIGQUIT THREAD | setaddset() failed");
        exit(EXIT_FAILURE);
    }

    int sigNum;
    if (sigwait(&sigset, &sigNum) != SUCCESS) {
        perror("SIGQUIT THREAD | sigwait() failed");
        exit(EXIT_FAILURE);
    }

    printf("SIGQUIT THREAD | SIGQUIT (%d) detected\n", sigNum);

    return NULL;
}

int main(int argc, char **argv) {
    printThreadInfo("MAIN THREAD", getpid(), getppid(), gettid(), pthread_self());

    // -------------------- CREATING THREADS --------------------

    pthread_t blockingThread;
    if (pthread_create(&blockingThread, NULL, blockingThreadRoutine, NULL) != SUCCESS) {
        perror("MAIN THREAD | pthread_create() failed on blocking thread");
        return EXIT_FAILURE;
    }

    pthread_t sigintThread;
    if (pthread_create(&sigintThread, NULL, sigintThreadRoutine, NULL) != SUCCESS) {
        perror("MAIN THREAD | pthread_create() failed on SIGINT thread");
        return EXIT_FAILURE;
    }

    pthread_t sigquitThread;
    if (pthread_create(&sigquitThread, NULL, sigquitThreadRoutine, NULL) != SUCCESS) {
        perror("MAIN THREAD | pthread_create() failed on SIGQUIT thread");
        return EXIT_FAILURE;
    }

    // -------------------- SENDING SIGNALS --------------------

    sleep(ONE_SECOND);
    printf("MAIN THREAD | Sending signals to blocking thread...\n");
    if (pthread_kill(blockingThread, SIGINT) != SUCCESS) {
        perror("MAIN THREAD | pthread_kill() failed on blocking thread");
        return EXIT_FAILURE;
    }
    if (pthread_kill(blockingThread, SIGQUIT) != SUCCESS) {
        perror("MAIN THREAD | pthread_kill() failed on blocking thread");
        return EXIT_FAILURE;
    }

    sleep(ONE_SECOND);
    printf("MAIN THREAD | Sending SIGINT signal to SIGINT thread...\n");
    if (pthread_kill(sigintThread, SIGINT) != SUCCESS) {
        perror("MAIN THREAD | pthread_kill() failed on SIGINT thread");
        return EXIT_FAILURE;
    }

    sleep(ONE_SECOND);
    printf("MAIN THREAD | Sending SIGQUIT signal to SIGQUIT thread...\n");
    if (pthread_kill(sigquitThread, SIGQUIT) != SUCCESS) {
        perror("MAIN THREAD | pthread_kill() failed on SIGQUIT thread");
        return EXIT_FAILURE;
    }

    // -------------------- JOINING THREADS --------------------

    if (pthread_join(blockingThread, NULL) != SUCCESS) {
        perror("MAIN THREAD | pthread_join() failed on blocking thread");
        return EXIT_FAILURE;
    }

    if (pthread_join(sigintThread, NULL) != SUCCESS) {
        perror("MAIN THREAD | pthread_join() failed on SIGINT thread");
        return EXIT_FAILURE;
    }

    if (pthread_join(sigquitThread, NULL) != SUCCESS) {
        perror("MAIN THREAD | pthread_join() failed on SIGQUIT thread");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
