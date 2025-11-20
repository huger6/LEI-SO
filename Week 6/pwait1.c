#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int status;
    if (fork()) {
        printf("À espera...\n");
        wait(&status);
        printf("O meu filho já terminou\n");
        printf("Terminou status: exit: %d sinal: %d\n", status>>8, status & 0xFF);
    }
    else {
        printf("PF: PID=%d\n", getpid());
        sleep(15);
        return 5;
    }
    return 0;
}

// Testar com $ pwait1

// Aguardar o tempo de sleep e verificar a var "status" com:
// $ pwait1 &

// Antes de terminar o tempo de sleep fazer:
// $ kill -SIGTERM <pid>

// Verificar se o processo termina de imediato e o número do sinal é colocado na variável status