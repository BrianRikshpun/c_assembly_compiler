#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro_parser.h"
#include "data.h"
#include "assembler.h"

int ends_with_as(const char *filename) {
    int len = strlen(filename);
    return len > 7 && strcmp(filename + len - 7, ".as.txt") == 0;
}

void print_codes() {
    for (int i = 0; i < LIST_LENGTH; i++) {
        printf("%hd\n", codes[i]);
    }
}

int main(int argc, char *argv[]) {
    FILE *file;
    char line[100];

    allocate_labels();
    allocate_externals();
    allocate_internals();

    for(int i = 1; i < argc; i++) {
        if(!ends_with_as(argv[i])) {
            printf("File %s does not end with .as.txt\n", argv[i]);
            continue;
        }

        file = fopen(argv[i], "r");
        if(file == NULL) {
            printf("Could not open file %s\n", argv[i]);
            continue;
        }

        char output_filename[100];
        strcpy(output_filename, argv[i]);
        output_filename[strlen(argv[i]) - 7] = '\0';
        strcat(output_filename, ".am");

        output_file = fopen(output_filename, "w");
        if(output_file == NULL) {
            printf("Could not open output file %s\n", output_filename);
            fclose(file);
            continue;
        }

        while(fgets(line, sizeof(line), file)) {
            replace_macros(line, file);
        }

        fclose(file);
        fclose(output_file);

        process_am_file(output_filename);
    }

    //print_codes();

    free_labels();
    free_externals();
    free_internals();

    return 0;
}

