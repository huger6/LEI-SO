#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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

    // Parse command line options (-o can appear anywhere)
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

    // Extract file names, ignoring -o and its argument
    char *files[50];
    int num_files = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0) { i++; continue; } // skip -o and its filename
        if (argv[i][0] == '-') continue; // skip any other option
        files[num_files++] = argv[i];
    }

    if (num_files == 0) {
        fprintf(stderr, "No input files provided.\n");
        exit(EXIT_FAILURE);
    }

    // Pipe
    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid;
    int i;

    // Each process creates the next one
    for (i = 0; i < num_files; i++) {

        pid = fork();

        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid > 0) {
            break;
        }
    }

    // Determine file index for this process in the chain
    int my_index;
    if (pid > 0)
        my_index = i; // parent in chain
    else
        my_index = num_files - 1; // last child in chain

    Result res;
    memset(&res, 0, sizeof(Result));

    res.pid = getpid();
    res.ppid = getppid();
    snprintf(res.filename, sizeof(res.filename), "%s", files[my_index]);

    count(files[my_index], &res.lines, &res.words, &res.chars);

    // Write this result into the pipe
    close(fd[0]); // Close read end
    write(fd[1], &res, sizeof(Result));
    close(fd[1]);

    // Gather results with the original parent
    if (getppid() != getpid()) {
        // All children simply exit after sending results.
        exit(EXIT_SUCCESS);
    }

    // Wait for all children
    while (wait(NULL) > 0);

    // Read results from pipe
    close(fd[1]); // close write end

    Result results[50];
    int count_results = 0;

    while (read(fd[0], &res, sizeof(Result)) > 0) {
        results[count_results++] = res;
    }

    close(fd[0]);

    // Print results
    print_results(results, count_results, output_file);

    return 0;
}

