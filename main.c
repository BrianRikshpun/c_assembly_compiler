#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro_parser.h"
#include "data.h"
#include "assembler.h"

int ends_with_as(const char *filename) {
    int len = strlen(filename);
    return len > 3 && strcmp(filename + len - 3, ".as") == 0;
}

void print_codes() {
    for (int i = 0; i < LIST_LENGTH; i++) {
        printf("%hd\n", codes[i]);
    }
}

int main(int argc, char *argv[]) {
    FILE *file;
    char line[100];
    char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    allocate_labels();
    allocate_externals();
    allocate_internals();


    for(int i = 1; i < argc; i++) {
        if (!ends_with_as(argv[i])) {
            printf("File %s does not end with .as\n", argv[i]);
            continue;
        }

        file = fopen(argv[i], "r");
        if (file == NULL) {
            printf("Could not open file %s\n", argv[i]);
            continue;
        }

        char output_filename[100];

        strcpy(output_filename, argv[i]);
        output_filename[strlen(argv[i]) - 3] = '\0';
        strcat(output_filename, ".am");

        output_file = fopen(output_filename, "w");
        if (output_file == NULL) {
            printf("Could not open output file %s\n", output_filename);
            fclose(file);
            continue;
        }

        while (fgets(line, sizeof(line), file)) {
            replace_macros(line, file);
        }

        fclose(file);
        fclose(output_file);

        process_am_file(output_filename);



        // Open .ob file for writing
        output_filename[strlen(output_filename) - 3] = '\0';
        strcat(output_filename, ".ob");
        output_file = fopen(output_filename, "w");
        if (output_file == NULL) {
            printf("Could not open output file %s\n", output_filename);
            continue;
        }

        int non_zero_codes = 0;
        int non_zero_variables = 0;

        // Count non-zero values in the codes list
        for (int j = 0; j < LIST_LENGTH; j++) {
            if (codes[j] != 0) {
                non_zero_codes++;
            }
        }

        // Count non-zero values in the variables list
        for (int j = 0; j < LIST_LENGTH; j++) {
            if (variables[j] != 0) {
                non_zero_variables++;
            }
        }

        // Write the lengths of the non-zero codes and variables lists
        fprintf(output_file, "%d %d\n", non_zero_codes, non_zero_variables);

        // Write the base-64 representations of the non-zero shorts in the codes list
        for (int j = 0; j < LIST_LENGTH; j++) {
            if (codes[j] != 0) {
                fprintf(output_file, "%c%c\n", base64_chars[codes[j] & 0x3F], base64_chars[(codes[j] >> 6) & 0x3F]);
            }
        }

        // Write the base-64 representations of the non-zero shorts in the variables list
        for (int j = 0; j < LIST_LENGTH; j++) {
            if (variables[j] != 0) {
                fprintf(output_file, "%c%c\n", base64_chars[variables[j] & 0x3F],
                        base64_chars[(variables[j] >> 6) & 0x3F]);
            }
        }

        // Write to .ent file
        output_filename[strlen(output_filename) - 3] = '\0';
        strcat(output_filename, ".ent");
        FILE *ent_file = fopen(output_filename, "w");
        if (ent_file != NULL) {
            for (int j = 0; j < internals.size; j++) {
                fprintf(ent_file, "%s %d\n", internals.array[j], internals.row_ids[j]);
            }
            fclose(ent_file);
        }

        // Write to .ext file
        output_filename[strlen(output_filename) - 4] = '\0';
        strcat(output_filename, ".ext");
        FILE *ext_file = fopen(output_filename, "w");
        if (ext_file != NULL) {
            for (int j = 0; j < externals.size; j++) {
                fprintf(ext_file, "%s %d\n", externals.array[j], externals.row_ids[j]);
            }
            fclose(ext_file);
            fclose(output_file);

        }
    }
    //print_codes();
    free_labels();
    free_externals();
    free_internals();

    return 0;
}

