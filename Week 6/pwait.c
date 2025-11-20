#include <stdio.h>
#include <unistd.h>

int main() {
    if (fork()) {
        sleep(2);
        printf("PP: Vou terminar\n");
    }
    else {
        printf("PF: À espera que o pai termine\n");
        while (getppid() != 1);
        printf("PF: Já terminou, fui adotado pelo init\n");
    }
    return 0;
}