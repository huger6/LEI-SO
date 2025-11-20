#include <stdio.h>
#include <unistd.h>

int main(void) {
    printf("PID: %d\n", getpid());
    while(1);
    return 0;
}

// Run mypid.c with:
//$ mypid &
//$ ps 
// To interrupt:
// kill -SIGTERM <pid>
