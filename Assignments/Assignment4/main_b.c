#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

int countLines(FILE *file);
int countWords(FILE *file, char **lw, char *lw_position);
int countChars(FILE *file);


int main(int argc, char *argv[]) {
    FILE *file;
    char *filename;
    int lines, words, chars;
    char *lw = NULL;
    char lw_position[30];
    if (argc < 2) {
        printf("Usage: %s <filename> [-l] [-w] [-c] [-lw]\n", argv[0]);
        return 1;
    }
    filename = argv[1];
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        return 1;
    }
    lines = countLines(file);
    words = countWords(file, &lw, lw_position);
    rewind(file);
    chars = countChars(file);
    int showLines = 0, showWords = 0, showChars = 0, showLongestWord = 0;
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-l") == 0) {
            showLines = 1;
        } 
        else if (strcmp(argv[i], "-w") == 0) {
            showWords = 1;
        } 
        else if (strcmp(argv[i], "-c") == 0) {
            showChars = 1;
        }
        else if (strcmp(argv[i], "-lw") == 0) {
            showLongestWord = 1;
        }
    }
    if (showLines) {
        printf("Lines: %d\n", lines);
    }
    if (showWords) {
        printf("Words: %d\n", words);
    }
    if (showChars) {
        printf("Characters: %d\n", chars);
    }
    if (showLongestWord) {
        printf("Longest Word: [%s]\n", lw);
        printf("%s\n", lw_position);
    }
    fclose(file);
    if (lw != NULL) free(lw);

    return 0;
}

int countLines(FILE *file) {
    rewind(file);
    int lines = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        lines++;
    }
    rewind(file);
    return lines;
}

int countWords(FILE *file, char **lw, char *lw_position) {
    rewind(file);
    int words = 0;
    char line[MAX_LINE_LENGTH];
    int inWord = 0;
    size_t maxLen = 0;
    int nLines = 0;
    size_t wordLen = 0;
    int wordsPerLine = 0;
    
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        nLines++;
        int i = 0;
        while (line[i]) {
            if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n') {
                if (!inWord) {
                    words++;
                    inWord = 1;
                    wordLen++;
                }
                else {
                    wordLen++;
                }
            } 
            else {
                if (inWord) {
                    if (wordLen > maxLen) {
                        maxLen = wordLen;
                        if (*lw) free(*lw);
                        *lw = strndup(&line[i - wordLen], wordLen); // only avaliable in linux
                        sprintf(lw_position, "Line: %d; Position: %d", nLines, i - wordLen);
                    }
                    inWord = 0;
                    wordLen = 0;
                }
            }
            i++;
        }
    }
    rewind(file);
    return words;
}

int countChars(FILE *file) {
    rewind(file);
    int chars = 0;
    char c;
    while ((c = fgetc(file)) != EOF) {
        chars++;
    }
    rewind(file);
    return chars;
}
