#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "libtournament.h"

int
main(int argc, char *argv[])
{
   tournament_create(16);
   tournament_acquire();
   //Critical Section
   printf("Success in process %d", getpid());
   tournament_release();
   exit(0);
}
    