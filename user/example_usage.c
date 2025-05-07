#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int lock_id;
    for (int j = 0 ; j <15 ;j++){
        lock_id = peterson_create();

        if (lock_id < 0) {
            printf("Failed to create lock\n");
            //exit(1);
            break;
        }
        int fork_ret = fork();
        int role = fork_ret > 0 ? 0 : 1;
        
        if (fork_ret > 0) { //parent
            for(int i = 0; i < 6; i++){
                if (peterson_acquire(lock_id, role) < 0) {
                    printf("Failed to acquire lock\n");
                    exit(1);
                }
                // Critical section
                printf("parent success %d\n", lock_id);
                //end Critical section
                if (peterson_release(lock_id, role) < 0) {
                    printf("Failed to release lock\n");
                    exit(1);
                }
            }
            wait(0);
            //if (peterson_destroy(lock_id) < 0) {
            //    printf("Parent failed to destroy lock\n");
            //}
        }
        else{
            for(int i = 0; i < 6; i++){
                if (peterson_acquire(lock_id, role) < 0) {
                    printf("Failed to acquire lock\n");
                    exit(1);
                }
                // Critical section
                printf("child success %d\n", lock_id);
                //end Critical section
                if (peterson_release(lock_id, role) < 0) {
                    printf("Failed to release lock\n");
                    exit(1);
                }
                //sleep(5);
            }
            exit(0);
        } 
    }
    if (peterson_destroy(lock_id) < 0) {
        printf("Failed to destroy lock\n");
        exit(1);
    }

    lock_id = peterson_create();

    if (lock_id < 0) {
        printf("Failed to create lock\n");
        exit(1);
    }

    if (peterson_acquire(lock_id, 0) < 0) {
        printf("Failed to acquire lock\n");
        exit(1);
    }
    // Critical section
    printf("parent success %d\n", lock_id);
    //end Critical section
    if (peterson_release(lock_id, 0) < 0) {
        printf("Failed to release lock\n");
        exit(1);
    }

    printf("got till here %d", lock_id);

    exit(0);
}
    