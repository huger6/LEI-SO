#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <getopt.h>

#include "results.h"
#include "utils.h"

#define MAX_FILES 100
#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "USAGE: %s <file1> ... <fileN> [-o output_file]\n", argv[0]);
        return 1;
    }

    char *output_file = NULL;
    int opt;

    // Read -o in any position
    while ((opt = getopt(argc, argv, "o:")) != -1) {
        switch (opt) {
            case 'o':
                output_file = optarg;
                break;

            default:
                fprintf(stderr, "USAGE: %s <file1> ... <fileN> [-o output_file]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Extract files ignoring -o and its argument
    char *files[MAX_FILES];
    int file_count = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0) { i++; continue; }
        if (argv[i][0] == '-') continue;
        files[file_count++] = argv[i];
    }

    if (file_count == 0) {
        fprintf(stderr, "No input files provided.\n");
        exit(EXIT_FAILURE);
    }

    // To store all results
    Result results[MAX_FILES];
    int results_count = 0;

    // One pipe per file
    int pipefd[MAX_FILES][2];

    for (int i = 0; i < file_count; i++) {

        if (pipe(pipefd[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // ---------------- CHILD ----------------
            close(pipefd[i][READ_END]);

            Result res;
            memset(&res, 0, sizeof(Result));

            res.pid = getpid();
            res.ppid = getppid();
            snprintf(res.filename, sizeof(res.filename), "%s", files[i]);

            count(files[i], &res.lines, &res.words, &res.chars);

            write(pipefd[i][WRITE_END], &res, sizeof(Result));
            close(pipefd[i][WRITE_END]);

            exit(0);
        }

        // ---------------- PARENT ----------------
        close(pipefd[i][WRITE_END]);

        Result res;
        read(pipefd[i][READ_END], &res, sizeof(Result));
        close(pipefd[i][READ_END]);

        results[results_count++] = res;
    }


    // Wait for ALL children
    while (wait(NULL) > 0);

    // Print everything with the function from the second version
    print_results(results, results_count, output_file);

    return 0;
}

