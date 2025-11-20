#include <stdio.h>
#include <unistd.h>

int main() {
    char s[100];
    if (fork()) {
        scanf("%[^\n]", s);
        printf("PP: s=[%s]\nVou terminar\n", s);
    }
    else {
        scanf("%[^\n]", s);
        printf("PF: s=[%s]\nVou terminar\n", s);
    }
}

// Apenas PP lê do stdin. **Confirmar**
