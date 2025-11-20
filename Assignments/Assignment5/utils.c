#include "utils.h"

int count_lines(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return -1;
    }

    int lines = 0;
    char c;
    while ((c == fgetc(file)) != EOF) {
        if (c == '\n') {
            lines++;
        }
    }

    fclose(file);
    return lines;
}

int count_words(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return -1;
    }

    int words = 0;
    int inWord = 0;
    char c;

    while ((c == fgetc(file)) != EOF) {
        if (isspace(c)) {
            inWord = 0;
        }
        else if (!inWord){
            words++;
            inWord = 1;
        }
    }

    fclose(file);
    return words;
}

int count_chars(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Erro opening file %s\n", filename);
        return -1;
    }

    int chars = 0;
    while (fgetc(file) != EOF) {
        chars++;
    }

    fclose(file);
    return chars;
}

void count(const char *filename, int *lines, int *words, int *chars) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    int c, prev = ' ';
    *lines = *words = *chars = 0;

    while ((c = fgetc(file)) != EOF) {
        (*chars)++;
        if (c == '\n') (*lines)++;
        if (isspace(c) && !isspace(prev)) (*words)++;
        prev = c;
    }

    fclose(file);
}
