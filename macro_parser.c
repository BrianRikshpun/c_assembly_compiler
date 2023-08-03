#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro_parser.h"

#define MAX_LINE_LENGTH 100
#define MAX_LINES 100

FILE *output_file;

// Struct for storing macro definitions
typedef struct {
    char name[MAX_LINE_LENGTH];
    char body[MAX_LINES][MAX_LINE_LENGTH];
    int body_len;
} Macro;

Macro macros[MAX_LINES];
int macros_len = 0;


void replace_macros(char* line, FILE* file) {


    if (line == NULL) {
        printf("Error: line is null.\n");
        return;
    }
    if (file == NULL) {
        printf("Error: file is null.\n");
        return;
    }

    // Check if the line defines a macro
    printf("%s",line);
    char *mcro = strstr(line, "mcro");
    if (mcro != NULL) {
        // Store the macro definition
        strcpy(macros[macros_len].name, mcro + 5);  // Skip "mcro "
        macros[macros_len].body_len = 0;
        // Assuming the body of the macro is on the next line
        fgets(line, MAX_LINE_LENGTH, file);
        while (strstr(line, "endmcro") == NULL) {
            strcpy(macros[macros_len].body[macros[macros_len].body_len++], line);
            fgets(line, MAX_LINE_LENGTH, file);
        }
        macros_len++;
        return;
    }

    // Check if the line invokes a macro
    for (int j = 0; j < macros_len; j++) {
        if (strcmp(line, macros[j].name) == 0) {
            // Replace the macro invocation with its definition
            for (int k = 0; k < macros[j].body_len; k++) {
                fprintf(output_file, "%s", macros[j].body[k]);
            }
            return;
        }
    }

    if(output_file == NULL) {
        printf("Error: output file is not open.\n");
        return;
    }

    if(line == NULL) {
        printf("Error: line is null.\n");
        return;
    }
    // If the line is not a macro invocation, print it
    fprintf(output_file, "%s", line);
}