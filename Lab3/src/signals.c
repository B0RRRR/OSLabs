#define _POSIX_C_SOURCE 200809L

#include "signals.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

volatile sig_atomic_t parent_notified = 0;
volatile sig_atomic_t child_got_signal = 0;

void parent_sigusr2_handler(int sig) {
    (void)sig;
    parent_notified = 1;
}

void child_sigusr1_handler(int sig) {
    (void)sig;
    child_got_signal = 1;
}

void install_handler(int signum, void (*handler)(int)) {
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(signum, &sa, NULL) == -1) {
        fprintf(stderr, "sigaction(%d) failed: %s\n", signum, strerror(errno));
        exit(EXIT_FAILURE);
    }
}
