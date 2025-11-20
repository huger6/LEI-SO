# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

int main(int argc, char *argv[]) {
    char *command;
    char *infile = "in.txt";
    char *outfile = "out.txt";
    char *errorfile = "errors.txt";
    if (argc < 2) {
        printf("Usage: %s <option>\n", argv[0]);
        printf("Options:\n");
        printf("1. Count lines in in.txt\n");
        printf("2. Count lines in in.txt and write to out.txt\n");
        printf("3. Count lines in in.txt, write to out.txt, and redirect errors to errors.txt\n");
        return 1;
    }
    int option = atoi(argv[1]);
    switch (option) {
        case 1:
            command = "wc -l < in.txt";
            break;
        case 2:
            command = "wc -l < in.txt > out.txt";
            break;
        case 3:
            command = "wc -l < in.txt > out.txt 2> errors.txt";
            break;
        default:
            printf("Invalid option\n");
            return 1;
    }
    
    execlp("sh", "sh", "-c", command, NULL);
    // If execlp returns, it means there was an error
    perror("Error executing command");
    return 1;
}