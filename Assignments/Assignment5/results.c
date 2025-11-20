#include "results.h"

void print_results(Result *res, int n, const char *output_file) {
    FILE *out = fopen(output_file, "w");
    if (!out) {
        perror("Erro ao abrir o ficheiro de output");
        exit(EXIT_FAILURE);
    }
    // File
    fprintf(out, "PID\tPPID\tFicheiro\tLinhas\tPalavras\tChars\n");
    // Terminal
    fprintf(stdout, "PID\tPPID\tFicheiro\tLinhas\tPalavras\tChars\n");

    int lines_total = 0, words_total = 0, chars_total = 0;

    // Print results
    for (int i = 0; i < n; i++) {
        // File
        fprintf(out, "%d\t%d\t%s\t%d\t%d\t%d\n", 
            res[i].pid, res[i].ppid, res[i].filename, 
            res[i].lines, res[i].words, res[i].chars);
        // Terminal
        fprintf(stdout, "%d\t%d\t%s\t%d\t%d\t%d\n", 
            res[i].pid, res[i].ppid, res[i].filename, 
            res[i].lines, res[i].words, res[i].chars);

        // Acquire total results
        lines_total += res[i].lines;
        words_total += res[i].words;
        chars_total += res[i].chars;
    }

    // Print results totals

    // File
    fprintf(out, "TOTAIS:\t\t\t%d\t%d\t%d\n", lines_total, words_total, chars_total);
    // Terminal
    fprintf(out, "TOTAIS:\t\t\t%d\t%d\t%d\n", lines_total, words_total, chars_total);
    
    if (out) fclose(out);
}