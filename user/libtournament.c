#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "libtournament.h"

#define MAX_PROCESSES 64

int lock_id_map[MAX_PROCESSES - 1];
int process_pid_map[MAX_PROCESSES];
int tournament_role;
int _processes;

int int_log2(int x) {
    int r = 0;
    while (x > 1) {
        x >>= 1;
        r++;
    }
    return r;
}

int tournament_create(int processes) {
    int lock_id, fork_ret, role;
    _processes = processes;
    int ID = 0;
    int level_start_idx = 0;
    int num_levels = int_log2(_processes);

    for (int level = 0; level < num_levels; level++) {
        int lock_idx = level_start_idx + ID;

        // Create lock for this level-path
        lock_id = peterson_create();
        if (lock_id < 0) {
            printf("could not create Peterson lock in index %d\n", lock_idx);
            return -1;
        }
        lock_id_map[lock_idx] = lock_id;

        fork_ret = fork();
        if (fork_ret < 0) {
            printf("could not fork\n");
            return -1;
        }
        role = fork_ret > 0 ? 0 : 1;
        ID= ID * 2 + role;
        level_start_idx += (1 << level); // 2^level locks before this level
    }
    // Leaf process: assign role and store PID
    tournament_role = ID;
    process_pid_map[ID] = getpid();
    
    return ID;
}

int tournament_acquire(void){
    int lock_idx;
    int ID= tournament_role;
    int level_start_idx= _processes-1;
    for(int level= int_log2(_processes)-1; level>=0; level--){
        level_start_idx -=  (1 << level);
        ID /= 2;   
        lock_idx= level_start_idx + ID;
        if (peterson_acquire(lock_id_map[lock_idx] , ID % 2) <0){
            printf("Could not acquire lock in index %d", lock_idx);
            return -1;
        }
    }
    return 0;

}

int tournament_release(void) {
    int ID = 0;
    int lock_idx;
    int level_start_idx = -1;
    int num_levels = int_log2(_processes);

    // Walk from top to bottom to compute the path
    for (int level = 0; level < num_levels; level++) {
        int role = (tournament_role >> (num_levels - level - 1)) & 1;
        ID = ID * 2 + role;
        level_start_idx += (1 << level);
        lock_idx = level_start_idx + ID;
        if (peterson_release(lock_id_map[lock_idx], role) <0){
            printf("Could not release lock in index %d", lock_idx);
            return -1;
        } 
    }
    return 0;

}

