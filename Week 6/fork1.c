#include <stdio.h>
#include <unistd.h>

int main() {
    printf("1\n");
    fork();
    printf("2\n");
}