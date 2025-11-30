#ifndef SIGNALS_H
#define SIGNALS_H

#include <signal.h>

/* Глобальные флаги */
extern volatile sig_atomic_t parent_notified;
extern volatile sig_atomic_t child_got_signal;

void install_handler(int signum, void (*handler)(int));
void parent_sigusr2_handler(int sig);
void child_sigusr1_handler(int sig);

#endif
