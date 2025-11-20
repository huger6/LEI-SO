#include <stdio.h>
#include <unistd.h>

int main(void) {
    printf("PID: %d; Parent PID: %d\n", getpid(), getppid());
    while(1);
    return 0;
}

// Run myppid.c with:
//$ myppid &
//$ ps -fH
// To interrupt:
// kill -SIGTERM <pid>
