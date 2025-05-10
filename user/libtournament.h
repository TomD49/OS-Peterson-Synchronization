#ifndef TOURNAMENT_LOCK_H
#define TOURNAMENT_LOCK_H

#include "kernel/types.h"

#define MAX_PROCESSES 64

extern int lock_id_map[MAX_PROCESSES - 1];
extern int process_pid_map[MAX_PROCESSES];
extern int tournament_role;
extern int _processes;

int int_log2(int x);
int tournament_create(int processes);
int tournament_acquire(void);
int tournament_release(void);

#endif
