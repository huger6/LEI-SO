#include <stdio.h>
#include <unistd.h>

int main() {
    printf("PID: %d\n,", getpid());
    printf("Invoca uma nova shell, com o programa ps\n");
    system("ps -fH");
    printf("Transfere para o programa ps\n");
    execlp("ps", "ps", "-fH", NULL);
    printf("Isto nunca vai ser executado");
    return 0;
}