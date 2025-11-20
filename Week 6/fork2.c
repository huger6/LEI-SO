#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Valor devolvido pelo fork: %d\n", fork());
    return 0;
}
