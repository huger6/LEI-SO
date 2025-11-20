#include <stdio.h>
#include <unistd.h>

int main() {
    int i = 10;
    if (fork() != 0) {
        i++;
        printf("PP:i=%d\n", i);
    }
    else {
        i += 2;
        printf("PF:i=%d\n", i);
    }
    i++;
    printf("i=%d\n", i);
    return 0;
}