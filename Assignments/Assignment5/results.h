#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct result {
    pid_t pid;
    pid_t ppid;
    char filename[100];
    int lines;
    int words;
    int chars;
} Result;


void print_results(Result *res, int n, const char *output_file);