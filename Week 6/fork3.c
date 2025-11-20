#include <stdio.h>
#include <unistd.h>

int main() {
    int pid;
    printf("1\n");
    pid = fork();
    if (pid != 0) {
        printf("Sou o processo pai. Criei um processo filho\n");
    }
    else {
        printf("Sou um novo processo.\n");
    }
    printf("2\n");
    return 0;
}