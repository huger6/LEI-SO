#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


int count_lines(const char *filename);
int count_words(const char *filename);
int count_chars(const char *filename);
void count(const char *filename, int *lines, int *words, int *chars);