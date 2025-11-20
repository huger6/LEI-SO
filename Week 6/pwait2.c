#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
    int pid, status;
    if (!(pid=fork())) {
        sleep(1);
    }
    else {
        while(waitpid(pid, &status, WNOHANG) != pid)
            printf("Ainda não terminou\n");
        printf("Terminou (PID=%d) (Status=%d)\n", pid, status);
    }
}