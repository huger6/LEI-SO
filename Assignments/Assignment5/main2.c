# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>

# define MAX_FILES 100
# define READ_END 0
# define WRITE_END 1

int count_lines(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return -1;
    }
    int lines = 0;
    char c;
    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            lines++;
        }
    }
    fclose(file);
    return lines;
}
int count_words(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return -1;
    }
    int words = 0;
    int inWord = 0;
    char c;
    while ((c = fgetc(file)) != EOF) {
        if (c == ' ' || c == '\n' || c == '\t') {
            inWord = 0;
        } else {
            if (!inWord) {
                words++;
                inWord = 1;
            }
        }
    }
    fclose(file);
    return words;
}
int count_chars(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return -1;
    }
    int chars = 0;
    char c;
    while ((c = fgetc(file)) != EOF) {
        chars++;
    }
    fclose(file);
    return chars;
}
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s file1 file2 ... [-o output_file]\n", argv[0]);
        return 1;
    }
    char *output_file = NULL;
    int file_count = 0;
    char *files[MAX_FILES];
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0) {
            if (i == argc - 1) {
                fprintf(stderr, "Missing output file name after -o\n");
                return 1;
            }
            output_file = argv[++i];
        } else {
            files[file_count++] = argv[i];
        }
    }
    int total_lines = 0, total_words = 0, total_chars = 0;
    int pipefd[MAX_FILES][2];
    for (int i = 0; i < file_count; i++) {
        if (pipe(pipefd[i]) == -1) {
            fprintf(stderr, "Error creating pipe\n");
            return 1;
        }
        pid_t pid = fork();
        if (pid == -1) {
            fprintf(stderr, "Error forking process\n");
            return 1;
        } else if (pid == 0) {
            // Child process
            close(pipefd[i][READ_END]);
            int lines = count_lines(files[i]);
            int words = count_words(files[i]);
            int chars = count_chars(files[i]);
            write(pipefd[i][WRITE_END], &lines, sizeof(int));
            write(pipefd[i][WRITE_END], &words, sizeof(int));
            write(pipefd[i][WRITE_END], &chars, sizeof(int));
            close(pipefd[i][WRITE_END]);
            exit(0);
        } else {
            // Parent process
            close(pipefd[i][WRITE_END]);
            int lines, words, chars;
            read(pipefd[i][READ_END], &lines, sizeof(int));
            read(pipefd[i][READ_END], &words, sizeof(int));
            read(pipefd[i][READ_END], &chars, sizeof(int));
            total_lines += lines;
            total_words += words;
            total_chars += chars;
            printf("PID: %d\tParent PID: %d\tFile: %s\tLines: %d\tWords: %d\tChars: %d\n",
                   getpid(), getppid(), files[i], lines, words, chars);
            close(pipefd[i][READ_END]);
        }
    }
    printf("TOTALS:\tLines: %d\tWords: %d\tChars: %d\n", total_lines, total_words, total_chars);
    if (output_file != NULL) {
        FILE *fp = fopen(output_file, "w");
        if (fp == NULL) {
            fprintf(stderr, "Error opening output file %s\n", output_file);
            return 1;
        }
        fprintf(fp, "TOTALS:\tLines: %d\tWords: %d\tChars: %d\n", total_lines, total_words, total_chars);
        fclose(fp);
    }
    return 0;
}
